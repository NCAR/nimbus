#
#    Makefile for n2asc program
#
CC=		g++
BINDIR=		${JLOCAL}/bin
MANDIR=		${JLOCAL}/man/man1
WWWDIR=		/net/www/raf/Software

# Solaris
#
INCLUDES= -I${JLOCAL}/include
DEFINES = -DSVR4
LIBS    = -R /usr/dt/lib -lXm -lXt -lX11 -Wl,-Bstatic -lnetcdf -Wl,-Bdynamic
LIB_DIRS= -L${JLOCAL}/lib
ANONFTP = /net/ftp/pub/archive/RAF-src/bin.Solaris2.9

# Linux
#
INCLUDES= -I${JLOCAL}/include
DEFINES = 
LIB_DIRS= -L${JLOCAL}/lib
LIBS    = -lXm -lXp -lXt -lXext -lX11 -lnetcdf -lhdf5 -lhdf5_hl
ANONFTP = /net/ftp/pub/archive/RAF-src/bin.RHEL6

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
	test -d $(BINDIR) || mkdir $(BINDIR)
	cp ${PROG} ${BINDIR}
	test -d $(MANDIR) || mkdir $(MANDIR)
	cp ${PROG}.man ${MANDIR}/${PROG}.1

publish: $(PROG)
	cp ${PROG} $(ANONFTP)
	cp ${PROG}.man ${ANONFTP}
	cp ${PROG}.html ${WWWDIR}

clean:
	rm -f ${OBJS} ${PROG}

lint:
	lint ${INCLUDES} ${SRCS} ${LINTLIBS}

print:
	enscript -2Gr -b${PROG} ${HDRS} ${SRCS}

${OBJS}:	define.h
netcdf.o:	/usr/include/netcdf.h
n2asc.o:	fbr.h
