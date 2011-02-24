#
#    Makefile for calculator program
#
CC=		g++
BINDIR=		${JLOCAL}/bin
MANDIR=		${JLOCAL}/man/manl
LINTLIBS=	${LIBS}
INCLUDES=	-I${JLOCAL}/include -I/usr/X11R6/include
CFLAGS=		-Wall -g -O2 ${INCLUDES}

# Solaris
#LIBS=		-lVarDB -lraf -Wl,-Bstatic -lnetcdf -Wl,-Bdynamic -lXm -lXt -lX11

#Linux
LIBS=		-L../lib -L${JLOCAL}/lib -L/usr/X11R6/lib -lVarDB -lraf -lnetcdf -lXm -lXt -lX11 -lXext

SRCS=	vared.c initv.c Xwin.c ccb.c
HDRS=	define.h ${JLOCAL}/include/raf/vardb.h fbr.h
OBJS=	vared.o initv.o Xwin.o ccb.o
PROG=	vared

.c.o:
	${CC} ${CFLAGS} -c $*.c

${PROG}: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} ${LIBS} -o $@

install: ${PROG}
	test -d ${BINDIR} || mkdir -p ${BINDIR}
	cp ${PROG} ${BINDIR}

clean:
	rm -f ${OBJS} ${PROG}

lint:
	lint ${INCLUDES} ${SRCS} ${LINTLIBS}

print:
	enscript -2Gr -b${PROG} ${HDRS} ${SRCS}

${OBJS}:	${HDRS}
