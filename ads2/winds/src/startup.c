/* winds.c  -- main() startup for WINDS

WINDS is the WINdow Display System and is a copyrighted product:

Copyright 1994
University Corporation for Atmospheric Research
All Rights Reserved

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <xview/xview.h>
#include <xview/canvas.h>
#include <xview/panel.h>
#include <xview/fullscreen.h>
#include <xview/cms.h>
#include <sys/param.h>
#include <netdb.h>
#include <sys/types.h>

/****************************  WINDS INCLUDES  ******************************/

#include <constants.h>
#include <network.h>
#include <xview.h>
#include <colors.h>
#include <lengths.h>
#include <calib.h>
#include <help.h>
#include <ctape.h>
#include <display.h>
#include <files.h>
#include <shmem.h>
#include <header.h>
#include <errors.h>
#include <socket.h>
#include <tasks.h>
#include <op_panels.h>
#include <geom.h>
#include <config.h>
#include <ops.h>
#include <vardb.h>
#include <proto.h>

/******************************************************************************/

#define MAXVERSIONLENGTH 80

static char command[256];
#define MAXCMDLINELENGTH (MAXHOSTNAMELEN*10) 
static char WINDSCmdLine[MAXCMDLINELENGTH]; /* string reflecting startup cmd */

static char WinputProgramName[MAX_FLNM_LNGTH]="winput";
static int Argc;
static char **Argv;
static char WINDSVersion[MAXVERSIONLENGTH];
static int NumWindsNetHosts=0;	/* number of network hosts participating
					in concurrent winds display	*/
static char WindsNetHost[MAXWINDSNETHOSTS][MAXHOSTNAMELEN];
static char *display_host;	/* name of host used for display	*/
static char x_display[MAXHOSTNAMELEN];	/* display value -- of form hostname:x	*/
static char *winds_path;	/* WINDS environment variable		*/
static int VarDataBase=FALSE;	/* Variable Database file exists	*/
static int GotDataDir=FALSE;	/* whether an argument specifying the
					DATA_DIR environment var was seen on
					command line */
static char TZstring[16];
static int base_frame_height,          	/* height of base frame		*/
    base_frame_width;           	/* width of base frame		*/
struct Fl *tp_hdr;
int my_pid,				/* pid of process		*/
    window_x[NUM_QUADRANTS],		/* x-coords of display canvasses */
    window_y[NUM_QUADRANTS],		/* y-coords of display canvasses */
    window_width[NUM_WINDOWS],		/* width of display canvasses	*/
    window_height[NUM_WINDOWS],		/* height of display canvasses	*/
    display_height,                  	/* total height of display area --
					and thus the y-coord of control area */
    control_height,             	/* height of control canvas	*/
    control_width,              	/* width of control canvas	*/
    num_of_printees,              	/* number of variables being printed */
    WindsControlHost,              	/* TRUE if this host is control host */
    WindsComputeHost,              	/* TRUE if this host is compute host */
    connected_to_ads,              	/* TRUE if host is connected to ADS */
    *is_digital;			/* boolean array:  digital v. analog */

static int TimeWindowWidth=0;

static char NetCDFFilename[MAX_FLNM_LNGTH];
static int DataStartTime;
static int DataEndTime;

/* XGetAttr these once and save them
*/
int time_length, proj_length;

int WINDSCleanUp = FALSE;		/* whether or not to kill off WINDS
					   displays,etc. at session end */
int calib_running = FALSE;		/* TRUE when calib window is up */

/* drawing objects
*/
Window paint_window[NUM_WINDOWS]; 	/* event windows */

Drawable pw_xid[NUM_WINDOWS];

void event_proc(),    			/* event handler */
     show_time(),
     get_help(),
     draw_copyrite(),
     CanvasRepaintProc(); 

char *proj_number,			/* project # as per netconfig file */
     *get_day(),			/* returns current day string */
     *get_year(),			/* returns current year string */
     *ProjectDirectory,			/* home directory for PROJ_DIR stuff */
     *DataDirectory,			/* home directory for DATA_DIR stuff */
     UserHomeDir[MAX_FLNM_LNGTH],	/* User's home directory */
     winds_bin_path[MAX_FLNM_LNGTH],	/* directory for WINDS binaries */
     WindsHostsPath[MAX_FLNM_LNGTH],	/* directory for WINDS proj/hosts */
     winds_helppath[MAX_FLNM_LNGTH],    /* directory for WINDS.info help */
     CurrentDate[NAMLEN*2],             	/* current date */
     project[NAMLEN*2];          	/* project # string for ctl panel */

int setup_mode=FALSE;			/* TRUE if WINDS invoked in initial
					   setup mode */
char	*progName = "WINDS:";


/******************************  MAIN()  ************************************/

/*ARGSUSED*/
main(int argc, char **argv)
{
  /* initialize winds session
   */
  WINDSInitStuff(argc, argv);

_Xdebug = TRUE;
  /* xview startup 
   */
  xv_init(XV_INIT_ARGC_PTR_ARGV, &argc, argv, XV_X_ERROR_PROC,
		xlib_error_proc, XV_ERROR_PROC, xview_error_proc, NULL);
_Xdebug = TRUE;
  /* set resources
   */
  WINDSSetResources();

  /* define color map entries
   */
  (*SetupColors[GetDrawType()])();

  /* create windows
   */
  WINDSInitWindows();

  /* set various constants
   */
  WINDSGetConstants();

  enable_timer(GetBaseFrame());
  xv_main_loop(GetBaseFrame());
  winds_cleanup(AFTER_LOOP);
  return 0;

}

/*************************  FixUpDisplayNames()  ******************************/

FixUpDisplayNames()
{
  /* create x-display name: if display designator (i.e., <hostname>:x ) exists,
   * leave it as is; else, append default. This will fail if a hostname with
   * an embedded ":" is sent without a display designator...
   */
  if (strchr(display_host,':'))
    (void)sprintf(x_display,"%s",display_host);
  else
    (void)sprintf(x_display,"%s:0.0",display_host);

  /* create actual hostname: remove display designator. This will munge any
   * valid hostnames with embedded ":" ... sorry about that...
   */
  (void)TruncateString(display_host,":");
  if (ConvertIPAddrToName(display_host)!=OK) {
    (void)fprintf(stderr,"FATAL: can't get hostname from argument %s\n",
		display_host);
     RequestAlertNow("FATAL: WINDS is exiting because it can't determine a hostname from given IP address. Please contact a WINDS expert about this message.");
    winds_cleanup(ERROR);
  }
}

/****************************  GET_WINDS_ARGS()  ******************************/

get_winds_args(int argc, char **argv)
{
  extern char *optarg;
  extern int opterr;
  static char DataDirEnv[256];
  int c;
  int cmdlineindex;


  /* set defaults
   */
  SetParcelTrackingEnabled(FALSE);
  if ((display_host = (char *)strdup((char *)getenv("DISPLAY"))) == NULL) {
    fprintf(stderr,"WINDS WARNING: DISPLAY variable not set;\n");
    fprintf(stderr,"  ...defaulting WINDS session to compute host %s\n",
	GetComputeHost());
    display_host = (char *)strdup(GetComputeHost()); 
  } 

  FixUpDisplayNames();
  sprintf(display_file, "\0");
  sprintf(display_pnum, "\0");
  sprintf(display_mode, "\0");
  strcpy(color_map, DEFAULT_COLOR_MAP);
  sprintf(WINDSCmdLine, "winds ");
  cmdlineindex = strlen(WINDSCmdLine);
  opterr=0;

  while ((c = getopt(argc, argv, CMDLINE_OPTIONS)) != -1) {
    /*
     * args -, n, and o are implemented in winds startup script, and translated
     * here to original args given solely for purposes of maintaining an exact
     * copy of user's startup command line.
     */
    switch (c) {
      case '-':
        sprintf(&WINDSCmdLine[cmdlineindex],"- ");
        break;
      case 'n':
        sprintf(&WINDSCmdLine[cmdlineindex],"-nolog ");
        break;
      case 'o':
        sprintf(&WINDSCmdLine[cmdlineindex],"-onboard ");
        break;
      case 'q':
        SetQCenabled();
        break;
      case 'P':
        SetParcelTrackingEnabled(TRUE);
        break;
      case 'F':
        /* disable screen grabbing when under dbx control
         */
        fullscreendebug=1;
        break;
      case 'r':
        strcpy(display_mode,DEVCTL_CDF_MODE);
        strcpy(display_pnum,substring(optarg,'-'));
        strcpy(display_file,&optarg[nindex(optarg,'-')+1]);
        fprintf(stderr,"WINDS mode: %s\n",display_mode);
        fprintf(stderr,"WINDS project: %s\n",display_pnum);
        fprintf(stderr,"WINDS file: %s\n",display_file);
        sprintf(&WINDSCmdLine[cmdlineindex],"-m%s -p%s -f%s ",
		display_mode,display_pnum,display_file);
        break;
      case 'N':
        strcpy(display_mode,NETCDF_MODE);
        strcpy(display_pnum,substring(optarg,'-'));
        strcpy(display_file,&optarg[nindex(optarg,'-')+1]);
        fprintf(stderr,"WINDS mode: %s\n",display_mode);
        fprintf(stderr,"WINDS project: %s\n",display_pnum);
        fprintf(stderr,"WINDS file: %s\n",display_file);
        sprintf(&WINDSCmdLine[cmdlineindex],"-m%s -p%s -f%s ",
		display_mode,display_pnum,display_file);
        break;
      case 'D':
        sprintf(DataDirEnv,"DATA_DIR=%s",optarg);
        putenv(DataDirEnv);
        SetGotDataDir(TRUE);
        sprintf(&WINDSCmdLine[cmdlineindex],"-D%s ",optarg);
        break;
      case 'I':
        strcpy(WinputProgramName,optarg);
        break;
      case 'h':
        /* display host -- if WINDS is called with no arguments, display host
         * is assumed to be the host on which WINDS is running. This argument
         * is essential and ergo is set below in command line string, just in
         * case not found here.
         */
        display_host=(char *)strdup(optarg);
        FixUpDisplayNames();
        (void)fprintf(stderr,"WINDS DISPLAY host: %s\n",display_host);
        break;
      case 'c':
        /* display color map
         */
        (void)strcpy(color_map,optarg);
        (void)fprintf(stderr,"WINDS color: %s\n",color_map);
        (void)sprintf(&WINDSCmdLine[cmdlineindex],"-c%s ",color_map);
        break;
      case 'm':
        /* display mode
         */
        (void)strcpy(display_mode,optarg);
        (void)fprintf(stderr,"WINDS mode: %s\n",display_mode);
        (void)sprintf(&WINDSCmdLine[cmdlineindex],"-m%s ",display_mode);
        break;
      case 'p':
        /* display project number
         */
        (void)strcpy(display_pnum,optarg);
        (void)fprintf(stderr,"WINDS project: %s\n",display_pnum);
        (void)sprintf(&WINDSCmdLine[cmdlineindex],"-p%s ",display_pnum);
        break;
      case 'f':
        /* display dataset file descriptor
         */
        (void)strcpy(display_file,optarg);
        (void)fprintf(stderr,"WINDS file: %s\n",display_file);
        (void)sprintf(&WINDSCmdLine[cmdlineindex],"-f%s ",display_file);
        break;
    }

    cmdlineindex=strlen(WINDSCmdLine);
  } 

  (void)sprintf(&WINDSCmdLine[cmdlineindex],"-h%s",display_host);
}

