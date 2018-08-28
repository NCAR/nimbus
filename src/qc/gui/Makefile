#
# Makefile for Valkyrie GUI
#



# x11-lib:  where do we look for X11/motif libraries
x11-lib = -L/usr/X11R6/lib
# (on mistral & co., all can be found in default library path -- on other 
# systems, you may have to try some of these directories in x11-lib):
#	-L/usr/lib/X11R5 
#	-L/usr/local/X.V11R5/lib 
#	-L/usr/local/X11R5.sun4.SunOS.4.1.2/lib

# x11-include: where do we look for X11/motif include files
x11-include = -I/usr/X11R6/include
# (on mistral & co., all can be found in default include path -- on other 
# systems, you may have to try some of these directories in x11-include):
#	-I/usr/local/X.V11R5/include 
#	-I/usr/include/X11R5/X11/   
#	-I/usr/local/X11R5.sun4.SunOS.4.1.2/include/Xm  
#	-I/usr/include/X11R5/  
#	-I/usr/X11R5/include

# some other relevent directories
timerlib = ../timerlib
socklib = ../socklib

# to compile
compile-c++ = g++
compile-c++-flags = -c -ansi -O 

# to link
link = g++
link-flags =



#
# gui target
#
gui-target = qc-gui
gui-objs = buttonbar.o main.o mainscreen.o warningbox.o window.o text.o
gui-c++-flags = -I$(timerlib) -I$(socklib) $(x11-include)
gui-link-flags = -L$(timerlib) $(x11-lib)



#
# all targets
#
all-targets = $(gui-target)
all-objs = $(gui-objs)

all: $(all-targets)


clean:
	rm -f *~ core a.out *.o $(all-objs) $(all-targets)

.DEFAULT:
	co $@


#
# GUI rules
# 

# link rule
$(gui-target): $(gui-objs) $(timerlib)/libvsig.a
	$(link) $(link-flags) $(gui-objs) -o $@ $(gui-link-flags) -lc -lX11 \
	-lXm -lXt -lvsig -lm

# recompile rule
$(gui-objs): %.o: %.cc 
	$(compile-c++) $(compile-c++-flags) $(gui-c++-flags) $< -o $@

# specific dependencies
buttonbar.o: buttonbar.h

main.o: buttonbar.h mainscreen.h warningbox.h window.h text.h

mainscreen.o: mainscreen.h warningbox.h window.h buttonbar.h text.h \
	$(timerlib)/gui2sig.h $(socklib)/error.h $(socklib)/message.h 

warningbox.o: warningbox.h window.h buttonbar.h text.h \
	$(timerlib)/gui2sig.h $(socklib)/error.h $(socklib)/message.h 

window.o: window.h 

text.o: text.h 
