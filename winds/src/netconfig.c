#include <stdio.h>
#include <netdb.h>
#include <sys/utsname.h>

/****************************  WINDS INCLUDES  ******************************/

#include <network.h>
#include <printers.h>
#include <constants.h>
#include <files.h>
#include <socket.h>
#include <lengths.h>
#include <display.h>
#include <proto.h>

/***************************  GLOBAL WITHIN FILE  ****************************/

static int scrn_width,                         /* width of entire screen */
           scrn_height;                        /* height of entire screen */
static FILE *Netconfig; 		/* netconfig file handles */
static char WindsControlHostName[SYS_NMLN];
static char disp_host[SYS_NMLN],
            port_server[MAX_FLNM_LNGTH],
            port_server_host[SYS_NMLN],
            location[16],
            ptra[16],
            ptrb[16],		
	    proj_no[16];			/* fields in netconfig file */
static char NetconfigRecord[MAX_NETCONFIG_RECORD_LENGTH];
static char Proj_no[16];
static int InputMode;				/* ADS, netcdf, etc. */
static int FlushOnImagePrint=FALSE;		/* TRUE if given image printer
						needs flush on image print */
static char filename[MAX_FLNM_LNGTH]; 
static int ResetOnStartup=FALSE;		/* TRUE if given image printer
						need reset on startup */
static int Lcd_display=FALSE;
static int Flip_display=FALSE;
static int LstatPort=ERROR;
static int NonLocalPortServer=FALSE;
static int BwImages=ERROR;
static int ColorImages=ERROR;
static int NumSessions=ERROR;
static int NumDisplays=ERROR;
static int NumPrintRows=ERROR;
static int Parallel=FALSE;		/* TRUE if port out is parallel */
static int NeedPauseOnLists=TRUE;
static int ScreenWidth=ERROR,
	   ScreenHeight=ERROR;

static struct {
 int DisplayNumber;
 char disphost[SYS_NMLN];
} Session[MAX_EXPECTED_SESSIONS];

char display_mode[MAX_FLNM_LNGTH],      /* name of mode used in display */
     display_pnum[MAX_FLNM_LNGTH],      /* name of project # used for display */
     display_file[MAX_FLNM_LNGTH],      /* name of data file used for display */
     color_map[MAX_FLNM_LNGTH];         /* color map to be used */

extern char *ProjectDirectory;

open_netconfig()
{
FILE *WINDSfopen();

 (void)sprintf(&filename[0],NETCONFIGFILE,ProjectDirectory);
 if ( (Netconfig=WINDSfopen(filename,"r")) == NULL)
  return ERROR;
 skip_doc_lines();
 return TRUE;
}

close_netconfig()
{
 (void)WINDSfclose(Netconfig);
}

/**************************  SKIP_DOC_LINES()  **************************/

skip_doc_lines()
{
char dummy[16];
/*
sift out comments and read header lines in netconfig file
*/
 do {
  (void)fgets(NetconfigRecord,MAX_NETCONFIG_RECORD_LENGTH,Netconfig);
 } while (NetconfigRecord[0]=='#');
 (void)sscanf(NetconfigRecord,"%s %s %s %s %s %s %s %s\n",
  dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy);
 do {
  (void)fgets(NetconfigRecord,MAX_NETCONFIG_RECORD_LENGTH,Netconfig);
 } while (NetconfigRecord[0]=='#');
 (void)sscanf(NetconfigRecord,"%s %s %s %s %s %s %s %s\n",
  dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy);
}

/**************************  GET_NUM_WINDS_SESSIONS()  **************************/

get_num_winds_sessions()
{
 return NumSessions;
}

/**************************  SET_WINDS_SESSIONS()  **************************/

