#
# mcrtd C++ class makefile
#
I2D=	../include
O2D=	../obj
LIBRAF=	${JLOCAL}/include

INCLUDES=	-I${I2D} -I../../include -I${JLOCAL}/include
DEFINES=
CXXFLAGS=	-g -O ${INCLUDES} ${DEFINES}

OBJS=	Netcdf.o DataBuffer.o OutFile.o
SRCS=	Netcdf.cc DataBuffer.cc OutFile.cc

#HDRS=	${I2D}/mcr.h

all: ${OBJS}


${OBJS}: ${HDRS}

DataBuffer.o:		${I2D}/DataBuffer.h ${I2D}/Netcdf.h ${LIBRAF}/hdrAPI.h
Netcdf.o:		${I2D}/Netcdf.h
OutFile.o:		${I2D}/OutFile.h ${I2D}/Netcdf.h

clean:
	${RM} *.o
