L=              $(DSM)/lib/
OC=             $(DSM)/obj/
OD=		$(DISC)/obj/
S=		./
O=		../obj/
I=		../include/
IC=		$(DSM)/include/
ID=		$(DISC)/include/
IH=             $(HEADER)/include/
IL=             $(LOCAL)/include/
LD=             $(DISC)/lib/

DSM_LIBS=       $(L)libdsm.a
DISC_LIBS=      $(LD)libvxdisc.a

FLAGS = -c -m68040 -D__cplusplus -msoft-float -fno-builtin -DCPU=MC68LC040 \
	-I/home/local/vw/target/h \
	-I/home/local/vw/target/config/masp \
	-I/home/local/vw/target/h/drv -I$(I) \
	-I$(IC) -I$(ID) -I$(IH) -I$(IL)

INCS = $(I)dasIncl.h \
          $(I)maspDas.h \
          $(I)masp.h \
          $(I)packet.h \
          $(I)template.h \
          $(I)vmema.h \
          $(I)aircraft.h \
          $(I)telSer.h  

DAS.O = $(OC)Bc635Vme.o $(OC)MsgQueue.o $(OC)TodClock.o $(OD)DsmConfig.o

all:  dsmClass $(O)maspDas.o 

dsmClass:
	make -f $(DSM)/class/Makefile
     
$(O)maspDas.o: $(S)maspDas.cc $(DAS.O) $(INCS)
	cc68k $(FLAGS) $(S)maspDas.cc -o $(O)maspDas_st.o
	ld68k -r $(O)maspDas_st.o $(DISC_LIBS) $(DSM_LIBS) -o $(O)maspDas.o