set_winds_sessions(client,pnum,computehost,disphost)
char *client,*pnum,*computehost,*disphost;
{
int display_num;
/*
given project number for this host, determine all entries in netconfig with
same project number running in non-post-processing mode. Set global array
with display numbers from each, return how many there are total.
*/
 if ( (open_netconfig()==ERROR)) {
  set_session(client,disphost,0);
  return;
 } 
 display_num=NumSessions=0;
 while (fgets(NetconfigRecord,MAX_NETCONFIG_RECORD_LENGTH,Netconfig)!=NULL) {
  if (NetconfigRecord[0]!='#') {
/*
sift out comments
*/
   (void)sscanf(NetconfigRecord,"%s %s %s %d %d %s %s %s",
    location,disp_host,port_server,
    &scrn_width,&scrn_height,ptra,ptrb,proj_no);
   if (post_processing_mode()) {
    if (post_processing_locn(location) && 
        !strcasecmp(substring(&proj_no[0],'-'),pnum) && 
        !strcasecmp(disp_host,disphost) ) {
     set_session(client,disp_host,display_num);
/*
take first one only in post-processing mode
*/
     break;
    }
   } else {
    if (!strcasecmp(&proj_no[0],pnum) && !post_processing_locn(location) 
/*** commented out 1/7/94: with compute host different from display_host,
the display host would not get entered here; get_session(displayhost) would
return -1 which causes problems elsewhere, e.g. in CheckForNetworkMessages().
Besides, if it's a winds slave OR the computehost, shouldn't ANY case be true?

        && (IsWindsSlave(location) || !strcasecmp(disp_host,computehost) )
***/
   ) {
    set_session(client,disp_host,display_num);
    }
   }
   display_num++;
  }
 }
 close_netconfig();
}

/*************************  SET_SESSION()  *****************************/

int
set_session(client,host,number)
char *client;
int number;
char *host;
{
 if (NumSessions==MAX_EXPECTED_SESSIONS) {
  (void)fprintf(stderr,"WARNING: too many hosts in one network session: %d\n");
  (void)fprintf(stderr,"Will NOT provide net-comm'n with any more hosts\n");
  return;
 }
 Session[NumSessions].DisplayNumber=number;
 (void)sprintf(Session[NumSessions].disphost,"%s",host);
 NumSessions++;
/***
 (void)fprintf(stderr,"%s: adding %s to network session list\n",client,host);
***/
}

/*************************  GET_DISPLAY_HOST()  *****************************/

char *
get_display_host(session)
int session;
{
 return(Session[session].disphost);
}

/*************************  GET_SESSION_NUMBER()  *****************************/

int
get_session_number(host)
char *host;
{
int sess;

 for (sess=0; sess<NumSessions; sess++) {
  if (!strcasecmp (host,Session[sess].disphost))
   return sess;
 }
 return 0;
}

/*************************  GET_DISPLAY_NUMBER()  *****************************/

int
get_display_number(int session)
{
 return(Session[session].DisplayNumber);
}

/**************************  GET_ENTRY()  **************************/

