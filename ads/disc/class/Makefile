S=              ../class/
I=              ../include/
O=              ../obj/
L=              ../lib/

ID=		../../dsm/include/
IL=		$(JLOCAL)/include/
IV=		$(WIND_BASE)/target/config/mv162lc/

#WRITEFLAGS = -DTAPE_WRITE -DDISK_WRITE
#WRITEFLAGS = -DTAPE_WRITE
WRITEFLAGS = -DDISK_WRITE

# Linux
POFLAGS =  -c -g $(WRITEFLAGS) -I$(I) -I$(IL) -I$(IW) -I/usr/X11R6/include
POFLAGS =  -c -g $(WRITEFLAGS) -I$(I) -I$(IL) -I/usr/X11R6/include

CPVFLAGS = -c -m68040 -msoft-float -DVXWORKS -DCPU=MC68LC040 \
	   -fno-builtin -nostdinc \
           -D__cpluscplus -I$(WIND_BASE)/target/h \
           -I$(I) -I$(ID) -I$(IL) -I$(IV)
#           -I$(I) -I$(ID) -I$(IL) -I$(IV) -I$(IW)
          
all: $(O)AnalogWin.o $(O)DiscNatsComm.o $(O)DsmCollect.o $(O)DsmConfig.o $(O)DsmHeaders.o $(O)DsmMessage.o $(O)DsmNet.o $(O)DsmNets.o $(O)DsmTables.o $(O)DsmTape.o $(O)DsmDisk.o $(O)Event.o $(O)FlightWin.o $(O)GenericButtons.o $(O)InstrumentWin.o $(O)McrWin.o $(O)NatsData.o $(O)Palt.o $(O)PickTable.o $(O)Pms1dWin.o $(O)Pms2dWin.o $(O)Queue.o $(O)RandomBuf.o $(O)StatusWin.o $(O)SerialBuf.o $(O)SyncQueue.o $(O)SyncVar.o $(O)TapeDrv.o $(O)TapeHeader.o $(O)Tasx.o $(O)TcpSocket.o $(O)TextFile.o $(O)UdpSocket.o $(O)UnixTask.o $(O)VarConfig.o $(O)portable.o

#$(O)GpsTime.o


