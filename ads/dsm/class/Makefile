S=		../class/
I=		../include/
O=              ../obj/
L=              ../lib/
IH=		../../header/include/
IL=		$(JLOCAL)/include/
ID=		../../disc/include/
SD=		../../disc/class/
IV=		$(WIND_BASE)/target/config/mv162lc/
OD=		../../disc/obj/
OH=		../../header/obj/

AR68K=	ar68k
RL68K=	ranlib68k

CFLAGS = -c -m68040 -DCPU=MC68LC040 -msoft-float -fno-builtin -D__cpluscplus \
         -DVXWORKS -I$(WIND_BASE)/target/h -nostdinc \
         -I$(I) -I$(IH) -I$(ID) -I$(IL) -I$(IV) 

all: ${O}APN232.o ${O}AnaAuxTable.o ${O}AnalogCal.o ${O}AnalogTable.o ${O}ArincGps.o ${O}Bc635Vme.o ${O}Counter.o ${O}DigOut.o ${O}DigitalIn.o ${O}DigitalTable.o ${O}Dpres.o ${O}DsmNatsComm.o ${O}DsmTapeCtl.o ${O}Dsp56002.o ${O}Exb162.o ${O}GpsTans2.o ${O}GpsTans3.o ${O}GreyVme.o ${O}GreyVmeTable.o ${O}HwGps.o ${O}HwGpsTable.o ${O}HwIrs.o ${O}HwIrsTable.o ${O}IP429.o ${O}IPcounter.o ${O}IsioPort.o ${O}Mcr.o ${O}McrTable.o ${O}MsgQueue.o ${O}Ophir3.o ${O}Pms1Vme.o ${O}Pms1VmeTable.o ${O}Pms2dHouse.o ${O}Pms2dhTable.o ${O}PmsVme2d.o ${O}PmsVme2dTable.o ${O}PpsGps.o ${O}SampleTable.o ${O}SccClass.o ${O}SerialTable.o ${O}SerialTod.o ${O}SyncRecord.o ${O}Synchro.o ${O}SynchroTable.o ${O}TodClock.o ${O}TvmConfig.o ${O}TvmDisplay.o ${O}TvmFixedFmt.o ${O}TvmFltNumber.o ${O}TvmStatus.o ${O}TvmTape.o ${O}TvmUtilities.o ${O}UvHyg.o ${O}Vm3118.o ${O}Vmio12.o ${O}Vmio14Count.o ${O}Vmio14In.o ${O}Vmio14Out.o $(O)GreyVmeHouse.o $(O)GreyVmehTable.o $(O)JplTdl.o  $(O)Spp.o $(O)LhTdl.o $(O)Rdma.o $(O)Neph.o $(O)Climet.o $(O)Mca.o $(O)Cmigits3.o $(O)Garmin.o


ANAAUXTABLE.H = $(I)AnaAuxTable.h $(IL)header.h
ANALOGCAL.H = $(I)AnalogCal.h $(I)AnalogTable.h $(ID)DsmConfig.h \
	$(ID)DsmMessage.h $(I)Dsp56002.h $(ID)TapeHeader.h $(ID)messageDefs.h
ANALOGTABLE.H = $(I)AnalogTable.h $(ID)dsmctl.h $(IL)header.h $(ID)messageDefs.h
APN232.H = $(I)APN232.h $(I)Vmio12.h
APN232.O = $(O)Vmio12.o
ARINCGPS.H = $(I)ArincGps.h $(I)Bc635Vme.h $(I)IP429.h $(I)arinc429.h \
	$(ID)dsmctl.h $(IL)header.h

BC635VME.H = $(I)Bc635Vme.h

CLIMET.H = $(I)Climet.h $(IL)header.h $(ID)messageDefs.h $(ID)dsmctl.h \
        $(I)IsioPort.h
CLIMET.O = $(O)IsioPort.o

MCA.H = $(I)Mca.h $(IL)header.h $(ID)messageDefs.h $(ID)dsmctl.h \
        $(I)IsioPort.h
MCA.O = $(O)IsioPort.o

COUNTER.H = $(I)Counter.h $(I)DigitalTable.h $(ID)SerialBuf.h $(I)Vmio12.h \
	$(I)Vmio14Count.h

CMIDGETS.H = $(I)Cmigits3.h $(IL)header.h $(ID)messageDefs.h $(ID)dsmctl.h \
	$(I)IsioPort.h
CMIDGETS.O = $(O)IsioPort.o 

DIGITALIN.H = $(I)DigitalIn.h $(I)APN232.h $(I)DigitalTable.h $(ID)SerialBuf.h \
	$(I)Vmio12.h $(I)Vmio14In.h
DIGITALTABLE.H = $(I)DigitalTable.h $(ID)dsmctl.h $(ID)TapeHeader.h
DIGOUT.H = $(I)DigOut.h $(ID)DsmMessage.h $(ID)dsmctl.h $(ID)messageDefs.h \
	$(I)Vmio12.h $(I)Vmio14Out.h $(ID)TextFile.h $(I)dsmVmeDefs.h