int
get_netconfig_entry(char hostname[], char mode[], char pnum[], char file[])
{
int display_num;
int pp_mode;
int any_pnum,
    any_file;
char *curr_pnum,*curr_file;
/*
callers need not specify location, project number or filename; 
in this case, conduct wild-card search accordingly. A match occurs when
all values used match those in the current entry being examined. Case is
not considered in the matching.

check for correct usage in call:

hostname MUST be specified

in the following, use of "NULL" implies either a (nil) string or a string of
length zero:

mode, pnum, and file may each be NULL; if any are non-NULL, those values are
used in search for a match. If all are NULL, then first entry matching 
hostname is used.

if mode is NULL: non-post-processing mode assumed. Value for file not used. 
Value for pnum IS used; if pnum is NULL, any pnum will match.

if pnum is NULL: first entry with matching mode (or non-post-processing mode, 
if mode is NULL) and file (or any file, if file is NULL) is used.

if file is NULL: first entry with matching mode (or any mode, if mode is NULL) 
and pnum (or any pnum, if pnum is NULL) is used.  Value for file is used in 
search only if a post-processing mode is explicitly specified.  If a 
non-post-processing mode specified, any file value is ignored.
*/
 any_pnum=!strlen(pnum);
 any_file=!strlen(file);
 pp_mode=strlen(mode) && post_processing_locn(mode);

 if (!strlen(hostname)) {
  (void)fprintf(stderr,"WARNING: incorrect usage -- need hostname\n");
  return ERROR;
 }
 if (!pp_mode && !any_file && strcmp(mode,DEVCTL_CDF_MODE)) {
  (void)fprintf(stderr,
   "WARNING: file specified in real-time mode; value ignored\n");
 }

 if ( !strlen(mode) && any_pnum ) {
/*
take the first one found with any mode
*/
  if ( (display_num=get_entry_by_name(hostname)) == ERROR) {
   (void)fprintf(stderr,"WARNING: %s not found in netconfig; insufficient info\n",
    hostname);
   return(ERROR);
  } else {
   return display_num;
  }
 }
 if ( (open_netconfig()!=ERROR)) {
  display_num=0;
  while (fgets(NetconfigRecord,MAX_NETCONFIG_RECORD_LENGTH,Netconfig)!=NULL) {
   if (NetconfigRecord[0]!='#') {
/*
sift out comments
*/
    (void)sscanf(NetconfigRecord,"%s %s %s %d %d %s %s %s",
     location,disp_host,port_server,
     &scrn_width,&scrn_height,ptra,ptrb,proj_no);
    if (DiskDataToNetCDF() || (pp_mode && post_processing_locn(location))) {
/*
simulated ads-to-netcdf mode or post-processing mode
*/
     if (!any_pnum)
      curr_pnum=(char *)substring(proj_no,'-');
     if (!any_file)
      curr_file=(char *)&proj_no[nindex(proj_no,'-')+1];
     if ( !strcasecmp(hostname,disp_host) &&
          (!strcasecmp(location,mode))  &&
          (any_pnum || !strcasecmp(curr_pnum,pnum))  &&
          (any_file || !strcasecmp(curr_file,file))
     ) {
      close_netconfig();
      return display_num;
     }
    } else if (!post_processing_locn(location)) {
/*
non-post-processing mode
*/
     if ( !strcasecmp(&hostname[0],&disp_host[0]) &&
          (!strlen(mode) || !strcasecmp(location,mode))  &&
          (any_pnum || !strcasecmp(proj_no,pnum))
     ) {
      close_netconfig();
      return display_num;
     }
    }
    display_num++;
   }
  }
  close_netconfig();
 }
/*
entry not found; if enough information, can still run the session: must 
supply mode, project number, and, if post-processing mode, the file arguments
*/
 if (!strlen(mode) || any_pnum || (pp_mode && any_file)) {
  (void)fprintf(stderr,"WARNING: no entry in netconfig and insufficient info\n");
  return(ERROR);
 } else {
/*
we have the host, mode, project number, and file if needed; set defaults for 
other stuff and proceed. Note that, however, in this case a valid display
number is NOT returned, simply TRUE is returned.
*/
  (void)sprintf(location,"%s",mode); 
  (void)sprintf(disp_host,"%s",hostname); 
  (void)sprintf(port_server,"%s",hostname); 
  scrn_width=1136; scrn_height=880;
  (void)sprintf(ptra,"none"); (void)sprintf(ptrb,"none");
  if (DiskDataToNetCDF() || pp_mode) {
   (void)sprintf(proj_no,"%s-%s",pnum,file); 
  } else {
   (void)sprintf(proj_no,"%s",pnum); 
  }
  return TRUE;
 }
}

/**************************  GET_ENTRY_BY_LOCN()  **************************/

int
get_entry_by_locn(char locn[])
{
int display_num;

 if ( (open_netconfig()==ERROR)) {
  return(ERROR);
 }
 display_num=0;
 while (fgets(NetconfigRecord,MAX_NETCONFIG_RECORD_LENGTH,Netconfig)!=NULL) {
  if (NetconfigRecord[0]!='#') {
/*
sift out comments
*/
   (void)sscanf(NetconfigRecord,"%s %s %s %d %d %s %s %s", 
    location,disp_host,port_server, 
    &scrn_width,&scrn_height,ptra,ptrb,proj_no);
   if ((strcasecmp(&locn[0],&location[0]))==0 ) { 
    close_netconfig();
    if (!(strcmp(location,"nfs")))
     SetSaveDirectory();
    return display_num; 
   } 
   display_num++; 
  } 
 }
 close_netconfig();
 return(ERROR);
}

/**************************  GET_ENTRY_BY_NAME()  **************************/

int
get_entry_by_name(char hostname[])
{
int display_num;

 if ( (open_netconfig()==ERROR)) {
  return(ERROR);
 }
 display_num=0;
 while (fgets(NetconfigRecord,MAX_NETCONFIG_RECORD_LENGTH,Netconfig)!=NULL) {
  if (NetconfigRecord[0]!='#') {
/*
sift out comments
*/
   (void)sscanf(NetconfigRecord,"%s %s %s %d %d %s %s %s",
    location,disp_host,port_server,
    &scrn_width,&scrn_height,ptra,ptrb,proj_no);
   if ((strcasecmp(hostname, disp_host))==0 ) {
    close_netconfig();
    return display_num;
   }
   display_num++;
  }
 }
 close_netconfig();
 return(ERROR);
}

/**************************  GetScreenWidth()  *****************************/

GetScreenWidth()
{
 return ScreenWidth;
}

/**************************  GetScreenHeight()  *****************************/

GetScreenHeight()
{
 return ScreenHeight;
}

/*****************************************************************************/

SetScreenDimensions(int width, int height)
{
 ScreenWidth=width;
 ScreenHeight=height;
}

/**************************  SET_DISPLAY_TYPE()  *****************************/

SetScreenGeometry()
{
 SetScreenDimensions(scrn_width,scrn_height);
}

/**************************  SET_NUM_DISPLAYS()  *****************************/

set_num_displays()
{
 NumDisplays=NUM_DISPLAY_QUADRANTS;
}

/**************************  GET_NUM_DISPLAYS()  *****************************/

get_num_displays()
{
 if (NumDisplays==ERROR) {
  (void)fprintf(stderr,"WARNING: NumDisplays == ERROR.  Using %d...\n",
   NUM_DISPLAY_QUADRANTS);
  return NUM_DISPLAY_QUADRANTS;
 }
 return NumDisplays;
}


/***************************  SET_PROJ_NO()  **************************/

char *
set_proj_no()
{
 (void)sprintf(&Proj_no[0],"%s",proj_no);  
}

/***************************  GET_PP_FILENAME()  **************************/

char *
get_pp_filename()
{
int j;

/* netconfig file entry expected to consist of string "pnum-file", where pnum
   is the project number and file is a netCDF file-name convention 
*/
  j=nindex(Proj_no,'-');
  if (!j) {

/*  Position of "-" character must be non-zero!
*/
   (void)fprintf(stderr,"WARNING: no filename defined in netconfig file\n");
   return((char *)NULL);
  }
  return(&Proj_no[j+1]);
}

/***************************  POST_PROCESSING_LOCN()  *************************/

post_processing_locn(char locn[])
{
 return (locn!=NULL &&
      !strcasecmp(locn,NETCDF_MODE));
}

/***************************  POST_PROCESSING_MODE()  *************************/

post_processing_mode()
{
 return (GetInputMode()==NETCDF);
}

/***************************  GET_PROJ_NUMBER()  **************************/

char *
get_proj_number()
{
/* SetInputMode() must have been called prior, as well as set_proj_no().
   Both should have been called during a get_entry() sequence.
*/
 if (GetInputMode()==RAWFILE_NETCDF || post_processing_mode()) {

/* netconfig file entry expected to consist of string "pnum-fltno", where pnum
   is the project number and fltno is the netCDF naming convention for that 
   file.
*/
  return((char *)substring(Proj_no,'-'));
 } else
  return((char *)Proj_no);
}

/* note that the following defines should be matched in scripts definitions file
$WINDS/scripts/raf.defines
*/
static char WindsControlHostPosn[NUM_CONTROLHOST_LOCNS][NAMLEN]=
{CONTROL_CENTRAL,DEV_CENTRAL,INIT_SETUP_MODE,NETCDF_MODE,DEVCTL_CDF_MODE};
static char SlaveHostPosn[NUM_WINDS_SLAVE_LOCNS][NAMLEN]=
{COCKPIT,FORWARD,OUTBOARD_LEFT,OUTBOARD_RIGHT,INBOARD_LEFT,INBOARD_RIGHT,DHCP_0,DHCP_1,DHCP_2,DHCP_3,DHCP_4,DHCP_5,DEV};

