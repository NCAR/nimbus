#
# Makefile for Header API
#
LIBDIR=		${JLOCAL}/lib
MANDIR=		${JLOCAL}/man/man3
INCLUDEDIR=	${JLOCAL}/include
CC=		gcc

LIBNAME=	hdr_api
HEADER=		ctape.h

INCLUDES=	-I${INCLUDEDIR}
DEFINES=	
CFLAGS=		-Wall -g ${INCLUDES} ${DEFINES}

OBJS=	init.o lookup.o getnext.o getvarlist.o gettype.o getitemlen.o	\
	getstartlen.o getlocn.o getrate.o gethdr.o getaddr.o getname.o	\
	getoffset.o getres.o getxvert.o getorder.o getcof.o getlr.o	\
	getdmeaddr.o getitemtype.o getpointer.o getps.o getintf.o	\
	getsn.o getserial.o gethskp.o getgain.o getnbins.o

SRCS=	init.c lookup.c getnext.c getvarlist.c gettype.c getitemlen.c	\
	getstartlen.c getlocn.c getrate.c gethdr.c getaddr.c getname.c	\
	getoffset.c getres.c getxvert.c getorder.c getcof.c getlr.c	\
	getdmeaddr.c getitemtype.c getpointer.c getps.c getintf.c	\
	getsn.c getserial.c gethskp.c getgain.c getnbins.c

HDRS=	ctape.h hdr_api.h ${INCLUDEDIR}/header.h

.SUFFIXES: .o .h .c

.c.o:
	${CC} ${CFLAGS} -c $*.c

all: lib${LIBNAME}.a

${OBJS}: ${HDRS}

lib${LIBNAME}.a: ${OBJS}
	rm -f lib${LIBNAME}.a
	ar cr lib${LIBNAME}.a ${OBJS}

install: all
	test -d ${LIBDIR} || mkdir ${LIBDIR}
	test -d ${INCLUDEDIR} || mkdir ${INCLUDEDIR}
	install -p --mode=664  lib${LIBNAME}.a ${LIBDIR}
	install -p --mode=444  ${HEADER} ${INCLUDEDIR}
	test -d ${MANDIR} || mkdir ${MANDIR}
	install -p --mode=664  ${LIBNAME}.3 ${MANDIR}

clean:
	rm -f *.o *.a *.ln
