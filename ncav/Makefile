#
#    Makefile for Skeleton program
#
CC=		gcc
BINDIR=		${LOCAL}/bin
RELEASE=	${LOCAL}/ncav
MANDIR=		${LOCAL}/man/man1


# Solaris
#
INCLUDES= -I${JLOCAL}/include
DEFINES = -DSVR4
LIBS    = -R /usr/dt/lib -lraf -lXm -lXt -lX11 -lnetcdf -lm
LIB_DIRS= -L${JLOCAL}/lib
ANONFTP = /jnet/ftp/pub/src/bin.Solaris2.9

# Linux
#
INCLUDES= -I${JLOCAL}/include
DEFINES = 
LIB_DIRS= -L/usr/X11R6/lib -L${JLOCAL}/lib
LIBS    = -lraf -lXm -lXp -lXt -lXext -lX11 -lnetcdf -lm
ANONFTP = /net/ftp/pub/archive/RAF-src/bin.RedHat9.0


CFLAGS=	-g ${INCLUDES} ${DEFINES}

SRCS=	ncav.c globals.c init.c timeseg.c sort.c search.c \
	netcdf.c cb_main.c cb_time.c cb_pause.c Xwin.c error.c

HDRS=	define.h

OBJS=	ncav.o globals.o init.o timeseg.o sort.o search.o \
	netcdf.o cb_main.o cb_time.o cb_pause.o Xwin.o error.o

PROG=	ncav

.c.o:
	${CC} ${CFLAGS} -c $*.c

${PROG}: ${OBJS}
	${CC} ${CFLAGS} ${LIB_DIRS} ${OBJS} ${LIBS} -o $@

install: ${PROG}
	cp ${PROG} ${BINDIR}
	cp ${PROG}.man ${MANDIR}/${PROG}.1
	cp ${PROG} $(ANONFTP)
	cp ${PROG}.man ${ANONFTP}

release:
	tar cFFf - . | (cd ${RELEASE}; tar xfBp -)
	sed s/-g/-O2/g Makefile > ${RELEASE}/Makefile
	rm -f ${RELEASE}/LOCK.make

clean:
	rm -f ${OBJS} ${PROG}

print:
	enscript -2Gr -b${PROG} ${HDRS} ${SRCS}

${OBJS}:	define.h
ncav.o:		fbr.h
netcdf.o:	${LOCAL}/include/netcdf.h
