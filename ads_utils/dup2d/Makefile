#
#    Makefile for dnload
#
DESTDIR=	/home/local
BINDIR=		${DESTDIR}/bin
MANDIR=		${DESTDIR}/man/manl
INCDIR=		${DESTDIR}/include

LIBS=		-lraf
INCLUDES=	-I$(INCDIR)

CC=		gcc
CFLAGS=		${INCLUDES}

SRCS=	dup2d.c
PROG=	dup2d

.c.o:
	${CC} ${CFLAGS} -c $*.c

${PROG}: dup2d.c
	${CC} ${CFLAGS} dup2d.c ${LIBS} -o $@

install: ${PROG}
	cp ${PROG} ${BINDIR}

clean:
	rm -f core ${PROGS}

print:
	enscript -2Gr -bHdrUtils ${HDRS} ${SRCS}