ANALOGWIN.H= $(I)AnalogWin.h $(I)DsmConfig.h $(I)DsmHeaders.h $(I)DsmMessage.h $(I)TapeHeader.h $(I)dsmctl.h $(IL)header.h
DISCNATSCOMM.H= $(I)DiscNatsComm.h $(I)SerialBuf.h $(I)natsDefs.h
DSMCOLLECT.H= $(I)DsmCollect.h $(O)DsmConfig.o $(I)DsmHeaders.h $(I)DsmMessage.h $(I)DsmTables.h $(I)RandomBuf.h $(I)TapeHeader.h $(I)dsmctl.h $(IL)header.h
DSMCONFIG.H= $(I)DsmConfig.h $(I)dsmctl.h
DSMHEADERS.H= $(I)DsmHeaders.h $(I)DsmConfig.h $(I)TapeHeader.h $(I)dsmctl.h
DSMMESSAGE.H= $(I)DsmMessage.h $(I)DsmNet.h $(IL)header.h $(I)messageDefs.h $(ID)MsgQueue.h
DSMNET.H= $(I)DsmNet.h $(I)dsmctl.h $(IL)header.h $(I)messageDefs.h $(I)SerialBuf.h $(I)TcpSocket.h
DSMNETS.H= $(I)DsmNets.h $(I)DsmConfig.h $(I)dsmctl.h
DSMTABLES.H= $(I)DsmTables.h $(I)DsmConfig.h $(I)DsmHeaders.h $(O)PickTable.o $(I)TapeHeader.h $(I)dsmctl.h 
DSMTAPE.H= $(I)DsmTape.h $(I)DsmMessage.h $(I)TapeDrv.h $(I)TapeHeader.h $(I)dsmctl.h
DSMDISK.H= $(I)DsmDisk.h $(I)DsmMessage.h $(I)DsmConfig.h $(I)TapeHeader.h $(I)dsmctl.h $(I)UnixTask.h
EVENT.H= $(I)DsmConfig.h $(I)UdpSocket.h $(IL)Events.h
DAT.H= $(I)TapeDrv.h 
FLIGHTWIN.H= $(I)FlightWin.h 
GENBUTT.H= $(I)GenericButtons.h 
GPSTIME.H= $(I)GpsTime.h 
INST.H= $(I)InstrumentWin.h
MCRWIN.H= $(I)McrWin.h $(I)DsmMessage.h $(I)dsmctl.h $(I)messageDefs.h $(IL)header.h
MESSAGEWIN.H= $(I)MessageWin.h $(I)messageDefs.h 
NATSDATA.H= $(I)NatsData.h $(O)PickTable.o $(I)RandomBuf.h $(I)TapeHeader.h $(IL)header.h 
NETWORK.H= $(I)NetworkWin.h $(O)DsmConfig.o $(I)dsmctl.h $(I)messageDefs.h 
PALT.H= $(I)Palt.h
PALT.O= $(O)SyncVar.o
PICKTABLE.H= $(I)PickTable.h $(I)dsmctl.h
PMS1DWIN.H= $(I)Pms1dWin.h $(I)DsmMessage.h $(I)dsmctl.h $(IL)header.h
PMS2DWIN.H= $(I)Pms2dWin.h $(I)DsmMessage.h $(I)dsmctl.h $(IL)header.h
STATUSWIN.H= $(I)StatusWin.h 
QUEUE.H= $(I)Queue.h
QUITWIN.H= $(I)QuitWin.h 
RANDOMBUF.H= $(I)RandomBuf.h $(I)dsmctl.h
RECORDWIN.H= $(I)RecordWin.h $(I)DsmMessage.h
SERIALBUF.H= $(I)SerialBuf.h $(I)dsmctl.h
SYNCQUEUE.H= $(I)SyncQueue.h
SYNCVAR.H= $(I)SyncVar.h $(I)TapeHeader.h $(IL)header.h
TAPEHEADER.H= $(I)TapeHeader.h $(IL)header.h $(I)dsmctl.h
TASX.H= $(I)Tasx.h
TASX.O= $(O)SyncVar.o
TEXTFILE.H= $(I)TextFile.h
TCPSOCKET.H= $(I)TcpSocket.h $(I)dsmctl.h
UDPSOCKET.H= $(I)UdpSocket.h $(I)dsmctl.h
UNIXTASK.H= $(I)UnixTask.h $(I)dsmctl.h
VARCONFIG.H= $(I)VarConfig.h $(IL)header.h

$(O)AnalogWin.o: $(S)AnalogWin.cc $(ANALOGWIN.H) 
	g++ $(POFLAGS) $(S)AnalogWin.cc -o $(O)AnalogWin.o
	ar r $(L)libdisc.a $(O)AnalogWin.o

$(O)DiscNatsComm.o: $(S)DiscNatsComm.cc $(DISCNATSCOMM.H) 
	g++ $(POFLAGS) $(S)DiscNatsComm.cc -o $(O)DiscNatsComm.o
	ar r $(L)libdisc.a $(O)DiscNatsComm.o

$(O)DsmCollect.o: $(S)DsmCollect.cc $(DSMCOLLECT.H) 
	g++ $(POFLAGS) $(S)DsmCollect.cc -o $(O)DsmCollect.o
	ar r $(L)libdisc.a $(O)DsmCollect.o

$(O)DsmConfig.o: $(S)DsmConfig.cc $(DSMCONFIG.H) 
	g++ $(POFLAGS) $(S)DsmConfig.cc -o $(O)DsmConfig.o
	ar r $(L)libdisc.a $(O)DsmConfig.o
	cc68k $(CPVFLAGS) $(S)DsmConfig.cc -o $(O)DsmConfig.o
	ar68k r $(L)libvxdisc.a $(O)DsmConfig.o
	ranlib68k $(L)libvxdisc.a

$(O)DsmHeaders.o: $(S)DsmHeaders.cc $(DSMHEADERS.H)
	g++ $(POFLAGS) $(S)DsmHeaders.cc -o $(O)DsmHeaders.o
	ar r $(L)libdisc.a $(O)DsmHeaders.o

