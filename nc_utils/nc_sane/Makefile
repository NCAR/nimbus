#
#    Makefile for netCDF merge program
#
DESTDIR=	${JLOCAL}/
BINDIR=		bin
ANONFTP=	/net/ftp/pub/archive/RAF-src/bin.RedHat9.0
MANDIR=		man/man1
LIBS=		-L${JLOCAL}/lib -lnetcdf
INCLUDES=	-I${JLOCAL}/include
CXXFLAGS=	-Wall -g ${INCLUDES}

SRCS=	nc_sane.cc
HDRS=	
OBJS=	nc_sane.o
PROG=	nc_sane

${PROG}: ${OBJS}
	${CXX} ${CXXFLAGS} ${OBJS} ${LIBS} -o $@

install: ${PROG}
	test -d ${DESTDIR}${BINDIR} || mkdir ${DESTDIR}${BINDIR}
	cp ${PROG} ${DESTDIR}${BINDIR}
#	test -d ${DESTDIR}${MANDIR} || mkdir ${DESTDIR}${MANDIR}
#	cp ${PROG}.man ${DESTDIR}${MANDIR}/${PROG}.1

publish: $(PROG)
	cp ${PROG} $(ANONFTP)
#	cp ${PROG}.man ${ANONFTP}

clean:
	rm -f ${OBJS} ${PROG}