DPRES.H = $(I)Dpres.h $(IL)header.h $(ID)messageDefs.h $(ID)dsmctl.h \
	$(I)IsioPort.h
DPRES.O = $(O)IsioPort.o 
DSMNATSCOMM.H = $(I)DsmNatsComm.h $(I)dsmVmeDefs.h $(I)IsioPort.h \
	$(ID)natsDefs.h $(ID)dsmctl.h
DSMTAPECTL.H = $(I)DsmTapeCtl.h $(ID)dsmctl.h $(ID)DsmMessage.h $(I)Exb162.h \
	$(ID)TapeHeader.h
DSMTAPECTL.O = $(O)Exb162.o 

DSP56002.H = $(I)Dsp56002.h $(I)AnalogTable.h $(ID)SerialBuf.h

EXB162.H = $(I)Exb162.h $(I)exbDefs.h

GPSTANS2.H = $(I)GpsTans2.h $(ID)messageDefs.h $(IL)header.h $(I)IsioPort.h 
GPSTANS2.O = $(O)IsioPort.o
GPSTANS3.H = $(I)GpsTans3.h $(ID)messageDefs.h $(IL)header.h $(I)IsioPort.h 
GPSTANS3.O = $(O)IsioPort.o
GARMIN.H = $(I)Garmin.h $(ID)messageDefs.h $(IL)header.h $(I)IsioPort.h
GARMIN.O = $(O)IsioPort.o
GREYVME.H = $(I)GreyVme.h $(ID)SerialBuf.h $(IL)header.h $(ID)messageDefs.h \
	$(ID)dsmctl.h
GREYVMETABLE.H= $(I)GreyVmeTable.h $(ID)dsmctl.h $(IL)header.h $(ID)TapeHeader.h

HWGPS.H = $(I)HwGps.h $(I)IP429.h $(I)arinc429.h $(ID)dsmctl.h $(IL)header.h
HWGPSTABLE.H = $(I)HwGpsTable.h $(ID)dsmctl.h $(IL)header.h $(ID)TapeHeader.h
HWIRS.H = $(I)HwIrs.h $(I)Bc635Vme.h $(I)IP429.h $(I)arinc429.h $(ID)dsmctl.h \
	$(IL)header.h
HWIRSTABLE.H = $(I)HwIrsTable.h $(ID)dsmctl.h $(IL)header.h $(ID)TapeHeader.h

IP429.H = $(I)IP429.h $(ID)dsmctl.h
IPCNT.H = $(I)IPcounter.h $(ID)dsmctl.h
ISIOPORT.H = $(I)IsioPort.h $(IL)header.h $(I)isio.h $(I)scn68562.h \
	$(ID)messageDefs.h

JPLTDL.H = $(I)JplTdl.h $(IL)header.h $(ID)messageDefs.h $(ID)dsmctl.h \
        $(I)IsioPort.h
JPTDL.O = $(O)IsioPort.o

LHTDL.H = $(I)LhTdl.h $(IL)header.h $(ID)messageDefs.h $(ID)dsmctl.h \
        $(I)IsioPort.h
LHTDL.O = $(O)IsioPort.o

MCR.H = $(I)Mcr.h $(I)McrTable.h $(ID)dsmctl.h $(ID)messageDefs.h $(I)Vmio12.h \
	$(I)Vmio14Out.h $(ID)DsmMessage.h
MCRTABLE.H = $(I)McrTable.h $(ID)dsmctl.h $(ID)TapeHeader.h

OPHIR3.H = $(I)Ophir3.h $(IL)header.h $(ID)messageDefs.h $(I)IsioPort.h \
	$(IH)Ophir3Blk.h
OPHIR3.O = $(O)IsioPort.o $(OH)Ophir3Blk.o

PMS1VME.H = $(I)Pms1Vme.h $(ID)dsmctl.h $(IL)header.h
PMS1VMETABLE.H= $(I)Pms1VmeTable.h $(ID)dsmctl.h $(IL)header.h $(ID)TapeHeader.h
PMSVME2D.H = $(I)PmsVme2d.h $(ID)dsmctl.h $(IL)header.h $(ID)messageDefs.h \
	$(ID)SerialBuf.h
PMS2DHOUSE.H = $(I)Pms2dHouse.h $(ID)dsmctl.h $(IL)header.h $(ID)messageDefs.h \
	$(ID)SerialBuf.h
PMSVME2DTABLE.H = $(I)PmsVme2dTable.h $(ID)dsmctl.h $(IL)header.h \
	$(ID)TapeHeader.h
GREYVMEHOUSE.H = $(I)GreyVmeHouse.h $(ID)dsmctl.h $(IL)header.h \
        $(ID)messageDefs.h $(ID)SerialBuf.h
