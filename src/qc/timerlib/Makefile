#
# Makefile for Valkyrie Timer Library
#



# system configuration
#
# one of the following configuration strings must be set in order for
# the timer library to build properly.  
#
# (default is for solaris release 5.3 w/sunos 4 kernel)
#
#sysflags = -DSUN4 -DSOLARIS
#sysflags = -DSUN4 -DSUNOS
#sysflags =


# directories
socklib = ../socklib

# to compile
compile-c++ = g++
compile-c++-flags = -c -O

compile-c = gcc
compile-c-flags = -c -O


# to make libraries
ar = ar
ar-flags = rv


#
# timer library target
#
timerlib-target		= libvsig.a
timerlib-c++-objs	= gui2sig.o sigdata.o 
timerlib-c-objs		= valklist.o valksig.o
timerlib-cflags		= -I$(socklib) $(sysflags)

timerlib-objs 		= $(timerlib-c-objs) $(timerlib-c++-objs)


#
# rules for all targets
#
all-targets = $(timerlib-target)
all-objs = $(timerlib-objs)

all: $(all-targets)

clean:
	rm -f *~ core a.out *.o $(all-objs) $(all-targets)

.DEFAULT:
	co $@



#
# timerlib rules
#

# library creation rule
$(timerlib-target): $(timerlib-objs) $(socklib)/error.o
	$(ar) $(ar-flags) $(timerlib-target) $(timerlib-objs) \
		$(socklib)/error.o

# recompile rules
$(timerlib-c++-objs): %.o: %.c
	$(compile-c++) $(compile-c++-flags) $(timerlib-cflags) $< -o $@

$(timerlib-c-objs): %.o: %.c
	$(compile-c) $(compile-c-flags) $(timerlib-cflags) $< -o $@


# specific dependencies
gui2sig.o : gui2sig.h valksig.h valklist.h sigdata.h sigerr.h \
	$(socklib)/error.h

sigdata.o : valklist.h gui2sig.h sigerr.h sigdata.h $(socklib)/error.h

valklist.o : valklist.h sigdata.h sigerr.h

valksig.o : valksig.h sigerr.h