/*************************  GetGotDataDir()  ***************************/

GetGotDataDir()
{
 return GotDataDir;
}

/*************************  SetGotDataDir()  ***************************/

SetGotDataDir(got)
int got;
{
 GotDataDir=got;
}

/*************************  GetWinputProgramName()  ***************************/

char *
GetWinputProgramName()
{
 return WinputProgramName;
}

/*************************  WINDSSetResources()  ******************************/

WINDSSetResources()
{
 defaults_set_boolean("OpenWindows.SelectDisplaysMenu",True);
 defaults_set_string("window.color.foreground",
  defaults_get_string("WINDS.PanelFontColor","string","black"));
 defaults_set_string("window.color.background",
  defaults_get_string("WINDS.PanelBkgndColor","string","white"));
 defaults_set_string("OpenWindows.WindowColor",
  defaults_get_string("WINDS.PanelColor","string","cornflower blue"));
}

/****************************  SetFrameLabel()  ******************************/

SetFrameLabel(message)
char *message;
{
 (void)fprintf(stderr,"%s\n",message);
 if (!GetBaseFrame()) {
  return;
 }
 (void)XSetAttr(GetBaseFrame(),XV_LABEL,message,NULL);
 X11FlushDisplay(); 
}

/****************************  SetVarDataBase()  ******************************/

SetVarDataBase(found)
int found;
{
 VarDataBase=found;
}

/****************************  GetVarDataBase()  ******************************/

GetVarDataBase()
{
 return VarDataBase;
}

char VarAttrsFileName[MAX_FLNM_LNGTH];

/*****************************************************************************/

char *
GetAttrModProgName()
{
 return "vared";
}

/*****************************************************************************/

char *
GetVarAttrsFileName()
{
 return VarAttrsFileName;
}

/*****************************************************************************/

GetVarAttrs()
{
 ReleaseVarDB();   
 (void)sprintf(VarAttrsFileName,VARDB_ATTR,ProjectDirectory,proj_number);
 if (InitializeVarDB(VarAttrsFileName)==ERR) {
  SetVarDataBase(FALSE);
 } else {
  SetVarDataBase(TRUE);
 }
 chg_plot_attrs(TIME_SERIES);
}

/*****************************************************************************/

StartAllWINDSTasks()
{
 if (write_static_ids(GetComputeHost())==ERROR) {
  RequestAlertNow("FATAL: problem completing write of static shared memory ID's file...contact WINDS expert with this information!");
  winds_cleanup(ERROR);
 }
/* 
kill any old WINDS tasks (that might be holding on to socket addresses), and
create new process dialogue sockets.  kill_winds_tasks AFTER getting static
shared mem addresses for pid's.
*/
 kill_winds_tasks();
 start_winds_tasks();
}

/****************************  WINDS_STARTUP()  ******************************/

winds_startup()
{
int	need_start_input;
FILE	*NoStartWinputFileHandle;
char	data_filename[MAX_FLNM_LNGTH];
char	hdr_file[MAX_FLNM_LNGTH];
int	cntr = 60;	/* 60 second count down */

/*
first get user requested size for stored history, or, if that is a problem, set
the default 
*/
 {
 int size;
 if ((size=get_config_parm(SHMEM_SEX))==ERROR)
  set_shmem_size(DEFAULT_SHMEM_SEX);
 else
  set_shmem_size(size);
 }
/* 
attach to statically sized shared memory segments
*/
 SetFrameLabel("WINDS getting shared memory information...");
 if (get_ids_static("WINDS")==ERROR) {
  RequestAlertNow("FATAL: problem getting static shared memory ids...contact WINDS expert with this information!");
  winds_cleanup(ERROR);
 }
 if (getaddr_static()==ERROR) {
  RequestAlertNow("FATAL: problem getting static shared memory addresses...contact WINDS expert with this information!");
  winds_cleanup(ERROR);
 }
/* 
get header read in from disk
*/
 if (post_processing_mode()) {
  set_fltno((char *)get_pp_filename());
 } else {
  SetFrameLabel("WINDS getting header...");
  if (GetInputMode()==DISK_DATA
   || GetInputMode()==RAWFILE_NETCDF
  )
/*
header in project setup does not have date or flight number; but each raw
dataset is assumed to have a header at top of file. Use that instead.
*/
   sprintf(hdr_file,RAW_DATAPATH,DataDirectory,proj_number);
  else
   {
   char	rtFileFileName[256], host[80], *p;
   FILE	*fpNameFile;

   gethostname(host, 80);
   if ((p = strchr(host, '.')) ) *p = '\0';

   sprintf(rtFileFileName, "%s/hosts/%s/rtdata.filename", ProjectDirectory, host);

   /* The real-time file name base is stored in the following file.
    * Read this name, we want the header from here, as it has flight #.
    */
   if ((fpNameFile = fopen(rtFileFileName, "r")) == NULL)
     {
     fprintf(stderr, "startup.c: Open of %s failed, fatal.\n", rtFileFileName);
     exit(1);
     }

   fgets(hdr_file, 256, fpNameFile);
   hdr_file[strlen(hdr_file)-1] = '\0';
   fclose(fpNameFile);

   strcat(hdr_file, ".ads");
fprintf(stderr, "WINDS: rt file %s opened\n", rtFileFileName);
   }

fprintf(stderr, "WINDS: Header file = %s\n", hdr_file);
  while ( InitHeader(hdr_file) == ERROR && --cntr > 0) {
   fprintf(stderr,"FATAL: couldn't read header file\n");
   sleep(1);
  }

fprintf(stderr, "WINDS: initHeader succeeded\n");
  if (cntr <= 0)
   ShowDatasetMessageAndExit(hdr_file);

  tp_hdr=(struct Fl*)GetHeader();
/*
on startup in real-time, flight number is set to whatever is set in current
header, until adscmd module (ADS) or discWin process (ADS II) informs it
is changed
*/
  set_fltno((char *)hdr_fltno());
  read_2d_prb_names();
fprintf(stderr, "WINDS: header retreived\n");
 }

/*
delay making control panel until after header is init'd so we can determine
whether we need ADS I or ADS II control buttons
*/
 SetFrameLabel("WINDS creating Control Panel...");
 MakeControlPanel();
 (void)fprintf(stderr,"%s on %s\n",
  (char *)get_fltno(),(char *)GetWINDSAircraftName());
/* 
perform chores specific to WINDS management and control
*/
 need_start_input=FALSE;
 SetFrameLabel("WINDS starting up other tasks...");
 if (WindsControlHost) {
  char NoStartWinputFilename[MAX_FLNM_LNGTH];
  FILE *WINDSfopen();
/*
get addresses of shmem vars for pids of tasks to be started
*/
  if (!GetProcessPids()) {
   RequestAlertNow("FATAL: problem completing shared memory mgmt...contact WINDS expert with this information!");
   winds_cleanup(ERROR);
  }
/* 
determine whether a previous WindsControlHost session exited
without killing off other displays -- in which case, some of
the following chores are not desired
*/
  (void)sprintf(&NoStartWinputFilename[0],NO_START_INPUT,ProjectDirectory,proj_number);
  if ((NoStartWinputFileHandle=WINDSfopen(NoStartWinputFilename,"r"))==NULL) {
/* 
no flag file: proceed with chores
*/
   StartAllWINDSTasks();
   need_start_input=TRUE;
  } else {
/*
flag file is there, meaning winput should have been left running. Verify that
indeed it is, and if not notify user and start it up 
*/
   if (!ConfirmWinputRunning(FALSE)) {
    RequestAlertNow("WARNING: last WINDS session left a flag that winput process should have been left running, but winput was not there. This new WINDS session will now start up a winput process instead, removing that flag to prevent confusion next time around...");
    (void)unlink(NoStartWinputFilename);
    StartAllWINDSTasks();
    need_start_input=TRUE;
   }
  }
  (void)WINDSfclose(NoStartWinputFileHandle);
 } else {
/* 
non-WindsControlHost: make sockets and startup printer tasks
*/
  start_winds_tasks();
 }
/* 
wait on input process successfully finding data input file (if post-processing
mode)
*/
 SetFrameLabel("WINDS waiting on shmem initialization...");
 if (post_processing_mode() 
  || GetInputMode()==DISK_DATA
  || GetInputMode()==RAWFILE_NETCDF
 ) {
  WaitOnDatasetFound();
 }
/*
wait for input process to set length of derived variable names array, then
use it to set maximum shmem block size
*/
 wait_on_allocn_sizing("WINDS");
 set_max_shmem_size("WINDS");
/*
follow through on chores if input had to be started
*/
 if (need_start_input) {
  if (get_ids_dynamic("WINDS")==ERROR) {
   RequestAlertNow("FATAL: problem getting dynamic shared memory ids...contact WINDS expert with this information!");
   winds_cleanup(ERROR);
  }
  if (WindsControlHost) 
   if (write_dynamic_ids("WINDS")==ERROR) {
    RequestAlertNow("FATAL: problem completing write of dynamic shared memory ID's file...contact WINDS expert with this information!");
    winds_cleanup(ERROR);
   }
 } else {
  read_dynamic_ids("WINDS",GetComputeHost());
 }
 if (getaddr_dynamic()==ERROR) {
  RequestAlertNow("FATAL: problem getting dynamic shared memory addresses...contact WINDS expert with this information!");
  winds_cleanup(ERROR);
 }
/*
AFTER dynamic shmem addresses are gotten, initialize h:m:s arrays IF input
is being re-started
*/
 if (need_start_input) 
  InitTimeStamps();
/*
wait on input process to initialize variable name lists
*/
 WaitOnVariableLists();
/* 
get plot attributes AFTER variable name lists have been initialized
...which is waited on by WaitOnVariableLists()
*/
 GetVarAttrs();
/*
create internal sorted list of var names AFTER plot attributes have been done
*/
 InitMenuList();
/* 
initialize cross-reference array of type classification (analog v. digital)
*/
 if (!post_processing_mode())
  init_hdr_types();
/* 
get project-specific configuration AFTER variable name lists have been initialized
*/
 get_config();
/* 
get date set, either from system clock or from request to input process
*/
 set_date_string();
/* 
read in display setup file AFTER input has initialized varname arrays and 
responded to date request above
*/
 SetCurrentPlotspexFolderName("default");
 GetSavedPlotspexFolderName();
 ReadInPlotspex();
/*
determine length of longest name in lists - helpful in laying out panel menu
geometry
*/
 SetMaxNameLength();
/* 
get displays established AFTER input has initialized varname arrays
*/
 fill_displays(0);

/* get any boundary and landmark data read in (for drawing onto track plots)
*/
 (void)sprintf(&data_filename[0],BOUNDARIES,ProjectDirectory,proj_number);
 init_boundary_data(data_filename);
 (void)sprintf(&data_filename[0],LANDMARKS,ProjectDirectory,proj_number);
 init_landmark_data(data_filename);
printf("WINDS: Leaving function from hell.\n");
}

