#
#    Makefile for ASCII to netCDF translators.
#
CC=		g++
DESTDIR=
BINDIR=		/jnet/local/bin
ANONFTP=	/net/ftp/pub/archive/RAF-src/bin.RHEL6
MANDIR=		/jnet/local/man/man1
LIBS=		-L${JLOCAL}/lib -lVarDB -lnetcdf -lhdf5 -lhdf5_hl -lraf
LINTLIBS=	${LIBS}
INCLUDES=	-I$(JLOCAL)/include 
CFLAGS=		-Wall -g ${INCLUDES} -DVARDB

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

clean:
	rm -f ${OBJS} ${PROG} core

${OBJS}: ${HDRS}
