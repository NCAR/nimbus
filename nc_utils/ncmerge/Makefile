#
#    Makefile for netCDF merge program
#
CC=		gcc
DESTDIR=	${JLOCAL}/
BINDIR=		bin
ANONFTP=	/net/ftp/pub/archive/RAF-src/bin.RedHat9.0
MANDIR=		man/man1
LIBS=		-L${JLOCAL}/lib -lnetcdf -lraf
LINTLIBS=	${LIBS}
INCLUDES=	-I${JLOCAL}/include
CFLAGS=		-Wall -g ${INCLUDES}

SRCS=	ncmerge.c
HDRS=	
OBJS=	ncmerge.o
PROG=	ncmerge

.c.o:
	${CC} ${CFLAGS} -c $*.c

${PROG}: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} ${LIBS} -o $@

install: ${PROG}
	test -d ${DESTDIR}${BINDIR} || mkdir ${DESTDIR}${BINDIR}
	cp ${PROG} ${DESTDIR}${BINDIR}
	test -d ${DESTDIR}${MANDIR} || mkdir ${DESTDIR}${MANDIR}
	cp ${PROG}.man ${DESTDIR}${MANDIR}/${PROG}.1

publish: $(PROG)
	cp ${PROG} $(ANONFTP)
	cp ${PROG}.man ${ANONFTP}

clean:
	rm -f ${OBJS} ${PROG}

lint:
	lint ${INCLUDES} ${SRCS} ${LINTLIBS}

print:
	enscript -2Gr -b${PROG} ${HDRS} ${SRCS}
