#
#    Makefile for dnload
#
DESTDIR=	${JLOCAL}
BINDIR=		${DESTDIR}/bin
MANDIR=		${DESTDIR}/man/manl
INCDIR=		${DESTDIR}/include
ANONFTP=	/net/ftp/pub/archive/RAF-src/bin.RedHat9.0


LIBS=		-L${JLOCAL}/lib -lnetcdf_c++ -lnetcdf
INCLUDES=	-I$(INCDIR)

CC=		g++
CFLAGS=		${INCLUDES}

SRCS=	ncReorder.cc
PROG=	ncReorder

.c.o:
	${CC} ${CFLAGS} -c $*.c

${PROG}: ncReorder.cc ${JLOCAL}/lib/libnetcdf_c++.a
	${CC} ${CFLAGS} ncReorder.cc ${LIBS} -o $@

install: ${PROG}
	cp ${PROG} ${BINDIR}
	cp ${PROG} $(ANONFTP)

clean:
	rm -f core ${PROG}

print:
	enscript -2Gr -bncReorder ${HDRS} ${SRCS}
