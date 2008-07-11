# This Makefile only re-compiles the "dsm" software, currently does nothing
# for old ADS software.
#
DESTDIR=

SUBDIR= header disc dsm

all: ${SUBDIR}

${SUBDIR}: FRC
	cd $@; make ${MFLAGS}
#	cd $@; pmake -k ${MFLAGS}

install clean lint print:
	for i in ${SUBDIR}; do						\
	    (cd $$i; make ${MFLAGS} DESTDIR=${DESTDIR} $@); done
#	    (cd $$i; pmake -k ${MFLAGS} DESTDIR=${DESTDIR} $@); done

FRC:
