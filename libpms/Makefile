#
# Makefile for Header API
#
DESTDIR=
LIBDIR=		${JLOCAL}/lib
LINTDIR=	${JLOCAL}/lib/lint
MANDIR=		${JLOCAL}/man/man3
INCLUDEDIR=	${JLOCAL}/include
CC=			gcc

LIBNAME=	pms
HEADER=		pms.h

INCLUDES=	-I. -I${INCLUDEDIR}
DEFINES=	
CFLAGS=		-O2 ${INCLUDES} ${DEFINES}

OBJS=		init.o

SRCS=		init.c

HDRS=		pms.h

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
#	rm -f lib${LIBNAME}_p.a
#	cd profiled; ar cr ../lib${LIBNAME}_p.a ${OBJS}
#	@ranlib lib${LIBNAME}_p.a

lint:
	lint -C${LIBNAME} ${INCLUDES} *.c

install: all
	cp -p lib${LIBNAME}.a ${DESTDIR}/${LIBDIR}
#	cp -p lib${LIBNAME}_p.a ${DESTDIR}/${LIBDIR}
#	-install -f ${DESTDIR}/${MANDIR} ${LIBNAME}.3
	cp -p ${HEADER} ${JLOCAL}/include

clean:
	rm -f *.o *.a *.ln
