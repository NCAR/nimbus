#
# Makefile for Nimbus input program.
#
CC		= g++

NIMBUS	= ../..
NINC	= ../../include/
NSRC	= $(NIMBUS)/src/
WINC	= $(WINDS)/include/

RELEASE	= /jnet/solaris/nimbus
NOBJ	= obj/
LOCAL_INC = ${JLOCAL}/include/

INCLUDES = -I${NINC} -I${LOCAL_INC} -I${LOCAL}/include
INCLUDES = -I${NINC} -I${LOCAL_INC}

CFLAGS	= -g -O2 -L../../lib
COFLAGS	= -g -O2 -c $(INCLUDES)


CLIBS	= -lstd -luser -lxlate -lAircraft -lhdr_api -lpms -lVarDB -lraf -lm -lncaru -lnetcdf -lraf++ -lXm -lXt -lX11 -lpq

# Linux
#
INCLUDES = -I$(NINC) -I$(LOCAL_INC) -I/usr/X11R6/include
LIB_DIRS= -L/usr/X11R6/lib -L${JLOCAL}/lib -L/usr/lib/pgsql
CLIBS   = -lstd -luser -lxlate -lAircraft -lhdr_api -lpms -lVarDB -lraf -lm -lncaru -lnetcdf -lraf++ -lXm -lXt -lX11 -lpq


INPUT.O = $(NOBJ)adsIO.o $(NOBJ)amlibfn.o $(NOBJ)async.o $(NOBJ)average.o\
	$(NOBJ)calco.o $(NOBJ)cb_defs.o $(NOBJ)cb_edit.o $(NOBJ)cb_flight.o\
	$(NOBJ)cb_main.o $(NOBJ)cb_pause.o $(NOBJ)cb_time.o $(NOBJ)circbuff.o\
	$(NOBJ)compute.o $(NOBJ)depend.o $(NOBJ)despike.o $(NOBJ)error.o\
	$(NOBJ)filter.o $(NOBJ)globals.o $(NOBJ)hdr_decode.o $(NOBJ)hrloop.o\
	$(NOBJ)init.o $(NOBJ)interp.o $(NOBJ)logfile.o $(NOBJ)lrloop.o\
	$(NOBJ)memalloc.o $(NOBJ)netcdf.o $(NOBJ)nimbus.o $(NOBJ)order.o\
	$(NOBJ)phase_shift.o $(NOBJ)pms1d.o $(NOBJ)popen.o $(NOBJ)probe.o\
	$(NOBJ)rd_defs.o $(NOBJ)rd_ddor.o $(NOBJ)rd_mod.o $(NOBJ)rd_lag.o\
	$(NOBJ)rd_spike.o $(NOBJ)rd_sum.o $(NOBJ)rtloop.o $(NOBJ)rec_decode.o\
	$(NOBJ)search.o $(NOBJ)setup.o $(NOBJ)sort.o $(NOBJ)textfile.o\
	$(NOBJ)timeseg.o $(NOBJ)Xwin.o $(NOBJ)dq.o $(NOBJ)UdpSocket.o $(NOBJ)psql.o\
	$(NOBJ)injectsd.o


# ------------------------------------------------------------------------
nimbus:	include $(INPUT.O) amlib
	rm -f obj/pms1d_rt.o
	${CC} $(CFLAGS) ${LIB_DIRS} $(INPUT.O) -o nimbus $(CLIBS)


include:
	cd ${NINC}; make

amlib:
	cd ${NSRC}/amlib; make

obj: $(INPUT.O) amlib $(NOBJ)pms1d_rt.o

release:
	(cd ${JLOCAL}/dev/nimbus; tar cFFf - include src/filter src/amlib | (cd ${RELEASE}; tar xfBp -))
	find ${RELEASE} \( -name '*.c' -o -name '*.h' -o -name Makefile \) -exec chmod go-w {} \;
	chmod -R ug+w ${RELEASE}
	chmod go-w ${RELEASE}/include/amlib.fns
	rm -f ${RELEASE}/src/filter/LOCK.make

install:	nimbus
	cp nimbus ${JLOCAL}/bin
lint:
	lint ${INCLUDES} *.c
#	lint ${INCLUDES} *.c ${CLIBS}

clean:
	rm -f core* nimbus $(NOBJ)*.o LOCK.make mk.tmp

$(NINC)nimbus.h: $(LOCAL_INC)constants.h
$(NINC)decode.h: $(LOCAL_INC)header.h


# ------------------------------------------------------------------------

NIMBUS.H = $(NINC)nimbus.h $(NINC)gui.h $(NINC)version.h $(NINC)fbr.h
$(NOBJ)nimbus.o: nimbus.c $(NIMBUS.H)
	${CC} $(COFLAGS) $(DEFINES) nimbus.c -o $(NOBJ)nimbus.o

