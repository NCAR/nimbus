#
#    Makefile for X Header Dump (xdh)
#
CC=		gcc
BINDIR=		${JLOCAL}/bin
MANDIR=		${JLOCAL}/man/manl
LINTLIBS=	${LIBS}
INCLUDES=	-I/usr/X11R6/include -I${JLOCAL}/include

# Solaris
CFLAGS=		-DSVR4 -Wall -g -O2 ${INCLUDES}
LIBS=		-lraf -lhdr_api -lXm -lXt -lX11

# Linux
CFLAGS=		-Wall -g -O2 ${INCLUDES}
LIBS=		-L/${JLOCAL}/lib -L/usr/X11R6/lib -lraf -lhdr_api -lXm -lXt -lX11


SRCS=	xhd.c Xwin.c ccb.c

OBJS=	xhd.o Xwin.o ccb.o

HDRS=	${JLOCAL}/include/raf/header.h

PROG=	xhd

.c.o:
	${CC} ${CFLAGS} -c $*.c

${PROG}: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} ${LIBS} -o $@

install: ${PROG}
	test -d ${BINDIR} || mkdir ${BINDIR}
	cp ${PROG} ${BINDIR}

clean:
	rm -f ${OBJS} ${PROG}

lint:
	lint ${INCLUDES} ${SRCS} ${LINTLIBS}

print:
	enscript -2Gr -b${PROG} ${HDRS} ${SRCS}

${OBJS}:	${HDRS}
${HWOBJS}:	hardwire.h
