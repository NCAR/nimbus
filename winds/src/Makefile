#
# using /opt/gnu/bin/pmake
#
#  makefile -- make the WINDS executable program
# 
# This makefile is working with /opt/gnu/bin/pmake and Sun's bundled
# SVR4-transition compiler /usr/xpg2bin/cc.
#
#.SCCS_GET:
#
# default flags, assuming development mode (debug on)
#
NIMBUS=		/code/chris/nimbus
OPENWINHOME=	/usr/openwin
LOCAL_INCL=     $(JLOCAL)/include/
INC= 		$(WINDS)/include/
NIMBINC=	$(NIMBUS)/include/
OBJ= 		$(WINDS)/obj/
OBJ=
EXEC=  		$(WINDS)/bin/
WDEF= 		$(WINDS)/defaults/
#
# gnu gcc flags
#
CC = gcc
WARNINGS = 
#
CFLAGS = -c -g -O $(WARNINGS)
#
LDFLAG =
#
# paths for include files
#
XVIEW_INCL=     -I$(OPENWINHOME)/include
WINDS_INCL=	-I$(INC) -I$(NIMBINC)
SHARE_INCL=	-I$(LOCAL_INCL)
INCLUDE=        $(XVIEW_INCL) $(WINDS_INCL) $(SHARE_INCL)
#
# library paths
#
LIB_PATH=       -L/jnet/local/lib -L/usr/openwin/lib -L/usr/X11R6/lib
#
# libraries
#
UNIXLIBS=     	-lxview -lolgx -lnetcdf -lX11 -lXp -lXext -lm
RAFLIBS=     	-lhdr_api -lVarDB -lraf -lpms
#
WLIBS=     	$(UNIXLIBS) $(RAFLIBS)
#
# in-house libs that may change
#
LIB_DEPENDS=	$(JLOCAL)/lib/libraf.a $(JLOCAL)/lib/libhdr_api.a
#
#.PATH.c:	$(WINDS)/src/

CXXFLAGS = -g -O $(WARNINGS) $(INCLUDE)

.c.o: 
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
#
# make development version of all WINDS programs
#
WINDSALL = winds winput portserver postscriptserver windcomm MONITOR HELP WINDSPRINT
winds_all: $(WINDSALL)
#
# the following target will extract the sccs versions of all WINDS files 
# needed for release and make a release version of WINDS. 
#
# PLEASE NOTE: you must invoke this with /usr/bin/make, as pmake will not
# behave very nicely on calling up these scripts.
#
release:
	/bin/csh $(WINDS)/scripts/maint sccs
	/bin/csh $(WINDS)/scripts/get_sccs_winds
	/bin/csh $(WINDS)/scripts/make_winds_release
ctags:
	ctags *.c
#
########################     WINDS PROCESS  #################################
#############################################################################

# extract libraf.a and libhdr_api.a into separate object modules to
# facilitate purify'ing them. You must maintain these object modules
# separately from their own makefiles: omit the X*.c from libraf, and
# the fortran *.f sources to preclude undefined references.

WINDS.O = $(OBJ)startup.o $(OBJ)netconfig.o $(OBJ)xdraw.o $(OBJ)psdraw.o \
	$(OBJ)ipc.o $(OBJ)ipcdisp.o $(OBJ)events.o $(OBJ)help.o $(OBJ)menus.o \
	$(OBJ)plotspex-novideo.o $(OBJ)errors.o $(OBJ)shmem.o $(OBJ)socket.o \
	$(OBJ)timer.o $(OBJ)plotattrs.o $(OBJ)etc.o $(OBJ)event_ops.o \
	$(OBJ)print_image.o $(OBJ)xy_plt.o $(OBJ)plots.o $(OBJ)display.o \
	$(OBJ)plot_events.o $(OBJ)xy_menus.o $(OBJ)axes.o $(OBJ)scale.o \
	$(OBJ)lists.o $(OBJ)listprint.o $(OBJ)fixes.o $(OBJ)calib.o \
	$(OBJ)calib_events.o $(OBJ)stats.o $(OBJ)history.o \
	$(OBJ)regress.o $(OBJ)hdr_wrap.o $(OBJ)lstats.o $(OBJ)pms1d.o \
	$(OBJ)pms1d_xaxis.o $(OBJ)pms1d_ops.o $(OBJ)pms1d_util.o $(OBJ)pms2d.o \
	$(OBJ)new_setup.o $(OBJ)ctl_panel.o $(OBJ)op_panels.o $(OBJ)text_ops.o \
	$(OBJ)panels.o $(OBJ)vectors.o $(OBJ)asciifeed.o $(OBJ)flat_panel.o \
	$(OBJ)boundaries.o $(OBJ)lmrks.o $(OBJ)geom.o $(OBJ)panel_menus.o \
	$(OBJ)filemgr.o $(OBJ)xv_errors.o $(OBJ)tasks.o $(OBJ)multiprint.o \
	$(OBJ)windsops.o $(OBJ)fonts.o $(OBJ)panel_list.o $(OBJ)xview.o \
	$(OBJ)config.o $(OBJ)config_mgmt.o $(OBJ)netcdf.o $(OBJ)adscmd.o 

WINDSCOMPONENTS =  $(LIB_PATH) $(WLIBS)

winds: $(EXEC)winds.novideo

$(EXEC)winds.novideo: $(WINDS.O) $(LIB_DEPENDS)
	$(CC) $(LDFLAG) $(WINDS.O) $(WINDSCOMPONENTS) -o $(EXEC)winds

$(OBJ)windsops.o: $(INC)display.h $(LOCAL_INCL)constants.h $(INC)files.h \
	$(INC)lengths.h $(LOCAL_INCL)raf.h

$(OBJ)hdr_wrap.o: $(LOCAL_INCL)ctape.h $(LOCAL_INCL)constants.h

$(OBJ)multiprint.o: $(LOCAL_INCL)constants.h $(INC)display.h $(INC)help.h \
	$(INC)printers.h $(INC)socket.h

$(OBJ)errors.o: $(INC)errors.h $(INC)help.h $(INC)display.h $(INC)lengths.h \
		$(INC)network.h $(LOCAL_INCL)constants.h

$(OBJ)plotattrs.o: $(INC)plotattrs.h $(INC)display.h $(INC)files.h \
	$(INC)errors.h $(LOCAL_INCL)constants.h $(INC)help.h $(INC)lengths.h \
	$(INC)xy_plt.h $(INC)proto.h $(LOCAL_INCL)vardb.h

$(OBJ)etc.o: $(INC)xy_plt.h $(INC)display.h $(LOCAL_INCL)constants.h \
	$(INC)help.h $(INC)files.h $(INC)lengths.h $(INC)errors.h $(INC)ops.h \
	$(INC)macros.h $(LOCAL_INCL)header.h $(INC)network.h $(LOCAL_INCL)ctape.h

$(OBJ)events.o: $(INC)help.h $(INC)ops.h $(INC)xview.h $(INC)tasks.h\
	$(LOCAL_INCL)header.h $(LOCAL_INCL)constants.h $(INC)xy_plt.h\
	$(INC)pms2d.h $(INC)display.h $(INC)errors.h 

$(OBJ)event_ops.o: $(INC)display.h $(INC)ops.h $(INC)help.h\
	$(LOCAL_INCL)constants.h $(INC)xy_plt.h $(INC)timer.h\
	$(INC)printers.h $(INC)lengths.h $(INC)vectors.h $(INC)socket.h\
	$(INC)colors.h $(INC)op_panels.h $(INC)network.h 

$(OBJ)help.o: $(INC)help.h $(INC)gticons.h $(LOCAL_INCL)constants.h \
	$(INC)colors.h $(INC)macros.h $(INC)display.h $(INC)op_panels.h\
	$(INC)errors.h $(INC)network.h

$(OBJ)menus.o: $(INC)help.h $(INC)lengths.h $(INC)plotspex.h $(INC)ops.h\
	$(INC)xview.h $(LOCAL_INCL)constants.h $(INC)files.h $(INC)display.h\
	$(INC)geom.h $(INC)pms1d.h

$(OBJ)plotspex-novideo.o: $(INC)lengths.h $(INC)plotspex.h $(INC)files.h\
	$(INC)ops.h $(LOCAL_INCL)constants.h $(INC)errors.h $(INC)display.h\
	$(INC)help.h $(INC)xy_plt.h $(INC)pms2d.h $(LOCAL_INCL)header.h\
	$(INC)vectors.h $(INC)macros.h $(INC)geom.h $(INC)op_panels.h\
	$(INC)pms1d.h
	$(CC) $(CFLAGS) $(INCLUDE) -c plotspex.c -o $(OBJ)plotspex-novideo.o

$(OBJ)print_image.o: $(INC)lengths.h $(LOCAL_INCL)constants.h $(INC)files.h\
	$(INC)colors.h $(INC)help.h $(INC)printers.h $(INC)socket.h\
	$(INC)display.h

$(OBJ)ipcdisp.o: $(LOCAL_INCL)constants.h $(INC)socket.h $(INC)printers.h \
	$(INC)display.h

$(OBJ)ipc.o: $(LOCAL_INCL)constants.h $(INC)socket.h 

$(OBJ)startup.o: $(LOCAL_INCL)constants.h $(INC)network.h $(INC)xview.h\
	$(INC)display.h $(INC)help.h $(INC)lengths.h $(INC)colors.h\
	$(INC)files.h $(INC)shmem.h  $(LOCAL_INCL)header.h $(INC)errors.h\
	$(INC)socket.h $(INC)tasks.h $(INC)event_attr.h $(INC)calib.h\
	$(INC)op_panels.h $(INC)geom.h $(INC)config.h $(INC)version.h\
	$(INC)proto.h $(LOCAL_INCL)vardb.h

$(OBJ)tasks.o: $(INC)tasks.h $(INC)lengths.h $(LOCAL_INCL)constants.h\
	$(INC)errors.h $(INC)files.h $(INC)socket.h $(INC)printers.h

$(OBJ)timer.o: $(INC)shmem.h $(INC)colors.h $(INC)display.h $(INC)timer.h\
	$(LOCAL_INCL)constants.h $(INC)help.h $(INC)lengths.h $(INC)xy_plt.h\
	$(INC)errors.h $(INC)ops.h $(INC)socket.h $(INC)printers.h\
	$(INC)files.h $(INC)proto.h $(INC)pms1d.h $(INC)network.h\
	$(INC)plotspex.h $(LOCAL_INCL)raf.h

$(OBJ)psdraw.o: $(INC)display.h

$(OBJ)xdraw.o: $(LOCAL_INCL)constants.h $(INC)colors.h $(INC)display.h

$(OBJ)flat_panel.o: $(INC)printers.h $(INC)socket.h $(INC)lengths.h\
	$(INC)shmem.h $(INC)display.h $(INC)flat_panel.h\
	$(LOCAL_INCL)constants.h $(INC)macros.h

$(OBJ)asciifeed.o: $(INC)printers.h $(INC)socket.h $(INC)lengths.h\
	$(INC)shmem.h $(INC)display.h $(LOCAL_INCL)constants.h

$(OBJ)vectors.o: $(INC)vectors.h $(INC)colors.h $(INC)lengths.h $(INC)xy_plt.h\
	$(INC)help.h $(INC)macros.h $(INC)ops.h $(LOCAL_INCL)constants.h\
	$(INC)display.h

$(OBJ)boundaries.o: $(INC)track.h $(INC)macros.h $(INC)files.h

$(OBJ)brdcst.o: $(LOCAL_INCL)constants.h $(INC)socket.h
$(OBJ)filemgr.o:
$(OBJ)panel_list.o: $(INC)help.h $(LOCAL_INCL)constants.h $(INC)display.h \
	$(INC)errors.h
$(OBJ)xview.o:
$(OBJ)fonts.o: $(INC)display.h $(INC)help.h \
	$(LOCAL_INCL)constants.h
$(OBJ)xv_errors.o:
$(OBJ)lmrks.o: $(INC)lengths.h $(LOCAL_INCL)constants.h $(INC)track.h \
	$(INC)macros.h $(INC)display.h $(INC)colors.h $(INC)help.h

$(OBJ)geom.o: $(INC)geom.h $(INC)display.h $(INC)op_panels.h $(INC)ops.h \
	$(LOCAL_INCL)constants.h 

$(OBJ)config.o: $(INC)config.h $(INC)lengths.h $(INC)files.h $(LOCAL_INCL)constants.h \
	$(INC)display.h

$(OBJ)config_mgmt.o: $(INC)config.h $(INC)lengths.h $(INC)files.h $(LOCAL_INCL)constants.h \
	$(INC)display.h

########################  XY PLOT MODULE  ###################################
#############################################################################

$(OBJ)xy_plt.o:	$(INC)xy_plt.h $(INC)help.h $(INC)lengths.h $(INC)files.h\
	$(INC)ops.h $(LOCAL_INCL)constants.h $(INC)display.h $(INC)plotattrs.h

$(OBJ)plots.o:  $(INC)xy_plt.h $(LOCAL_INCL)constants.h $(INC)lengths.h\
	$(INC)ops.h $(INC)help.h $(INC)colors.h $(INC)errors.h $(INC)xview.h\
	$(INC)macros.h $(INC)display.h $(INC)vectors.h $(INC)shmem.h

$(OBJ)listprint.o:  $(INC)lengths.h $(LOCAL_INCL)constants.h $(INC)ops.h\
	$(INC)display.h $(INC)geom.h $(INC)printers.h $(INC)socket.h

$(OBJ)lists.o:  $(INC)lengths.h $(INC)ops.h $(LOCAL_INCL)constants.h\
	$(INC)errors.h $(INC)colors.h $(INC)help.h $(INC)display.h\
	$(INC)geom.h $(INC)macros.h

$(OBJ)fixes.o:  $(INC)lengths.h $(INC)colors.h $(LOCAL_INCL)constants.h\
	$(INC)errors.h $(INC)help.h $(INC)ops.h $(INC)macros.h\
	$(INC)display.h

$(OBJ)display.o:  $(INC)xy_plt.h $(INC)lengths.h $(INC)shmem.h $(INC)ops.h\
	$(INC)help.h $(INC)colors.h $(INC)timer.h $(LOCAL_INCL)constants.h\
	$(INC)socket.h $(INC)printers.h $(LOCAL_INCL)header.h $(INC)macros.h\
	$(INC)display.h $(INC)track.h $(INC)network.h $(INC)vectors.h\
	$(INC)geom.h

$(OBJ)xy_menus.o:  $(INC)lengths.h $(LOCAL_INCL)constants.h\
	$(INC)help.h $(INC)xy_plt.h $(INC)ops.h $(INC)files.h\
	$(INC)menu_attr.h $(INC)plotattrs.h $(INC)network.h $(INC)errors.h

$(OBJ)plot_events.o:  $(INC)xy_plt.h $(INC)lengths.h $(INC)ops.h \
	$(INC)timer.h $(INC)help.h $(LOCAL_INCL)constants.h $(INC)errors.h \
	$(LOCAL_INCL)header.h $(INC)macros.h $(INC)display.h $(INC)op_panels.h

$(OBJ)axes.o:  $(INC)xy_plt.h $(INC)lengths.h $(LOCAL_INCL)constants.h\
	$(INC)help.h $(INC)ops.h $(INC)macros.h $(INC)display.h

$(OBJ)scale.o:  $(INC)xy_plt.h $(INC)help.h $(INC)shmem.h $(INC)ops.h $(INC)display.h

######################## CALIBRATION MODULE  ################################
#############################################################################

$(OBJ)calib.o:  $(INC)calib.h $(INC)xview.h $(INC)lengths.h $(INC)ops.h \
	$(INC)lengths.h $(LOCAL_INCL)header.h $(INC)display.h $(INC)help.h \
	$(INC)event_attr.h $(INC)geom.h $(INC)menu_attr.h $(LOCAL_INCL)constants.h

$(OBJ)calib_events.o:  $(INC)lengths.h $(INC)xy_plt.h $(INC)errors.h\
	$(INC)xview.h $(INC)calib.h $(INC)shmem.h $(INC)tasks.h $(INC)ops.h\
	$(INC)files.h $(INC)macros.h $(LOCAL_INCL)constants.h $(INC)display.h\
	$(INC)plotspex.h

$(OBJ)stats.o:  $(INC)shmem.h $(INC)calib.h $(INC)colors.h $(INC)xy_plt.h \
	$(INC)lengths.h $(INC)macros.h $(INC)display.h $(INC)socket.h \
	$(LOCAL_INCL)constants.h

$(OBJ)regress.o:  $(INC)calib.h $(INC)lengths.h $(LOCAL_INCL)constants.h

$(OBJ)history.o:  $(INC)calib.h $(INC)lengths.h $(INC)files.h\
	$(LOCAL_INCL)constants.h $(INC)socket.h $(INC)help.h $(INC)display.h\
	$(LOCAL_INCL)header.h $(INC)xy_plt.h

######################## STATISTICS MODULE  ################################
#############################################################################

$(OBJ)lstats.o:  $(INC)printers.h $(LOCAL_INCL)constants.h $(INC)shmem.h\
	$(LOCAL_INCL)header.h $(INC)lengths.h $(INC)macros.h $(INC)socket.h\
	$(INC)files.h

############################  PMS1D MODULE  #################################
#############################################################################

$(OBJ)pms1d.o: $(INC)pms1d.h $(INC)display.h $(INC)ops.h $(INC)lengths.h \
	$(INC)colors.h

$(OBJ)pms1d_util.o: $(INC)pms1d.h $(INC)colors.h $(INC)lengths.h \
	$(INC)errors.h \

$(OBJ)pms1d_xaxis.o: $(INC)display.h $(INC)files.h $(INC)pms1d.h \
	$(LOCAL_INCL)pms.h

$(OBJ)pms1d_ops.o: $(INC)pms1d.h $(INC)lengths.h $(INC)plotspex.h \
	$(INC)errors.h $(INC)ops.h $(INC)colors.h

############################  ADSCMD MODULE  ################################
#############################################################################

$(OBJ)adscmd.o: $(LOCAL_INCL)header.h $(INC)network.h $(INC)Events.h \
	$(LOCAL_INCL)constants.h

############################  PMS2D MODULE  #################################
#############################################################################

$(OBJ)pms2d.o: $(INC)pms2d.h $(INC)colors.h $(INC)lengths.h $(INC)files.h \
	$(LOCAL_INCL)constants.h $(INC)plotspex.h $(INC)errors.h \
	$(INC)p2d_layout.h $(LOCAL_INCL)header.h $(INC)eth_shm.h \
	$(INC)op_panels.h $(INC)help.h $(INC)proto.h

############################  PANEL MODULES  ################################
#############################################################################

$(OBJ)panels.o: $(INC)help.h $(INC)lengths.h $(LOCAL_INCL)constants.h $(INC)macros.h \
	$(INC)colors.h

$(OBJ)panel_menus.o: $(INC)panel_menus.h $(INC)display.h $(INC)help.h \
	$(INC)lengths.h $(INC)files.h $(INC)event_attr.h \
	$(INC)plotattrs.h $(LOCAL_INCL)constants.h $(INC)printers.h $(INC)panel_menus.h \
	$(INC)plotspex.h $(INC)socket.h $(INC)errors.h

$(OBJ)ctl_panel.o: $(INC)ctl_panel.h $(INC)help.h $(INC)display.h\
	$(LOCAL_INCL)constants.h $(INC)socket.h $(INC)tasks.h $(INC)errors.h\
	$(INC)xview.h $(INC)lengths.h $(INC)printers.h $(INC)geom.h\
	$(INC)shmem.h $(INC)ops.h $(INC)network.h $(INC)event_attr.h\
	$(INC)op_panels.h $(INC)panel_menus.h $(INC)colors.h

$(OBJ)op_panels.o: $(INC)display.h $(INC)op_panels.h $(INC)help.h\
	$(INC)event_attr.h $(INC)ops.h $(INC)lengths.h\
	$(LOCAL_INCL)constants.h $(INC)xview.h $(INC)network.h\
	$(INC)plotspex.h $(INC)pms1d.h $(INC)printers.h  $(INC)proto.h

$(OBJ)text_ops.o: $(INC)display.h $(INC)op_panels.h $(INC)lengths.h\
	$(INC)xview.h $(INC)ops.h $(INC)plotspex.h $(INC)help.h\
	$(INC)errors.h $(INC)xy_plt.h $(LOCAL_INCL)constants.h\
	$(INC)macros.h $(INC)geom.h

$(OBJ)new_setup.o: $(INC)display.h $(INC)lengths.h $(INC)help.h \
	$(INC)plotspex.h $(LOCAL_INCL)constants.h \
	$(INC)xy_plt.h $(INC)new_setup.h $(INC)event_attr.h $(INC)op_panels.h \
	$(INC)panel_menus.h

############################  PRT_SRVR PROCESS  #############################
#############################################################################

portserver: $(EXEC)prt_srvr
PORTSERVERCOMPONENTS = $(LIB_PATH) -lraf 

PRT_SRVR.O = $(OBJ)socket.o $(OBJ)prt_srvr.o $(OBJ)netconfig.o
$(EXEC)prt_srvr:  $(PRT_SRVR.O)
	$(CC) $(LDFLAG)  $(PRT_SRVR.O) $(PORTSERVERCOMPONENTS) -o $(EXEC)prt_srvr 
$(OBJ)prt_srvr.o: prt_srvr.c \
	$(INC)colors.h $(LOCAL_INCL)constants.h $(INC)socket.h \
	$(INC)lengths.h $(INC)printers.h $(INC)help.h $(INC)network.h \
	$(INC)files.h $(INC)display.h
#
# don't use implicit rule here because the -Werror deal makes compile
# fail, and I'm not interested in corrected the warnings in this code since
# it works just fine...
#
	$(CC) -c -g -O $(INCLUDE) -c $< -o $@

############################  PSPRINT PROCESS  #############################
#############################################################################

postscriptserver: $(EXEC)psprint
POSTSCRIPTSERVERCOMPONENTS = $(LIB_PATH) -lraf 

PSPRINT.O = $(OBJ)socket.o $(OBJ)psprint.o $(OBJ)netconfig.o
$(EXEC)psprint:  $(PSPRINT.O)
	$(CC) $(LDFLAG) $(PSPRINT.O) $(POSTSCRIPTSERVERCOMPONENTS) -o $(EXEC)psprint 
$(OBJ)psprint.o: $(INC)postscript.h $(INC)colors.h $(INC)socket.h \
$(INC)lengths.h $(INC)files.h $(LOCAL_INCL)constants.h $(INC)help.h

############################  SHARED FILES   ################################
#############################################################################

$(OBJ)socket.o: $(INC)socket.h $(LOCAL_INCL)constants.h

$(OBJ)netconfig.o: $(INC)network.h $(LOCAL_INCL)constants.h $(INC)files.h \
	$(INC)socket.h $(INC)lengths.h $(INC)printers.h $(INC)display.h

$(OBJ)shmem.o: $(INC)shmem.h $(LOCAL_INCL)constants.h $(INC)display.h $(INC)lengths.h \
	$(LOCAL_INCL)header.h $(INC)files.h $(INC)eth_shm.h $(INC)pms1d.h

############################  INPUT PROCESS  ################################
#############################################################################
#
# ATTENTION FTP CLIENTS FOR WINDS SOURCE DISTRIBUTION:
# remove all lines in NIMBUS.O dependency list below.
#
# ATTENTION WINDS DEVELOPERS: please adhere to this convention -- i.e., any
# new object code targets needed for processing netCDF files should go
# into WINPUT.O define -- else, place new target into NIMBUS.O define.
# See Gary Horton for clarification.  
#  
# 
NOBJ    = $(NIMBUS)/src/filter/obj/
NIMBUS.O = $(NOBJ)globals.o $(NOBJ)hdr_decode.o $(NOBJ)rd_lag.o $(NOBJ)popen.o \
	$(NOBJ)depend.o $(NOBJ)order.o $(NOBJ)rec_decode.o $(NOBJ)compute.o \
	$(NOBJ)rd_defs.o $(NOBJ)amlibfn.o $(NOBJ)pms1d_rt.o $(NOBJ)memalloc.o \
	$(NOBJ)textfile.o $(NOBJ)search.o $(NOBJ)rd_mod.o $(NOBJ)average.o \
	$(NOBJ)rd_sum.o $(NOBJ)rd_ddor.o $(NOBJ)injectsd.o \
	$(NOBJ)calco.o $(NOBJ)rd_spike.o $(NOBJ)probe.o

WINPUT.O = $(OBJ)winputops.o $(OBJ)netcdf.o $(OBJ)netconfig.o \
	   $(OBJ)shmem.o $(OBJ)ipc.o $(OBJ)socket.o $(OBJ)config_mgmt.o \
	   $(OBJ)hdr_wrap.o $(OBJ)winput-startup.o $(OBJ)brdcst.o \
	   $(OBJ)timegap.o

LIBEXEC = $(NIMBUS)/lib/

LINKLIBS = -lstd -luser -lxlate -lval -lnetcdf -lAircraft -lhdr_api -lm -lVarDB -lraf++ -lraf -lpms


winput: $(EXEC)winput

WINPUTCOMPONENTS = -L$(LIBEXEC) -L${JLOCAL}/lib $(LINKLIBS)
$(EXEC)winput: $(NIMBUS.O) $(WINPUT.O)
#	cd $(NIMBUS)/src/filter; make obj
	g++ -g -O $(WINPUT.O) $(NIMBUS.O) $(WINPUTCOMPONENTS) -o $(EXEC)/winput
#
# source files with names used in both winds and winput, even though in 
# different directories, must have explicit targets so we don't use wrong source
#
$(NOBJ)netcdf.o: $(NIMBUS)/src/filter/netcdf.c
	cd $(NIMBUS)/src/filter; make obj

$(NOBJ)pms1d_rt.o: $(NIMBUS)/src/filter/pms1d_rt.c
	cd $(NIMBUS)/src/filter; make obj

###################  C FILES

INPUT.H=$(NIMBINC)nimbus.h $(NIMBINC)decode.h $(LOCAL_INCL)ctape.h $(LOCAL_INCL)raf.h $(INC)shmem.h $(INC)network.h $(INC)eth_shm.h $(INC)socket.h $(INC)files.h $(INC)lengths.h $(INC)config.h $(INC)macros.h $(INC)ctl_panel.h
$(OBJ)winputops.o: $(INPUT.H)

NETCDF.H = $(LOCAL_INCL)netcdf.h
$(OBJ)netcdf.o: $(NETCDF.H)

WINPUT-STARTUP.H = $(LOCAL_INCL)constants.h $(INC)socket.h $(INC)lengths.h \
$(INC)files.h $(INC)network.h $(INC)config.h $(INC)shmem.h
$(OBJ)winput-startup.o: $(WINPUT-STARTUP.H)


##########################  WINDCOMM PROCESS  ##############################
##############################################################################

windcomm: $(EXEC)windComm
WINDCOMMCOMPONENTS = $(LIB_PATH) -lraf

WINDCOMM.O = $(SRC)windComm.cc $(OBJ)shmem.o
WINDCOMM.H = $(INC)eth_shm.h $(INC)shmem.h

$(EXEC)windComm: $(WINDCOMM.O) $(WINDCOMM.H)
	g++ -g -O $(WINDCOMM.O) $(WINDS_INCL) -I$(LOCAL_INCL) -L$(JLOCAL)/lib -o $(EXEC)windComm -lraf -lraf++ -lz

##########################  HELP MODULES  ####################################
##############################################################################

HELP: $(WINDS)/doc/WINDS.info $(JLOCAL)/man/man1/winds.1

HELPCOMPONENTS = $(WINDS)/doc/winds.info.nroff
$(WINDS)/doc/WINDS.info: $(HELPCOMPONENTS)
	/bin/rm -f $(WINDS)/doc/WINDS.info
	nroff $(HELPCOMPONENTS) > $(WINDS)/doc/WINDS.info

$(JLOCAL)/man/man1/winds.1: $(WINDS)/release/doc/winds.1
	mkdir -p $(JLOCAL)/man/man1
	/bin/cp $(WINDS)/release/doc/winds.1 $(JLOCAL)/man/man1/winds.1

########################  netbcast PROCESS ####################################
##############################################################################

netbcast: $(EXEC)netdata
$(EXEC)netdata: $(OBJ)netdata.o $(OBJ)socket.o
	$(CC) $(LDFLAG) -o $(EXEC)netdata $(OBJ)netdata.o $(OBJ)socket.o
$(OBJ)netdata.o: $(INC)socket.h $(LOCAL_INCL)constants.h

##########################   MONITOR   #######################################
##############################################################################

MONITOR: $(EXEC)monitor 

MONITOR.O = $(OBJ)monitor.o $(OBJ)netconfig.o $(OBJ)socket.o
$(EXEC)monitor: $(MONITOR.O)
	$(CC) $(LDFLAG) $(MONITOR.O) -o $(EXEC)monitor -L${JLOCAL}/lib -lraf
$(OBJ)monitor.o: $(INC)socket.h $(INC)files.h

##########################   WINDSPRINT   ####################################
##############################################################################

WINDSPRINT: $(EXEC)windsprint

$(EXEC)windsprint: $(OBJ)windsprint.o 
	$(CC) windsprint.c -g -O -o $(EXEC)windsprint

##########################  FTP SOURCE  ####################################
##############################################################################

ftp:
	
	/bin/csh $(WINDS)/scripts/get_ftp_winds 

##########################  SCCS COMPONENTS ##################################
##############################################################################

components:
	$(WINDS)/scripts/GetWINDS-SCCSComponents 
	$(WINDS)/scripts/GetSCCSComponents winds $(WINDSCOMPONENTS) 
	$(WINDS)/scripts/GetSCCSComponents winput $(WINPUTCOMPONENTS) 
	$(WINDS)/scripts/GetSCCSComponents portserver $(PORTSERVERCOMPONENTS) 
	$(WINDS)/scripts/GetSCCSComponents postscriptserver $(POSTSCRIPTSERVERCOMPONENTS) 
	$(WINDS)/scripts/GetSCCSComponents $(WINDCOMMCOMPONENTS) 
