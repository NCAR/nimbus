S=              ./
I=		../include/
O=              ../obj/
B=              ../bin/
L=		../lib/
IL=		$(JLOCAL)/include/
RELEASE=	${JLOCAL}/ads/

#WRITEFLAGS = -DTAPE_WRITE -DDISK_WRITE
#WRITEFLAGS = -DTAPE_WRITE
WRITEFLAGS = -DDISK_WRITE
 
# Linux
CFLAGS =  -g $(WRITEFLAGS) -lm -lnsl -I$(I) -I$(IL)
PFLAGS =  -g $(WRITEFLAGS) -I$(I) -I$(IL)
DISC_LIBS=	$(L)libdisc.a -lnsl
X11_LIBS=      -L/usr/X11R6/lib -lXm -lXt -lX11 -lXp -lXext
          
COMM.O = $(O)DiscNatsComm.o $(O)DsmCollect.o $(O)DsmConfig.o $(O)DsmHeaders.o $(O)DsmMessage.o $(O)DsmNet.o $(O)DsmNets.o $(O)DsmTables.o $(O)NatsData.o $(O)Palt.o $(O)RandomBuf.o $(O)SyncQueue.o $(O)SyncVar.o $(O)TapeHeader.o $(O)Tasx.o $(O)TcpSocket.o $(O)UdpSocket.o $(O)Queue.o

TAPE.O = $(O)DsmConfig.o $(O)DsmMessage.o $(O)DsmNet.o $(O)DsmTape.o $(O)TapeHeader.o $(O)TcpSocket.o $(O)TapeDrv.o $(O)DsmDisk.o $(O)UnixTask.o

WIN.O = $(O)AnalogWin.o $(O)DsmConfig.o $(O)DsmHeaders.o $(O)DsmMessage.o $(O)DsmNet.o $(O)FlightWin.o $(O)InstrumentWin.o $(O)McrWin.o $(O)Pms1dWin.o $(O)Pms2dWin.o $(O)StatusWin.o $(O)TapeHeader.o $(O)TcpSocket.o $(O)UdpSocket.o $(O)UnixTask.o $(O)GenericButtons.o $(O)TextFile.o

SIM.O = $(O)DsmNet.o $(O)TcpSocket.o

COMM.H = $(IL)header.h $(I)dsmctl.h ${IL}Events.h

TAPE.H = $(IL)header.h $(I)dsmctl.h 

WIN.H = 

SIM.H = $(IL)header.h

all: discClass hdrClass $(B)bldhdr3 $(B)disphdr $(B)discComm $(B)discTape $(B)discWin $(B)ckcfg $(B)cknats $(B)suntcp $(B)sunudp $(B)tcprecv $(B)tcpsend $(B)windsudp $(B)discSim $(B)simRcv

bldhdr3: $(B)bldhdr3
disphdr: $(B)disphdr
discComm: $(B)discComm
discTape: $(B)discTape
discWin: $(B)discWin
discSim: $(B)discSim
simRcv: $(B)simRcv
suntcp: $(B)suntcp
sunudp: $(B)sunudp
exatst: $(B)exatst

discClass:
	cd ../class; make

hdrClass:
	cd ../../header/class; make

clean:
	rm -f ${O}/* ${L}/* ${B}/ck* ${B}/disc*

install:
	cp ../bin/* ${RELEASE}/disc/bin


$(B)bldhdr3: $(S)bldhdr3.c $(IL)header.h
	gcc $(CFLAGS) bldhdr3.c -o $(B)bldhdr3

$(B)disphdr: ${S}disphdr.cc $(O)TapeHeader.o $(IL)header.h
	g++ $(PFLAGS) disphdr.cc $(DISC_LIBS) -o $(B)disphdr

$(B)discComm: ${S}discComm.cc $(COMM.O) $(COMM.H)
	g++ $(PFLAGS) discComm.cc $(DISC_LIBS) -lpthread -o $(B)discComm

$(B)discTape: ${S}discTape.cc $(TAPE.O) $(TAPE.H)
	g++ $(PFLAGS) discTape.cc $(DISC_LIBS) -lpthread -o $(B)discTape

$(B)discWin: $(S)discWin.cc $(WIN.O) $(WIN.H)
	g++ $(PFLAGS) discWin.cc $(DISC_LIBS) $(X11_LIBS) -o $(B)discWin

$(B)discSim: $(S)discSim.cc $(SIM.O) $(SIM.H)
	g++ $(PFLAGS) discSim.cc $(DISC_LIBS) -o $(B)discSim

$(B)simRcv: $(S)simRcv.cc $(SIM.O) $(SIM.H)
	g++ $(PFLAGS) simRcv.cc $(DISC_LIBS) -o $(B)simRcv

$(B)suntcp: suntcp.cc $(O)TcpSocket.o $(O)SerialBuf.o
	g++ $(PFLAGS) suntcp.cc $(DISC_LIBS) -o $(B)suntcp

$(B)sunudp: sunudp.cc $(O)UdpSocket.o $(O)SerialBuf.o
	g++ $(PFLAGS) sunudp.cc $(DISC_LIBS) -o $(B)sunudp

$(B)tcprecv: tcprecv.cc $(O)TcpSocket.o $(O)SerialBuf.o
	g++ $(PFLAGS) tcprecv.cc $(DISC_LIBS) -o $(B)tcprecv

$(B)tcpsend: tcpsend.cc $(O)TcpSocket.o $(O)SerialBuf.o
	g++ $(PFLAGS) tcpsend.cc $(DISC_LIBS) -o $(B)tcpsend

$(B)windsudp: windsudp.cc $(O)UdpSocket.o 
	g++ $(PFLAGS) windsudp.cc $(DISC_LIBS) -o $(B)windsudp

$(B)ckcfg: $(S)ckcfg.cc $(O)DsmConfig.o $(O)VarConfig.o
	g++ $(PFLAGS) ckcfg.cc $(DISC_LIBS) -o $(B)ckcfg

$(B)cknats: $(S)cknats.cc $(O)DiscNatsComm.o
	g++ $(PFLAGS) cknats.cc $(DISC_LIBS) -o $(B)cknats

#$(B)exatst: $(S)exatst.cc $(O)Exa.o  
#	g++ $(PFLAGS) exatst.cc $(DISC_LIBS) -o $(B)exatst

#$(B)exatst1: $(S)exatst1.cc $(O)Exa.o  
#	g++ $(PFLAGS) exatst1.cc $(DISC_LIBS) -o $(B)exatst1
