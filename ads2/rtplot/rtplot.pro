TARGET = rtplot
TEMPLATE = app

QWT_DIR = /opt/local/aeros/qwt-5.0

# Input
CONFIG += debug
HEADERS += CanvasWindow.h DataPlot.h define.h extern.h ../nimbus/include/xlatefns.h
SOURCES += amlibfn.cc \
           CanvasWindow.cc \
           dataIO.cc \
           DataPlot.cc \
           error.cc \
           global.cc \
           hdr_decode.cc \
           init.cc \
           memalloc.cc \
           misc.cc \
           rec_decode.cc \
           rtplot.cc
INCLUDEPATH += $(JLOCAL)/include ../nimbus/include $${QWT_DIR}/include
OBJECTS += ../nimbus/src/filter/calco.o ../nimbus/src/filter/search.o ../nimbus/src/filter/textfile.o ../nimbus/src/filter/popen.o ../nimbus/src/filter/log.o ../nimbus/src/filter/config.o ../nimbus/src/filter/rd_defs.o
LIBS += -L$(JLOCAL)/lib -lraf++ -L../nimbus/lib -lxlate -lraf -L$${QWT_DIR}/lib -lqwt -lnetcdf
