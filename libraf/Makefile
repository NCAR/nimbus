#
# Outside users, you may remove util.c cio.c if they don't compile.
#
DESTDIR=
LIBDIR=		${JLOCAL}/lib
MANDIR=		${JLOCAL}/man/man3
INCLUDEDIR=	${JLOCAL}/include
CC=		gcc
FF=		f77

LIBNAME=	raf

INCLUDES=	-I${JLOCAL}/include
CFLAGS=		-g -O2 ${INCLUDES} 
FFLAGS=		-O

OBJS=	strupr.o tapeIO.o getmem.o Xerror.o Xquery.o Xfile.o Xwarn.o \
	util.o raf_queue.o chost.o date.o portable.o ac.o get_ac_name.o \
	nc_time.o
# cio.o flote.o sendhc.o sendpr.o 

SRCS=	strupr.c tapeIO.c getmem.c Xerror.c Xquery.c Xfile.c Xwarn.c \
	util.c raf_queue.c chost.c date.c portable.c ac.c get_ac_name.c \
	nc_time.c
# cio.c flote.f sendhc.f sendpr.f 

HDRS=	ac.h

.SUFFIXES: .o .h .c .cc

.c.o:
	${CC} ${CFLAGS} -c $*.c -o $*.o

.cc.o:
	${CC} ${CFLAGS} -c $*.cc

.f.o:
	${FF} ${FFLAGS} -c $*.f

all: lib${LIBNAME}.a

ac.o: ac.h
nc_time.o: /usr/include/netcdf.h

lib${LIBNAME}.a: ${OBJS}
	rm -f lib${LIBNAME}.a
	ar cr lib${LIBNAME}.a ${OBJS}

install: all
	install -p --mode=664 lib${LIBNAME}.a ${DESTDIR}/${LIBDIR}
	install -p --mode=444 *.h ${DESTDIR}/${INCLUDEDIR}/raf

include:
	install -c -m 0444 ${LIBNAME}.h ${DESTDIR}/${INCLUDEDIR}

clean:
	rm -f *.o *.a *.ln
