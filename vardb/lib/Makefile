#
# Makefile for Variable DataBase API
#
LIBDIR=		${JLOCAL}/lib
MANDIR=		${JLOCAL}/man/man3
INCLUDEDIR=	${JLOCAL}/include
CC=		g++

LIBNAME=	VarDB
HEADER=		vardb.h vardb.hh

INCLUDES=	-I${INCLUDEDIR}
DEFINES=
CFLAGS=		-g -O2 ${INCLUDES} ${DEFINES}

COBJS=		init.o get.o set.o save.o merge.o sort.o category.o std_name.o
CPPOBJS=	vardb.o

SRCS=		init.c get.c set.c save.c merge.c sort.c category.c std_name.c
CPPSRCS=	vardb.cc

CHDR=	vardb.h
CPPHDR=	vardb.hh

.SUFFIXES: .o .h .c

.c.o:
	${CC} ${CFLAGS} -c $*.c

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
