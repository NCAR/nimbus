#
#    Makefile for ASCII to netCDF translators.
#
CC=		gcc
BINDIR=		${LOCAL}/bin
MANDIR=		${LOCAL}/man/man1
INCLUDES=	-I$(JLOCAL)/include
CFLAGS=		-Wall -g ${INCLUDES} -DVARDB

# Solaris
HOST=	mead
LIBS=		-L/jnet/solaris/lib -lVarDB -lnetcdf
ANONFTP=	/jnet/ftp/pub/src/bin.Solaris2.9

# Linux
HOST=	syrah
LIBS=		-L${JLOCAL}/lib -lVarDB -lraf++ -lraf -lnetcdf 
ANONFTP=	/net/ftp/pub/archive/RAF-src/bin.RedHat9.0

SRCS=   asc2cdf.c plain.c nasaAmes.c nasaLangley.c getmem.c
OBJS=   asc2cdf.o plain.o nasaAmes.o nasaLangley.o getmem.o
HDRS=	define.h

PROG=	asc2cdf

.c.o:
	${CC} ${CFLAGS} -c $*.c

${PROG}: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} ${LIBS} -o $@

install: ${PROG}
	cp ${PROG} ${BINDIR}
	cp ${PROG}.man ${MANDIR}/${PROG}.1
	scp ${PROG} ${HOST}:${BINDIR}
	scp ${PROG}.man ${HOST}:${MANDIR}/${PROG}.1
	cp ${PROG} $(ANONFTP)
	cp ${PROG}.man ${ANONFTP}

clean:
	rm -f ${OBJS} ${PROG} core

${OBJS}: ${HDRS}