/**************************  IsWindsSlave()  ******************************/

IsWindsSlave(char locn[])
{
int loc;

 for (loc=0; loc<NUM_WINDS_SLAVE_LOCNS; loc++) {
  if (!strcasecmp(locn,SlaveHostPosn[loc]))
   return TRUE;
 }
 return FALSE;
}

/*********************  IsWindsComputeHost()  ******************************/

IsWindsComputeHost(char host[])
{
 return (!(strcmp(host,(char *)GetComputeHost())));
}

/*********************  IsWindsControlHost()  ******************************/

IsWindsControlHost(char host[])
{
 return (post_processing_mode() ||
  !strcmp((char *)GetControlHost(),host));
}

/************************  FindAndSetControlHost()  **************************/

FindAndSetControlHost()
/*
determine which host is the control host for network session, assuming
Proj_no has been set. This function will search the netconfig
file sequentially and return the FIRST hostname with matching project #
and matching position as any listed in WindsControlHost_posn[] array, taken
in order listed.

We assume that Proj_no has already been set before call to this function.

This func should only be used if it is assumed there IS a netconfig entry
for the project number and it is the ONLY control-position entry for that
number in the netconfig file!

ALSO: since this call alters the postion in netconfig, call it ONLY
after all set..()'s have been made on original get_entry_xxx() call!
*/
{
int locn;
 
 for (locn=0; locn<NUM_CONTROLHOST_LOCNS; locn++) {
  if (get_entry_by_locn(WindsControlHostPosn[locn])!=ERROR) {
   if (!(strcasecmp(proj_no,get_proj_number()))) {
/*
found a winds-host
*/
    SetControlHost(disp_host);
    return(TRUE);
   }
  } else {
/*
no go
*/
   return(FALSE);
  }
 } 
/*
not in ANY of the host modes
*/
 return(FALSE);
}

/***************************  SetControlHost()  *****************************/

SetControlHost(char host[])
{
  (void)sprintf(WindsControlHostName,"%s",host);
}

/***************************  GetControlHost()  *****************************/

char *
GetControlHost()
{
 return WindsControlHostName;
}

/***************************  GET_DISP_LOCN()  *****************************/

char *
get_disp_locn()
{
 return(location);
}

/********************  FLAT_PANEL_DISPLAY_DATA_NEEDED()  **********************/

flat_panel_display_data_needed()
{
 if (!(strcasecmp(ptra,"lcd")) || !(strcasecmp(ptrb,"lcd")))
  Lcd_display=TRUE;
 if (!(strcasecmp(ptra,"flip")) || !(strcasecmp(ptrb,"flip")))
  Flip_display=TRUE;
 return lcd_display() || flip_display();
}

/***************************  LCD_DISPLAY()  **************************/

lcd_display()
{
 return Lcd_display;
}

/***************************  FLIP_DISPLAY()  **************************/

flip_display()
{
 return Flip_display;
}

/***************************  ASCII_DATA_NEEDED()  **************************/

AsciiDataNeeded()
{
 return( !(strcasecmp(ptra,"ascii")) || !(strcasecmp(ptrb,"ascii")) );
}

/**********************  UsingNonLocalPortServer()  *************************/

UsingNonLocalPortServer()
{
 return NonLocalPortServer;
}

/***************************  GetInputMode()  ******************************/

int GetInputMode()
{
 return InputMode;
}

/***************************  SetInputMode()  *****************************/

SetInputMode()
/*
value of "posn" column in current netconfig position determines InputMode
*/
{
 InputMode=ERROR;
 if (data_from_ads()) {
  InputMode = ADS_DATA;
  SetDisplayMode(CONTROL_CENTRAL);
 } else if (DiskDataToNetCDF()) {
  InputMode = RAWFILE_NETCDF;
  SetDisplayMode(DEVCTL_CDF_MODE);
 } else if (rawdata_from_disk()) {
  InputMode = DISK_DATA;
  SetDisplayMode(DEV_CENTRAL);
 } else if (netcdf_mode()) {
  InputMode = NETCDF;
  SetDisplayMode(NETCDF_MODE);
 } else if (init_setup_mode()) {
  InputMode = INIT_SETUP;
  SetDisplayMode(INIT_SETUP_MODE);
 } else if (IsWindsSlave(get_disp_locn())) {
  InputMode = WINDS_SLAVE;
  SetDisplayMode(COCKPIT);
 }
 return InputMode;
}

