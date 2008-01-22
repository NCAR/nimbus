#
#    Makefile for Skeleton program
#
CC=		gcc
LIB_DIRS=	-L/usr/X11R6/lib -L${JLOCAL}/lib
LIBS=		-lraf -lm -Wl,-Bstatic -lnetcdf -Wl,-Bdynamic -lXm -lXt -lX11
INCLUDES=	-I${JLOCAL}/include
CFLAGS=		-Wall -g -O2 ${INCLUDES}

SRCS=	skel.c globals.c init.c timeseg.c sort.c amlib.c search.c lrloop.c\
	compute.c netcdf.c cb_main.c cb_time.c cb_pause.c Xwin.c error.c

HDRS=	define.h

OBJS=	skel.o globals.o init.o timeseg.o sort.o amlib.o search.o lrloop.o\
	compute.o netcdf.o cb_main.o cb_time.o cb_pause.o Xwin.o error.o

PROG=	skel

.c.o:
	${CC} ${CFLAGS} -c $*.c

${PROG}: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} ${LIB_DIRS} ${LIBS} -o $@

clean:
	rm -f ${OBJS} ${PROG}

print:
	enscript -2Gr -b${PROG} ${HDRS} ${SRCS}

${OBJS}:	define.h
netcdf.o:	${JLOCAL}/include/netcdf.h
