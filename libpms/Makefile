#
# Makefile for Header API
#
LIBDIR=		${JLOCAL}/lib
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
	${CC} ${CFLAGS} -c $*.c

all: lib${LIBNAME}.a

${OBJS}: ${HDRS}

lib${LIBNAME}.a: ${OBJS}
	rm -f lib${LIBNAME}.a
	ar cr lib${LIBNAME}.a ${OBJS}

install: all
	install -Cp --mode=664 lib${LIBNAME}.a ${LIBDIR}
	install -Cp --mode=444 ${HEADER} ${JLOCAL}/include

clean:
	rm -f *.o *.a *.ln
