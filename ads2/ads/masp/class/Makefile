S=	./
O=	../obj/
I=	../include/


CXXFLAGS = -c -m68040 -D__cplusplus -msoft-float -fno-builtin -DCPU=MC68LC040 \
           -I/home/local/vw/target/h \
           -I/home/local/vw/target/config/masp \
           -I/home/local/vw/target/h/drv -I$(I)


all:  $(O)Bc635Vme.o ${O}TodClock.o ${O}MsgQueue.o 


BC635VME.H = $(I)Bc635Vme.h
TODCLOCK.H = $(I)TodClock.h


$(O)Bc635Vme.o: $(S)Bc635Vme.cc $(BC635VME.H)
        cc68k $(CXXFLAGS) $(S)Bc635Vme.cc -o $(O)Bc635Vme.o
        ${AR68K} r $(L)libdsm.a $(O)Bc635Vme.o
        ${RL68K} $(L)libdsm.a

$(O)MsgQueue.o: $(S)MsgQueue.cc $(I)MsgQueue.h
        cc68k $(CFLAGS) $(S)MsgQueue.cc -o $(O)MsgQueue.o
        ${AR68K} r $(L)libdsm.a $(O)MsgQueue.o
        ${RL68K} $(L)libdsm.a

$(O)TodClock.o: $(S)TodClock.cc $(TODCLOCK.H)
        cc68k $(CFLAGS) $(S)TodClock.cc -o $(O)TodClock.o
        ${AR68K} r $(L)libdsm.a $(O)TodClock.o
        ${RL68K} $(L)libdsm.a