/*************************  INIT_SETUP_MODE()  *****************************/

init_setup_mode()
{ 
 return(!(strcasecmp(get_disp_locn(),INIT_SETUP_MODE)));
}

/*************************  NETCDF_MODE()  *****************************/

netcdf_mode()
{ 
 return(!(strcasecmp(get_disp_locn(),NETCDF_MODE)));
}

/*************************  DiskDataToNetCDF()  *****************************/

DiskDataToNetCDF()
/* 
true if a raw data disk file is being read to create a netcdf file which will 
be the input source
*/
{ 
 return(!strcasecmp(get_disp_locn(),DEVCTL_CDF_MODE));
}

/*************************  RAWDATA_FROM_DISK()  *****************************/

rawdata_from_disk()
/* 
true if input source is from raw data disk file
*/
{ 
 return(!strcasecmp(get_disp_locn(),DEV_CENTRAL));
}

/***************************  DATA_FROM_ADS()  *****************************/

data_from_ads()
{
 return(!(strcasecmp(get_disp_locn(),CONTROL_CENTRAL)));
}

/***************************  GET_DISP_HOST()  *****************************/

char *
get_disp_host()
{
 return(disp_host);
}

/***************************  GET_PRINTER()  ******************************/

get_printer(char port[])
{
 char *printer;

 if ( !(strcasecmp(port,"ttyS0"))) {
  printer=ptra;
 } else if ( !(strcasecmp(port,"ttyS1"))) {
  printer=ptrb;
 } else {
  return NONE;
 }

 Parallel=FALSE;
 if (!(strcasecmp(printer,"pjet"))) {
  return PJET;
 } else if (!(strcasecmp(printer,"1200c"))) {
  return HP1200C;
 } else if (!(strcasecmp(printer,"p1200c"))) {
  return PHP1200C;
 } else if (!(strcasecmp(printer,"ppjet"))) {
  return PPJET;
 } else if (!(strcasecmp(printer,"spjet"))) {
  return SPJET;
 } else if (!(strcasecmp(printer,"epson"))) {
  return EPSON;
 } else if (!(strcasecmp(printer,"lcd"))) {
  return LCD;
 } else if (!(strcasecmp(printer,"flip"))) {
  return FLIP;
 } else if (!(strcasecmp(printer,"ascii"))) {
  return ASCIIFEED;
 } else if (!(strcasecmp(printer,"none"))) {
  return NONE;
 } else if (!(strcasecmp(printer,"none"))) {
  return NONE;
 }
}

char * SaveDir;

/***************************  SetSaveDirectory()  ************************/

SetSaveDirectory()
{
 SaveDir=(char *)strdup(port_server);
}

/***************************  GetSaveDirectory()  ************************/

char *
GetSaveDirectory()
{
 return(SaveDir);
}

/***************************  GET_PORT_SERVER_HOST()  ************************/

char *
get_port_server_host()
{
 return(port_server_host);
}

/***************************  SET_PORT_SERVER()  *****************************/

set_port_server_host()
{
 (void)sprintf(port_server_host,"%s",port_server);
 NonLocalPortServer=FALSE;
/*
keyword "none" has special meaning; port server host with this name is SOL
*/
 if (strcasecmp(port_server,"none") && strcasecmp(disp_host,port_server)) {
/*
display host is using some other host's printer for images
*/
  NonLocalPortServer=TRUE;
 }
}

/*************************  SET_PRINTER_PORTS()  ******************************/

set_printer_ports()
{
 set_lstat_port();
 set_bw_image_port();
 set_color_image_port();
}

/***************************  GET_BW_IMAGE_PORT()  ******************************/

get_bw_image_port()
{
 return BwImages;
}

/***************************  SET_BW_IMAGE_PORT()  ******************************/

