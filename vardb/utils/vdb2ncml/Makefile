#
#    Makefile for vardb utilities
#
CC=		g++
BINDIR=		${JLOCAL}/bin
MANDIR=		${JLOCAL}/man/manl
LINTLIBS=	${LIBS}
INCLUDES=	-I${JLOCAL}/include -I/usr/include/netcdf
CFLAGS=		-g ${INCLUDES}

LIBS=	-L${JLOCAL}/lib -lVarDB -lpms -lraf -lnetcdf -lhdf5_hl -lhdf5

SRCS=	vdb2xml.cc
OBJS=	vdb2xml.o
HDRS=	${JLOCAL}/include/raf/vardb.h
PROG=	vdb2ncml

.c.o:
	${CC} ${CFLAGS} -c $*.cc

${PROG}: ${SRCS} ${HDRS}
	${CC} ${CFLAGS} ${SRCS} ${LIBS} -o $@

install: ${PROG}
	cp ${PROG} ${BINDIR}

clean:
	rm -f ${PROG} ${OBJS}

lint:
	lint ${INCLUDES} ${SRCS} ${LINTLIBS}

print:
	enscript -2Gr -b${PROG} ${HDRS} ${SRCS}
