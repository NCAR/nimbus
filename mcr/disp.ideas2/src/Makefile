#
# Makefile for mcrtd real-time display
#
BIN	= ${JLOCAL}/bin

I2D	= ../include
C2D	= ../class
O2D	= ../class

INCLUDES= -I${I2D} -I${JLOCAL}/include
#INCLUDES= -I${I2D} -I/usr/openwin/include -I/usr/dt/include
LIB_DIRS= -L/usr/X11R6/lib -L${JLOCAL}/lib

DEFINES	=
CXXFLAGS= -g ${INCLUDES} ${DEFINES}

LIBS	= -lraf++ -lXm -lXt -lX11
#LIBS	= -L${JLOCAL}/lib -L/usr/X11R6/lib -lraf++ -lXm -lXt -lX11

PROG	= mcrtd
HDRS	= ${I2D}/mcrtd.h

SRCS=	mcrtd.cc cb_canvas.cc
OBJS=	mcrtd.o cb_canvas.o


O2DOB=	${O2D}/CanvasWindow.o ${O2D}/DataBuffer.o ${O2D}/XColorMap.o


${PROG}: ${OBJS} classDir
	${CXX} ${CXXFLAGS} ${LIB_DIRS} ${OBJS} ${O2DOB} ${LIBS} -o $@

classDir:
	cd ${O2D}; make

install: ${PROG}
	cp ${PROG} $(BIN)

clean:
	${RM} core ${OBJS} ${PROG}

${OBJS}:	${HDRS}

mcrtd.o:	${I2D}/CanvasWindow.h ${I2D}/DataBuffer.h
