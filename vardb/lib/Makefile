#
# Makefile for Variable DataBase API
#
LIBDIR=		${JLOCAL}/lib
MANDIR=		${JLOCAL}/man/man3
INCLUDEDIR=	${JLOCAL}/include

LIBNAME=	VarDB
HEADER=		vardb.h vardb.hh

INCLUDES=	-I${INCLUDEDIR}
DEFINES=
CXXFLAGS=	-Wall -g -O2 ${INCLUDES} ${DEFINES}

COBJS=	init.o get.o set.o save.o merge.o sort.o category.o std_name.o
CPPOBJS=vardb.o

SRCS=	init.cc get.cc set.cc save.cc merge.cc sort.cc category.cc std_name.cc
CPPSRCS=vardb.cc

CHDR=	vardb.h
CPPHDR=	vardb.hh

all: lib${LIBNAME}.a

${COBJS}: ${CHDR}
${CPPOBJS}: ${CPPHDR}

lib${LIBNAME}.a: ${COBJS} ${CPPOBJS}
	rm -f lib${LIBNAME}.a
	ar cr lib${LIBNAME}.a ${COBJS} ${CPPOBJS}

lib${LIBNAME}_p.a: ${COBJS} ${CPPOBJS}
	rm -f lib${LIBNAME}_p.a
	cd profiled; ar cr ../lib${LIBNAME}_p.a ${COBJS} ${CPPOBJS}

install: all
	install -p --mode=664 lib${LIBNAME}.a ${LIBDIR}
	install -p --mode=444 ${HEADER} ${INCLUDEDIR}/raf
	install -p --mode=664 vardb.3 ${MANDIR}

clean:
	rm -f *.o *.a *.ln
