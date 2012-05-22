#
#    Makefile for hdrdump & ADScvt
#
DESTDIR=
BINDIR=		/opt/local/bin
MANDIR=		/usr/local/man/manl
INCDIR=		/opt/local/include
LIBS=		-L/opt/local/lib -lhdr_api
LINTLIBS=	/usr/local/lib/lint/llib-lhdr_api.ln
INCLUDES=	-I$(INCDIR)
CC=		gcc
CFLAGS=		-g ${INCLUDES}

SRCS=	hdrdump.c ADScvt.c ohdump.c hd.c
PROGS=	hdrdump ADScvt ohdump hd

.c.o:
	${CC} ${CFLAGS} -c $*.c

all:	hdrdump ADScvt ohdump hd

hdrdump: hdrdump.c
	${CC} ${CFLAGS} hdrdump.c ${LIBS} -o $@

hd: hd.c
	${CC} ${CFLAGS} hd.c ${LIBS} -o $@

ohdump: ohdump.c
	${CC} ${CFLAGS} ohdump.c -o $@

ADScvt: ADScvt.c
	${CC} ${CFLAGS} ADScvt.c -o $@

hdrcvt852: hdrcvt852.c
	${CC} ${CFLAGS} hdrcvt852.c -lhdr_api1 -o $@

install: ${PROG}
	cp hdrdump ${DESTDIR}${BINDIR}
	cp ADScvt ${DESTDIR}${BINDIR}
	cp ohdump ${DESTDIR}${BINDIR}

clean:
	rm -f ${OBJS} ${PROGS}

lint:
	lint ${INCLUDES} ${SRCS} ${LINTLIBS}

print:
	enscript -2Gr -bHdrUtils ${HDRS} ${SRCS}

hdrdump.o:	$(INCDIR)/ctape.h $(INCDIR)/header.h
ohdump.o:	./oldhdr.h
hdrcvt852.o:	$(INCDIR)/ctape.h $(INCDIR)/header.h
ADScvt.o:	$(INCDIR)/header.h