/*********************  ConfirmWinputRunning()  *****************************/

ConfirmWinputRunning(restart) 
int restart;
{
/*
confirm that winput is running
*/
 if (GetProtocolDebug())
  (void)fprintf(stderr,"WINDS PROTOCOL: ask winput if it's still there...\n");
 display_request(ARE_YOU_LISTENING,"Check for winput running...");
 if (!input_acknowledge(FALSE)) {
  if (GetProtocolDebug())
   (void)fprintf(stderr,"WINDS PROTOCOL: winput NOT responding.");
  (void)fprintf(stderr,"WARNING: WINPUT should have been there but wasn't\n");
  if (restart) {
/*
it's not there, restart it
*/
   (void)fprintf(stderr,
   "WINDS: re-starting WINPUT\n");
   start_task(INPUT);
  }
  return FALSE;
 } else {
  if (GetProtocolDebug())
   (void)fprintf(stderr,"WINDS PROTOCOL: winput responds OK");
  (void)fprintf(stderr,"WINDS: WINPUT is there...\n");
  return TRUE;
 }
}

/*********************  WaitOnDatasetFound()  *****************************/
WaitOnDatasetFound()
/*
block with 5-second time-out, 5 times, waiting for input to find requested
dataset or to respond with message about some other problem.
*/
{
  int attempts,length,found;
  char *s;

 attempts=0;

 do {
  CheckForNetworkMessages();
  length=MAX_SOCKET_BUFF_LNGTH;

  if (GetProtocolDebug())
   (void)fprintf(stderr,"WINDS PROTOCOL: wait on reply from DISP_REQ_REPLY: ");

  s=(char *)read_from_socket(DISP_REQ_REPLY,&length,BLOCK_TIME_OUT);
  if (strcmp(s,EBADREAD)) {
/* 
it's a good read
*/
   (void)sscanf(s,"%d",&found);
   switch (found) {
   case DATASETNOTFOUND:
    if (GetProtocolDebug())
     (void)fprintf(stderr,"WINDS PROTOCOL: DATASETNOTFOUND\n");
/*
WINPUT couldn't find it, and has sent a message describing where it looked
*/
    length=MAX_SOCKET_BUFF_LNGTH;
    s=(char *)read_from_socket(DISP_REQ_REPLY,&length,BLOCKING);
    if (GetProtocolDebug())
     (void)fprintf(stderr,"WINDS PROTOCOL: got dataset not found message text\n");
    ShowDatasetMessageAndExit(s);
   case SEARCHNOTDONE:
/*
WINPUT hasn't found it yet; give it a little time
*/
    if (GetProtocolDebug())
     (void)fprintf(stderr,"WINDS PROTOCOL: winput still searching...\n");
    (void)sleep(2);
    break;
   case DATASETISFOUND:
/*
found; get further info about file
*/
    length=MAX_SOCKET_BUFF_LNGTH;
    s=(char *)read_from_socket(DISP_REQ_REPLY,&length,BLOCKING);
    if (GetProtocolDebug())
     (void)fprintf(stderr,"WINDS PROTOCOL: DATASETISFOUND\n");
    (void)sscanf(s,"%s %d %d",NetCDFFilename,&DataStartTime,&DataEndTime);
    if (post_processing_mode())
     (void)fprintf(stderr,"NetCDF File: %s Start: %s End: %s\n",
      NetCDFFilename,(char *)strdup((char *)get_time_from_base(DataStartTime)),
      (char *)strdup((char *)get_time_from_base(DataEndTime)));
    return; 
   }
/*
either a time out on socket or search is not yet done - tell user what's 
going on and try again
*/
  }
  RequestAlertNow("Waiting for input process to find requested dataset...");
/* 
confirm that winput hasn't died since last time we checked, which sometimes 
happens as it begins to process the dataset. 
*/
/***
  if (!ConfirmWinputRunning(FALSE)) {
   RequestAlertNow("FATAL: input process is not responding and may have some problem running. Check your console for messages and contact a WINDS expert about this message...");
   winds_cleanup(ERROR);
  }
***/
 } while (++attempts<3);
 RequestAlertNow("Giving up...assuming input process couldn't find requested dataset or has some other problem. Contact a WINDS expert about this message...");
 winds_cleanup(ERROR);
}

/***************************************************************************/

char *
GetNetCDFFilename()
{
 return NetCDFFilename;
}

/***************************************************************************/

GetTimeInt(starttime)
int starttime;
{
 int time,hr,min,sec;

 if (post_processing_mode()) {
  if (starttime) {
   time=GetDataStartTime();
  } else {
   time=GetDataEndTime();
  }
 } else {
  if (starttime) {
   time=GetStartTimeIndex();
   hr=GetHMS(time*3); min=GetHMS(time*3+1); sec=GetHMS(time*3+2);
  } else {
   time=GetEndTimeIndex();
   hr=GetHMS(time*3); min=GetHMS(time*3+1); sec=GetHMS(time*3+2);
  }
 }

 if (post_processing_mode())
  return GetTimeIntFromBase(time);
 else
  return GetTimeIntFromHMS(hr,min,sec);
}

/***************************************************************************/

GetEndTimeIndex()
{
 return (GetWINDSElapsedTime()-1)%get_datablock_size();
}

/***************************************************************************/

GetStartTimeIndex()
{
 return (GetWINDSElapsedTime()<get_time_frame_size()?0:
  GetWINDSElapsedTime()-get_time_frame_size()%get_datablock_size());
}

/***************************************************************************/

GetDataStartTime()
{
 return DataStartTime;
}

/***************************************************************************/

GetDataEndTime()
{
 return DataEndTime;
}

/****************************  GetWINDSCmdLine()  ******************************/

char *
GetWINDSCmdLine()
/*
return the string representing the startup command line
*/
{
 return WINDSCmdLine;
}

/******************  ShowDatasetMessageAndExit()  ****************************/

ShowDatasetMessageAndExit(filenames)
char *filenames;
{
char alertmsg[1024];

 time_to_log();
 (void)sprintf(alertmsg,"FATAL PROBLEM: Dataset and/or header files for dataset indicated in command line were not found. The command line used to start this session looks like this: %s. Given that, WINDS then looked for the following filenames, but without success: %s. This WINDS session will now terminate; please check available datasets and, if necessary, consult the WINDS manual(s) for help in session startup procedures.",(char *)GetWINDSCmdLine(),filenames);
 RequestAlertNow(alertmsg);
 winds_cleanup(ERROR);
}

/**************************  MAKEDIR()  **************************************/

