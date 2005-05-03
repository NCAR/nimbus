#
#    Makefile for dup.ads program
#
CC=		g++
BINDIR=		${JLOCAL}/bin
INCLUDES=	-I${JLOCAL}/include
CXXFLAGS=	${INCLUDES}

LIBS=		-L${JLOCAL}/lib -lraf++ -lz

SRCS=	dup.ads.cc
OBJS=	dup.ads.o
PROG=	dup.ads

${PROG}: ${OBJS}
	${CC} ${CXXFLAGS} ${OBJS} ${LIBS} -o $@

install: ${PROG}
	cp ${PROG} ${BINDIR}

clean:
	rm -f ${OBJS} ${PROG}
