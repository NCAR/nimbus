#
# Makefile for Valkyrie validation library
#


###############################################################################
#
# description of development environment
#

#
# directories where various classes of files are stored 
#
src=.
inc=.
obj=.
out=.

# directory for socket library code 

sock=../socklib

#
# unix tools and command line arguments
#

# for generating libraries
ar = ar
ar-flags = cr

# to compile
compile-c++ = g++
compile-c++-flags = -c -ansi -O -I$(inc) -I$(sock) $(intflags) \
	$(extflags) 

compile-c = gcc
compile-c-flags = -c -ansi -O -I$(inc) -I$(sock) $(intflags) \
	$(extflags) 

# to link
link = g++
link-flags = -g -L$(out) 

# to compress backups
compress = gzip
compress-flags = -v -f
compress-suffix = gz

# to optimize libraries
ranlib = ranlib
ranlib-flags = 

backup-name = $(project-name)
backup-suffix = $(compress-suffix)

#
# this word summarizes the entire project (for backups, etc.)
#
project-name = vallib


##
#
# targets and object files for our various end-targets
#
##


# -DVAL_DIAGS         --> general validation diagnostics
# -DVAL_USEGUI        --> turn on GUI
# -DVAL_DISKLOG       --> turn on disk logging

#val-mode = -DVAL_DIAGS -DVAL_USEGUI -DVAL_DISKLOG
#val-mode = -DVAL_USEGUI -DVAL_DISKLOG
#val-mode = -DVAL_DISKLOG


# !!! ultimately just in /home/local/bin ?
val-guipath = -DVAL_GUIPATH=\"$(WINDS)/bin/qc-gui\"

winput-includes = -I../../../include -I${JLOCAL}/include \
		-I$(WINDS)/include

#
# valkyrie validation library target
#
val-target = $(out)/libval.a
val-objs = \
	$(obj)/val.o \
	$(obj)/qc.o \
	$(obj)/valqueue.o
val-cflags = $(val-mode) $(val-guipath) $(winput-includes)
val-libsuffix = val

#
# list all targets and all objects here
#

all-targets = $(val-target) 
all-objs = $(val-objs)



all: $(all-targets)


###############################################################################
#
# general rules that perform actions
#


#
# deletes all dependent files for a fresh rebuild
#
clean: 
	rm -f core a.out *.o $(all-objs) $(all-targets) 

#
# backs up development platform
#
backup:
	touch ../$(backup-name).tar
	rm ../$(backup-name).tar 
	touch ../$(backup-name).tar.$(backup-suffix)
	mv ../$(backup-name).tar.$(backup-suffix) ../bkup
	tar cf ../$(backup-name).tar *
	$(compress) $(compress-flags) ../$(backup-name).tar


#
# rule to check stuff out through RCS if they don't exist (by default)
#
#.DEFAULT:
#	co $@


###############################################################################
#
# begin dependencies here
#


#
# include files -- shared between targets
#

val-h = $(inc)/val.h $(inc)/valqueue.h

qc-h = $(val-h) $(inc)/qc.h


##
#
# valkyrie validation library
#
##

$(val-target) : $(val-objs) $(sock)/report.o $(sock)/queue.o

#(make library)   -- assumes $(sock)/report.o is up to date (!!)
	$(ar) $(ar-flags) $(val-target) $(val-objs) $(sock)/report.o \
		$(sock)/queue.o
#	$(ranlib) $(ranlib-flags) $(val-target)  (hope not required !!)

#(make executable)
#	$(link) $(link-flags) $(val-objs) -o $@ 
#	strip $@

$(val-objs): $(obj)/%.o: $(src)/%.cc $(inc)/stddefs.h
	$(compile-c++) $(compile-c++-flags) $(val-cflags) $< -o $@

$(sock)/report.o:
	# 
	# problem !! -- building the library requires $(sock)/report.o
	# to be present, which isn't.  the library will be incomplete !!
	#

$(obj)/val.o: $(val-h)

$(obj)/qc.o: $(val-h) $(qc-h)

$(obj)/valqueue.o: $(inc)/valqueue.h

# (not part of val-objs)
#$(obj)/cint.o: $(src)/cint.cc $(inc)/cint.h
#	$(compile-c) $(compile-c-flags) $< -o $@

