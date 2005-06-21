TARGET = rtplot
TEMPLATE = app

NIMBUS = $${JLOCAL}/raf/nimbus_resampler
NOBJ = $$(NIMBUS)/src/filter/obj

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
OBJECTS += $(NIMBUS)/src/filter/obj/calco.o $(NIMBUS)/src/filter/obj/search.o $(NIMBUS)/src/filter/obj/textfile.o $(NIMBUS)/src/filter/obj/popen.o $(NIMBUS)/src/filter/obj/log.o $(NIMBUS)/src/filter/obj/config.o
LIBS += -L$(JLOCAL)/lib -lraf++ -L$(NIMBUS)/lib -lxlate -lraf -L$(JLOCAL)/src/qwt-4.2.0/lib -lqwt
