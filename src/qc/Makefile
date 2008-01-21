#
# Makefile for Valkyrie quality control system
#


# (general definitions)
RELEASE=	${JLOCAL}/dev/nimbus/src/qc
make = make



#
# 'all' rule
#
# build entire system
#
all:
	cd ../../include; $(make)
	cd socklib; $(make)
	cd vallib; $(make)
	cd timerlib; $(make)
	cd gui; $(make)


#
# 'clean' rule
#
# clean up entire system
#
clean:
	cd socklib; $(make) clean
	cd vallib; $(make) clean
	cd timerlib; $(make) clean
	cd gui; $(make) clean



#
# 'backup' rule
#
# puts a backup of the system in your home directory and puts the
# previous backup in $(HOME)/bkup
#
compress = gzip
compress-flags = -v -f
compress-suffix = gz

backup-name = qc
backup-suffix = $(compress-suffix)

backup: 
	touch $(HOME)/$(backup-name).tar
	rm $(HOME)/$(backup-name).tar 
	touch $(HOME)/$(backup-name).tar.$(backup-suffix)
	mv $(HOME)/$(backup-name).tar.$(backup-suffix) $(HOME)/bkup
	cd ..;tar cf $(HOME)/$(backup-name).tar $(backup-name)
	$(compress) $(compress-flags) $(HOME)/$(backup-name).tar


#
# 'release' rule
#
# copy entire directory hierarchy to $(RELEASE) -- this is a NCAR convention
#
release:
	(tar cFFf - . | (cd ${RELEASE}; tar xfBp -))
	rm -f ${RELEASE}/LOCK.make


#
# 'install' rule
#
# install files in their appropriate WINPUT/WINDS destinations
#
install: all
	cp vallib/qc.h ../../include
	cp vallib/libval.a ../../lib
	cp man/qc.3 ${JLOCAL}/man/man3
#	cp gui/qc-gui $(WINDS)/bin
