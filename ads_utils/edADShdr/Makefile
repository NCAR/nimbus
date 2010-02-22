#
#    Makefile for edADShdr program
#
CC=		gcc
BINDIR=		${JLOCAL}/bin
MANDIR=		${JLOCAL}/man/manl
LINTLIBS=	${LIBS}
INCLUDES=	-I${JLOCAL}/include
CFLAGS=		-O2 ${INCLUDES}

# Solaris
LIBS=		-lraf -lXm -lXt -lX11

#Linux
LIBS=		-L${JLOCAL}/lib -lraf -lXm -lXt -lX11 -lXp -lXext

SRCS=	edADShdr.c init.c Xwin.c ccb.c
HDRS=	define.h
OBJS=	edADShdr.o init.o Xwin.o ccb.o
PROG=	edADShdr

.c.o:
	${CC} ${CFLAGS} -c $*.c

${PROG}: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} ${LIBS} -o $@

install: ${PROG}
	cp ${PROG} ${BINDIR}
#	install -m 444 ${PROG}.l ${MANDIR}

clean:
	rm -f ${OBJS} ${PROG}

lint:
	lint ${INCLUDES} ${SRCS} ${LINTLIBS}

print:
	enscript -2Gr -b${PROG} ${HDRS} ${SRCS}

#${OBJS}:	${HDRS}
