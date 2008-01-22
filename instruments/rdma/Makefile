#
#    Makefile for dnload
#
DESTDIR=	${JLOCAL}
BINDIR=		${DESTDIR}/bin
MANDIR=		${DESTDIR}/man/manl
INCDIR=		${DESTDIR}/include

LIBS=		-lraf++ -Wl,-Bstatic -lnetcdf_c++ -lnetcdf -Wl,-Bdynamic
INCLUDES=	-I$(INCDIR)
LIBDIR=		-L$(LOCAL)/lib -L$(JLOCAL)/lib
LIBDIR=		-L$(JLOCAL)/lib

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
