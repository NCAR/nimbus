#
#    Makefile for dnload
#
DESTDIR=	${JLOCAL}
BINDIR=		${DESTDIR}/bin
MANDIR=		${DESTDIR}/man/manl
INCDIR=		${DESTDIR}/include

LIBS=		-L${JLOCAL}/lib -lraf++
INCLUDES=	-I$(INCDIR)

CC=		g++
CFLAGS=		${INCLUDES}

SRCS=	dnload.cc
PROG=	dnload

.c.o:
	${CC} ${CFLAGS} -c $*.c

${PROG}: dnload.cc ${JLOCAL}/lib/libraf++.a
	${CC} ${CFLAGS} dnload.cc ${LIBS} -o $@

install: ${PROG}
	cp ${PROG} ${BINDIR}

clean:
	rm -f core ${PROG}

print:
	enscript -2Gr -bdnload ${HDRS} ${SRCS}
