#
# Makefile for Variable DataBase API
#
DESTDIR=
LIBDIR=		${JLOCAL}/lib
LINTDIR=	${JLOCAL}/lib/lint
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
#	${CC} -pg ${CFLAGS} -c $*.c
#	mv $*.o profiled/$*.o
	${CC} ${CFLAGS} -c $*.c

all: lib${LIBNAME}.a

${OBJS}: ${HDRS}

lib${LIBNAME}.a: ${OBJS}
	rm -f lib${LIBNAME}.a
	ar cr lib${LIBNAME}.a ${OBJS}
#	@ranlib lib${LIBNAME}.a

lib${LIBNAME}_p.a: ${OBJS}
	rm -f lib${LIBNAME}_p.a
	cd profiled; ar cr ../lib${LIBNAME}_p.a ${OBJS}
#	@ranlib ../lib${LIBNAME}_p.a

lint:
	lint -C${LIBNAME} ${INCLUDES} *.c

install: all
	cp -p lib${LIBNAME}.a ${DESTDIR}/${LIBDIR}
#	@ranlib ${DESTDIR}/${LIBDIR}/lib${LIBNAME}.a
#	cp -p lib${LIBNAME}_p.a ${DESTDIR}/${LIBDIR}
#	@ranlib ${DESTDIR}/${LIBDIR}/lib${LIBNAME}_p.a
	cp -p vardb.3 ${DESTDIR}/${MANDIR}
	cp -p ${HEADER} ${DESTDIR}/${INCLUDEDIR}

clean:
	rm -f *.o *.a *.ln
