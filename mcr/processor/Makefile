#
#    Makefile for MCR ascii dump
#
DESTDIR=	/home/local
BINDIR=		${DESTDIR}/bin
MANDIR=		${DESTDIR}/man/manl
INCDIR=		${DESTDIR}/include
INCDIR=		/home/local/include

LIBS=		
INCLUDES=	-I$(INCDIR)

CC=		gcc
CFLAGS=		-g ${INCLUDES}

SRCS=	ascii.c
PROG=	ascii

.c.o:
	${CC} ${CFLAGS} -c $*.c

${PROG}: ascii.c
	${CC} ${CFLAGS} ascii.c ${LIBS} -o $@

install: ${PROG}
	cp ${PROG} ${BINDIR}

clean:
	rm -f core ${PROG}

print:
	enscript -2Gr -bascii ${HDRS} ${SRCS}
