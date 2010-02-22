#
#    Makefile for dnload
#
DESTDIR=	${JLOCAL}
BINDIR=		${DESTDIR}/bin
MANDIR=		${DESTDIR}/man/manl
INCDIR=		${DESTDIR}/include
ANONFTP=	/net/ftp/pub/archive/RAF-src/bin.RHEL5


LIBS=		-Wl,-Bstatic -lnetcdf_c++ -lnetcdf -Wl,-Bdynamic
INCLUDES=	-I$(INCDIR) -I/usr/include/netcdf

CC=		g++
CFLAGS=		${INCLUDES}

SRCS=	ncReorder.cc
PROG=	ncReorder

.c.o:
	${CC} ${CFLAGS} -c $*.c

${PROG}: ncReorder.cc
	${CC} ${CFLAGS} ncReorder.cc ${LIBS} -o $@

install: ${PROG}
	test -d ${BINDIR} || mkdir ${BINDIR}
	cp ${PROG} ${BINDIR}

publish: $(PROG)
	cp ${PROG} $(ANONFTP)

clean:
	rm -f core ${PROG}

print:
	enscript -2Gr -bncReorder ${HDRS} ${SRCS}
