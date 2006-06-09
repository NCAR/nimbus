DESTDIR=
NIMBUS=		../../..
LIBDIR=		${NIMBUS}/lib

LIBNAME=	user

INCLUDES=	-I${NIMBUS}/include -I${JLOCAL}/include -I${LOCAL}/include
INCLUDES=	-I${NIMBUS}/include -I${JLOCAL}/include
DEFINES=
CFLAGS=		${COPS} ${INCLUDES} ${DEFINES}

OBJS=	bits.o chemcal.o circles.o climet.o co2c.o conccn.o cvi.o\
	efield.o geopot.o licor.o licor_wyo.o masp.o ndvi.o ophir3.o\
	prcltrk1.o prcltrk2.o raz.o rdma.o rlwp.o sail.o spefm.o\
	xpsfdc.o IDEAS2.o ACEASIA.o DYCOMS.o cloudbase.o icing.o\
	ultrafine.o RICO.o nclh.o

SRCS=	bits.c chemcal.c circles.c climet.c co2c.c conccn.c cvi.c\
	efield.c geopot.c licor.c licor_wyo.c masp.c ndvi.c ophir3.c\
	prcltrk1.c prcltrk2.c raz.c rdma.c rlwp.c sail.c spefm.c\
	xpsfdc.c IDEAS2.c ACEASIA.c DYCOMS.c cloudbase.c icing.c\
	ultrafine.c RICO.c nclh.c

HDRS=	${NIMBUS}/include/nimbus.h ${NIMBUS}/include/amlib.h \
	${JLOCAL}/include/constants.h ${JLOCAL}/include/header.h

.SUFFIXES: .o .h .c

.c.o:
#	${CC} -pg ${CFLAGS} -c $*.c
#	mv $*.o profiled/$*.o
	${CC} ${CFLAGS} -c $*.c

all: ${LIBDIR}/lib${LIBNAME}.a ${LIBDIR}/lib${LIBNAME}_p.a

${OBJS}: ${HDRS}

${LIBDIR}/lib${LIBNAME}.a: ${OBJS}
	rm -f ${LIBDIR}/lib${LIBNAME}.a
	ar cq ${LIBDIR}/lib${LIBNAME}.a ${OBJS}

${LIBDIR}/lib${LIBNAME}_p.a: ${OBJS}
#	rm -f ${LIBDIR}/lib${LIBNAME}_p.a
#	cd profiled; ar cq ${LIBDIR}/lib${LIBNAME}_p.a ${OBJS}

install: all
#	cp lib${LIBNAME}.a ${DESTDIR}/${LIBDIR}
#	cp lib${LIBNAME}_p.a ${DESTDIR}/${LIBDIR}

clean:
	rm -f *.o *.a *.ln profiled/*.o
