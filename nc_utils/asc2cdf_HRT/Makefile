#
#    Makefile for ASCII to netCDF translators.
#
CC=		gcc
DESTDIR=
BINDIR=		/home/local/bin
ANONFTP=	/home/users/ftp/pub/src/bin.sol2
RELEASE=	/home/local/nimbus/src/asc2cdf
MANDIR=		/home/local/man/man1
LIBS=		-lVarDB -lnetcdf -lraf
LINTLIBS=	${LIBS}
INCLUDES=	-I$(JLOCAL)/include
CFLAGS=		-Wall -g ${INCLUDES}

SRCS=   asc2cdf.c plain.c nasaAmes.c nasaLangley.c
OBJS=   asc2cdf.o plain.o nasaAmes.o nasaLangley.o
HDRS=	define.h

PROG=	asc2cdf

.c.o:
	${CC} ${CFLAGS} -c $*.c

${PROG}: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} ${LIBS} -o $@

install: ${PROG}
	cp ${PROG} ${DESTDIR}${BINDIR}
	cp ${PROG}.man ${DESTDIR}${MANDIR}/${PROG}.1
	cp ${PROG} $(ANONFTP)
	cp ${PROG}.man ${ANONFTP}


release:
	tar cFFf - . | (cd ${RELEASE}; tar xfBp -)
	sed s/-g/-O/g Makefile > ${RELEASE}/Makefile
	rm -f ${RELEASE}/LOCK.make

clean:
	rm -f ${OBJS} ${PROG} core

${OBJS}: ${HDRS}