$(O)DsmMessage.o: $(S)DsmMessage.cc $(DSMMESSAGE.H)
	g++ $(POFLAGS) $(S)DsmMessage.cc -o $(O)DsmMessage.o
	ar r $(L)libdisc.a $(O)DsmMessage.o
	cc68k $(CPVFLAGS) $(S)DsmMessage.cc -o $(O)DsmMessage.o
	ar68k r $(L)libvxdisc.a $(O)DsmMessage.o
	ranlib68k $(L)libvxdisc.a

$(O)DsmNet.o: $(S)DsmNet.cc $(DSMNET.H) $(O)DsmConfig.o
	g++ $(POFLAGS) $(S)DsmNet.cc -o $(O)DsmNet.o
	ar r $(L)libdisc.a $(O)DsmNet.o
	cc68k $(CPVFLAGS) $(S)DsmNet.cc -o $(O)DsmNet.o
	ar68k r $(L)libvxdisc.a $(O)DsmNet.o
	ranlib68k $(L)libvxdisc.a

$(O)DsmNets.o: $(S)DsmNets.cc $(DSMNETS.H)
	g++ $(POFLAGS) $(S)DsmNets.cc -o $(O)DsmNets.o
	ar r $(L)libdisc.a $(O)DsmNets.o

$(O)DsmTables.o: $(S)DsmTables.cc $(DSMTABLES.H) 
	g++ $(POFLAGS) $(S)DsmTables.cc -o $(O)DsmTables.o
	ar r $(L)libdisc.a $(O)DsmTables.o

$(O)DsmTape.o: $(S)DsmTape.cc $(DSMTAPE.H) 
	g++ $(POFLAGS) $(S)DsmTape.cc -o $(O)DsmTape.o
	ar r $(L)libdisc.a $(O)DsmTape.o

$(O)DsmDisk.o: $(S)DsmDisk.cc $(DSMDISK.H) 
	g++ $(POFLAGS) $(S)DsmDisk.cc -o $(O)DsmDisk.o
	ar r $(L)libdisc.a $(O)DsmDisk.o

$(O)Event.o: $(S)Event.cc $(EVENT.H) 
	g++ $(POFLAGS) $(S)Event.cc -o $(O)Event.o
	ar r $(L)libdisc.a $(O)Event.o

$(O)FlightWin.o: $(S)FlightWin.cc $(FLIGHTWIN.H) 
	g++ $(POFLAGS) $(S)FlightWin.cc -o $(O)FlightWin.o
	ar r $(L)libdisc.a $(O)FlightWin.o

$(O)GenericButtons.o: $(S)GenericButtons.cc $(GENBUTT.H) 
	g++ $(POFLAGS) $(S)GenericButtons.cc -o $(O)GenericButtons.o
	ar r $(L)libdisc.a $(O)GenericButtons.o

$(O)GpsTime.o: $(S)GpsTime.cc $(GPSTIME.H) 
	g++ $(POFLAGS) $(S)GpsTime.cc -o $(O)GpsTime.o
	ar r $(L)libdisc.a $(O)GpsTime.o

$(O)InstrumentWin.o: $(S)InstrumentWin.cc $(INST.H) 
	g++ $(POFLAGS) $(S)InstrumentWin.cc -o $(O)InstrumentWin.o
	ar r $(L)libdisc.a $(O)InstrumentWin.o

$(O)McrWin.o: $(S)McrWin.cc $(MCRWIN.H) 
	g++ $(POFLAGS) $(S)McrWin.cc -o $(O)McrWin.o
	ar r $(L)libdisc.a $(O)McrWin.o

$(O)MessageWin.o: $(S)MessageWin.cc $(MESSAGEWIN.H) 
	g++ $(POFLAGS) $(S)MessageWin.cc -o $(O)MessageWin.o
	ar r $(L)libdisc.a $(O)MessageWin.o

$(O)NatsData.o: $(S)NatsData.cc $(NATSDATA.H) 
	g++ $(POFLAGS) $(S)NatsData.cc -o $(O)NatsData.o
	ar r $(L)libdisc.a $(O)NatsData.o
	cc68k $(CPVFLAGS) $(S)NatsData.cc -o $(O)NatsData.o
	ar68k r $(L)libvxdisc.a $(O)NatsData.o
	ranlib68k $(L)libvxdisc.a