makedir(prefix,suffix)
char *prefix,*suffix;
/*
check that pathname as constructed from prefix and suffix exists. If not, make 
it.
*/
{
char pathname[MAX_FLNM_LNGTH];

 (void)sprintf(pathname,"%s%s%s",
  prefix,strlen(suffix)&&strlen(prefix)?"/":"",suffix);
 errno=0;
/*
make directory with rwx for all
*/
 if ( mkdir(pathname,0x1ff)==ERROR) {
  switch (errno) {
   case EEXIST:
    break;
   default:
   (void)fprintf(stderr,"FATAL: errno %d creating dir %s\n",
    errno,pathname);
   RequestAlertNow("FATAL: WINDS is exiting due to error creating a directory. Please contact a WINDS expert about this message.");
   winds_cleanup(ERROR);
  }
 }
}

/**********************  GetDisplayHost()  ******************************/
char *GetDisplayHost()
{
 return display_host;
}

/* -------------------------------------------------------------------- */
void WINDSInitStuff(int argc, char **argv)
{
  FILE	*WINDSfopen();
  char	filename[MAX_FLNM_LNGTH];

  static int first_call = TRUE;
  static char displayenv[MAX_FLNM_LNGTH];

  if (first_call) {
    (void)sprintf(TZstring,"%s","TZ=GMT");
    (void)putenv(TZstring);

    /* ensure all files & directories created have rwx for all
     */
    (void)umask(0);
    first_call = FALSE;

    /* get environment
     */
    if ((winds_path = (char *)getenv("WINDS")) == NULL) {
      RequestAlertNow("FATAL: WINDS process is exiting due to WINDS environment variable not being set. Contact a WINDS expert about this problem.");
      winds_cleanup(ERROR);
    }

    if ((DataDirectory = (char *)getenv("DATA_DIR")) == NULL)
       DataDirectory = "/home/data/";

    if ((ProjectDirectory = (char *)getenv("PROJ_DIR")) == NULL)
      ProjectDirectory = (char *)GetDefaultProjDir();


    /* help files search path adjustment
     */
    (void)sprintf(winds_helppath,HELPPATH,winds_path);
    (void)putenv(winds_helppath);

    /* etcetera path names
     */
    (void)sprintf(winds_bin_path,BINPATH,winds_path);
    (void)sprintf(WindsHostsPath,HOSTPATH,ProjectDirectory);

    /* get the version # for log file, base frame display
     */
    SetWINDSVersion();
    (void)fprintf(stderr,"WINDS Version %s\n", (char *)GetWINDSVersion());

    /* get WINDS arguments, utility values, etc.
     */
    my_pid = getpid();
    SetComputeHost();
    Argc = argc;
    Argv = argv;
    get_winds_args(Argc, Argv);
    (void)fprintf(stderr,"Startup Command: %s\n", (char *)GetWINDSCmdLine());
    (*SetColorMap[GetDrawType()])(color_map);

    /* for some reason, MUST do this to get xv_init() call to function properly
     * ...call AFTER get_winds_args to have good value for x_display
     */
    (void)sprintf(displayenv,"DISPLAY=%s",x_display);
    (void)putenv (displayenv);

    /* wait for server on requested host to come up
     */
    while ((Display *)XOpenDisplay(x_display) == NULL) {
      static int times_to_try=60;

      /* even though XOpenDisplay returns a Display *, don't bother getting
       * it here; it seems to be not completely initialized and causes a "Bad
       * Drawable" error on first Xlib graphics call.  A usable Display * gets
       * filled in InitDraw().
       */
      (void)fprintf(stderr,"Trying to connect to %s...will try %d more times\n",
					x_display,times_to_try);

      if (--times_to_try) {
        sleep(1);
      } else {
        fprintf(stderr,
        "WARNING: waited for %s server for 60 seconds...giving up\n",x_display);
        fprintf(stderr, "Host MUST have xhost permission, X-server running AND ethernet transceiver termination, \n");
        fprintf(stderr,"even if host is a standalone, not on a network!\n");
        RequestAlertNow("FATAL: WINDS is exiting because connection to server could not be established. Please contact a WINDS expert about this message.");
        winds_cleanup(ERROR);
      }
    } 
  }


  /* get netconfig entries for display host
   */
if (strchr(display_host, '.')) /* added cjw 9/2/99 */
  *(strchr(display_host, '.')) = '\0';

  if (get_netconfig_entry(display_host,display_mode,display_pnum,display_file) ==ERROR) {
    RequestAlertNow("FATAL: WINDS is exiting due to error trying to get the Netconfig File entry. Please contact a WINDS expert about this message.");
    winds_cleanup(ERROR);
  }

  SetScreenGeometry();
  set_num_displays();

  /* set input mode, project # string
   */
  if (SetInputMode()==ERROR) {
    RequestAlertNow("FATAL: WINDS is exiting due to invalid input mode. Please contact a WINDS expert about this message.");
    winds_cleanup(ERROR);
  }

  /* set filename for WINDS user preferences
   */
  if (post_processing_mode()) {
    if ((char *)getenv("HOME")==(char *)NULL) {
     (void)fprintf(stderr,"WARNING: HOME environment variable not set\n");
     (void)sprintf(UserHomeDir,"%s/%s",WindsHostsPath,display_host);
    } else {
     (void)sprintf(UserHomeDir,"%s",(char *)getenv("HOME"));
    }
  } else {

  /* onboard: use winds' home dir, per display host
   */
  (void)sprintf(UserHomeDir,"%s/%s",WindsHostsPath,display_host);
  }


  /* determine display-specific parameters
   */
  connected_to_ads = data_from_ads();
  set_proj_no();
  set_project();

  /* set flag if ascii data is to be sent out from this display station
   */
  (void)sprintf(&filename[0],ASCII_PARMS,ProjectDirectory,proj_number);
  SetAsciiFlag(filename);

  /* set flag if parcel tracking data is to be sent out from this display
   */
  (void)sprintf(&filename[0],FLAT_PANEL_PARMS,ProjectDirectory,proj_number);
  set_flat_panel_flag(filename);

  /* set port server host for this display station
   */
  set_port_server_host();

  /* now that pnum, etc., is set, determine control host, etc. This should NOT
   * be done until AFTER all set_xxx() calls have been made based on original
   * get_netconfig_entry() call!!!
   */
  switch (GetInputMode()) {
    case ERROR:
      (void)fprintf(stderr,"Input mode: ERROR\n");
      break;

    case ADS_DATA:
      (void)fprintf(stderr,"Input mode: ADS_DATA\n");
      if (!FindAndSetControlHost()) {
        (void)fprintf(stderr,"WARNING: no control host found; setting to %s\n",
         GetComputeHost());
        SetControlHost(GetComputeHost());
      }
      break;

    case RAWFILE_NETCDF:
      (void)fprintf(stderr,"Input mode: RAWFILE_NETCDF\n");
      if (!FindAndSetControlHost()) {
        (void)fprintf(stderr,"WARNING: no control host found; setting to %s\n", 
         GetComputeHost()); 
        SetControlHost(GetComputeHost());
      }
      break;

    case DISK_DATA:
      (void)fprintf(stderr,"Input mode: DISK_DATA\n");
      SetControlHost(GetDisplayHost());
      break;

    case NETCDF:
      (void)fprintf(stderr,"Input mode: NETCDF\n");
      SetControlHost(GetDisplayHost());
      break;

    case INIT_SETUP:
      (void)fprintf(stderr,"Input mode: INIT_SETUP\n");
      SetControlHost(GetDisplayHost());
      break;

    case WINDS_SLAVE:
      (void)fprintf(stderr,"Input mode: WINDS_SLAVE\n");
      if (!FindAndSetControlHost()) {
        (void)fprintf(stderr,"WARNING: no control host found; setting to %s\n", 
         GetComputeHost()); 
        SetControlHost(GetComputeHost());
      }
      break;

    default:
      (void)fprintf(stderr,"WARNING: Unknown Input mode: %d\n",GetInputMode());
      break;
  }

  WindsComputeHost=IsWindsComputeHost(display_host);
  WindsControlHost=IsWindsControlHost(display_host);
  (void)fprintf(stderr,
	"WINDS pid: %d; ComputeHost: %s; DisplayHost: %s ControlHost: %s\n",
	my_pid,GetComputeHost(),GetDisplayHost(),GetControlHost());
  (void)fprintf(stderr,"This %s session %s the WindsControlHost\n",
	display_host,WindsControlHost?"IS":"is NOT");

  /* using port server hostname, determine printer parameters.  Since we are
   * using get_entry_by_name(), the first entry with matching hostname in the 
   * netconfig file will be the one used, so that entry's ttya|b values must
   * be set correctly.
   */
  if ( get_entry_by_name(get_port_server_host())==ERROR) {
    (void)fprintf(stderr, "Port server host '%s' not found; NO printing of lists, stats, or images for %s\n", get_port_server_host(),display_host);
  } else {
    set_printer_ports();
    switch ( get_lstat_port()) {
      case HP9876SOCKET:
        fprintf(stderr, "printer hp9876 on %s does lists and stats for %s\n",
		get_port_server_host(),display_host);
        break;
      case PJETSOCKET:
        fprintf(stderr, "printer pjet on %s does lists and stats for %s\n",
		get_port_server_host(),display_host);
        break;
      case EPSONSOCKET:
        fprintf(stderr, "printer epson on %s does lists and stats for %s\n",
		get_port_server_host(),display_host);
        break;
      case ERROR:
        fprintf(stderr, "NO printer designated for lists or stats for %s\n",
		display_host);
        break;
    }

    switch ( get_bw_image_port()) {
      case HP9876SOCKET:
        (void)fprintf(stderr, "printer hp9876 on %s does b/w images for %s\n",
		get_port_server_host(),display_host);
        break;
      case PJETSOCKET:
        (void)fprintf(stderr, "printer pjet on %s does b/w images for %s\n",
		get_port_server_host(),display_host);
        break;
      case ERROR:
        (void)fprintf(stderr, "NO printer designated for b/w images for %s\n",
		display_host);
        break;
    }

    switch ( get_color_image_port()) {
      case PJETSOCKET:
       (void)fprintf(stderr, "printer pjet on %s does color images for %s\n",
		get_port_server_host(),display_host);
       break;
      case ERROR:
       (void)fprintf(stderr, "NO printer designated for color images for %s\n",
		display_host);
       break;
    }
  }

  num_of_printees=0;

  /* make subdirectories after X-connection is established and hostnames are set
  */
  makedir(WindsHostsPath,"");
  makedir(WindsHostsPath,GetControlHost());
  makedir(WindsHostsPath,GetComputeHost());
  makedir(WindsHostsPath,GetDisplayHost());
  makedir(ProjectDirectory,proj_number);

  /* set general printer configuration AFTER getting info from netconfig on 
   * serial port printer setup
   */
  GetPrintConfig();

  /* initialize state parameters not dependent on quadrant displays or that
   * need doing only once, at startup.
   */
  init_printer_parms();
  InitPMS1Dsetup();

  /* set network communications
   */
  make_sockets();

}

/****************************************************************************/

char *
GetWindsHostsPath()
{
 return WindsHostsPath;
}

/************************  SET_PROJECT()  ***********************************/

set_project()
{
  proj_number = (char *)strdup(get_proj_number());
  fprintf(stderr,"Project # %s (%s)\n", proj_number,(char *)GetWINDSProjectName());
  sprintf(project,"Proj. %s",proj_number);
  proj_length = strlen(project);
}

/****************************  SET_DATE_STRING()  ***************************/

set_date_string()
{
int disp_reply,length;
static char buffer[NAMLEN*2];
char *s;

 if (post_processing_mode()) {

/* analysis mode -- set date from info in data file
*/
  (void)sprintf(buffer,"%d",REQ_DATE);
  if (GetProtocolDebug())
   (void)fprintf(stderr,"WINDS PROTOCOL: request date from winput\n");
  if (write_to_socket(DISP_REQ,&buffer[0],strlen(&buffer[0]))==EBADWRITE)
   perror("request date from input write_to_socket");
/* 
wait for input side ACK
*/
  if (GetProtocolDebug())
   (void)fprintf(stderr,"WINDS PROTOCOL: waiting on input response on date request...\n");
  length=MAX_SOCKET_BUFF_LNGTH;
  s=(char *)read_from_socket(DISP_REQ_REPLY,&length,BLOCK_TIME_OUT);
  if (strcmp(s,EBADREAD)) {
   (void)sscanf(s,"%d",&disp_reply);
   if (disp_reply!=LISTENING) {
    if (GetProtocolDebug())
     (void)fprintf(stderr,"WINDS PROTOCOL: input is listening...\n");
    time_to_log();
    (void)fprintf(stderr,
     "WARNING: NON_ACK response from DISP_REQ_REPLY socket!\n");
    return;
   } else {
    if (GetProtocolDebug())
     (void)fprintf(stderr,"WINDS PROTOCOL: got date from input.\n");
    time_to_log();
    (void)fprintf(stderr,"WINDS got date from INPUT...\n");
    length=MAX_SOCKET_BUFF_LNGTH;
    s=(char *)read_from_socket(DISP_REQ_REPLY,&length,BLOCKING);
    SetCurrentDate(s);
   }
  } else {
   time_to_log();
   (void)fprintf(stderr,"WARNING: NO RESPONSE or BAD READ from socket!\n");
   RequestAlertNow("WARNING: winput process failed to respond to request for current date...this may indicate that it is not running. Contact a WINDS expert with this information.");
   winds_cleanup(ERROR);
  }
 } else if (GetInputMode()==DISK_DATA
         || GetInputMode()==RAWFILE_NETCDF
 ) {
/*
simulated real-time: get date from header
*/
  char *date;
  GetHeaderDate(&date);
  SetCurrentDate(date);
 } else {
/* 
real-time mode -- set date in real time
*/
  SetCurrentDate((char *)GetRealTimeDate());
 }
}

/************************  GetCurrentFlight()  ****************************/

char *
GetCurrentFlight()  
{
static char *currentflight=NULL;

/*
if currentflight is unallocated OR it has already been allocated via strdup() 
with a value different from current post-processing session flight, release 
previous memory and re-allocate with correct value.
*/
 if (!currentflight || 
    ((post_processing_mode() && 
     currentflight && strcmp(currentflight,(char *)get_pp_filename())))) {
  if (currentflight)
   (void)free(currentflight);
  if (GetInputMode()==RAWFILE_NETCDF || post_processing_mode()) {
    currentflight=((char *)strdup((char *)get_pp_filename()));
  } else {
   currentflight=((char *)hdr_fltno());
  }
 }
 if (currentflight) {
  if (strlen(currentflight)) {
   return currentflight;
  } else {
   return "unknown";
  }
 } else {
  return "unknown";
 }
}

/************************  GetCurrentDate()  ****************************/

char *
GetCurrentDate()
{
 return CurrentDate;
}

/************************  SetCurrentDate()  ****************************/

SetCurrentDate(date)
char *date;
/*
update either a playback or real-time notion of just what day it is, and
draw it in the Control Panel date box
*/
{
 strcpy(CurrentDate, date);
 DrawControlPanelDate(CurrentDate);
}

/************************  GetRealTimeDate()  ****************************/

char *
GetRealTimeDate()
/*
ask the system what day it is
*/
{
int month;
static char currentdate[16];

 month=get_month_num();
/* 
print months 1-9 with leading zero; assumed get_day() and get_year()
handle days 1-9 and years 0-9 similarly
*/
 (void)sprintf(currentdate,"%02d/%s/%s",month,get_day(),get_year());
 return currentdate;
}

/************************  INIT_STATE_PARMS()  ****************************/

init_state_parms(quadrant,type)
int quadrant,type;

/* initialize parameters presumably after a window quadrant is replaced by
   another setup.  These parameters are initialized at compile time statically,
   and this function is provided to cover the case where a different 
   window type takes over in a quadrant.  For xy-plot module, this should
   not be called until AFTER plot_type is set, since set_window_type depends
   on that value.
*/
{
 set_window_type(quadrant,type);
 init_status_parms(quadrant);
 if (hilite_window_type(quadrant))
  init_hilite_parms(quadrant);
 SetDisplayFont(quadrant);
}
 
/**************************  WINDSGetConstants()  **************************/

WINDSGetConstants()
{
/* various inits to minimize CPU crunch during one-second updating
*/
 time_length=strlen("xx:xx:xx");
}

/************************** CatchExitSig()  *********************************/
 
Notify_value CatchExitSig(client,sig,when) 
Notify_client client;    
int sig; 
Notify_signal_mode when; 
{ 
 (void)fprintf(stderr,"FATAL: caught exit signal (%s), no core dump\n",
  GetExitSignalName(sig));
 CheckForNetworkMessages();
 winds_cleanup(ERROR);
 return(NOTIFY_DONE); 
}

/*********************** CatchCoreDumpSig()  ******************************/
 
Notify_value CatchCoreDumpSig(client,sig,when) 
Notify_client client;    
int sig; 
Notify_signal_mode when; 
{ 
  (void)fprintf(stderr,"FATAL: caught signal (%s), dumping core\n",
   GetCoreSignalName(sig));
  winds_cleanup(INTERRUPT);
  return(NOTIFY_DONE); 
}

/**************************************************************************/
 
AdjustBaseFrameDimensions(nominalwidth,nominalheight)
int nominalwidth,nominalheight;
/*
Want the # of bytes in each quadrant sized setup window to be an EVEN number
of bytes in width, AFTER deducting the IMAGE_MARGIN, provided to preclude 
GetImage() errors in print_image(), so that a minimum of the image needs to be 
truncated in sends to printers. Adjust base_frame_width accordingly to meet 
that goal. (See print_image() for details.)

Fulfilling these criteria for the quad-size window does NOT provide for the
strip/full sizes, since those sizes are a factor of 2 larger but IMAGE_MARGIN
is being added in twice here. No big deal (pas grande chose) since print_image()
is doing any add'l needed truncation before sending image to printer. This
step here is simply to minimize that additional amount.
*/
{
 nominalwidth=MAX(nominalwidth,640);
 nominalheight=MAX(nominalheight,480);
 base_frame_width=((((nominalwidth-WINDS_WIDTH_MARGIN)/2)/(BYTE_LEN*2))
  *(BYTE_LEN*2)+(IMAGE_MARGIN+1))*2;
 base_frame_height=nominalheight-WINDS_HEIGHT_MARGIN;
}

/***************************************************************************/

SetWINDSDimensions()
{
 XSetAttr(GetBaseFrame(),
          XV_HEIGHT, base_frame_height,
          XV_WIDTH, base_frame_width,
	NULL);
 control_height=BUTTON_HEIGHT;
 display_height=base_frame_height-control_height;
 control_width=base_frame_width;
 SetStandardFont();
 SetTimeWindowWidth();
}

/***************************************************************************/

SetCanvasDimensions(which,x,y,width,height)
int which, x, y, width, height;
{
 XSetAttr(canvas[which],
	 XV_X, x,
	 XV_Y, y,
         XV_HEIGHT, height,
         XV_WIDTH, width,
	NULL);
}

/***********************************************************************/
 
GetBaseFrameWidth()
{
 return base_frame_width;
}

/***********************************************************************/
 
GetBaseFrameHeight()
{
 return base_frame_height;
}

/***********************************************************************/
 
