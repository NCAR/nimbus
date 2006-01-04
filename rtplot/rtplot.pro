TARGET = rtplot
TEMPLATE = app

NIMBUS = $${JLOCAL}/raf/nimbus
NOBJ = $$(NIMBUS)/src/filter/

# Input
CONFIG += debug
HEADERS += CanvasWindow.h DataPlot.h define.h extern.h $$(NIMBUS)/include/xlatefns.h
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
INCLUDEPATH += $(JLOCAL)/include $(NIMBUS)/include $(JLOCAL)/src/qwt-4.2.0/include
OBJECTS += $(NIMBUS)/src/filter/calco.o $(NIMBUS)/src/filter/search.o $(NIMBUS)/src/filter/textfile.o $(NIMBUS)/src/filter/popen.o $(NIMBUS)/src/filter/log.o $(NIMBUS)/src/filter/config.o $(NIMBUS)/src/filter/rd_defs.o
LIBS += -L$(JLOCAL)/lib -lraf++ -L$(NIMBUS)/lib -lxlate -lraf -L$(JLOCAL)/src/qwt-4.2.0/lib -lqwt -lnetcdf
