#
# Makefile for Variable DataBase API
#
LIBDIR=		${JLOCAL}/lib
MANDIR=		${JLOCAL}/man/man3
INCLUDEDIR=	${JLOCAL}/include
CC=		g++

LIBNAME=	VarDB
HEADER=		vardb.h

INCLUDES=	-I${INCLUDEDIR}
DEFINES=
CFLAGS=		-g -O2 ${INCLUDES} ${DEFINES}

OBJS=	Vardb.o

SRCS=	Vardb.c

HDRS=	Vardb.h

.SUFFIXES: .o .h .c

.c.o:
	${CC} ${CFLAGS} -c $*.c

all: lib${LIBNAME}.a

${OBJS}: ${HDRS}

lib${LIBNAME}.a: ${OBJS}
	rm -f lib${LIBNAME}.a
	ar cr lib${LIBNAME}.a ${OBJS}

lib${LIBNAME}_p.a: ${OBJS}
	rm -f lib${LIBNAME}_p.a
	cd profiled; ar cr ../lib${LIBNAME}_p.a ${OBJS}

install: all
	install -p --mode=664 lib${LIBNAME}.a ${LIBDIR}
	install -p --mode=444 ${HEADER} ${INCLUDEDIR}/raf
	install -p --mode=664 vardb.3 ${MANDIR}

clean:
	rm -f *.o *.a *.ln

test:
	g++ test.cc -L . -lVarDB -lxerces-c -o go;./go
