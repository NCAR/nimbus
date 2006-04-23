#
#    Makefile for n2asc program
#
CC=		g++
BINDIR=		${LOCAL}/bin
MANDIR=		${LOCAL}/man/man1
WWWDIR=		/net/www/raf/Software

# Solaris
#
INCLUDES= -I${JLOCAL}/include
DEFINES = -DSVR4
LIBS    = -R /usr/dt/lib -lXm -lXt -lX11 -lnetcdf
LIB_DIRS= -L${JLOCAL}/lib
ANONFTP = /net/ftp/pub/archive/RAF-src/bin.Solaris2.9
IHOST	= mead

# Linux
#
INCLUDES= -I${JLOCAL}/include -I/usr/X11R6/include
DEFINES = 
LIB_DIRS= -L/usr/X11R6/lib -L${JLOCAL}/lib
LIBS    = -lXm -lXp -lXt -lXext -lX11 -lnetcdf
ANONFTP = /net/ftp/pub/archive/RAF-src/bin.RedHat9.0
IHOST	= syrah

CFLAGS=		-Wall -g -O2 ${INCLUDES} ${DEFINES}


SRCS=	n2asc.c globals.c init.c timeseg.c search.c lrloop.c\
	netcdf.c cb_main.c cb_time.c cb_pause.c Xwin.c error.c Xerror.c\
	Xfile.c Xquery.c Xwarn.c

HDRS=	define.h

OBJS=	n2asc.o globals.o init.o timeseg.o search.o lrloop.o\
	netcdf.o cb_main.o cb_time.o cb_pause.o Xwin.o error.o Xerror.o\
	Xfile.o Xquery.o Xwarn.o

PROG=	n2asc

.c.o:
	${CC} ${CFLAGS} -c $*.c

${PROG}: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} ${LIB_DIRS} ${LIBS} -o $@

install: ${PROG}
	cp ${PROG} ${BINDIR}
	test -d $(MANDIR) || mkdir $(MANDIR)
	cp ${PROG}.man ${MANDIR}/${PROG}.1
	cp ${PROG}.html ${WWWDIR}

publish: $(PROG)
	scp ${PROG} ${IHOST}:${BINDIR}
	scp ${PROG}.man ${IHOST}:${MANDIR}/${PROG}.1
	cp ${PROG} $(ANONFTP)
	cp ${PROG}.man ${ANONFTP}

clean:
	rm -f ${OBJS} ${PROG}

lint:
	lint ${INCLUDES} ${SRCS} ${LINTLIBS}

print:
	enscript -2Gr -b${PROG} ${HDRS} ${SRCS}

${OBJS}:	define.h
netcdf.o:	${JLOCAL}/include/netcdf.h
n2asc.o:	fbr.h
