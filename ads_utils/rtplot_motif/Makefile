#
#    Makefile for rtplot
#
CC	= gcc
BIN	= ${JLOCAL}/bin
RELEASE = ${JLOCAL}/rtplot
NOBJ	= ${NIMBUS}/src/filter/obj
MAN1	= ${JLOCAL}/man/man1
LIBS	= -lraf++ -lXm -lXt -lX11 -lxlate -lraf -lm
LINTLIBS= -lm
INCLUDES= -I${JLOCAL}/include -I${NIMBUS}/include
CXXFLAGS= -g ${INCLUDES}
CFLAGS= -g ${INCLUDES}
LIB_DIRS= -L${JLOCAL}/lib -L/usr/X11R6/lib

SRCS=	rtplot.cc global.cc init.cc autoscale.cc dataIO.cc CanvasWindow.cc\
	hdr_decode.cc ccb.cc Xplot.cc misc.cc memalloc.cc Colors.cc\
	amlibfn.cc rtloop.cc rec_decode.cc misc1.c

OBJS=	rtplot.o global.o init.o autoscale.o dataIO.o CanvasWindow.o\
	hdr_decode.o ccb.o Xplot.o misc.o memalloc.o amlibfn.o Colors.o\
	rtloop.o rec_decode.o misc1.o

OOBJ=	${NOBJ}/calco.o ${NOBJ}/sort.o ${NOBJ}/search.o ${NOBJ}/textfile.o ${NOBJ}/popen.o

HDRS=	define.h extern.h
PROG=	rtplot

${PROG}: ${OBJS}
	${CXX} ${CXXFLAGS} -L${NIMBUS}/lib ${LIB_DIRS} ${OBJS} ${OOBJ} ${LIBS} -o $@

release:
	tar cFFf - . | (cd ${RELEASE}; tar xfBp -)
	sed s/-g/-O2/g Makefile > ${RELEASE}/Makefile
	tar cFFf - . | (cd /home/linux/rtplot; tar xfBp -)
	sed s/-g/-O/g Makefile > /home/linux/rtplot/Makefile
	rm -f ${RELEASE}/LOCK.make

install: ${PROG}
	cp ${PROG} ${BIN}
	cp ${PROG}.1 ${MAN1}

clean:
	rm -f core ${OBJS} ${PROG}

lint:
	lint ${INCLUDES} ${SRCS} ${LINTLIBS}

print:
	enscript -2Gr -b${PROG} ${HDRS} ${SRCS}

${OBJS}:	${HDRS}
amlibfn.o:	${NIMBUS}/include/xlatefns.h
rtplot.o:	${HDRS} fbr.h
