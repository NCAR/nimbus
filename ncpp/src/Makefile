#
# Makefile for ncpp
#
CXX	= g++
BIN	= ${JLOCAL}/bin
WWW	= /net/www/docs/raf/Software
RELEASE = ${JLOCAL}/ncpp

INC	= ../include
RAFINC	= ${JLOCAL}/include
COD	= ../class

#	Linux
#
DEFINES	= -DPNG
INCLUDES= -I${INC} -I/usr/X11R6/include -I${RAFINC}
LIB_DIRS= -L/usr/X11R6/lib -L${JLOCAL}/lib
LIBS	= -lraf++ -lXm -lXt -lX11 -lXext -lnetcdf_c++ -lnetcdf -lpng -lz
ANONFTP	= /net/ftp/pub/archive/RAF-src/bin.Fedora3

#	Mac OS X
#
#DEFINES	= -DPNG
#INCLUDES= -I${INC} -I/sw/include -I/usr/X11R6/include -I${RAFINC}
#LIB_DIRS= -L/usr/X11R6/lib -L${JLOCAL}/lib -L/sw/lib
#LIBS	= -lraf++ -lXm -lXt -lX11 -lXext -lnetcdf_c++ -lnetcdf -lpng -lz
#LIBS	= -lraf++ -lXm -lXt -lX11 -lXext -lnetcdf_c++ -lnetcdf


#	Solaris
#
#INCLUDES= -I${INC} -I${RAFINC} -I${LOCAL}/include -I/usr/dt/include
#LIBS	= -lraf++ -Wl,-Bstatic -lpng -lz -Wl,-Bdynamic -R /usr/dt/lib -lXm -lXt -lX11 -lnetcdf_c++ -lnetcdf -lm
#DEFINES	= -DSVR4 -DPNG
#ANONFTP	= /net/ftp/pub/archive/RAF-src/bin.Solaris2.9


CXXFLAGS= -g -O ${INCLUDES} ${DEFINES}

PROG	= ncpp
HDRS	= ${INC}/define.h

SRCS=	ncpp.cc global.cc autoscale.cc ccb.cc cb_canvas.cc\
	cb_control.cc cb_edit.cc cb_menus.cc cb_movie.cc cb_printer.cc\
	cb_text.cc ticlabel.cc

OBJS=	ncpp.o global.o autoscale.o ccb.o cb_canvas.o cb_control.o\
	cb_edit.o cb_menus.o cb_movie.o cb_printer.o cb_text.o\
	ticlabel.o


COBS=	${COD}/Ascii.o ${COD}/CanvasWindow.o ${COD}/DataFile.o ${COD}/DataSet.o\
	${COD}/DataSource.o ${COD}/EditDiameters.o ${COD}/EditPlotParms.o\
	${COD}/FileMgr.o\
	${COD}/Header.o ${COD}/MovieControl.o ${COD}/Panel.o ${COD}/PlotInfo.o\
	${COD}/PlotMgr.o ${COD}/Probe.o ${COD}/SetMgr.o ${COD}/Titles.o\
	${COD}/VarsWin.o ${COD}/Colors.o\
	${COD}/fssp.o ${COD}/f300.o ${COD}/pcasp.o ${COD}/x260.o\
	${COD}/s100.o ${COD}/s200.o ${COD}/x200.o ${COD}/y200.o ${COD}/Probe200.o\
	${COD}/2D_P.o ${COD}/2D_C.o


${PROG}: ${OBJS} classDir
	${CXX} ${CXXFLAGS} ${LIB_DIRS} ${OBJS} ${COBS} ${LIBS} -o $@
	rm -f core

classDir:
	cd ${COD}; make "CXXFLAGS=${CXXFLAGS}"

install: ${PROG}
	test -d $(BIN) || mkdir $(BIN)
	cp ${PROG} $(BIN)

publish: $(PROG)
	cp ${PROG} $(ANONFTP)
	cp ${PROG}.html $(WWW)

clean:
	(cd ../class; make clean)
	${RM} core* ${OBJS} ${PROG}

print:
	enscript -2Gr -b${PROG} ${HDRS} ${SRCS}

${OBJS}:	${HDRS}

ncpp.o:	${RAFINC}/raf/Application.h ${INC}/CanvasWindow.h ${RAFINC}/raf/Cursor.h ${INC}/FileMgr.h ${INC}/DataFile.h ${INC}/MovieControl.h ${INC}/PlotMgr.h ${RAFINC}/raf/Printer.h ${INC}/SetMgr.h ${RAFINC}/raf/XmFile.h ${RAFINC}/raf/Canvas.h ${INC}/fbr.h


autoscale.o:	${INC}/SetMgr.h ${INC}/Panel.h

cb_canvas.o:	${RAFINC}/raf/Canvas.h ${INC}/PlotMgr.h

cb_control.o:	${RAFINC}/raf/Canvas.h ${INC}/FileMgr.h ${INC}/MovieControl.h ${INC}/PlotMgr.h ${INC}/SetMgr.h

cb_edit.o:	${RAFINC}/raf/Application.h ${INC}/EditDiameters.h

cb_menus.o:	${RAFINC}/raf/Application.h ${INC}/PlotMgr.h ${RAFINC}/raf/Cursor.h ${INC}/FileMgr.h ${RAFINC}/raf/PostScript.h ${RAFINC}/raf/XmFile.h ${INC}/SetMgr.h ${INC}/PlotMgr.h

cb_movie.o:	${INC}/MovieControl.h

cb_printer.o:	${RAFINC}/raf/Printer.h

cb_text.o:	${RAFINC}/raf/Application.h ${INC}/Ascii.h ${INC}/Header.h ${INC}/FileMgr.h ${INC}/SetMgr.h ${INC}/PlotMgr.h ${INC}/SetMgr.h ${RAFINC}/raf/TextWindow.h ${RAFINC}/raf/XmFile.h ${RAFINC}/raf/XmError.h ${INC}/VarsWin.h