GREYVMEHTABLE.H = $(I)GreyVmehTable.h $(ID)dsmctl.h $(IL)header.h \
	$(ID)TapeHeader.h

NEPH.H = $(I)Neph.h $(IL)header.h $(ID)messageDefs.h $(ID)dsmctl.h \
	$(I)IsioPort.h
NEPH.O = $(O)IsioPort.o

PPSGPS.H = $(I)PpsGps.h $(IL)header.h $(ID)messageDefs.h $(I)IsioPort.h

RDMA.H = $(I)Rdma.h $(IL)header.h $(ID)messageDefs.h $(ID)dsmctl.h \
	$(I)IsioPort.h
RDMA.O = $(O)IsioPort.o

SAMPLETABLE.H = $(I)SampleTable.h $(IL)header.h $(ID)dsmctl.h $(I)AnalogTable.h\
        $(I)AnaAuxTable.h $(I)DigitalTable.h $(I)GreyVmeTable.h $(I)McrTable.h\
        $(I)HwIrsTable.h $(I)HwGpsTable.h $(I)PmsVme2dTable.h\
        $(I)Pms1VmeTable.h\
        $(I)SerialTable.h $(ID)TapeHeader.h $(I)UvHyg.h $(I)SynchroTable.h 
	
SAMPLETABLE.O = $(O)AnaAuxTable.o $(O)AnalogTable.o $(O)DigitalTable.o $(O)GreyVmeTable.o $(O)HwIrsTable.o $(O)McrTable.o $(O)PmsVme2dTable.o $(O)SerialTable.o $(OD)TapeHeader.o $(O)Pms1VmeTable.o 

SCCCLASS.H = $(I)SccClass.h $(I)SerialTable.h
SERIALTABLE.H = $(I)SerialTable.h $(ID)dsmctl.h $(IL)header.h $(ID)TapeHeader.h
SERIALTOD.H = $(I)SerialTod.h $(ID)dsmctl.h $(IL)header.h $(I)IsioPort.h \
	$(ID)messageDefs.h  $(I)Bc635Vme.h
SERIALTOD.O = $(O)IsioPort.o $(O)Bc635Vme.o
SYNCRECORD.H = $(I)SyncRecord.h $(I)Bc635Vme.h $(I)Counter.h $(ID)DsmConfig.h \
	$(I)DigitalIn.h $(I)Dpres.h $(I)Dsp56002.h $(I)GpsTans2.h $(I)GpsTans3.h\
	$(IL)header.h $(I)HwIrs.h $(I)HwGps.h $(I)Mcr.h $(I)Ophir3.h \
	$(I)Pms1Vme.h $(I)PpsGps.h $(ID)RandomBuf.h $(I)SampleTable.h \
	$(I)Synchro.h $(ID)TapeHeader.h $(I)UvHyg.h $(I)Vm3118.h

SYNCHROTABLE.H = $(I)SynchroTable.h $(IL)header.h
SYNCHRO.H = $(I)Synchro.h  $(ID)SerialBuf.h

PMS2DHTABLE.H = $(I)Pms2dhTable.h $(IL)header.h

TEXTFILE.H = $(ID)TextFile.h
TODCLOCK.H = $(I)TodClock.h

TVMCONFIG.H = $(I)TvmConfig.h $(IL)header.h $(I)tvmDefs.h
TVMDISPLAY.H = $(I)TvmDisplay.h $(I)SccClass.h
TVMDISPLAY.O = $(O)SccClass.o
TVMFIXEDFMT.H = $(I)TvmFixedFmt.h $(I)tvmDefs.h $(I)TvmDisplay.h
TVMFIXEDFMT.O = $(O)TvmDisplay.o
TVMFLTNUMBER.H = $(I)TvmFltNumber.h $(I)tvmDefs.h $(I)TvmDisplay.h
TVMFLTNUMBER.O = $(O)TvmDisplay.o
TVMSTATUS.H = $(I)TvmStatus.h $(I)tvmDefs.h $(I)TvmDisplay.h
TVMSTATUS.O = $(O)TvmDisplay.o
TVMTAPE.H = $(I)TvmTape.h $(ID)dsmctl.h $(ID)messageDefs.h $(I)tvmDefs.h \
	$(I)TvmDisplay.h
TVMTAPE.O = $(O)TvmDisplay.o
TVMUTILITIES.H = $(I)TvmUtilities.h $(I)tvmDefs.h $(I)TvmDisplay.h
TVMUTILITIES.O = $(O)TvmDisplay.o

SPP.H = $(I)Spp.h $(IL)header.h $(ID)messageDefs.h $(I)IsioPort.h 
SPP.O = $(O)IsioPort.o

UVHYG.H = $(I)UvHyg.h $(IL)header.h $(ID)messageDefs.h $(I)IsioPort.h \
	$(IH)UvBlk.h