$(O)NetworkWin.o: $(S)NetworkWin.cc $(NETWORKWIN.H) 
	g++ $(POFLAGS) $(S)NetworkWin.cc -o $(O)NetworkWin.o
	ar r $(L)libdisc.a $(O)NetworkWin.o

$(O)Palt.o: $(S)Palt.cc $(PALT.H) $(PALT.O) 
	g++ $(POFLAGS) $(S)Palt.cc -o $(O)Palt.o
	ar r $(L)libdisc.a $(O)Palt.o
	cc68k $(CPVFLAGS) $(S)Palt.cc -o $(O)Palt.o
	ar68k r $(L)libvxdisc.a $(O)Palt.o
	ranlib68k $(L)libvxdisc.a

$(O)PickTable.o: $(S)PickTable.cc $(PICKTABLE.H) 
	g++ $(POFLAGS) $(S)PickTable.cc -o $(O)PickTable.o
	ar r $(L)libdisc.a $(O)PickTable.o
	cc68k $(CPVFLAGS) $(S)PickTable.cc -o $(O)PickTable.o
	ar68k r $(L)libvxdisc.a $(O)PickTable.o
	ranlib68k $(L)libvxdisc.a

$(O)Pms1dWin.o: $(S)Pms1dWin.cc $(PMS1DWIN.H) 
	g++ $(POFLAGS) $(S)Pms1dWin.cc -o $(O)Pms1dWin.o
	ar r $(L)libdisc.a $(O)Pms1dWin.o

$(O)Pms2dWin.o: $(S)Pms2dWin.cc $(PMS2DWIN.H) 
	g++ $(POFLAGS) $(S)Pms2dWin.cc -o $(O)Pms2dWin.o
	ar r $(L)libdisc.a $(O)Pms2dWin.o

$(O)StatusWin.o: $(S)StatusWin.cc $(STATUSWIN.H) 
	g++ $(POFLAGS) $(S)StatusWin.cc -o $(O)StatusWin.o
	ar r $(L)libdisc.a $(O)StatusWin.o

$(O)Queue.o: $(S)Queue.cc $(QUEUE.H) 
	g++ $(POFLAGS) $(S)Queue.cc -o $(O)Queue.o
	ar r $(L)libdisc.a $(O)Queue.o

$(O)QuitWin.o: $(S)QuitWin.cc $(QUITWIN.H) 
	g++ $(POFLAGS) $(S)QuitWin.cc -o $(O)QuitWin.o
	ar r $(L)libdisc.a $(O)QuitWin.o

$(O)RandomBuf.o: $(S)RandomBuf.cc $(RANDOMBUF.H) 
	g++ $(POFLAGS) $(S)RandomBuf.cc -o $(O)RandomBuf.o
	ar r $(L)libdisc.a $(O)RandomBuf.o
	cc68k $(CPVFLAGS) $(S)RandomBuf.cc -o $(O)RandomBuf.o
	ar68k r $(L)libvxdisc.a $(O)RandomBuf.o
	ranlib68k $(L)libvxdisc.a

$(O)RecordWin.o: $(S)RecordWin.cc $(RECORDWIN.H) 
	g++ $(POFLAGS) $(S)RecordWin.cc -o $(O)RecordWin.o
	ar r $(L)libdisc.a $(O)RecordWin.o

$(O)SerialBuf.o: $(S)SerialBuf.cc $(SERIALBUF.H) 
	g++ $(POFLAGS) $(S)SerialBuf.cc -o $(O)SerialBuf.o
	ar r $(L)libdisc.a $(O)SerialBuf.o
	cc68k $(CPVFLAGS) $(S)SerialBuf.cc -o $(O)SerialBuf.o
	ar68k r $(L)libvxdisc.a $(O)SerialBuf.o
	ranlib68k $(L)libvxdisc.a

$(O)SyncQueue.o: $(S)SyncQueue.cc $(SYNCQUEUE.H) 
	g++ $(POFLAGS) $(S)SyncQueue.cc -o $(O)SyncQueue.o
	ar r $(L)libdisc.a $(O)SyncQueue.o