GLOBALS.H = $(NINC)nimbus.h $(NINC)decode.h
$(NOBJ)globals.o: globals.c $(GLOBALS.H)
	${CC} $(COFLAGS) $(DEFINES) globals.c -o $(NOBJ)globals.o

INIT.H = $(NINC)nimbus.h $(NINC)decode.h $(NINC)gui.h
$(NOBJ)init.o: init.c $(INIT.H)
	${CC} $(COFLAGS) $(DEFINES) init.c -o $(NOBJ)init.o

SPIKE.H = $(NINC)nimbus.h $(NINC)decode.h
$(NOBJ)rd_spike.o: rd_spike.c $(SPIKE.H)
	${CC} $(COFLAGS) $(DEFINES) rd_spike.c -o $(NOBJ)rd_spike.o

LAG.H = $(NINC)nimbus.h $(NINC)decode.h $(NINC)bounds.h
$(NOBJ)rd_lag.o: rd_lag.c $(LAG.H)
	${CC} $(COFLAGS) $(DEFINES) rd_lag.c -o $(NOBJ)rd_lag.o

SORT.H = 
$(NOBJ)sort.o: sort.c $(SORT.H)
	${CC} $(COFLAGS) $(DEFINES) sort.c -o $(NOBJ)sort.o

CALCO.H = $(NINC)nimbus.h
$(NOBJ)calco.o: calco.c $(CALCO.H)
	${CC} $(COFLAGS) $(DEFINES) calco.c -o $(NOBJ)calco.o

MODULO.H = $(NINC)nimbus.h $(NINC)decode.h
$(NOBJ)rd_mod.o: rd_mod.c $(MODULO.H)
	${CC} $(COFLAGS) $(DEFINES) rd_mod.c -o $(NOBJ)rd_mod.o

SUM.H = $(NINC)nimbus.h $(NINC)decode.h
$(NOBJ)rd_sum.o: rd_sum.c $(SUM.H)
	${CC} $(COFLAGS) $(DEFINES) rd_sum.c -o $(NOBJ)rd_sum.o

ADSIO.H = $(NINC)nimbus.h $(NINC)decode.h $(LOCAL_INC)ctape.h $(LOCAL_INC)raf.h
$(NOBJ)adsIO.o: adsIO.c $(ADSIO.H)
	${CC} $(COFLAGS) $(DEFINES) adsIO.c -o $(NOBJ)adsIO.o

TIMESEG.H = $(NINC)nimbus.h $(NINC)decode.h $(NINC)gui.h
$(NOBJ)timeseg.o: timeseg.c $(TIMESEG.H)
	${CC} $(COFLAGS) $(DEFINES) timeseg.c -o $(NOBJ)timeseg.o

REC_DECODE.H = $(NINC)nimbus.h $(NINC)decode.h
$(NOBJ)rec_decode.o: rec_decode.c $(REC_DECODE.H)
	${CC} $(COFLAGS) $(DEFINES) rec_decode.c -o $(NOBJ)rec_decode.o

AVERAGE.H = $(NINC)nimbus.h
$(NOBJ)average.o: average.c $(AVERAGE.H)
	${CC} $(COFLAGS) $(DEFINES) average.c -o $(NOBJ)average.o

DESPIKE.H = $(NINC)nimbus.h
$(NOBJ)despike.o: despike.c $(DESPIKE.H)
	${CC} $(COFLAGS) $(DEFINES) despike.c -o $(NOBJ)despike.o

INTERP.H = $(NINC)nimbus.h $(NINC)decode.h
$(NOBJ)interp.o: interp.c $(INTERP.H)
	${CC} $(COFLAGS) $(DEFINES) interp.c -o $(NOBJ)interp.o

PSQL.H = $(NINC)nimbus.h $(NINC)decode.h
$(NOBJ)psql.o: psql.cc $(PSQL.H)
	${CC} $(COFLAGS) $(DEFINES) psql.cc -o $(NOBJ)psql.o

RTLOOP.H = $(NINC)nimbus.h $(NINC)decode.h ${NINC}gui.h
$(NOBJ)rtloop.o: rtloop.c $(RTLOOP.H)
	${CC} $(COFLAGS) $(DEFINES) rtloop.c -o $(NOBJ)rtloop.o

LRLOOP.H = $(NINC)nimbus.h $(NINC)decode.h $(NINC)circbuff.h $(NINC)gui.h $(NINC)injectsd.h
$(NOBJ)lrloop.o: lrloop.c $(LRLOOP.H)
	${CC} $(COFLAGS) $(DEFINES) lrloop.c -o $(NOBJ)lrloop.o