UVHYG.O = $(O)IsioPort.o $(OH)UvBlk.o
VM3118.H = $(I)Vm3118.h $(I)dsmVmeDefs.h $(ID)SerialBuf.h $(I)AnaAuxTable.h
VMIO12.H = $(I)Vmio12.h
VMIO14IN.H= $(I)Vmio14In.h $(I)Vmio12.h
VMIO14OUT.H= $(I)Vmio14Out.h $(I)Vmio12.h
VMIO14COUNT.H= $(I)Vmio14Count.h $(I)Vmio12.h
VMIO27COUNT.H= $(I)Vmio27Count.h $(I)Vmio12.h

$(O)Aerosol.o: $(S)Aerosol.cc $(AEROSOL.H) $(AEROSOL.O)
	cc68k $(CFLAGS) $(S)Aerosol.cc -o $(O)Aerosol.o
	${AR68K} r $(L)libdsm.a $(O)Aerosol.o
	${RL68K} $(L)libdsm.a
 
$(O)ArincGps.o: $(S)ArincGps.cc $(ARINCGPS.H)
	cc68k $(CFLAGS) $(S)ArincGps.cc -o $(O)ArincGps.o
	${AR68K} r $(L)libdsm.a $(O)ArincGps.o
	${RL68K} $(L)libdsm.a

$(O)AnaAuxTable.o: $(S)AnaAuxTable.cc $(ANAAUXTABLE.H)
	cc68k $(CFLAGS) $(S)AnaAuxTable.cc -o $(O)AnaAuxTable.o
	${AR68K} r $(L)libdsm.a $(O)AnaAuxTable.o
	${RL68K} $(L)libdsm.a

$(O)AnalogCal.o: $(S)AnalogCal.cc $(ANALOGCAL.H)
	cc68k $(CFLAGS) $(S)AnalogCal.cc -o $(O)AnalogCal.o
	${AR68K} r $(L)libdsm.a $(O)AnalogCal.o
	${RL68K} $(L)libdsm.a

$(O)AnalogTable.o: $(S)AnalogTable.cc $(ANALOGTABLE.H)
	cc68k $(CFLAGS) $(S)AnalogTable.cc -o $(O)AnalogTable.o
	${AR68K} r $(L)libdsm.a $(O)AnalogTable.o
	${RL68K} $(L)libdsm.a

$(O)APN232.o: $(S)APN232.cc $(APN232.H) $(APN232.O)
	cc68k $(CFLAGS) $(S)APN232.cc -o $(O)APN232.o
	${AR68K} r $(L)libdsm.a $(O)APN232.o
	${RL68K} $(L)libdsm.a

$(O)Bc635Vme.o: $(S)Bc635Vme.cc $(BC635VME.H)
	cc68k $(CFLAGS) $(S)Bc635Vme.cc -o $(O)Bc635Vme.o
	${AR68K} r $(L)libdsm.a $(O)Bc635Vme.o
	${RL68K} $(L)libdsm.a

$(O)Climet.o: $(S)Climet.cc $(CLIMET.H) $(CLIMET.O)
	cc68k $(CFLAGS) $(S)Climet.cc -o $(O)Climet.o
	${AR68K} r $(L)libdsm.a $(O)Climet.o
	${RL68K} $(L)libdsm.a

$(O)Cmigits3.o: $(S)Cmigits3.cc $(CMIGITS.H) $(CMIGITS.O)
	cc68k $(CFLAGS) $(S)Cmigits3.cc -o $(O)Cmigits3.o
	${AR68K} r $(L)libdsm.a $(O)Cmigits3.o
	${RL68K} $(L)libdsm.a

$(O)Mca.o: $(S)Mca.cc $(MCA.H) $(MCA.O)
	cc68k $(CFLAGS) $(S)Mca.cc -o $(O)Mca.o
	${AR68K} r $(L)libdsm.a $(O)Mca.o
	${RL68K} $(L)libdsm.a

$(O)Counter.o: $(S)Counter.cc $(COUNTER.H)
	cc68k $(CFLAGS) $(S)Counter.cc -o $(O)Counter.o
	${AR68K} r $(L)libdsm.a $(O)Counter.o
	${RL68K} $(L)libdsm.a

$(O)DigitalIn.o: $(S)DigitalIn.cc $(DIGITALIN.H)
	cc68k $(CFLAGS) $(S)DigitalIn.cc -o $(O)DigitalIn.o
	${AR68K} r $(L)libdsm.a $(O)DigitalIn.o
	${RL68K} $(L)libdsm.a

$(O)DigitalTable.o: $(S)DigitalTable.cc $(DIGITALTABLE.H)
	cc68k $(CFLAGS) $(S)DigitalTable.cc -o $(O)DigitalTable.o
	${AR68K} r $(L)libdsm.a $(O)DigitalTable.o
	${RL68K} $(L)libdsm.a

