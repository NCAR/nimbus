#
#    Makefile for vardb utilities
#
CC=		g++
BINDIR=		${JLOCAL}/bin
MANDIR=		${JLOCAL}/man/manl
LINTLIBS=	${LIBS}
INCLUDES=	-I${JLOCAL}/include
CFLAGS=		${INCLUDES}

LIBS=	-L${JLOCAL}/lib -lVarDB -lraf

SRCS=	vdbdump.cc
OBJS=	vdbdump.o
HDRS=	define.h vardb.h
PROG=	vdbdump

.c.o:
	${CC} ${CFLAGS} -c $*.cc

${PROG}:
	${CC} ${CFLAGS} ${SRCS} ${LIBS} -o $@

install: ${PROG}
	cp ${PROG} ${BINDIR}

clean:
	rm -f ${PROG} ${OBJS}

lint:
	lint ${INCLUDES} ${SRCS} ${LINTLIBS}

print:
	enscript -2Gr -b${PROG} ${HDRS} ${SRCS}
