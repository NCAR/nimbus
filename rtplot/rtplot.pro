######################################################################
# Automatically generated by qmake (1.03a) Tue Dec 31 08:45:27 2002
######################################################################

TEMPLATE = app

# Input
CONFIG += debug
NOBJ = $$(NIMBUS)/src/filter/obj
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
INCLUDEPATH += $$(JLOCAL)/include $$(NIMBUS)/include
OBJECTS += $$(NIMBUS)/src/filter/obj/calco.o $$(NIMBUS)/src/filter/obj/search.o $$(NIMBUS)/src/filter/obj/textfile.o $$(NIMBUS)/src/filter/obj/popen.o
LIBS += -L$$(JLOCAL)/lib -lqwt -lraf++ -L$$(NIMBUS)/lib -lxlate -lraf