$(O)DsmNatsComm.o: $(S)DsmNatsComm.cc $(DSMNATSCOMM.H)
	cc68k $(CFLAGS) $(S)DsmNatsComm.cc -o $(O)DsmNatsComm.o
	${AR68K} r $(L)libdsm.a $(O)DsmNatsComm.o
	${RL68K} $(L)libdsm.a

$(O)DsmTapeCtl.o: $(S)DsmTapeCtl.cc $(DSMTAPECTL.H) $(DSMTAPECTL.O)
	cc68k $(CFLAGS) $(S)DsmTapeCtl.cc -o $(O)DsmTapeCtl.o
	${AR68K} r $(L)libdsm.a $(O)DsmTapeCtl.o
	${RL68K} $(L)libdsm.a

$(O)Dsp56002.o: $(S)Dsp56002.cc $(DSP56002.H)
	cc68k $(CFLAGS) $(S)Dsp56002.cc -o $(O)Dsp56002.o
	${AR68K} r $(L)libdsm.a $(O)Dsp56002.o
	${RL68K} $(L)libdsm.a

$(O)Exb162.o: $(S)Exb162.cc $(EXB162.H)
	cc68k $(CFLAGS) $(S)Exb162.cc -o $(O)Exb162.o
	${AR68K} r $(L)libdsm.a $(O)Exb162.o
	${RL68K} $(L)libdsm.a

$(O)GpsTans2.o: $(S)GpsTans2.cc $(GPSTANS2.H) $(GPSTANS2.O)
	cc68k $(CFLAGS) $(S)GpsTans2.cc -o $(O)GpsTans2.o
	${AR68K} r $(L)libdsm.a $(O)GpsTans2.o
	${RL68K} $(L)libdsm.a

$(O)GpsTans3.o: $(S)GpsTans3.cc $(GPSTANS3.H) $(GPSTANS3.O)
	cc68k $(CFLAGS) $(S)GpsTans3.cc -o $(O)GpsTans3.o
	${AR68K} r $(L)libdsm.a $(O)GpsTans3.o
	${RL68K} $(L)libdsm.a

$(O)Garmin.o: $(S)Garmin.cc $(GARMIN.H) $(GARMIN.O)
	cc68k $(CFLAGS) $(S)Garmin.cc -o $(O)Garmin.o
	${AR68K} r $(L)libdsm.a $(O)Garmin.o
	${RL68K} $(L)libdsm.a

$(O)GreyVme.o: $(S)GreyVme.cc $(GREYVME.H)
	cc68k $(CFLAGS) $(S)GreyVme.cc -o $(O)GreyVme.o
	${AR68K} r $(L)libdsm.a $(O)GreyVme.o
	${RL68K} $(L)libdsm.a

$(O)GreyVmeTable.o: $(S)GreyVmeTable.cc $(GREYVMETABLE.H)
	cc68k $(CFLAGS) $(S)GreyVmeTable.cc -o $(O)GreyVmeTable.o
	${AR68K} r $(L)libdsm.a $(O)GreyVmeTable.o
	${RL68K} $(L)libdsm.a

$(O)GreyVmeHouse.o: $(S)GreyVmeHouse.cc $(GREYVMEHOUSE.H)
	cc68k $(CFLAGS) $(S)GreyVmeHouse.cc -o $(O)GreyVmeHouse.o
	${AR68K} r $(L)libdsm.a $(O)GreyVmeHouse.o
	${RL68K} $(L)libdsm.a

$(O)GreyVmehTable.o: $(S)GreyVmehTable.cc $(GREYVMEHTABLE.H)
	cc68k $(CFLAGS) $(S)GreyVmehTable.cc -o $(O)GreyVmehTable.o
	${AR68K} r $(L)libdsm.a $(O)GreyVmehTable.o
	${RL68K} $(L)libdsm.a

$(O)HwGps.o: $(S)HwGps.cc $(HWGPS.H)
	cc68k $(CFLAGS) $(S)HwGps.cc -o $(O)HwGps.o
	${AR68K} r $(L)libdsm.a $(O)HwGps.o
	${RL68K} $(L)libdsm.a

$(O)HwGpsTable.o: $(S)HwGpsTable.cc $(HWGPSTABLE.H)
	cc68k $(CFLAGS) $(S)HwGpsTable.cc -o $(O)HwGpsTable.o
	${AR68K} r $(L)libdsm.a $(O)HwGpsTable.o
	${RL68K} $(L)libdsm.a

$(O)HwIrs.o: $(S)HwIrs.cc $(HWIRS.H)
	cc68k $(CFLAGS) $(S)HwIrs.cc -o $(O)HwIrs.o
	${AR68K} r $(L)libdsm.a $(O)HwIrs.o
	${RL68K} $(L)libdsm.a

