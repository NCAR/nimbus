#
#    Makefile for Skeleton program
#
CC=             gcc
BINDIR=         ${JLOCAL}/bin
MANDIR=         ${JLOCAL}/man/man1

# Solaris
#
INCLUDES= -I${JLOCAL}/include
DEFINES = -DSVR4
LIBS    = -R /usr/dt/lib -lraf -lXm -lXt -lX11 -Wl,-Bstatic -lnetcdf -Wl,-Bdynamic
LIB_DIRS= -L${JLOCAL}/lib
ANONFTP = /jnet/ftp/pub/src/bin.Solaris2.9

# Linux
#
INCLUDES= -I${JLOCAL}/include -I/usr/include/netcdf
DEFINES =
LIB_DIRS= -L${JLOCAL}/lib
LIBS    = -lraf -lXm -lXp -lXt -lXext -lX11 -Wl,-Bstatic -lnetcdf -Wl,-Bdynamic
ANONFTP = /net/ftp/pub/archive/RAF-src/bin.RHEL5


CFLAGS=         -O2 ${INCLUDES} ${DEFINES}

SRCS=	Xwin.c cb_pause.c error.c init.c netcdf.c sort.c cb_main.c cb_time.c\
	globals.c ncextr.c search.c timeseg.c

OBJS=	Xwin.o cb_pause.o error.o init.o netcdf.o sort.o cb_main.o cb_time.o\
	globals.o ncextr.o search.o timeseg.o

PROG=	ncextr

.c.o:
	${CC} ${CFLAGS} -c $*.c

${PROG}: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} ${LIB_DIRS} ${LIBS} -o $@

install: ${PROG}
	cp ${PROG} ${DESTDIR}${BINDIR}
	cp ${PROG}.man ${DESTDIR}${MANDIR}/${PROG}.1

publish:
	cp ${PROG} $(ANONFTP)
	cp ${PROG}.man ${ANONFTP}

clean:
	rm -f ${OBJS} ${PROG}

print:
	enscript -2Gr -b${PROG} ${HDRS} ${SRCS}

${OBJS}:	define.h
netcdf.o:	/usr/include/netcdf/netcdf.h
ncextr.o:	fbr.h
