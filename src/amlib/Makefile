DESTDIR=

SUBDIR= std user xlate
CC= "CC=g++"
CFLAGS= "COPS=-Wall -g -O2"

all: ${SUBDIR}

${SUBDIR}: FRC
	cd $@; make ${MFLAGS} ${CC} ${CFLAGS}

install clean lint print:
	for i in ${SUBDIR}; do						\
	    (cd $$i; make ${MFLAGS} DESTDIR=${DESTDIR} $@); done

FRC:
