DESTDIR=	
NIMBUS=		../../..
LIBDIR=		${NIMBUS}/lib

LIBNAME=	std

INCLUDES=	-I${NIMBUS}/include -I${JLOCAL}/include -I${LOCAL}/include
INCLUDES=	-I${NIMBUS}/include -I${JLOCAL}/include
DEFINES=
CFLAGS=		${COPS} ${INCLUDES} ${DEFINES}

OBJS=	200x.o 200y.o 260x.o akdf.o akrd.o altc.o angcr.o asas.o atb.o atf.o\
	atfh.o atkp.o atrf.o boomln.o cnctr.o con2c.o con2p.o conc2d.o\
	deni.o diff.o dpc.o dpcrc.o dprho.o esubt.o f300.o flux.o fssp.o gpsc.o\
	gsf.o gust.o ias.o initAC.o inlet.o ins.o irc.o irs.o kinglwc.o\
	lwcc.o mr.o mrla.o nevzorov.o palt.o plwcc.o pms1d.o pms2d.o psbc.o\
	psfc.o psfdc.o psurf.o pswc.o qcbc.o qcfc.o qcrc.o qcwc.o radc.o\
	raltm.o ratio.o reference.o reff.o rhodt.o rhola.o rhomr.o rhrho.o rhum.o\
	solang.o sphum.o ssdf.o ssrd.o tas.o tasf.o tash.o tasr.o tasw.o\
	tasx.o teo3c.o theta.o thetae.o thetav.o thf.o tvir.o wd.o ws.o\
	xmac2.o xmach2.o ggps.o qcfrc.o

PMS1OBJ=200x.o 200y.o 260x.o asas.o fssp.o f300.o pms2d.o

SRCS=	200x.c 200y.c 260x.c akdf.c akrd.c altc.c angcr.c asas.c atb.c atf.c\
	atfh.c atkp.c atrf.c boomln.c cnctr.c con2c.c con2p.c conc2d.c\
	deni.c diff.c dpc.c dpcrc.c dprho.c esubt.c f300.c flux.c fssp.c gpsc.c\
	gsf.c gust.c ias.c initAC.c inlet.c ins.c irc.c irs.c kinglwc.c\
	lwcc.c mr.c mrla.c nevzorov.c palt.c plwcc.c pms1d.c pms2d.c psbc.c\
	psfc.c psfdc.c psurf.c pswc.c qcbc.c qcfc.c qcrc.c qcwc.c radc.c\
	raltm.c ratio.c reference.c reff.c rhodt.c rhola.c rhomr.c rhrho.c rhum.c\
	solang.c sphum.c ssdf.c ssrd.c tas.c tasf.c tash.c tasr.c tasw.c\
	tasx.c teo3c.c theta.c thetae.c thetav.c thf.c tvir.c wd.c ws.c\
	xmac2.c xmach2.c ggps.c qcfrc.c

HDRS=	${NIMBUS}/include/nimbus.h ${NIMBUS}/include/amlib.h \
	${JLOCAL}/include/constants.h ${JLOCAL}/include/header.h

.SUFFIXES: .o .h .c

.c.o:
#	${CC} -pg ${CFLAGS} -c $*.c
#	mv $*.o profiled/$*.o
	${CC} ${CFLAGS} -c $*.c

all: ${LIBDIR}/lib${LIBNAME}.a ${LIBDIR}/lib${LIBNAME}_p.a

${OBJS}:	${HDRS}
${PMS1OBJ}:	pms1d_cv
irs.o ins.o gust.o wd.o ws.o:	${NIMBUS}/include/gust.h

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
