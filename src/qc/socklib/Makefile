#
# Makefile for Valkyrie Socket Library
# 


#
# if the socket library ever gives messages indicating an invalid
# port number, change this line and do:
#
#     make clean
#     cd ..
#     make
#
socket-port-number = -DCONNECT_PORT=5403

# to compile
compile-c++ = g++
compile-c++-flags = -c -ansi -O

compile-c = gcc
compile-c-flags = -c -ansi -O 



#
# socket library target
#
socklib-target		= #(none) -- just build .o files
socklib-c++-objs	= queue.o
socklib-c-objs		= report.o error.o
socklib-cflags		= $(socket-port-number)

socklib-objs 		= $(socklib-c-objs) $(socklib-c++-objs)


#
# rules for all targets
#
all-target = $(socklib-target)
all-objs = $(socklib-objs)

all: $(all-objs)

clean:
	rm -f *~ core a.out *.o $(all-objs) $(all-targets)

.DEFAULT:
	co $@


#
# socklib rules
#

# recompile rules
$(socklib-c++-objs): %.o: %.cc
	$(compile-c++) $(compile-c++-flags) $(socklib-cflags) $< -o $@

$(socklib-c-objs): %.o: %.c
	$(compile-c) $(compile-c-flags) $(socklib-cflags) $< -o $@

# specific dependencies
queue.o: report.h error.h message.h queue.h

report.o: sock.h message.h sleep.h port_host.h protos.h

error.o: sock.h message.h port_host.h sleep.h protos.h
