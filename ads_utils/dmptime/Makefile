#
#    Makefile for dmptime
#
DESTDIR=	${JLOCAL}
BINDIR=		${DESTDIR}/bin
MANDIR=		${DESTDIR}/man/manl
INCDIR=		${DESTDIR}/include/raf

LIBS=		-L${JLOCAL}/lib -lraf++ -lz
INCLUDES=	-I$(INCDIR)

CC=		g++
CFLAGS=		${INCLUDES}

SRCS=	dmptime.cc
PROG=	dmptime

.c.o:
	${CC} ${CFLAGS} -c $*.c

${PROG}: dmptime.cc ${JLOCAL}/lib/libraf++.a
	${CC} ${CFLAGS} dmptime.cc ${LIBS} -o $@

install: ${PROG}
	cp ${PROG} ${BINDIR}

clean:
	rm -f core ${PROG}

print:
	enscript -2Gr -bdmptime ${HDRS} ${SRCS}