set_bw_image_port()
{
/* determine which printer prints b/w images.  
*/
 if ( !(strcasecmp(ptra,"hptp")) || !(strcasecmp(ptrb,"hptp"))) {
  BwImages =HP9876SOCKET;
 } else if (!(strcasecmp(ptra,"1200c")) || !(strcasecmp(ptrb,"1200c"))
  || !(strcasecmp(ptra,"p1200c")) || !(strcasecmp(ptrb,"p1200c")) 
 ) {
   BwImages = PJETSOCKET;
   SetNeedPauseOnLists(FALSE);
   SetFlushOnImagePrint(TRUE);
   SetResetOnStartup(TRUE);
   SetNumPrintRows(HP1200_LIST_ROWS);
 } else if (! (strcasecmp(ptra,"pjet")) || !(strcasecmp(ptrb,"pjet")) 
  || !(strcasecmp(ptra,"spjet")) || !(strcasecmp(ptrb,"spjet"))
  || !(strcasecmp(ptra,"ppjet")) || !(strcasecmp(ptrb,"ppjet"))
 ) {
   BwImages = PJETSOCKET;
   SetNeedPauseOnLists(TRUE);
   SetFlushOnImagePrint(FALSE);
   SetResetOnStartup(TRUE);
   SetNumPrintRows(PJ_LIST_ROWS);
 } else 
  BwImages=ERROR;
}

/*********************  SetNeedPauseOnLists()  ******************************/

SetNeedPauseOnLists(int pause)
{
 NeedPauseOnLists=pause;
}

/*********************  GetNeedPauseOnLists()  ******************************/

GetNeedPauseOnLists()
{
 return NeedPauseOnLists;
}

/************************  SetNumPrintRows()  *********************************/

SetNumPrintRows(rows)
int rows;
{
 NumPrintRows=rows;
}

/************************  GetNumPrintRows()  *********************************/

GetNumPrintRows()
{
 return NumPrintRows;
}

/*********************  GET_COLOR_IMAGE_PORT()  ******************************/

get_color_image_port()
{
 return ColorImages;
}

/************************  SET_COLOR_IMAGE_PORT()  ******************************/

set_color_image_port()
{
/* determine which printer prints color images.  
*/
 if ( !(strcasecmp(ptra,"pjet")) || !(strcasecmp(ptrb,"pjet"))
   ||!(strcasecmp(ptra,"spjet")) || !(strcasecmp(ptrb,"spjet")) 
   ||!(strcasecmp(ptra,"1200c")) || !(strcasecmp(ptrb,"1200c")) 
   ||!(strcasecmp(ptra,"ppjet")) || !(strcasecmp(ptrb,"ppjet")) 
   ||!(strcasecmp(ptra,"p1200c")) || !(strcasecmp(ptrb,"p1200c")) 
 ) {
  ColorImages = PJETSOCKET;
  if (!(strcasecmp(ptra,"1200c")) || !(strcasecmp(ptrb,"1200c")) 
   || !(strcasecmp(ptra,"p1200c")) || !(strcasecmp(ptrb,"p1200c")) 
  ) {
   SetFlushOnImagePrint(FALSE);
  }
  if (! (strcasecmp(ptra,"pjet")) || !(strcasecmp(ptrb,"pjet")) 
   || !(strcasecmp(ptra,"spjet")) || !(strcasecmp(ptrb,"spjet"))
   || !(strcasecmp(ptra,"ppjet")) || !(strcasecmp(ptrb,"ppjet"))
  ) {
   SetResetOnStartup(TRUE);
  }
 } else
  ColorImages=ERROR;
}

/***************************  GET_LSTAT_PORT()  ******************************/

get_lstat_port()
{
 return LstatPort;
}

/***************************  SET_LSTAT_PORT()  ******************************/