INJECTSD.H = $(NINC)nimbus.h $(NINC)decode.h $(NINC)gui.h 
$(NOBJ)injectsd.o: injectsd.cpp $(INJECTSD.H)
	$(CC) $(COFLAGS) $(DEFINES) injectsd.cpp -o $(NOBJ)injectsd.o

HRLOOP.H = $(NINC)nimbus.h $(NINC)decode.h $(NINC)circbuff.h $(NINC)gui.h $(NINC)injectsd.h
$(NOBJ)hrloop.o: hrloop.c $(HRLOOP.H)
	${CC} $(COFLAGS) $(DEFINES) hrloop.c -o $(NOBJ)hrloop.o

FILTER.H = $(NINC)nimbus.h $(NINC)decode.h $(NINC)filters.h $(NINC)circbuff.h
$(NOBJ)filter.o: filter.c $(FILTER.H)
	${CC} $(COFLAGS) $(DEFINES) filter.c -o $(NOBJ)filter.o

FILTER.H = $(NINC)nimbus.h $(NINC)decode.h $(NINC)filters.h
$(NOBJ)filters.o: filters.c $(FILTER.H)
	${CC} $(COFLAGS) $(DEFINES) filters.c -o $(NOBJ)filters.o

PS.H = $(NINC)nimbus.h $(NINC)decode.h $(NINC)circbuff.h $(NINC)bounds.h
$(NOBJ)phase_shift.o: phase_shift.c $(PS.H)
	${CC} $(COFLAGS) $(DEFINES) phase_shift.c -o $(NOBJ)phase_shift.o

HD.H =	$(NINC)nimbus.h $(NINC)decode.h $(LOCAL_INC)ctape.h $(NINC)amlib.h \
	$(LOCAL_INC)raf.h
$(NOBJ)hdr_decode.o: hdr_decode.c $(HD.H)
	${CC} $(COFLAGS) $(DEFINES) hdr_decode.c -o $(NOBJ)hdr_decode.o

ORDER.H = $(NINC)nimbus.h $(NINC)decode.h
$(NOBJ)order.o: order.c $(ORDER.H)
	${CC} $(COFLAGS) $(DEFINES) order.c  -o $(NOBJ)order.o
 
COMPUTE.H = $(NINC)nimbus.h $(NINC)decode.h
$(NOBJ)compute.o: compute.c $(COMPUTE.H)
	${CC} $(COFLAGS) $(DEFINES) compute.c -o $(NOBJ)compute.o
 
DEPEND.H = $(NINC)nimbus.h $(NINC)decode.h
$(NOBJ)depend.o: depend.c $(DEPEND.H)
	${CC} $(COFLAGS) $(DEFINES) depend.c -o $(NOBJ)depend.o
 
CCB.H = $(NINC)nimbus.h $(NINC)gui.h $(NINC)version.h
$(NOBJ)cb_main.o: cb_main.c $(CCB.H)
	${CC} $(COFLAGS) $(DEFINES) cb_main.c -o $(NOBJ)cb_main.o

CCB.H = $(NINC)nimbus.h $(NINC)gui.h $(NINC)injectsd.h
$(NOBJ)cb_edit.o: cb_edit.c $(CCB.H)
	${CC} $(COFLAGS) $(DEFINES) cb_edit.c -o $(NOBJ)cb_edit.o

CCB.H = $(NINC)nimbus.h $(NINC)gui.h
$(NOBJ)cb_time.o: cb_time.c $(CCB.H)
	${CC} $(COFLAGS) $(DEFINES) cb_time.c -o $(NOBJ)cb_time.o

CCB.H = $(NINC)nimbus.h $(NINC)gui.h
$(NOBJ)cb_pause.o: cb_pause.c $(CCB.H)
	${CC} $(COFLAGS) $(DEFINES) cb_pause.c -o $(NOBJ)cb_pause.o

ERROR.H = $(NINC)nimbus.h $(NINC)gui.h
$(NOBJ)error.o: error.c $(ERROR.H)
	${CC} $(COFLAGS) $(DEFINES) error.c -o $(NOBJ)error.o

XWIN.H = 
$(NOBJ)Xwin.o: Xwin.c $(XWIN.H)
	${CC} $(COFLAGS) $(DEFINES) Xwin.c -o $(NOBJ)Xwin.o

MEMALLOC.H = $(NINC)nimbus.h $(NINC)decode.h $(LOCAL_INC)ctape.h
$(NOBJ)memalloc.o: memalloc.c $(MEMALLOC.H)
	${CC} $(COFLAGS) $(DEFINES) memalloc.c -o $(NOBJ)memalloc.o

WRITED.H = $(NINC)nimbus.h $(NINC)decode.h $(LOCAL)/include/netcdf.h
$(NOBJ)netcdf.o: netcdf.c $(WRITED.H)
	${CC} $(COFLAGS) $(DEFINES) netcdf.c -o $(NOBJ)netcdf.o
 
