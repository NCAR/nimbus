#
#    Makefile for make_amlib program
#	make_amlib sorts amlib.fns and creates amlibfn.h
#
CC=	gcc
CFLAGS=	-O

SRC=	make_amlib.c
PROG=	make_amlib

all:	make_amlib amlibfn.h

amlibfn.h:	amlib.fns make_amlib
	make_amlib

xlatefns.h:	amlib.fns make_amlib
	make_amlib

make_amlib: make_amlib.c
	${CC} ${CFLAGS} make_amlib.c -o make_amlib