ResizeWindows()
{ 
int new_width,new_height,quadrant;
static int resizedebug=TRUE;

 new_width=XGetAttr(GetBaseFrame(),XV_WIDTH);
 new_height=XGetAttr(GetBaseFrame(),XV_HEIGHT);
 if (new_width!=base_frame_width || new_height!=base_frame_height) {
  if (resizedebug)
   printf("new requested (base frame) width %d, height %d\n",
    new_width,new_height);
  AdjustBaseFrameDimensions(
/*
add nominal width margin to user-drawn width so that the starting point for
adjustment includes the window mgr borders; this coordinates with startup
where base frame width is adjusted for printing purposes to as close as 
possible to startup request
*/
   new_width+WINDS_WIDTH_MARGIN,
/*
add margin to requested height on user re-size; on startup, this is NOT
done and automatically provides for window manager borders and a thin sliver
at bottom of screen, but here the user has drawn it the way it's wanted, so
it's ok to cancel out that adjustment by padding with that margin here.
*/
   new_height+WINDS_HEIGHT_MARGIN);
  if (resizedebug)
   printf("adjusted base frame width %d, height %d\n",
    base_frame_width,base_frame_height);
  SetWINDSDimensions();
  SetCanvasDimensions(TIME_AND_PROJECT,0,display_height,
   GetTimeWindowWidth(),control_height);
  SetCanvasDimensions(HELP_IMAGE, 
   base_frame_width-NUM_BUTTONS*BUTTON_WIDTH, display_height,
   BUTTON_WIDTH*NUM_BUTTONS, BUTTON_HEIGHT);
  SetCanvasDimensions(COPYRIGHT,0,0,control_width, display_height);
  for (quadrant=0; quadrant<get_num_displays(); quadrant++) {
   set_geom_parms(quadrant,get_geom(quadrant));
   SetCanvasDimensions(quadrant,window_x[quadrant],window_y[quadrant],
    window_width[quadrant],window_height[quadrant]);
   SetOpPanelGeom(quadrant);
   SetDisplayFont(quadrant);
   (void)refresh_window(INITIAL,quadrant);
  }
  RemoveAllPanelMenus();
  ResizeMenusAndPanels();
  ShowTime();
  if (GetCalibWindowsMade()) {
   ResizeCalibWindows();
   if (GetCalibDisplayUp()) {
    ShowCalibDisplay();
   } else {
    HideCalibDisplay();
   }
  }
 } 
}

/*****************************************************************************/

SetTimeWindowWidth()
{
 SetMaxLenTimeWindowString();
 TimeWindowWidth=(GetMaxLenTimeWindowString()+2)*
  GetFontWidth(GetControlPanelFont());
}

/*****************************************************************************/

GetTimeWindowWidth()
{
 return (TimeWindowWidth);
}

/**********************  WINDSInitWindows()  *********************************/

WINDSInitWindows()
{
int quadrant;				/* which quarter-area of screen */
int signal;

 AdjustBaseFrameDimensions(GetScreenWidth(),GetScreenHeight());
 CreateBaseFrame("WINDS Starting up...");
 SetWINDSFonts(GetBaseFrame());
 SetWINDSDimensions();
/*
catch various signals and cleanup, then either dump core and exit or just exit
*/
 for (signal=0; signal < GetNumberCoreSignals(); signal++ ) {
  (void)notify_set_signal_func(GetBaseFrame(),CatchCoreDumpSig,
   GetCoreSignal(signal),NOTIFY_SYNC);
 }
 for (signal=0; signal < GetNumberExitSignals(); signal++ ) {
  if (signal != SIGTERM)
   (void)notify_set_signal_func(GetBaseFrame(),CatchExitSig,
    GetExitSignal(signal),NOTIFY_SYNC);
 }
 (void)notify_set_signal_func(GetBaseFrame(),CatchExitSig,SIGTERM,NOTIFY_ASYNC);

 canvas[TIME_AND_PROJECT]=XSetAttr(GetBaseFrame(), CANVAS,
         WIN_CMS, cms,
         CANVAS_REPAINT_PROC, show_time,
         NULL);
 SetCanvasDimensions(TIME_AND_PROJECT,0,display_height,
  GetTimeWindowWidth(),control_height);
 paint_window[TIME_AND_PROJECT]=canvas_paint_window(canvas[TIME_AND_PROJECT]);
 pw_xid[TIME_AND_PROJECT]=(Drawable)XGetAttr(paint_window[TIME_AND_PROJECT],
  XV_XID);
  (void)XSetAttr(paint_window[TIME_AND_PROJECT],
#include <event_attr.h>
          WIN_EVENT_PROC, event_proc,
         NULL);

/* create help image, menus */

 canvas[HELP_IMAGE] = XSetAttr(GetBaseFrame(), CANVAS,
                CANVAS_REPAINT_PROC, get_help,
                WIN_CMS, cms,
               NULL);
 SetCanvasDimensions(HELP_IMAGE, 
  base_frame_width-NUM_BUTTONS*BUTTON_WIDTH, display_height,
  BUTTON_WIDTH*NUM_BUTTONS, BUTTON_HEIGHT);
  paint_window[HELP_IMAGE]=canvas_paint_window(canvas[HELP_IMAGE]);
  (void)XSetAttr(paint_window[HELP_IMAGE],
#include <event_attr.h>
          WIN_EVENT_PROC, event_proc,
         NULL);

/* default geometry is function of display type
*/
 for (quadrant=0; quadrant<get_num_displays(); quadrant++) {

  set_geom_parms(quadrant,get_default_geom());
  canvas[quadrant]=XSetAttr(GetBaseFrame(), CANVAS,
         CANVAS_REPAINT_PROC, CanvasRepaintProc,
         WIN_CMS, cms,
        NULL);
  SetCanvasDimensions(quadrant, 
   window_x[quadrant],window_y[quadrant],
   window_width[quadrant],window_height[quadrant]);
  paint_window[quadrant]=canvas_paint_window(canvas[quadrant]);
  pw_xid[quadrant]=(Drawable)XGetAttr(paint_window[quadrant],XV_XID);
  (void)XSetAttr(paint_window[quadrant],
#include <event_attr.h>
          WIN_EVENT_PROC, event_proc,
         NULL);

 }
/* introduction and copyright window
*/
  canvas[COPYRIGHT]=XSetAttr(GetBaseFrame(), CANVAS,
         CANVAS_REPAINT_PROC, draw_copyrite,
         WIN_CMS, cms,
        NULL);
  SetCanvasDimensions(COPYRIGHT,0,0,control_width, display_height);
  paint_window[COPYRIGHT]=canvas_paint_window(canvas[COPYRIGHT]);
  pw_xid[COPYRIGHT]=(Drawable)XGetAttr(paint_window[COPYRIGHT],XV_XID);

 (*InitDraw[GetDrawType()])();

/* get miscellaneous fonts, create additional objects.  NOTE: be sure to
   make at least ONE PANEL here (before window_main_loop()) or the fonts
   will be confused.  DON'T ASK ME WHY.
*/
 make_help();
}

/***************************  MAKE_SOCKETS()  ********************************/

make_sockets()
{
/*
notifications/etc. between input/winds processes, e.g. need to re-init shared 
file memory maps, notify that date must change after cross midnite, etc.
*/
SetupWindsBroadcast("WINDS-INTERWINDS-TALK");
if (SetupWindsListening("WINDS-INTERWINDS-LISTEN")==ERROR) {
/*
socket is already bound. Find another host for winds startup and/or exit.
*/
 if (post_processing_mode())
  FindAvailableHost();
 else{
  RequestAlertNow("FATAL: WINDS is exiting due to failure establishing INTERWINDS-LISTEN socket. Please contact a WINDS expert about this message.");
  winds_cleanup(ERROR);
 }
}
/*
requests for PostScript printer output to psprint server on local host
*/
talk_to_socket("WINDS-POSTSCRIPT",POSTSCRIPT,POSTSCRIPT,GetComputeHost());
/* 
printer requests may be routed to anywhere on network, to provide for sharing
for hosts without a printer attached.  Use entries in netconfig file to 
determine where for this WINDS process.
*/
if (get_lstat_port()!=ERROR || UsingNonLocalPortServer()) {
 talk_to_socket("WINDS-PJETSOCKET",PJETSOCKET,PJETSOCKET,get_port_server_host());
 talk_to_socket("WINDS-EPSONSOCKET",EPSONSOCKET,EPSONSOCKET,get_port_server_host());
}

/* Flat panel displays are NOT a shareable resource.  Each process may only 
   talk to a flat panel server on its own ports.  There really is no need
   for sharing anyway, since the data looks exactly the same no matter where
   it comes from!
*/
 talk_to_socket("WINDS-ASCIIVALUES",ASCIIVALUES,ASCIIVALUES,display_host);
 talk_to_socket("WINDS-FLAT_PANEL_DATA",FLAT_PANEL_DATA,FLAT_PANEL_DATA,display_host);

/* DISP_REQ's all go to one place for now: the input process on WindsComputeHost
   display, which is on GetComputeHost().  ANY WINDS process may talk to this 
   socket.
*/
 talk_to_socket("WINDS-DISP_REQ",DISP_REQ,DISP_REQ,GetComputeHost());

 if (WindsControlHost) {
/* 
note that while WINDS processes other than WindsControlHost may talk to DISP_REQ
socket, they should NOT wait for a reply after making requests.  Only the
WindsControlHost expects one, so only the WindsControlHost need listen on the 
DISP_REQ_REPLY socket.
*/ 
  if (SetListeningPost("WINDS-DISP_REQ_REPLY",DISP_REQ_REPLY,DISP_REQ_REPLY)==ERROR) {
/*
socket is already bound. Find another host for winds startup and/or exit.
*/
   if (post_processing_mode())
    FindAvailableHost();
   else {
    RequestAlertNow("FATAL: couldn't establish DISP_REQ_REPLY socket...contact WINDS expert with this information!");
    winds_cleanup(ERROR);
   }
  }
/*
provide a "general" listening post to respond to extra-sessional winds processes
who are simply looking for an available cpu, etc.
*/
  if (SetListeningPost("WINDS-GENERAL_LISTENING",GENERAL_LISTENING,GENERAL_LISTENING)==ERROR) {
/*
socket is already bound. Find another host for winds startup and/or exit.
*/
   if (post_processing_mode())
    FindAvailableHost();
   else {
    RequestAlertNow("FATAL: couldn't establish GENERAL_LISTENING socket...contact WINDS expert with this information!");
    winds_cleanup(ERROR);
   }
  }
 }
}

