#
# Makefile for mcrp MCR processor
#
BIN	= ${JLOCAL}/bin

I2D	= ../include
C2D	= ../class
O2D	= ../class

INCLUDES= -I${I2D} -I${JLOCAL}/include -I../../include
#INCLUDES= -I${I2D} -I/usr/openwin/include -I/usr/dt/include -I../../include -I/jnet/shared/include

DEFINES	=
CXX	= g++
CXXFLAGS= -g -O ${INCLUDES} ${DEFINES} -L${DISC}/lib
CXXFLAGS= -g -O ${INCLUDES} ${DEFINES}

LIBS	= -L${JLOCAL}/lib -lraf++ -lnetcdf_c++ -lnetcdf

PROG	= mcrp
HDRS	= ../../include/mcr.h

SRCS=	mcrp.cc
OBJS=	mcrp.o


O2DOB=	${O2D}/Netcdf.o ${O2D}/DataBuffer.o ${O2D}/OutFile.o


${PROG}: classDir ${OBJS}
	${CXX} ${CXXFLAGS} ${OBJS} ${O2DOB} ${LIBS} -o $@

classDir:
	cd ${O2D}; gmake

install: ${PROG}
	cp ${PROG} $(BIN)

clean:
	${RM} core ${OBJS} ${PROG}

${OBJS}:	${HDRS}

mcrp.o:	${I2D}/Netcdf.h
