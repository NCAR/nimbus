S=              ../src/
I=		../include/
O=              ../obj/
B=              ../bin/
L=              ../lib/
LD=             ../../disc/lib/
LH=             ../../header/lib/
LC=		$(JLOCAL)/lib/gcc-lib/m68k-wrs-vxworks/2.95.2/msoft-float/
IL=		$(JLOCAL)/include/
IH=		../../header/include/
ID=		../../disc/include/
IV=		$(WIND_BASE)/target/config/mv162lc/
OD=		../../disc/obj/
OH=		../../header/obj/

DSM_LIBS=       $(L)libdsm.a $(LC)libgcc.a
DISC_LIBS=      $(LD)libvxdisc.a
HDR_LIBS=    	$(LH)libvxheader.a
RELEASE	=	${JLOCAL}/ads

CFLAGS = -c -m68040 -DCPU=MC68LC040 -msoft-float -DVXWORKS -fno-builtin \
         -I$(WIND_BASE)/target/h -nostdinc -D__cpluscplus \
         -I$(I) -I$(IH) -I$(ID) -I$(IL) -I$(IV) 

ASYNC.H= $(ID)dsmctl.h $(I)dsmVmeDefs.h $(I)ipicDefs.h $(IL)header.h $(ID)messageDefs.h

ASYNC.O = $(O)APN232.o $(O)AnalogCal.o $(O)Bc635Vme.o $(O)Counter.o $(O)DigitalIn.o $(O)Dsp56002.o $(OD)DsmConfig.o $(OD)DsmMessage.o $(O)GpsTans2.o $(O)GpsTans3.o $(O)GreyVme.o $(O)HwGps.o $(O)HwGpsTable.o $(O)HwIrs.o $(O)IP429.o $(O)IsioPort.o $(O)Mcr.o $(O)MsgQueue.o $(OD)Palt.o $(O)Pms1Vme.o $(OD)RandomBuf.o $(O)SampleTable.o $(O)SyncRecord.o $(OD)SyncVar.o $(OD)TapeHeader.o $(OD)Tasx.o $(OD)VarConfig.o $(O)Vmio12.o $(O)Vmio14In.o $(O)PpsGps.o $(O)SerialTod.o $(O)DigOut.o $(O)Dpres.o $(O)PmsVme2d.o $(O)Synchro.o $(O)SynchroTable.o $(O)TodClock.o $(O)Pms2dHouse.o $(O)Pms2dhTable.o $(O)IPcounter.o $(O)Vm3118.o $(O)GreyVmeHouse.o $(O)GreyVmehTable.o $(O)JplTdl.o $(O)Spp.o $(O)LhTdl.o $(O)Rdma.o $(O)Neph.o $(O)Climet.o $(O)Mca.o $(O)Cmigits3.o $(O)Garmin.o 

#$(O)Aimr.o $(O)NoNoy.o $(O)Noaa.o $(O)Ozone.o

COMM.H= $(ID)dsmctl.h $(IL)header.h $(I)dsmVmeDefs.h $(ID)messageDefs.h
COMM.O =   $(OD)DsmConfig.o $(O)DsmNatsComm.o $(OD)DsmNet.o $(O)MsgQueue.o $(OD)NatsData.o $(OD)TapeHeader.o $(OD)TcpSocket.o $(OD)UdpSocket.o

DISPLAY.H= $(ID)dsmctl.h $(I)dsmVmeDefs.h $(I)tvmDefs.h
DISPLAY.O= $(OD)DsmConfig.o $(OD)DsmMessage.o $(O)MsgQueue.o $(OD)SyncVar.o $(OD)TapeHeader.o $(O)TvmConfig.o $(O)TvmDisplay.o $(O)TvmFixedFmt.o $(O)TvmFltNumber.o $(O)TvmStatus.o $(O)TvmTape.o $(O)TvmUtilities.o

TAPE.H= $(ID)dsmctl.h $(I)dsmVmeDefs.h
TAPE.O= $(OD)DsmConfig.o $(OD)DsmMessage.o $(O)DsmTapeCtl.o $(O)MsgQueue.o $(OD)SerialBuf.o $(OD)TapeHeader.o $(O)Exb162.o