/****************************  SetWINDSVersion()  ******************************/

SetWINDSVersion()
{
 (void)sprintf(WINDSVersion,"WINDS %s",
#include <version.h>
 );
}

/****************************  GetWINDSVersion()  ******************************/

char *
GetWINDSVersion()
{
 return WINDSVersion;
}

/****************************  HIDE_COPYRIGHT()  ******************************/

hide_copyright()
{
 XSetAttr(canvas[COPYRIGHT],XV_SHOW,FALSE,NULL);
}

/****************************  DRAW_COPYRITE()  ******************************/

/*ARGSUSED*/
void
draw_copyrite(cvas,pwindow,area)
Canvas cvas;
Xv_Window pwindow;
Rectlist *area;
{
static int size_set=FALSE;
char *intro1="WINDS";
char *intro2="WINdow Display System";
char *copyrite1="Copyright 1994";
char *copyrite2="University Corporation for Atmospheric Research";
char *copyrite3="All Rights Reserved";
int font_width,
    font_height;

 if (!size_set) {
  SetCanvasDimensions(COPYRIGHT, 0, 0, control_width, display_height);
  size_set=TRUE;
 }
 (*SetContext[GetDrawType()])(pw_xid[COPYRIGHT],GXcopy,BKGND,0,0,
   GetFontID(GetCopyrightFont())); 
 font_height=GetFontHeight(GetCopyrightFont());
 font_width=GetFontWidth(GetCopyrightFont());
 (*DrawTtext[GetDrawType()])
  ((int)(control_width/2-
   (strlen(intro1)*font_width)
    /2),
    display_height/2-font_height*4,intro1);
 (*DrawTtext[GetDrawType()])(control_width/2-(strlen(intro2)*font_width)/2,
  display_height/2-font_height*2,intro2);
/***
 draw_cover_page();
***/
 (*DrawTtext[GetDrawType()])(control_width/2-(strlen(copyrite1)*font_width)/2,
  display_height/2+font_height*4,copyrite1);
 (*DrawTtext[GetDrawType()])(control_width/2-(strlen(copyrite2)*font_width)/2,
  display_height/2+font_height*6,copyrite2);
 (*DrawTtext[GetDrawType()])(control_width/2-(strlen(copyrite3)*font_width)/2,
  display_height/2+font_height*8,copyrite3);
}

/****************************  SHOW_TIME()  **********************************/

/*
provide non-argument call for access and for keeping lint happy
*/
ShowTime() 
{
 show_time((Canvas)0,(Xv_Window)0,(Rectlist *)0);
}

/*ARGSUSED*/
void
show_time(cvas,pwindow,area)
Canvas cvas;
Xv_Window pwindow;
Rectlist *area;
{
static int size_set=FALSE;

 if (!size_set) {
  SetCanvasDimensions(TIME_AND_PROJECT,0,display_height,
   GetTimeWindowWidth(),control_height);
  size_set=TRUE;
 }
 (*FillArea[GetDrawType()])(pw_xid[TIME_AND_PROJECT],0,0,
  GetTimeWindowWidth(),BUTTON_HEIGHT,WHT);
 set_time(current_time);
 DrawControlPanelDate((char *)GetCurrentDate());
 DrawControlPanelPnum();
 DrawControlPanelFltno();
}

/****************************  GET_HELP()  ************************************/

/*ARGSUSED*/
void
get_help(cvas,pwindow,area)
Canvas cvas;
Xv_Window pwindow;
Rectlist *area;
{
static int size_set=FALSE;

 if (!size_set) {
  SetCanvasDimensions(HELP_IMAGE, 
   base_frame_width-NUM_BUTTONS*BUTTON_WIDTH, display_height,
   BUTTON_WIDTH*NUM_BUTTONS, BUTTON_HEIGHT);
 }
}

/****************************  SET_SIZE()  ************************************/

set_quad_size(which)
int which;

/* it seems setting size BEFORE the REPAINT proc is called doesn't always 
   work.  Provide this to cover that.
*/
{
static int size_set[NUM_DISPLAY_QUADRANTS]={FALSE,FALSE,FALSE,FALSE};

 if (!size_set[which]) {
  SetCanvasDimensions(which, window_x[which], window_y[which],
   window_width[which], window_height[which]);
  size_set[which]=TRUE;
 }
}

/*****************************************************************************/

VerifyMonitor(StartMaster)
{
 char command[MAX_FLNM_LNGTH];

 if (StartMaster) {
/*
this is a verification at shutdown time; add argument to prevent
removal of the very flag file that implies the shutdown should occur.
Only the compute host should ever get this situation.
*/
  sprintf(command, "%s/scripts/VerifyMonitor -r &",winds_path);
 } else {
/*
this is an ordinary 5-minute check; verify monitor is running on the
display host.
*/
  if (strcmp(GetDisplayHost(),GetComputeHost()))
/*
display host and compute host are not one and the same,
original command will not work.
*/
   sprintf(command,"rsh %s %s/scripts/VerifyMonitor &", GetDisplayHost(),winds_path);
  else
   sprintf(command,"%s/scripts/VerifyMonitor &",winds_path);
 }
fprintf(stderr, "MONITOR: command = [%s]\n", command);
 ExecuteShellCommand(command);
}
 
/****************************  WINDS_CLEANUP()  ******************************/


winds_cleanup(exit_status)
int exit_status;
/* release system resources and exit
*/
{
FILE *file,*WINDSfopen();
char NoStartWinputFilename[MAX_FLNM_LNGTH];
char halt_filename[MAX_FLNM_LNGTH];
char temp[256];

 if (exit_status == OK) {

/* exit_session gets set after call to notify_user_error() with
   CTL_QUIT or SLAVE_QUIT argument
*/
  exit_session=FALSE;

/* if a calibration process is running, notify user that saving of history
   file must be done manually before killing
*/
  if (calib_running)
   notify_user_error(SAVE_HISTORY);

/* if no calibration process OR user indicated saving unnecessary, verify
   quit of entire session is desired
*/
  if (!calib_running || exit_session) {
   if (WindsControlHost) {
    WINDSCleanUp=FALSE;
    notify_user_error(CTL_QUIT);
   } else
    notify_user_error(SLAVE_QUIT);
  }  
/* if user indicated not ready to quit, return
*/
  if (!exit_session)
   return;
/* 
request printers to flush buffers on startup as needed
*/
  if (GetResetOnStartup())
   (void)write_to_socket(PJETSOCKET,FLUSHBUFFER,strlen(FLUSHBUFFER));

 } else if (exit_status==RECYCLE) {

/* keep session going, clean up only what's needed to restart with new
   project # and/or flight #
*/
  if (GetResetOnStartup())
   (void)write_to_socket(PJETSOCKET,FLUSHBUFFER,strlen(FLUSHBUFFER));
  kill_winds_tasks();
  detach_shmem();
  close_winds_sockets();
  SetWINDSisStarted(FALSE);
  WINDSInitStuff(Argc,Argv);
  return;

 } else {
/* 
exit status is other than OK; set flag to terminate everything else cleanly
*/
  if (WindsControlHost) 
   WINDSCleanUp=TRUE;
 }

 if ( WindsControlHost ) {

/* offer opportunity to kill off other WINDS displays.  If accepted,
   also kill input process, remove shared memory, and remove
   argument files.
*/
  if (WINDSCleanUp) {
   if (!system_shutdown && (
    GetInputMode()==DISK_DATA || 
    GetInputMode()==RAWFILE_NETCDF ||
    GetInputMode()==ADS_DATA )) {
    char filename[MAX_FLNM_LNGTH];
/*
create flag file for winds script to kill sessions only when in appropriate 
input mode, don't bother if system shutdown is pending
*/
    (void)sprintf(filename,EXITALLDISPLAYS,ProjectDirectory,GetComputeHost());
    (void)WINDSfopen(filename,"w");
   }
/* 
kill spawned processes 
*/
   kill_winds_tasks();
/* 
detach from shared memory segments
*/
   detach_shmem();

   strcpy(temp, getenv("WINDS"));
   strcat(temp, "/scripts/kill-winds-all");
   system(temp);
  } else {

/* leave flag file as notice for any subsequent WINDS session
   that the input process should be left running, shared mem
   is not needed, etc. 
*/
   (void)sprintf(&NoStartWinputFilename[0],NO_START_INPUT,ProjectDirectory,proj_number);
   if ( (file=WINDSfopen(NoStartWinputFilename,"w"))==NULL) {
    time_to_log();
    (void)fprintf(stderr,"WARNING: can't open writable %s in cleanup()\n",
    NoStartWinputFilename);
   }
   (void)WINDSfclose(file);
  }


  if (system_shutdown) {
   if (connected_to_ads) {
/* 
leave flag file as notice to winds monitor script
that all display hosts should be halted -- but only if truely 
connected to the ADS. Start by verifying that the monitor is indeed running.
*/
    VerifyMonitor(TRUE);
    (void)sprintf(&halt_filename[0],HALT_FILENAME,
     ProjectDirectory,GetComputeHost(),"winds");
    if ( (file=WINDSfopen(halt_filename,"w"))==NULL) {
     time_to_log();
     (void)fprintf(stderr,"WARNING: can't open writable %s in cleanup()\n",
     halt_filename);
    } 
   } 
   (void)WINDSfclose(file);
  }
 } else {
/*
non-control host
*/
  if (system_shutdown) {
/*
presumably this is not the compute host...it should NOT have been offered 
the shutdown ALL option...write flag file as notice to winds monitor script 
that THIS display host only should be halted 
*/
   (void)sprintf(&halt_filename[0],KILLHOSTSLISTFILE,ProjectDirectory);
   if ( (file=WINDSfopen(halt_filename,"w"))==NULL) {
    time_to_log();
    (void)fprintf(stderr,"WARNING: can't open writable %s in cleanup()\n",
    halt_filename);
   } else {
    (void)fprintf(file,"%s",GetDisplayHost());
   } 
   (void)WINDSfclose(file);
  }
 }
/*
close files
*/
 if (GetVarDataBase())
  ReleaseVarDB();
 time_to_log();
 if (exit_status == ERROR) {
  (void)fprintf(stderr,"WINDS exiting due to problems...\n");
  exit(ERROR);
 } else if (exit_status == INTERRUPT) {
  (void)fprintf(stderr,"WINDS dumping core due to problems...\n");
  abort();
  exit(INTERRUPT);
 } else {
  (void)fprintf(stderr,"normal WINDS termination...\n");
  exit(exit_status);
 }
}

