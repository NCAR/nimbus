#
#    Makefile for netCDF merge program
#
DESTDIR=	${JLOCAL}/
BINDIR=		bin
ANONFTP=	/net/ftp/pub/archive/RAF-src/bin.RHEL5
MANDIR=		man/man1
LIBS=		-L${JLOCAL}/lib -lnetcdf -lhdf5 -lhdf5_hl
INCLUDES=	-I${JLOCAL}/include
CXXFLAGS=	-Wall -g ${INCLUDES}

SRCS=	ncfillvar.cc
HDRS=	
OBJS=	ncfillvar.o
PROG=	ncfillvar

${PROG}: ${OBJS}
	${CXX} ${CXXFLAGS} ${OBJS} ${LIBS} -o $@

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