$(O)SyncVar.o: $(S)SyncVar.cc $(SYNCVAR.H) 
	g++ $(POFLAGS) $(S)SyncVar.cc -o $(O)SyncVar.o
	ar r $(L)libdisc.a $(O)SyncVar.o
	cc68k $(CPVFLAGS) $(S)SyncVar.cc -o $(O)SyncVar.o
	ar68k r $(L)libvxdisc.a $(O)SyncVar.o
	ranlib68k $(L)libvxdisc.a

$(O)TapeDrv.o: $(S)TapeDrv.cc $(DAT.H) 
	g++ $(POFLAGS) $(S)TapeDrv.cc -o $(O)TapeDrv.o
	ar r $(L)libdisc.a $(O)TapeDrv.o

$(O)TapeHeader.o: $(S)TapeHeader.cc $(TAPEHEADER.H)  
	g++ $(POFLAGS) $(S)TapeHeader.cc -o $(O)TapeHeader.o
	ar r $(L)libdisc.a $(O)TapeHeader.o
	cc68k $(CPVFLAGS) $(S)TapeHeader.cc -o $(O)TapeHeader.o
	ar68k r $(L)libvxdisc.a $(O)TapeHeader.o
	ranlib68k $(L)libvxdisc.a

$(O)Tasx.o: $(S)Tasx.cc $(TASX.H) $(TASX.O)
	g++ $(POFLAGS) $(S)Tasx.cc -o $(O)Tasx.o
	ar r $(L)libdisc.a $(O)Tasx.o
	cc68k $(CPVFLAGS) $(S)Tasx.cc -o $(O)Tasx.o
	ar68k r $(L)libvxdisc.a $(O)Tasx.o
	ranlib68k $(L)libvxdisc.a

$(O)TcpSocket.o: $(S)TcpSocket.cc $(TCPSOCKET.H) 
	g++ $(POFLAGS) $(S)TcpSocket.cc -o $(O)TcpSocket.o
	ar r $(L)libdisc.a $(O)TcpSocket.o
	cc68k $(CPVFLAGS) $(S)TcpSocket.cc -o $(O)TcpSocket.o
	ar68k r $(L)libvxdisc.a $(O)TcpSocket.o
	ranlib68k $(L)libvxdisc.a

$(O)TextFile.o: $(S)TextFile.cc $(TEXTFILE.H) 
	g++ $(POFLAGS) $(S)TextFile.cc -o $(O)TextFile.o
	ar r $(L)libdisc.a $(O)TextFile.o
	cc68k $(CPVFLAGS) $(S)TextFile.cc -o $(O)TextFile.o
	ar68k r $(L)libvxdisc.a $(O)TextFile.o
	ranlib68k $(L)libvxdisc.a

$(O)UdpSocket.o: $(S)UdpSocket.cc $(UDPSOCKET.H) 
	g++ $(POFLAGS) $(S)UdpSocket.cc -o $(O)UdpSocket.o
	ar r $(L)libdisc.a $(O)UdpSocket.o
	cc68k $(CPVFLAGS) $(S)UdpSocket.cc -o $(O)UdpSocket.o
	ar68k r $(L)libvxdisc.a $(O)UdpSocket.o
	ranlib68k $(L)libvxdisc.a

$(O)UnixTask.o: $(S)UnixTask.cc $(UNIXTASK.H) 
	g++ $(POFLAGS) $(S)UnixTask.cc -o $(O)UnixTask.o
	ar r $(L)libdisc.a $(O)UnixTask.o

$(O)VarConfig.o: $(S)VarConfig.cc $(VARCONFIG.H) 
	g++ $(POFLAGS) $(S)VarConfig.cc -o $(O)VarConfig.o
	ar r $(L)libdisc.a $(O)VarConfig.o
	cc68k $(CPVFLAGS) $(S)VarConfig.cc -o $(O)VarConfig.o
	ar68k r $(L)libvxdisc.a $(O)VarConfig.o
	ranlib68k $(L)libvxdisc.a

$(O)portable.o: $(S)portable.cc $(I)portable.h
	g++ $(POFLAGS) $(S)portable.cc -o $(O)portable.o
	ar r $(L)libdisc.a $(O)portable.o
	cc68k $(CPVFLAGS) $(S)portable.cc -o $(O)portable.o
	ar68k r $(L)libvxdisc.a $(O)portable.o
	ranlib68k $(L)libvxdisc.a
