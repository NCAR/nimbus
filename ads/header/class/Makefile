S=		$(HEADER)/class/
I=		$(HEADER)/include/
O=              $(HEADER)/obj/
L=              $(HEADER)/lib/
IL=		$(JLOCAL)/include/

POFLAGS =  -c -g -DSVR4 -I$(I) -I$(IL)
CPVFLAGS = -c -O -m68040 -msoft-float -DVXWORKS -fno-builtin -nostdinc \
           -D__cpluscplus -DCPU=MC68LC040 -I$(WIND_BASE)/target/h \
           -I$(I) -I$(IL)

all: $(O)Ophir3Blk.o $(O)UvBlk.o

OPHIR3BLK.H = $(I)Ophir3Blk.h $(IL)header.h
UVBLK.H = $(I)UvBlk.h $(IL)header.h

clean:
	rm ${O}/* ${L}/*

$(O)Ophir3Blk.o: $(S)Ophir3Blk.cc $(OPHIR3BLK.H)
	gcc $(POFLAGS) $(S)Ophir3Blk.cc -o $(O)Ophir3Blk.o
	ar r $(L)libheader.a $(O)Ophir3Blk.o
	cc68k $(CPVFLAGS) $(S)Ophir3Blk.cc -o $(O)Ophir3Blk.o
	ar68k rs $(L)libvxheader.a $(O)Ophir3Blk.o
	ranlib68k $(L)libvxheader.a

$(O)UvBlk.o: $(S)UvBlk.cc $(UVBLK.H)
	gcc $(POFLAGS) $(S)UvBlk.cc -o $(O)UvBlk.o
	ar r $(L)libheader.a $(O)UvBlk.o
	cc68k $(CPVFLAGS) $(S)UvBlk.cc -o $(O)UvBlk.o
	ar68k rs $(L)libvxheader.a $(O)UvBlk.o
	ranlib68k $(L)libvxheader.a
