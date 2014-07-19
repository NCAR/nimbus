#
#    Makefile for flt_time.  Program dumps take-off and landing times from
#    a netcDF file.
#
DESTDIR=	${JLOCAL}/
BINDIR=		bin
ANONFTP=	/net/ftp/pub/archive/RAF-src/bin.RHEL6
MANDIR=		man/man1
LDIR=		${JLOCAL}/netcdf-4.3.2/lib
LIBS=		-lnetcdf_c++ -lnetcdf -lhdf5 -lhdf5_hl
INCLUDES=	-I${JLOCAL}/include
CXXFLAGS=	-Wall -g ${INCLUDES}

SRCS=	flt_time.cc
HDRS=	
OBJS=	flt_time.o
PROG=	flt_time

${PROG}: ${OBJS}
	${CXX} ${CXXFLAGS} ${OBJS} ${LIBS} -o $@

install: ${PROG}
	test -d ${DESTDIR}${BINDIR} || mkdir ${DESTDIR}${BINDIR}
	cp ${PROG} ${DESTDIR}${BINDIR}
#	test -d ${DESTDIR}${MANDIR} || mkdir ${DESTDIR}${MANDIR}
#	cp ${PROG}.man ${DESTDIR}${MANDIR}/${PROG}.1

publish: $(PROG)
	cp ${PROG} $(ANONFTP)
	cp ${PROG}.man ${ANONFTP}

clean:
	rm -f ${OBJS} ${PROG}