AMLIBFN.H =  $(NINC)nimbus.h $(NINC)decode.h $(NINC)amlib.h $(NINC)amlibfn.h
$(NOBJ)amlibfn.o: amlibfn.c $(AMLIBFN.H)
	${CC} $(COFLAGS) $(DEFINES) amlibfn.c -o $(NOBJ)amlibfn.o

CB_DEF.H =  $(NINC)nimbus.h $(NINC)decode.h $(NINC)gui.h
$(NOBJ)cb_defs.o: cb_defs.c $(CB_DEF.H)
	${CC} $(COFLAGS) $(DEFINES) cb_defs.c -o $(NOBJ)cb_defs.o

CB_FLIGHT.H =  $(NINC)nimbus.h $(NINC)decode.h $(NINC)gui.h
$(NOBJ)cb_flight.o: cb_flight.c $(CB_FLIGHT.H)
	${CC} $(COFLAGS) $(DEFINES) cb_flight.c -o $(NOBJ)cb_flight.o

RD_DEF.H =  $(NINC)nimbus.h $(NINC)decode.h
$(NOBJ)rd_defs.o: rd_defs.c $(RD_DEF.H)
	${CC} $(COFLAGS) $(DEFINES) rd_defs.c -o $(NOBJ)rd_defs.o

RD_DDOR.H =  $(NINC)nimbus.h $(NINC)decode.h
$(NOBJ)rd_ddor.o: rd_ddor.c $(RD_DEF.H)
	${CC} $(COFLAGS) $(DEFINES) rd_ddor.c -o $(NOBJ)rd_ddor.o

PMS1D.H =  $(NINC)nimbus.h $(NINC)decode.h $(WINC)pms1d.h
$(NOBJ)pms1d_rt.o: pms1d_rt.c $(PMS1D.H)
	${CC} $(COFLAGS) -I$(WINC) $(DEFINES) pms1d_rt.c -o $(NOBJ)pms1d_rt.o

PMS1D.H =  
$(NOBJ)pms1d.o: pms1d.c $(PMS1D.H)
	${CC} $(COFLAGS) $(DEFINES) pms1d.c -o $(NOBJ)pms1d.o

SEARCH.H =  $(NINC)nimbus.h
$(NOBJ)search.o: search.c $(SEARCH.H)
	${CC} $(COFLAGS) $(DEFINES) search.c -o $(NOBJ)search.o

POPEN.H =  $(NINC)nimbus.h
$(NOBJ)popen.o: popen.c $(POPEN.H)
	${CC} $(COFLAGS) $(DEFINES) popen.c -o $(NOBJ)popen.o

TEXTFILE.H =  $(NINC)nimbus.h
$(NOBJ)textfile.o: textfile.c $(TEXTFILE.H)
	${CC} $(COFLAGS) $(DEFINES) textfile.c -o $(NOBJ)textfile.o

CIRCBUFF.H =  $(NINC)nimbus.h $(NINC)circbuff.h
$(NOBJ)circbuff.o: circbuff.c $(CIRCBUFF.H)
	${CC} $(COFLAGS) $(DEFINES) circbuff.c -o $(NOBJ)circbuff.o

SETUP.H =  $(NINC)nimbus.h $(NINC)gui.h $(NINC)injectsd.h
$(NOBJ)setup.o: setup.c $(SETUP.H)
	${CC} $(COFLAGS) $(DEFINES) setup.c -o $(NOBJ)setup.o

PROBE.H =  $(NINC)nimbus.h
$(NOBJ)probe.o: probe.c $(PROBE.H)
	${CC} $(COFLAGS) $(DEFINES) probe.c -o $(NOBJ)probe.o

ASYNC.H =  $(NINC)nimbus.h $(NINC)decode.h $(LOCAL)/include/netcdf.h
$(NOBJ)async.o: async.c $(ASYNC.H)
	${CC} $(COFLAGS) $(DEFINES) async.c -o $(NOBJ)async.o

LOG.H = $(NINC)nimbus.h
$(NOBJ)logfile.o: logfile.c $(LOG.H)
	${CC} $(COFLAGS) $(DEFINES) logfile.c -o $(NOBJ)logfile.o

DQ.H = $(NINC)nimbus.h
$(NOBJ)dq.o: dq.c $(DQ.H)
	${CC} $(COFLAGS) $(DEFINES) dq.c -o $(NOBJ)dq.o

UDP.H = $(NINC)UdpSocket.h
$(NOBJ)UdpSocket.o: UdpSocket.cc $(UDP.H)
	${CC} $(COFLAGS) $(DEFINES) UdpSocket.cc -o $(NOBJ)UdpSocket.o