all: hdrClass dsmClass $(O)dsmAsync.o $(O)dsmComm.o $(O)maspComm.o $(O)dsmDisplay.o $(O)dsmTape.o $(O)vxtcp.o $(O)ck3118.o $(O)ck429.o $(O)ckapn232.o $(O)ckctr.o $(O)ckdio.o $(O)ckdsp.o $(O)ckgrey.o $(O)ckirs.o $(O)ckophir.o $(O)ckscsi.o $(O)cktans2.o $(O)cktans3.o $(O)cktelem.o $(O)cktfp.o $(O)cktod.o $(O)ckuvh.o $(O)ckv1d.o $(O)vxtcp.o $(O)vxudp.o $(O)fptst.o $(O)ckarinc.o $(O)ckarincgps.o $(O)ckppsgps.o $(O)ckdpres.o $(O)cksynchro.o $(O)ck2d.o $(O)ckcnt.o $(O)ck2dgrey.o $(O)ckjpltdl.o $(O)ckspp.o $(O)cklhtdl.o $(O)ckrdma.o $(O)ckstod.o

# $(O)cknonoy.o $(O)cknoaa.o $(O)ckozone.o


install:
	cp ../obj/* ${RELEASE}/dsm/obj

clean:
	rm ${O}/* ${L}/*

hdrClass:
	make -f $(HEADER)/class/Makefile

dsmClass:
	make -f ../class/Makefile

$(O)dsmAsync.o: $(S)dsmAsync.cc $(ASYNC.O) $(ASYNC.H)
	cc68k $(CFLAGS) $(S)dsmAsync.cc -o $(O)dsmAsync_st.o
	ld68k -r $(O)dsmAsync_st.o $(DSM_LIBS) \
	$(HDR_LIBS) $(DISC_LIBS) -o $(O)dsmAsync.o

$(O)dsmComm.o: $(S)dsmComm.cc $(COMM.O) $(COMM.H)
	cc68k $(CFLAGS) $(S)dsmComm.cc -o $(O)dsmComm_st.o
	ld68k -r $(O)dsmComm_st.o $(DISC_LIBS) $(DSM_LIBS) \
	-o $(O)dsmComm.o

$(O)maspComm.o: $(S)dsmComm.cc $(COMM.O) $(COMM.H)
	cc68k $(CFLAGS) -DFOR_MASP $(S)dsmComm.cc -o $(O)maspComm_st.o
	ld68k -r $(O)maspComm_st.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)maspComm.o

$(O)dsmDisplay.o: $(S)dsmDisplay.cc $(DISPLAY.O) $(DISPLAY.H)
	cc68k $(CFLAGS) $(S)dsmDisplay.cc -o $(O)dsmDisplay_st.o
	ld68k -r $(O)dsmDisplay_st.o $(DISC_LIBS) $(DSM_LIBS) \
	-o $(O)dsmDisplay.o

$(O)dsmTape.o: $(S)dsmTape.cc $(TAPE.O) $(TAPE.H)
	cc68k $(CFLAGS) $(S)dsmTape.cc -o $(O)dsmTape_st.o
	ld68k -r $(O)dsmTape_st.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)dsmTape.o

$(O)ck3118.o: $(S)ck3118.cc $(O)Vm3118.o $(O)AnaAuxTable.o $(O)Bc635Vme.o
	cc68k $(CFLAGS) $(S)ck3118.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)ck3118.o
	/bin/rm $(O)tmp.o

$(O)ck429.o: $(S)ck429.cc $(O)IP429.o
	cc68k $(CFLAGS) $(S)ck429.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)ck429.o
	/bin/rm $(O)tmp.o

$(O)ckapn232.o: $(S)ckapn232.cc $(O)APN232.o
	cc68k $(CFLAGS) $(S)ckapn232.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) \
	-o $(O)ckapn232.o
	/bin/rm $(O)tmp.o

$(O)ckctr.o: $(S)ckctr.cc $(O)Vmio14Count.o $(O)Vmio12.o 
	cc68k $(CFLAGS) $(S)ckctr.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)ckctr.o
	/bin/rm $(O)tmp.o

$(O)ckcnt.o: $(S)ckcnt.cc $(O)IPcounter.o
	cc68k $(CFLAGS) $(S)ckcnt.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)ckcnt.o
	/bin/rm $(O)tmp.o

$(O)ckdio.o: $(S)ckdio.cc $(O)Vmio14In.o $(O)Vmio14Out.o $(O)Vmio12.o 
	cc68k $(CFLAGS) $(S)ckdio.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)ckdio.o
	/bin/rm $(O)tmp.o

$(O)ckdsp.o: $(S)ckdsp.cc $(O)AnalogTable.o $(O)Bc635Vme.o $(I)dsmVmeDefs.h
	cc68k $(CFLAGS) $(S)ckdsp.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)ckdsp.o
	/bin/rm $(O)tmp.o

$(O)ckgrey.o: $(S)ckgrey.cc $(O)GreyVme.o $(I)dsmVmeDefs.h $(IL)header.h
	cc68k $(CFLAGS) $(S)ckgrey.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DSM_LIBS) $(DISC_LIBS) -o $(O)ckgrey.o
	/bin/rm $(O)tmp.o

$(O)ck2dgrey.o: $(S)ck2dgrey.cc $(O)GreyVme.o $(I)dsmVmeDefs.h $(IL)header.h
	cc68k $(CFLAGS) $(S)ck2dgrey.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DSM_LIBS) $(DISC_LIBS) -o $(O)ck2dgrey.o
	/bin/rm $(O)tmp.o

$(O)ckirs.o: $(S)ckirs.cc $(O)IP429.o $(O)HwIrs.o
	cc68k $(CFLAGS) $(S)ckirs.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)ckirs.o
	/bin/rm $(O)tmp.o

$(O)ckarincgps.o: $(S)ckarincgps.cc $(O)IP429.o $(O)HwGps.o
	cc68k $(CFLAGS) $(S)ckarincgps.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)ckarincgps.o
	/bin/rm $(O)tmp.o

$(O)ckarinc.o: $(S)ckarinc.cc $(O)IP429.o $(O)ArincGps.o
	cc68k $(CFLAGS) $(S)ckarinc.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)ckarinc.o
	/bin/rm $(O)tmp.o

$(O)ckjpltdl.o: $(S)ckjpltdl.cc $(O)JplTdl.o
	cc68k $(CFLAGS) $(S)ckjpltdl.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)ckjpltdl.o
	/bin/rm $(O)tmp.o

$(O)cklhtdl.o: $(S)cklhtdl.cc $(O)LhTdl.o
	cc68k $(CFLAGS) $(S)cklhtdl.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)cklhtdl.o
	/bin/rm $(O)tmp.o

$(O)ckrdma.o: $(S)ckrdma.cc $(O)Rdma.o
	cc68k $(CFLAGS) $(S)ckrdma.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)ckrdma.o
	/bin/rm $(O)tmp.o

$(O)ckneph.o: $(S)ckneph.cc $(O)Neph.o
	cc68k $(CFLAGS) $(S)ckneph.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)ckneph.o
	/bin/rm $(O)tmp.o

$(O)cknoaa.o: $(S)cknoaa.cc $(O)Noaa.o
	cc68k $(CFLAGS) $(S)cknoaa.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)cknoaa.o
	/bin/rm $(O)tmp.o

$(O)ckozone.o: $(S)ckozone.cc $(O)Ozone.o
	cc68k $(CFLAGS) $(S)ckozone.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)ckozone.o
	/bin/rm $(O)tmp.o

$(O)cknonoy.o: $(S)cknonoy.cc $(O)NoNoy.o
	cc68k $(CFLAGS) $(S)cknonoy.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)cknonoy.o
	/bin/rm $(O)tmp.o

$(O)ckdpres.o: $(S)ckdpres.cc $(O)Dpres.o $(O)IsioPort.o
	cc68k $(CFLAGS) $(S)ckdpres.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)ckdpres.o
	/bin/rm $(O)tmp.o

$(O)ckstod.o: $(S)ckstod.cc $(O)SerialTod.o $(O)IsioPort.o
	cc68k $(CFLAGS) $(S)ckstod.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)ckstod.o
	/bin/rm $(O)tmp.o

$(O)ckophir.o: $(S)ckophir.cc $(O)Ophir3.o
	cc68k $(CFLAGS) $(S)ckophir.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)ckophir.o
	/bin/rm $(O)tmp.o

$(O)ckscsi.o: $(S)ckscsi.cc $(O)Exb162.o
	cc68k $(CFLAGS) $(S)ckscsi.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)ckscsi.o
	/bin/rm $(O)tmp.o

$(O)cksynchro.o: $(S)cksynchro.cc $(O)Synchro.o
	cc68k $(CFLAGS) $(S)cksynchro.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)cksynchro.o
	/bin/rm $(O)tmp.o

$(O)cktans2.o: $(S)cktans2.cc $(O)GpsTans2.o $(O)IsioPort.o
	cc68k $(CFLAGS) $(S)cktans2.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)cktans2.o
	/bin/rm $(O)tmp.o

$(O)ckppsgps.o: $(S)ckppsgps.cc $(O)PpsGps.o $(O)IsioPort.o
	cc68k $(CFLAGS) $(S)ckppsgps.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)ckppsgps.o
	/bin/rm $(O)tmp.o

$(O)cktans3.o: $(S)cktans3.cc $(O)GpsTans3.o $(O)IsioPort.o
	cc68k $(CFLAGS) $(S)cktans3.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)cktans3.o
	/bin/rm $(O)tmp.o

$(O)cktelem.o: $(S)cktelem.cc $(O)IsioPort.o
	cc68k $(CFLAGS) $(S)cktelem.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)cktelem.o
	/bin/rm $(O)tmp.o

$(O)cktfp.o: $(S)cktfp.cc $(O)Bc635Vme.o
	cc68k $(CFLAGS) $(S)cktfp.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)cktfp.o
	/bin/rm $(O)tmp.o

$(O)cktod.o: $(S)cktod.cc $(O)TodClock.o
	cc68k $(CFLAGS) $(S)cktod.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)cktod.o
	/bin/rm $(O)tmp.o

$(O)ckuvh.o: $(S)ckuvh.cc $(O)UvHyg.o
	cc68k $(CFLAGS) $(S)ckuvh.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)ckuvh.o
	/bin/rm $(O)tmp.o

$(O)ckspp.o: $(S)ckspp.cc $(O)Spp.o 
	cc68k $(CFLAGS) $(S)ckspp.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)ckspp.o
	/bin/rm $(O)tmp.o

$(O)ckv1d.o: $(S)ckv1d.cc $(O)Pms1Vme.o
	cc68k $(CFLAGS) $(S)ckv1d.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)ckv1d.o
	/bin/rm $(O)tmp.o

$(O)ckvar.o: $(S)ckvar.cc 
	cc68k $(CFLAGS) $(S)ckvar.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)ckvar.o
	/bin/rm $(O)tmp.o

$(O)ck2d.o: $(S)ck2d.cc $(O)PmsVme2d.o $(O)Pms2dHouse.o $(O)Bc635Vme.o 
	cc68k $(CFLAGS) $(S)ck2d.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DSM_LIBS) $(DISC_LIBS) -o $(O)ck2d.o
	/bin/rm $(O)tmp.o

$(O)ckmass.o: $(S)ckmass.cc $(O)MassSpec.o $(O)IsioPort.o
	cc68k $(CFLAGS) $(S)ckmass.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)ckmass.o
	/bin/rm $(O)tmp.o

$(O)vxtcp.o: $(S)vxtcp.cc $(OD)TcpSocket.o 
	cc68k $(CFLAGS) $(S)vxtcp.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)vxtcp.o
	/bin/rm $(O)tmp.o

$(O)vxudp.o: $(S)vxudp.cc $(OD)UdpSocket.o
	cc68k $(CFLAGS) $(S)vxudp.cc -o $(O)tmp.o
	ld68k -r $(O)tmp.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)vxudp.o
	/bin/rm $(O)tmp.o

$(O)fptst.o: $(S)fptst.cc
	cc68k $(CFLAGS) $(S)fptst.cc -o $(O)fptst.o
