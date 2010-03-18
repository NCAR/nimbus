#
#    Makefile for dnload
#
DESTDIR=	${JLOCAL}
BINDIR=		${DESTDIR}/bin
MANDIR=		${DESTDIR}/man/manl
INCDIR=		${DESTDIR}/include

LIBS=		-lraf++ -lnetcdf_c++ -lnetcdf
INCLUDES=	-I${INCDIR} -I/usr/include/netcdf
LIBDIR=		-L${JLOCAL}/lib

CC=		g++
CFLAGS=		-Wall -g -O2 ${INCLUDES}

SRCS=	rdmap.cc rdma_mat.cc rdma_inv.cc
PROG=	rdmap

${PROG}: ${SRCS} ${JLOCAL}/lib/libraf++.a
	${CC} ${CFLAGS} ${SRCS} ${LIBDIR} ${LIBS} -o $@

install: ${PROG}
	cp ${PROG} ${BINDIR}

clean:
	rm -f core ${PROG}
