#
# Makefile for Variable DataBase API
#
LIBDIR=		${JLOCAL}/lib
MANDIR=		${JLOCAL}/man/man3
INCLUDEDIR=	${JLOCAL}/include
CC=		gcc

LIBNAME=	VarDB
HEADER=		vardb.h

INCLUDES=	-I. -I${INCLUDEDIR}
DEFINES=
CFLAGS=		-g -O2 ${INCLUDES} ${DEFINES}

OBJS=	init.o get.o set.o save.o merge.o sort.o category.o

SRCS=	init.c get.c set.c save.c merge.c sort.c category.c

HDRS=	vardb.h

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
	install -Cp --mode=664 lib${LIBNAME}.a ${LIBDIR}
	install -Cp --mode=444 ${HEADER} ${INCLUDEDIR}
	install -Cp --mode=664 vardb.3 ${MANDIR}

clean:
	rm -f *.o *.a *.ln
