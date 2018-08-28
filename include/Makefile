#
#    Makefile for make_amlib program
#	make_amlib sorts amlib.fns and creates amlibfn.h
#
CC=	g++
CFLAGS=	-O

SRC=	make_amlib.cc
PROG=	make_amlib

all:	amlibfn.h svnInfo.h

svnInfo.h: .force
	./makeSvnInfoHeaderFile.sh

amlibfn.h:	amlib.fns make_amlib
	./make_amlib

xlatefns.h:	amlib.fns make_amlib
	./make_amlib

make_amlib: make_amlib.cc
	${CC} ${CFLAGS} make_amlib.cc -o make_amlib

clean:
	rm -f ${PROG} svnInfo.h amlibfn.h xlatefns.h amlibProto.h

.force:
