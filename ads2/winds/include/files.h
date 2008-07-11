/* 
files.h  --  names and management of config files used in WINDS.  ALL files
pathnames are prefixed either with variable string designating home 
directory for WINDS software or a tilde for user's home directory.
*/

/* executable tasks started by winds: filename is function of path to binary
or script
*/
#define INPUT_PROCESS  	"%s/winput"
#define WINDCOMM_PROCESS "%s/windComm"
#define PSPRINT_PROCESS "%s/psprint"
#define PRTSRVR_PROCESS "%s/prt_srvr"
/* 
help file path name
*/
#define HELPPATH	"HELPPATH=%s/doc:"
/* 
filename is function of project number

location for datafiles if environment variable DATA_DIR is defined
*/
#define NETCDF_SUFFIX    ".nc"
#define NETCDF_FILENAME  "%s.nc"
#define NETCDF_DATADIRPATH  "%s/%s"
/*
generic system default locations for netCDF datasets and raw datasets,
if env. var. DATA_DIR is not defined
*/
#define WINDS_HDRPATH   "%s/%s/H%s"
#define WINDS_DATAPATH  "%s/%s/D%s"
#define NETCDF_DATAPATH "%s/%s/%s"
#define RAW_DATAPATH 	"%s/%s/rawdata"

#define PROJECTNAME	"%s/%s/ProjectName"
#define PROJECTNUMBER	"%s/hosts/ProjectNumber"
#define HISTORY		"%s/%s/history_file"
#define CONFIG_FILE	"%s/%s/config"
#define PLOTSPEXFOLDERPREFIX  "plotspex"
#define PLOTSPEX  	"%s/%s/%s%s"  
#define PROJ_CALIB_ATTR	"%s/%s/calib_attr"
#define HEADER		"%s/%s/header"
#define ARGFILE  	"%s/%s/%s.%d"  
#define ADSCMD		"%s/%s/adscmd"
#define NO_START_INPUT 	"%s/%s/task_pids"
#define PROJ_ATTR	"%s/%s/plot_attr"
#define VARDB_ATTR	"%s/%s/VarDB"
#define PROJ_BULLETIN9	"%s/%s/bulletin.9"
#define BOUNDARIES      "%s/%s/regions"
#define COASTLINES      "%s/defaults/coast.data"
#define LANDMARKS       "%s/%s/landmarks"
#define ASCII_PARMS     "%s/%s/ascii_parms"
#define FLAT_PANEL_PARMS "%s/%s/flat_panel_parms"
#define STATS_SELNS      "%s/%s/stats_selns"
#define PMS_SPEC_FILE	"%s/%s/PMSspecs"
/* 
filename is function of compute host
*/
#define STATIC_IDS  	"%s/hosts/%s/static.ids"  
#define DYNAMIC_IDS  	"%s/hosts/%s/dynamic.ids"  
/*
miscellaneous winds text/image printing files - name is function of 
display host and/or image number
*/
#define IMAGE_FILENAME  "%s/%s.windsimage.%d"
#define STATS_PREFIX    "StatsFlush"
#define STATS_FILENAME  "%s/%s/%s.%d"
#define PSOUTPUTFILE 	"/jnet/tmp/%s.windsimage.%d"
/* 
following MUST match vars of same name in $WINDS/scripts/raf.defines, if
indeed that file is part of this WINDS distribution...
*/
#define HALT_FILENAME	"%s/hosts/%s/halt.%s"
#define EXITALLDISPLAYS	"%s/hosts/%s/ExitAllDisplays"
#define KILLHOSTSLISTFILE "%s/hosts/halt.winds"
/*
 following MUST match var of same name in $WINDS/scripts/winds
*/
#define SESSIONFILE "%s/hosts/%s/session"
/* 
statically named database files under $PROJ_DIR and $PROJ_DIR/defaults
*/
#define PRINTCONFIG     "%s/printconfig"
#define NETCONFIGFILE 	"%s/netconfig"  
#define BULLETIN9	"%s/defaults/bulletin.9"
#define DEF_CONFIG_FILE	"%s/defaults/config"
#define PLOT_ATTR	"%s/defaults/plot_attr"
#define CALIB_ATTR	"%s/defaults/calib_attr"
#define PSCODEFILE   	"%s/defaults/pscode.defs"
/*
user preference init file
*/
#define USERINIT	"%s/.windsinit"

/*
general pathname prefixes
*/
#define BINPATH		"%s/bin"
#define HOSTPATH	"%s/hosts"