$(O)HwIrsTable.o: $(S)HwIrsTable.cc $(HWIRSTABLE.H)
	cc68k $(CFLAGS) $(S)HwIrsTable.cc -o $(O)HwIrsTable.o
	${AR68K} r $(L)libdsm.a $(O)HwIrsTable.o
	${RL68K} $(L)libdsm.a

$(O)IP429.o: $(S)IP429.cc $(IP429.H)
	cc68k $(CFLAGS) $(S)IP429.cc -o $(O)IP429.o
	${AR68K} r $(L)libdsm.a $(O)IP429.o
	${RL68K} $(L)libdsm.a

$(O)IPcounter.o: $(S)IPcounter.cc $(IPCNT.H)
	cc68k $(CFLAGS) $(S)IPcounter.cc -o $(O)IPcounter.o
	${AR68K} r $(L)libdsm.a $(O)IPcounter.o
	${RL68K} $(L)libdsm.a

$(O)IsioPort.o: $(S)IsioPort.cc $(ISIOPORT.H)
	cc68k $(CFLAGS) $(S)IsioPort.cc -o $(O)IsioPort.o
	${AR68K} r $(L)libdsm.a $(O)IsioPort.o
	${RL68K} $(L)libdsm.a

$(O)Dpres.o: $(S)Dpres.cc $(DPRES.H) $(DPRES.O)
	cc68k $(CFLAGS) $(S)Dpres.cc -o $(O)Dpres.o
	${AR68K} r $(L)libdsm.a $(O)Dpres.o
	${RL68K} $(L)libdsm.a

$(O)JplTdl.o: $(S)JplTdl.cc $(JPLTDL.H) $(JPLTDL.O)
	cc68k $(CFLAGS) $(S)JplTdl.cc -o $(O)JplTdl.o
	${AR68K} r $(L)libdsm.a $(O)JplTdl.o
	${RL68K} $(L)libdsm.a

$(O)LhTdl.o: $(S)LhTdl.cc $(LHTDL.H) $(LHTDL.O)
	cc68k $(CFLAGS) $(S)LhTdl.cc -o $(O)LhTdl.o
	${AR68K} r $(L)libdsm.a $(O)LhTdl.o
	${RL68K} $(L)libdsm.a

$(O)Mcr.o: $(S)Mcr.cc $(MCR.H)
	cc68k $(CFLAGS) $(S)Mcr.cc -o $(O)Mcr.o
	${AR68K} r $(L)libdsm.a $(O)Mcr.o
	${RL68K} $(L)libdsm.a

$(O)DigOut.o: $(S)DigOut.cc $(DIGOUT.H)
	cc68k $(CFLAGS) $(S)DigOut.cc -o $(O)DigOut.o
	${AR68K} r $(L)libdsm.a $(O)DigOut.o
	${RL68K} $(L)libdsm.a

$(O)McrTable.o: $(S)McrTable.cc $(MCRTABLE.H)
	cc68k $(CFLAGS) $(S)McrTable.cc -o $(O)McrTable.o
	${AR68K} r $(L)libdsm.a $(O)McrTable.o
	${RL68K} $(L)libdsm.a

$(O)MsgQueue.o: $(S)MsgQueue.cc $(I)MsgQueue.h
	cc68k $(CFLAGS) $(S)MsgQueue.cc -o $(O)MsgQueue.o
	${AR68K} r $(L)libdsm.a $(O)MsgQueue.o
	${RL68K} $(L)libdsm.a

$(O)Neph.o: $(S)Neph.cc $(NEPH.H) $(NEPH.O)
	cc68k $(CFLAGS) $(S)Neph.cc -o $(O)Neph.o
	${AR68K} r $(L)libdsm.a $(O)Neph.o
	${RL68K} $(L)libdsm.a

$(O)Ophir3.o: $(S)Ophir3.cc $(OPHIR3.H) $(OPHIR3.O)
	cc68k $(CFLAGS) $(S)Ophir3.cc -o $(O)Ophir3.o
	${AR68K} r $(L)libdsm.a $(O)Ophir3.o
	${RL68K} $(L)libdsm.a

$(O)Pms1Vme.o: $(S)Pms1Vme.cc $(PMS1VME.H)
	cc68k $(CFLAGS) $(S)Pms1Vme.cc -o $(O)Pms1Vme.o
	${AR68K} r $(L)libdsm.a $(O)Pms1Vme.o
	${RL68K} $(L)libdsm.a

$(O)Pms1VmeTable.o: $(S)Pms1VmeTable.cc $(PMS1VMETABLE.H)
	cc68k $(CFLAGS) $(S)Pms1VmeTable.cc -o $(O)Pms1VmeTable.o
	${AR68K} r $(L)libdsm.a $(O)Pms1VmeTable.o
	${RL68K} $(L)libdsm.a

