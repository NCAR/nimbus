#
# mcrtd C++ class makefile
#
I2D=	../include
O2D=	../obj

INCLUDES=	-I${I2D} -I${JLOCAL}/include
DEFINES=
CXXFLAGS=	-g ${INCLUDES} ${DEFINES}

OBJS=	CanvasWindow.o DataBuffer.o XColorMap.o
SRCS=	CanvasWindow.cc DataBuffer.cc XColorMap.cc

HDRS=	${I2D}/mcrtd.h

all: ${OBJS}


${OBJS}: ${HDRS}

CanvasWindow.o:		${I2D}/CanvasWindow.h ${JLOCAL}/include/Window.h
DataBuffer.o:		${I2D}/DataBuffer.h ${JLOCAL}/include/hdrAPI.h
XColorMap.o:		${I2D}/XColorMap.h

clean:
	${RM} *.o
