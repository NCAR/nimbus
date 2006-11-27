#
#    Makefile for vardb utilities
#
CC=		g++
BINDIR=		${JLOCAL}/bin
MANDIR=		${JLOCAL}/man/manl
LINTLIBS=	${LIBS}
INCLUDES=	-I${JLOCAL}/include
CFLAGS=		${INCLUDES}

LIBS=	-L${JLOCAL}/lib -lVarDB -lraf -lnetcdf

SRCS=	vdb2ncml.cc
OBJS=	vdb2ncml.o
HDRS=	${JLOCAL}/include/vardb.h
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