$(O)PmsVme2d.o: $(S)PmsVme2d.cc $(PMSVME2D.H)
	cc68k $(CFLAGS) $(S)PmsVme2d.cc -o $(O)PmsVme2d.o
	${AR68K} r $(L)libdsm.a $(O)PmsVme2d.o
	${RL68K} $(L)libdsm.a

$(O)PmsVme2dTable.o: $(S)PmsVme2dTable.cc $(PMSVME2DTABLE.H)
	cc68k $(CFLAGS) $(S)PmsVme2dTable.cc -o $(O)PmsVme2dTable.o
	${AR68K} r $(L)libdsm.a $(O)PmsVme2dTable.o
	${RL68K} $(L)libdsm.a

$(O)PpsGps.o: $(S)PpsGps.cc $(PPSGPS.H) $(PPSGPS.O)
	cc68k $(CFLAGS) $(S)PpsGps.cc -o $(O)PpsGps.o
	${AR68K} r $(L)libdsm.a $(O)PpsGps.o
	${RL68K} $(L)libdsm.a

$(O)Rdma.o: $(S)Rdma.cc $(RDMA.H) $(RDMA.O)
	cc68k $(CFLAGS) $(S)Rdma.cc -o $(O)Rdma.o
	${AR68K} r $(L)libdsm.a $(O)Rdma.o
	${RL68K} $(L)libdsm.a

$(O)SampleTable.o: $(S)SampleTable.cc $(SAMPLETABLE.H) $(SAMPLETABLE.O)
	cc68k $(CFLAGS) $(S)SampleTable.cc -o $(O)SampleTable.o
	${AR68K} r $(L)libdsm.a $(O)SampleTable.o
	${RL68K} $(L)libdsm.a

$(O)SccClass.o: $(S)SccClass.cc $(SCCCLASS.H)
	cc68k $(CFLAGS) $(S)SccClass.cc -o $(O)SccClass.o
	${AR68K} r $(L)libdsm.a $(O)SccClass.o
	${RL68K} $(L)libdsm.a

$(O)SerialTable.o: $(S)SerialTable.cc $(SERIALTABLE.H)
	cc68k $(CFLAGS) $(S)SerialTable.cc -o $(O)SerialTable.o
	${AR68K} r $(L)libdsm.a $(O)SerialTable.o
	${RL68K} $(L)libdsm.a

$(O)SerialTod.o: $(S)SerialTod.cc $(SERIALTOD.H) $(SERIALTOD.O)
	cc68k $(CFLAGS) $(S)SerialTod.cc -o $(O)SerialTod.o
	${AR68K} r $(L)libdsm.a $(O)SerialTod.o
	${RL68K} $(L)libdsm.a

$(O)Synchro.o: $(S)Synchro.cc $(SYNCHRO.H) 
	cc68k $(CFLAGS) $(S)Synchro.cc -o $(O)Synchro.o
	${AR68K} r $(L)libdsm.a $(O)Synchro.o
	${RL68K} $(L)libdsm.a

$(O)SynchroTable.o: $(S)SynchroTable.cc $(SYNCHROTABLE.H)
	cc68k $(CFLAGS) $(S)SynchroTable.cc -o $(O)SynchroTable.o
	${AR68K} r $(L)libdsm.a $(O)SynchroTable.o
	${RL68K} $(L)libdsm.a

$(O)Pms2dHouse.o: $(S)Pms2dHouse.cc $(PMS2DHOUSE.H) 
	cc68k $(CFLAGS) $(S)Pms2dHouse.cc -o $(O)Pms2dHouse.o
	${AR68K} r $(L)libdsm.a $(O)Pms2dHouse.o
	${RL68K} $(L)libdsm.a

$(O)Pms2dhTable.o: $(S)Pms2dhTable.cc $(PMS2DHTABLE.H)
	cc68k $(CFLAGS) $(S)Pms2dhTable.cc -o $(O)Pms2dhTable.o
	${AR68K} r $(L)libdsm.a $(O)Pms2dhTable.o
	${RL68K} $(L)libdsm.a

$(O)Spp.o: $(S)Spp.cc $(SPP.H) $(SPP.O)
	cc68k $(CFLAGS) $(S)Spp.cc -o $(O)Spp.o
	${AR68K} r $(L)libdsm.a $(O)Spp.o
	${RL68K} $(L)libdsm.a

$(O)SyncRecord.o: $(S)SyncRecord.cc $(SYNCRECORD.H)
	cc68k $(CFLAGS) $(S)SyncRecord.cc -o $(O)SyncRecord.o
	${AR68K} r $(L)libdsm.a $(O)SyncRecord.o
	${RL68K} $(L)libdsm.a

$(O)TodClock.o: $(S)TodClock.cc $(TODCLOCK.H)
	cc68k $(CFLAGS) $(S)TodClock.cc -o $(O)TodClock.o
	${AR68K} r $(L)libdsm.a $(O)TodClock.o
	${RL68K} $(L)libdsm.a