/*****************************  CLOSE_WINDS_SOCKETS()  ***********************/

close_winds_sockets()
{
 if (WindsControlHost) {
  close_socket(DISP_REQ_REPLY);
  close_socket(GENERAL_LISTENING);
 }
 CloseWindsBroadcast();
 CloseWindsListening();
 close_socket(PJETSOCKET);
 close_socket(HP9876SOCKET);
 close_socket(EPSONSOCKET);
 close_socket(ASCIIVALUES);
 close_socket(FLAT_PANEL_DATA);
 close_socket(POSTSCRIPT);
 close_socket(DISP_REQ);
}

/************************  SetNumWindsNetHosts()  ****************************/ 

SetNumWindsNetHosts(numhosts)
int numhosts;
/*
set the number of windsnet participating hosts to numhosts
*/
{
 NumWindsNetHosts=numhosts;
}

/************************  GetNumWindsNetHosts()  ****************************/ 

GetNumWindsNetHosts()
/*
return the number of windsnet participating hosts 
*/
{
 return NumWindsNetHosts;
}

/***************************  LoadAverageIsLow()  ****************************/ 

LoadAverageIsLow(host)
char *host;
/*
NOTE: Unix/SunOS dependent code:

send an rup -t command to host, extract the last minute's load
average, and return TRUE if that average is less than desired level, else
return FALSE. Also return FALSE if output from rup doesn't contain
the "average:" string as leading indicator of where to find the desired
load.
*/
{
FILE *pipe;
char string[MAXHOSTNAMELEN];
float lastminute;

 (void)sprintf(command,"rup -t %s",host);
 (void)fprintf(stderr," waiting for rup -t from %s...\n",host);
 pipe=popen(command,"r");
 while (fscanf(pipe,"%s",string)!=EOF) {
  if (!strcmp(string,"average:")) {
   fscanf(pipe,"%f",&lastminute);
   pclose(pipe);
   (void)fprintf(stderr," load on %s is %f...\n",host,lastminute);
   return (lastminute <= MAXWINDSNETLOAD);
  }
 }
 pclose(pipe);
 return FALSE;
}

/***************************  GetWindsNetHosts()  ****************************/ 

GetWindsNetHosts()
/*
NOTE: Unix/SunOS dependent code:

get list from NIS of hostnames in the windsnet netgroup
*/
{
static int GotWindsNetHosts=FALSE;
FILE *pipe;

 if (GotWindsNetHosts)
  return;
 SetNumWindsNetHosts(0);
 (void)sprintf(command,
  "ypcat -k netgroup.byhost | grep windsnet | awk -F. '{print $1}'");
 pipe=popen(command,"r");
 while (GetNumWindsNetHosts()<MAXWINDSNETHOSTS &&
  (fscanf(pipe,"%s",WindsNetHost[GetNumWindsNetHosts()])!=EOF)) {
  SetNumWindsNetHosts(GetNumWindsNetHosts()+1);
 }
 pclose(pipe);
 GotWindsNetHosts=TRUE;
}

/***************************  FindAvailableHost()  ****************************/ 

FindAvailableHost()
/*
look through list of windsnet hosts for winds session sharing to
find one without a winds process running, by checking for existence
of the general listening post socket. If a host is found that is alive,
has an acceptably low load average, and does NOT have a WINDS socket
already bound, issue an rsh to start up winds there.

In either event, exit with ERROR status.
*/
{
int host;
FILE *pipe;
char result[2][128];
char message[1024];

 GetWindsNetHosts();
 for (host=0; host<GetNumWindsNetHosts(); host++ ) {
  (void)fprintf(stderr,"WINDS checking on %s as alternate host...\n",
   WindsNetHost[host]);
  if (   HostIsAlive(WindsNetHost[host]) 
      && LoadAverageIsLow(WindsNetHost[host])
      && !WINDSSocketBound(WindsNetHost[host])
      ) {
/*
found a host without a winds process binding the GENERAL_LISTENING socket,
which we interpret to mean no winds process is running on that host. Green
light to request starting winds up on that cpu.
*/
   (void)fprintf(stderr," %s is alive...\n",WindsNetHost[host]);
   (void)sprintf(command,"rsh %s %s %s%s",
    WindsNetHost[host],(char*)GetWINDSCmdLine(),
    GetGotDataDir()?"":"-D",
    GetGotDataDir()?"":(char *)getenv("DATA_DIR"));
   pipe=popen(command,"r");
   fscanf(pipe,"%s %s",result[0],result[1]);
   pclose(pipe);
   if (strcmp(result[0],"permission") || strcmp(result[1],"denied")) {
    (void)fprintf(stderr,"WINDS: Bingo: %s\n",command); 
    break;
   } else {
    (void)fprintf(stderr,
     "WARNING: permission denied problem on attempted command: %s\n",command);
   }
  } else {
   (void)fprintf(stderr,"  (NOT available as alternate host...)\n");
  }
 }
 (void)sprintf(message,
  "FATAL: WINDS session is already present on %s; remedy this by exiting WINDS session or identifying its process id (ps -cef | grep bin/winds) and killing it (kill -9 <pid>) on that host. If you find no WINDS process there, then there are likely some system resources that need cleaning up: to get all shared memory identifiers (3rd column of ipcs output), execute ipcs. Then, for each id, execute ipcrm -M <id>. If these identifiers belong to someone other than yourself, then that user (or root) will need to do the removals.",GetComputeHost());
 RequestAlertNow(message);
 winds_cleanup(ERROR);
} 
 
/***************************  WINDSSocketBound()  ****************************/
 
WINDSSocketBound(host)
char *host;
/*
establish a connection to the GENERAL_LISTENING socket on the host named.
Query it to determine if a WINDS process there already is running, create
a socket to wait for a response, and return TRUE if response is returned
with the time-out period, else return FALSE
*/
{
int length;
 
 (void)fprintf(stderr," checking for WINDS process on %s...\n",host);
 talk_to_socket("WINDS-GENERAL_LISTENING",GENERAL_LISTENING,GENERAL_LISTENING,host);
 (void)sprintf(command,"%d %s",ARE_YOU_LISTENING,(char *)GetComputeHost());
 write_to_socket(GENERAL_LISTENING,command,strlen(command));
 close_socket(GENERAL_LISTENING);
 
 if (listen_on_socket("WINDS-GENERAL_RESPONSE",GENERAL_RESPONSE,GENERAL_RESPONSE)!=OK) {
/*
this is unlikely. It means that another WINDS process on this host is in this 
exact same section of code and has this socket bound. Take the easy way out,
simply assume socket IS bound since we can't verify that it isn't.
*/
  (void)fprintf(stderr," Can't bind GENERAL_RESPONSE socket on %s...\n",host);
  close_socket(GENERAL_RESPONSE);
  return TRUE;
 }
 SetBlockTimeOut(3);
 length=MAX_SOCKET_BUFF_LNGTH;
 (void)read_from_socket(GENERAL_RESPONSE,&length,BLOCK_TIME_OUT);
 close_socket(GENERAL_RESPONSE);
 if (length) {
/*
we're not going to worry about the contents of string returned, just whether
or not a non-zero length anything was received
*/
  (void)fprintf(stderr," WINDS process IS present on %s...\n",host);
  return TRUE;
 } else {
  (void)fprintf(stderr," WINDS process NOT present on %s...\n",host);
  return FALSE;
 }
}

/************************************************************************/

GetCalibDisplayUp()
{
 return calib_running;
}


/***************************************************************************/

GetRealTimeVarValues(varname,start,count,values)
char *varname;
int start,count;
float *values;
/*
fill values array with count values of varname, starting at index start in the
shared memory arrays
*/
{
 int index,point,time_slice,rawmode,posn;

 posn=GetMenuListPosn(varname);
 rawmode=VarIsRaw(posn);
 posn=GetMenuListIndex(posn);
 if (rawmode) {
  for (index=0,time_slice=start; time_slice<start+count; index++,time_slice++) {
   point=time_slice%get_datablock_size();
   values[index]=GetRawDataValue(point*GetNumRawNames()+posn);
  }
 } else {
  for (index=0,time_slice=start; time_slice<start+count; index++,time_slice++) {
   point=time_slice%get_datablock_size();
   values[index]=GetDeriveDataValue(point*GetNumDerivedNames()+posn);
  }
 }
}

