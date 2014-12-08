#
#    Makefile for vardb utilities
#
BINDIR=		${JLOCAL}/bin
MANDIR=		${JLOCAL}/man/manl
LINTLIBS=	${LIBS}
INCLUDES=	-I${JLOCAL}/include

LIBS=	-L${JLOCAL}/lib -lVarDB -lxerces-c
CPPFLAGS=	-g ${INCLUDES} 
LDLIBS=${LIBS}

SRCS=	vdbdump.cc
OBJS=	vdbdump.o
HDRS=	${JLOCAL}/include/raf/vardb.h
PROG=	vdbdump

${PROG}: ${SRCS} ${HDRS}


install: ${PROG}
	cp ${PROG} ${BINDIR}

clean:
	rm -f ${PROG} ${OBJS}

lint:
	lint ${INCLUDES} ${SRCS} ${LINTLIBS}

print:
	enscript -2Gr -b${PROG} ${HDRS} ${SRCS}