$(O)TvmConfig.o: $(S)TvmConfig.cc $(TVMCONFIG.H)
	cc68k $(CFLAGS) $(S)TvmConfig.cc -o $(O)TvmConfig.o
	${AR68K} r $(L)libdsm.a $(O)TvmConfig.o
	${RL68K} $(L)libdsm.a

$(O)TvmFixedFmt.o: $(S)TvmFixedFmt.cc $(TVMFIXEDFMT.H) $(TVMFIXEDFMT.O)
	cc68k $(CFLAGS) $(S)TvmFixedFmt.cc -o $(O)TvmFixedFmt.o
	${AR68K} r $(L)libdsm.a $(O)TvmFixedFmt.o
	${RL68K} $(L)libdsm.a

$(O)TvmDisplay.o: $(S)TvmDisplay.cc $(TVMDISPLAY.H) $(TVMDISPLAY.O)
	cc68k $(CFLAGS) $(S)TvmDisplay.cc -o $(O)TvmDisplay.o
	${AR68K} r $(L)libdsm.a $(O)TvmDisplay.o
	${RL68K} $(L)libdsm.a

$(O)TvmFltNumber.o: $(S)TvmFltNumber.cc $(TVMFLTNUMBER.H) $(TVMFLTNUMBER.O)
	cc68k $(CFLAGS) $(S)TvmFltNumber.cc -o $(O)TvmFltNumber.o
	${AR68K} r $(L)libdsm.a $(O)TvmFltNumber.o
	${RL68K} $(L)libdsm.a

$(O)TvmStatus.o: $(S)TvmStatus.cc $(TVMSTATUS.H) $(TVMSTATUS.O)
	cc68k $(CFLAGS) $(S)TvmStatus.cc -o $(O)TvmStatus.o
	${AR68K} r $(L)libdsm.a $(O)TvmStatus.o
	${RL68K} $(L)libdsm.a

$(O)TvmTape.o: $(S)TvmTape.cc $(TVMTAPE.H) $(TVMTAPE.O)
	cc68k $(CFLAGS) $(S)TvmTape.cc -o $(O)TvmTape.o
	${AR68K} r $(L)libdsm.a $(O)TvmTape.o
	${RL68K} $(L)libdsm.a

$(O)TvmUtilities.o: $(S)TvmUtilities.cc $(TVMUTILITIES.H) $(TVMUTILITIES.O)
	cc68k $(CFLAGS) $(S)TvmUtilities.cc -o $(O)TvmUtilities.o
	${AR68K} r $(L)libdsm.a $(O)TvmUtilities.o
	${RL68K} $(L)libdsm.a

$(O)UvHyg.o: $(S)UvHyg.cc $(UVHYG.H) $(UVHYG.O)
	cc68k $(CFLAGS) $(S)UvHyg.cc -o $(O)UvHyg.o
	${AR68K} r $(L)libdsm.a $(O)UvHyg.o
	${RL68K} $(L)libdsm.a

$(O)Vm3118.o: $(S)Vm3118.cc $(VM3118.H)
	cc68k $(CFLAGS) $(S)Vm3118.cc -o $(O)Vm3118.o
	${AR68K} r $(L)libdsm.a $(O)Vm3118.o
	${RL68K} $(L)libdsm.a

$(O)Vmio12.o: $(S)Vmio12.cc $(VMIO12.H)
	cc68k $(CFLAGS) $(S)Vmio12.cc -o $(O)Vmio12.o
	${AR68K} r $(L)libdsm.a $(O)Vmio12.o
	${RL68K} $(L)libdsm.a

$(O)Vmio14In.o: $(S)Vmio14In.cc $(VMIO14IN.H)
	cc68k $(CFLAGS) $(S)Vmio14In.cc -o $(O)Vmio14In.o
	${AR68K} r $(L)libdsm.a $(O)Vmio14In.o
	${RL68K} $(L)libdsm.a

$(O)Vmio14Out.o: $(S)Vmio14Out.cc $(VMIO14OUT.H)
	cc68k $(CFLAGS) $(S)Vmio14Out.cc -o $(O)Vmio14Out.o
	${AR68K} r $(L)libdsm.a $(O)Vmio14Out.o
	${RL68K} $(L)libdsm.a

$(O)Vmio14Count.o: $(S)Vmio14Count.cc $(VMIO14COUNT.H)
	cc68k $(CFLAGS) $(S)Vmio14Count.cc -o $(O)Vmio14Count.o
	${AR68K} r $(L)libdsm.a $(O)Vmio14Count.o
	${RL68K} $(L)libdsm.a

$(O)Vmio27Count.o: $(S)Vmio27Count.cc $(VMIO27COUNT.H)
	cc68k $(CFLAGS) $(S)Vmio27Count.cc -o $(O)Vmio27Count.o
	${AR68K} r $(L)libdsm.a $(O)Vmio27Count.o
	${RL68K} $(L)libdsm.a