int
set_lstat_port()
{
/* determine which printer prints lists and stats.  Epson has highest
priority, followed by HP9876A and HP PaintJet.  Don't allow list and stats
that use some other host's printer for images.
*/
 if (UsingNonLocalPortServer()) {
  LstatPort = ERROR;
 } else {
  if ( !(strcasecmp(ptra,"epson")) || !(strcasecmp(ptrb,"epson"))) {
   LstatPort = EPSONSOCKET;
  } else if ( !(strcasecmp(ptra,"hptp")) || !(strcasecmp(ptrb,"hptp"))) {
   LstatPort =HP9876SOCKET;
  } else if ( !(strcasecmp(ptra,"pjet")) || !(strcasecmp(ptrb,"pjet"))
           || !(strcasecmp(ptra,"spjet")) || !(strcasecmp(ptrb,"spjet")) 
           || !(strcasecmp(ptra,"1200c")) || !(strcasecmp(ptrb,"1200c")) 
           || !(strcasecmp(ptra,"ppjet")) || !(strcasecmp(ptrb,"ppjet")) 
           || !(strcasecmp(ptra,"p1200c")) || !(strcasecmp(ptrb,"p1200c")) 
  ) {
   LstatPort = PJETSOCKET;
  } else
   LstatPort=ERROR;
 }

/* Hardwired by CJW 11/8/99. */
LstatPort = POSTSCRIPT;

}

/*****************************  GetDefaultProjDir()  **************************/

char *
GetDefaultProjDir()
{
 return(getenv("PROJ_DIR"));

}

/*************************  SetFlushOnImagePrint()  ***********************/

SetFlushOnImagePrint(int flush)
{
 FlushOnImagePrint=flush;
}

/**************************  GetFlushOnImagePrint()  **********************/

GetFlushOnImagePrint()
{
 return FlushOnImagePrint;
}

/*************************  SetResetOnStartup()  ***********************/

SetResetOnStartup(int reset)
{
 ResetOnStartup=reset;
}

/**************************  GetResetOnStartup()  **********************/

GetResetOnStartup()
{
 return ResetOnStartup;
}

/****************************  SetDisplayMode()  ******************************/

SetDisplayMode(char mode[])
{
 (void)sprintf(display_mode,"%s",mode);
}

/****************************  GetDisplayMode()  ******************************/

char *
GetDisplayMode()
{
 return display_mode;
}

/****************************  GetWINDSServer()  ******************************/

char *
GetWINDSServer()
{
static char EnvString[256];
static char *windsserver;
 
 if ( (windsserver=(char *)getenv("WindsServer"))!=NULL)
/*
the environment is supposed to already know about this. If not, query the
netconfig file.
*/
  return windsserver;
 if (get_entry_by_locn("nfs")!=ERROR) {
  windsserver=(char *)strdup(get_disp_host());
  (void)sprintf(EnvString,"WindsServer=%s",windsserver);
  (void)putenv(EnvString);
  return windsserver;
 } else {
  return NULL;
 }
}

/************************************************************************/
 
char *
GetProjectNumberFromFile()
{
char pnumfilename[MAX_FLNM_LNGTH];
FILE *pnumfile;  
static char pnum[MAX_FLNM_LNGTH]; 
 
 (void)sprintf(pnumfilename,PROJECTNUMBER,(char *)getenv("PROJ_DIR"));
 if ( (pnumfile=fopen(pnumfilename,"r"))!=NULL) {
  (void)fscanf(pnumfile,"%s",pnum);
  (void)fclose(pnumfile);
  return pnum;
 } else {
  return NULL; 
 }
}

/************************  NotifyWINDSProcess()  *****************************/

NotifyWINDSProcess(char message[])
{
char command[80];
char msg[1024];

 if (SocketIsBoundForListening(GENERAL_LISTENING)) {
  (void)sprintf(msg,"WARNING: someone is trying to write to a socket that is bound for listening; this causes problems with that message being received at ALL WINDS sessions currently running on local net! Message is as follows: \n\n%s\n",
  message);
  LogMsgToConsole(GetComputeHost(),"WINDS:IPC",msg);
  (void)fprintf(stderr,"%s",msg);
 } else {
  (void)sprintf(command,"%d",REQ_USER_ALERT);
  write_to_socket(GENERAL_LISTENING,command,strlen(command));
  write_to_socket(GENERAL_LISTENING,message,strlen(message));
  (void)fprintf(stderr,"Process %d requests a user alert\n",getpid());
 }
}
