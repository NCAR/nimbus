/*
winput-startup.c
*/
#include <stdio.h>
#include <sys/param.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <constants.h>
#include <network.h>
#include <socket.h>
#include <lengths.h>
#include <files.h>
#include <shmem.h>
#include <config.h>
#include <wproto.h>
#include <vardb.h>

char *DataDirectory, OutputFileName[1024];
char header_filename[MAX_FLNM_LNGTH];   /* path name for header file */
int Debug;              /* debug print variable 0=no,1=answers,2=int results */
int refill_shmem;
int Freeze=FALSE;
char *proj_number;      		/* project # of dataset input file */

static char buff[MAX_SOCKET_BUFF_LNGTH];
static int InputRate=REQ_SET_REALTIME;
static int DatasetIsFound=SEARCHNOTDONE;
/* 
paths searched for disk data files 
*/
static char DatasetSearchPath[MAX_FLNM_LNGTH*4];
/* 
names of disk files with data 
*/
static char RawDataFilename[MAX_FLNM_LNGTH]; 
static char NetCDFFilename[MAX_FLNM_LNGTH]; 

/**********************  SetInputRate()  *************************************/
void SetInputRate(int value)
{
 InputRate=value;
}

/**********************  GetInputRate()  *************************************/
int GetInputRate()
{
   /* In real-time on-board we want to check 10 times a second.
    */
   if (GetInputMode() == ADS_DATA)
      return(REQ_SET_REALTIME / 20);
   else
      return InputRate;
}

/*****************  RunStandardWINDSLoop()  *********************************/

void RunStandardWINDSLoop()
{
 static int PreviousInputRate=ERROR;
/*
set timer for SIGALRM to default one-second interval, and wait for it
...process must use sigset() to catch SIGALRM that will be delivered.
*/
 if ((int)signal(SIGALRM,catchalrm) < 0) {
  perror("sigset, input catchalrm");
 }
 if ((int)signal(SIGUSR2,Cleanup) < 0) {
  perror("sigset, input Cleanup");
 }
 SetAlarmTimer(GetInputRate());
 PreviousInputRate=GetInputRate();
 while (!elapsed_time && !hhmmss)
  pause();
 switch (GetInputMode()) {
  case INIT_SETUP:
/* 
set shared mem vars to values that convince display side (winds process)
to step thru setup sequence and display new-setup windows where called for
*/
   SetShmemElapsedTime(2);
   hhmmss[3]=12;
 }
 while (GetInputMode()==GetInputMode()) {
  if (GetInputRate()!=PreviousInputRate) {
/*
InputRate can be changed within catchalrm() routine, but we don't want to
reset the alarm timer there because it then throws catchalrm() out of sync
with itself (the resetting can cause a SIGALRM which gets caught before
catchalrm() exits, causing a sort of recursive headache). Instead, just
monitor current value and previous value of InputRate and reset timer
whenever they don't match. When *elapsed_time was being used in above 
while loop test instead of GetInputMode(), it changed between comparisons
and the while test came out false, as a result of above out-of-sync scenario!
*/
   PreviousInputRate=GetInputRate();
   SetAlarmTimer(GetInputRate());
  }
  pause();
 }
/*
whoops
*/
 NotifyWINDS("FATAL: Input process has exited due to an endless loop ending. I'm not making this up. A new input process is automatically startup up when this happens, but general communication between it and WINDS is out of sync. You have a choice of actions to take: (1) RECOMMENDED: exit this WINDS session and start a new one; or (2) execute the Control Panel 'Reset Input' operation (Etc/Input/Reset). With latter, things are mostly fixed up but not totally...it's your choice. VERY IMPORTANT: contact a WINDS expert about this message! We're trying to alleviate the problem and need to know if and when it occurs!",FALSE);
}

/********************  GetDisplayHost()  **************************************/
extern "C" {
char *GetDisplayHost()
{
/*
the effective "display" host for the winput process is deemed to be the
control station, since that is where input is controlled from.
*/
 return (char *)GetControlHost();
}

}
/************************** DoWinputStartupStuff()  *************************/

void DoWinputStartupStuff(char **argv, int argc) 
{
/*
get pathnames from environment
*/
 if ( (ProjectDirectory = (char *)getenv("PROJ_DIR"))==NULL) {
  ProjectDirectory=(char *)GetDefaultProjDir();
 }
 if ( (ProjectDirectory=(char *)getenv("PROJ_DIR"))==NULL) {
  NotifyWINDS("FATAL: Input process has exited due to PROJ_DIR environment variable not being set. This WINDS process will now also exit...contact a WINDS expert about this problem.",TRUE);
 }
 if ( (DataDirectory=(char *)getenv("DATA_DIR"))==NULL) {
  NotifyWINDS("FATAL: Input process has exited due to DATA_DIR environment variable not being set. This WINDS process will now also exit...contact a WINDS expert about this problem.",TRUE);
 }
/* 
establish compute server and location of WINDS stuff and project
setup directory
*/
 SetComputeHost();
 get_args(argc,argv);
/*
setup socket for exit status information. Must connect to this one as early
as possible in case something bad happens, then winput can tell winds what
it was.
*/
 talk_to_socket("WINPUT-GENERAL_LISTENING",GENERAL_LISTENING,GENERAL_LISTENING,
  GetComputeHost());

/* 
setup sockets for requests from and replies to display side.  NOTE that 
for now it's assumed that ONLY the WINDS Control host (WindsControlHost)
listens to the input process, but all WINDS sessions can talk to it.  On 
success, notify control host that input process is now listening and ready for
dialogue.
*/
 if (listen_on_socket("WINPUT-DISP_REQ",DISP_REQ,DISP_REQ)!=OK) {
  NotifyWINDS("FATAL: Input process has exited due to socket binding problem...likely because another input process is already running. Suggested action is to check for just such a process, get its pid, kill it, and restart the WINDS session. This WINDS process will now also exit...contact a WINDS expert about this problem if suggested action does not resolve it.",TRUE);
 }
 talk_to_socket("WINPUT-DISP_REQ_REPLY",DISP_REQ_REPLY,DISP_REQ_REPLY,GetComputeHost());
 (void)sprintf(buff,"%d",LISTENING);
 if (write_to_socket(DISP_REQ_REPLY,buff,strlen(buff))==EBADWRITE)
  perror("listening response write_to_socket");

/*
set up socket for broadcast networked data feeds
*/
 talk_to_socket("WINPUT-CALIB_NETDATA",CALIB_NETDATA,CALIB_NETDATA,BROADCAST);
/* 
establish operation mode, etc., from netconfig file
*/
if (get_netconfig_entry(GetControlHost(),display_mode,display_pnum,display_file)
  ==ERROR) {
  NotifyWINDS("FATAL: Input process has exited due to unmatched arguments in netconfig file. This WINDS process will now also exit...contact a WINDS expert about this problem.",TRUE);
 }
/*
determine input mode FIRST, then get project #
*/
 SetInputMode();
 set_proj_no();
 proj_number=(char *)strdup(get_proj_number());
/*
init header filename
*/
 (void)sprintf(&header_filename[0],HEADER, ProjectDirectory,proj_number);
/* 
establish input source
*/
 SetInputSource(GetInputMode());
/*
now that we know project number, determine display numbers of all hosts running 
concurrent sessions of WINDS and make socket connections to each for purposes
of making requests to display side. If post-processing mode, assume no
other sessions beyond local one.
*/
 SetupWindsBroadcast("WINPUT-INTERWINDS-TALK");
}

/*************************************************************************/

char *
GetNetcdfFileName()
/*
from the post-processing dataset-name given by startup command line argument
(or from examining the netconfig file), construct and return the implied 
netcdf filename, leaving off the "cdf" extension if it's already there.
*/
{
 static char *netcdf_fltno;
 static int index;
 static char filename[MAX_FLNM_LNGTH];

 if (!netcdf_fltno) {
  netcdf_fltno=(char *)strdup((char *)get_pp_filename());
  index=strlen(netcdf_fltno)-strlen(NETCDF_SUFFIX);
 }
 if (index>0 && !strcmp(&netcdf_fltno[index],NETCDF_SUFFIX)) {
  (void)sprintf(filename,"%s",netcdf_fltno);
 } else {
  (void)sprintf(filename,NETCDF_FILENAME,netcdf_fltno);
 }
 return filename;
}

/****************************  SetInputSource()  *************************/
void SetInputSource(int inputmode)
{
char *DataDirPath;

 switch (inputmode) {
  case ERROR:
   NotifyWINDS("FATAL: Input process has exited due to invalid value for input mode. This WINDS process will now also exit...contact a WINDS expert about this problem.",TRUE);
   break;
  case INIT_SETUP:
   break;
  case ADS_DATA:
   break;
  case RAWFILE_NETCDF:
/*
must call InitRawDataMode() before SetInputSource(), due to needs of 
CreateNetCDF() as fulfilled by Order(). Yeah, right.
*/
/*
calls InitAircraft()
*/
   InitRawDataMode();
   (void)sprintf(RawDataFilename, RAW_DATAPATH, DataDirectory, proj_number);
   (void)strcpy(header_filename, RawDataFilename);
   SetInputSource(NETCDF);
   break;
  case DISK_DATA:
   (void)sprintf(RawDataFilename, RAW_DATAPATH, DataDirectory, proj_number);
   (void)strcpy(header_filename, RawDataFilename);
   break;
  case NETCDF:
   if ( (char *)get_pp_filename()==NULL) {
    NotifyWINDS("FATAL: Input process has exited due to inability to get netcdf filename. This WINDS process will now also exit...contact a WINDS expert about this problem.",TRUE);
   }
/*
use system defined data path if one is set, else use WINDS defined path with
system defined WINDS as parent.  If that is undefined, error.
*/
   if ( (DataDirPath=(char *)getenv("DATA_DIR"))!=NULL) {
/*
DATA_DIR is defined; first try dataset name directly under that directory
*/
    (void)sprintf(&NetCDFFilename[0],NETCDF_DATADIRPATH,DataDirPath,
     (char *)GetNetcdfFileName());
    if (GetInputMode()==RAWFILE_NETCDF) {
     char vardbfilename[MAX_FLNM_LNGTH];
/*
this netcdf file will be created as we go...start it now before trying to
open it for reading
*/
     (void)sprintf(vardbfilename,VARDB_ATTR,ProjectDirectory,proj_number);
     if (InitializeVarDB(vardbfilename)==ERR) {
      NotifyWINDS("FATAL: Input process has exited due to failure in initializing Variable DataBase. This WINDS process will now also exit...contact a WINDS expert about this problem.",TRUE);
     }
/*     CreateNetCDF(NetCDFFilename); */
    }
    if (OpenNetCDFFile(NetCDFFilename,FALSE)==ERROR) {
/*
no luck; next try it as a proj-number subdirectory under that directory
*/
     AppendToSearchPath(NetCDFFilename);
     (void)sprintf(NetCDFFilename,NETCDF_DATADIRPATH,
      (char *)strcat((char *)strcat(DataDirPath,"/"),proj_number),
       (char *)GetNetcdfFileName());
     if (OpenNetCDFFile(NetCDFFilename,FALSE)!=ERROR) {
/*
found under proj-number subdir
*/
      dataset_init();
      SetDatasetIsFound(DATASETISFOUND);
      (void)fprintf(stderr,"WINPUT has opened netCDF file %s\n",NetCDFFilename);
      break;
     } else {
/*
no luck yet
*/
      AppendToSearchPath(NetCDFFilename);
     }
    } else {
/*
found directly under DATA_DIR dir
*/
     dataset_init();
     SetDatasetIsFound(DATASETISFOUND);
     (void)fprintf(stderr,"WINPUT has opened netCDF file %s\n",NetCDFFilename);
     break;
    }
   }
/*
either no DATA_DIR defined or dataset not found there: last resort, try
dataset name in proj-number subdir under WINDS default path
*/
   (void)sprintf(NetCDFFilename,NETCDF_DATAPATH,
    DataDirectory,proj_number,(char *)GetNetcdfFileName());
   if (OpenNetCDFFile(NetCDFFilename,FALSE)==ERROR) {
    perror("open netcdf_file for reading");
    (void)fprintf(stderr,
     "FATAL: can't open readable datafile %s...\n",NetCDFFilename);
     AppendToSearchPath(NetCDFFilename);
    SetDatasetIsFound(DATASETNOTFOUND);
   } else {
/*
found under proj-number subdir of WINDS default path
*/
    dataset_init();
    SetDatasetIsFound(DATASETISFOUND);
    (void)fprintf(stderr,"WINPUT has opened netCDF file %s\n",NetCDFFilename);
   }
 }
}

/****************************  AppendToSearchPath()  *************************/
void AppendToSearchPath(char *pathname)
{
 if (!pathname)
  return;
 if (strlen(DatasetSearchPath))
  (void)sprintf(&DatasetSearchPath[strlen(DatasetSearchPath)],", %s",pathname);
 else
  (void)sprintf(DatasetSearchPath,"%s",pathname);
}

/****************************  DoWinputShmemStuff()  *************************/
void DoWinputShmemStuff()
{
/*
get desired size of shared memory, AFTER project # established
*/
 {
 int size;
 if ((size=get_config_parm(SHMEM_SEX))==ERROR)
  set_shmem_size(DEFAULT_SHMEM_SEX);
 else
  set_shmem_size(size);
 }

/*
establish shared memory, AFTER shmem size established
*/
 if (!read_static_ids(GetComputeHost())) {
  NotifyWINDS("FATAL: Input process has exited due to failure in reading static shared memory ID's. This WINDS process will now also exit...contact a WINDS expert about this problem.",TRUE);
 }

 if (getaddr_static()==ERROR) {
  NotifyWINDS("FATAL: Input process has exited due to failure in getting static shared memory addresses. This WINDS process will now also exit...contact a WINDS expert about this problem.",TRUE);
 }

/* 
Initialization 
*/
 refill_shmem=FALSE;
 if (GetInputMode()==NETCDF) {
  *len_raw=0;
/*
reduce by one because BASETIME variable is not included in list of names
*/
  *len_derive=get_num_netcdf_vars();
 } else {
/*
either INIT_SETUP, ADS_DATA, RAWFILE_NETCDF, or DISK_DATA
*/
  DoDataInitStuff();
 }
}

/************************   InitNetCDFMode()  ***************************/
void InitNetCDFMode(int freeze)
{
 Freeze=freeze; 
 get_netcdf_varnames(derive_list,NAMLEN); 
}

/************************   FinishDataInitStuff()  ***************************/
void FinishDataInitStuff() 
{
/*
if post_processing mode, Freeze it
*/
 if (GetInputMode()==NETCDF) {
  InitNetCDFMode(TRUE);
 } else if (GetInputMode()==RAWFILE_NETCDF) {
  Freeze=FALSE; 
 } else { 
  InitRawDataMode();
 }
}

/******************   NotifyWINDS()  ************************************/
void NotifyWINDS(char *message, int exitstatus)
{
 NotifyWINDSProcess(message);
 if (exitstatus)
  cleanup();
}

/************************   GetNumberOfRawVars()  ***************************/
int GetNumberOfRawVars() 
{
  return *len_raw;
}

/********************   GetNumberOfDerivedVars()  ****************************/
int GetNumberOfDerivedVars()
{
  return *len_derive;
}

/************************   SetNumberOfRawVars()  ***************************/
void SetNumberOfRawVars(int num) 
{
  *len_raw=num;
}

/********************   SetNumberOfDerivedVars()  ****************************/
void SetNumberOfDerivedVars(int num)
{
  *len_derive=num;
}

/*************   FinishWinputShmemStuff()  **********************************/
void FinishWinputShmemStuff()
{
 read_dynamic_ids("WINPUT",(char *)GetComputeHost());
 if (getaddr_dynamic()==ERROR) {
  NotifyWINDS("FATAL: Input process has exited due to failure in getting dynamic shared memory addresses. This WINDS process will now also exit...contact a WINDS expert about this problem.",TRUE);
 }
/*
ensure chosen size will fit within bounds of kernel limits
*/
 set_max_shmem_size("WINPUT");
}

/*********************   IsDatasetFound()  ************************************/
int IsDatasetFound()
{
 return DatasetIsFound;
}

/*********************   SetDatasetIsFound()  *******************************/
void SetDatasetIsFound(int found)
{
 DatasetIsFound=found; 
 NotifyDatasetFound();
}

/****************   NotifyDatasetFound()  ************************************/
void NotifyDatasetFound()
{
 (void)sprintf(buff,"%d",IsDatasetFound());

 if (GetProtocolDebug())
  (void)fprintf(stderr,
   "WINPUT PROTOCOL: notify DISP_REQ_REPLY whether dataset found\n");
 if (write_to_socket(DISP_REQ_REPLY,buff,strlen(buff))==EBADWRITE)
  perror("acknowledge response write_to_socket");
 if (IsDatasetFound()==DATASETNOTFOUND) {
/*
notify display side of problem and wait for it to kill this process
*/
  if (GetProtocolDebug())
   (void)fprintf(stderr,
    "WINPUT PROTOCOL: notify DISP_REQ_REPLY that dataset is NOT found\n");
  if (write_to_socket(DISP_REQ_REPLY,&DatasetSearchPath[0],
   strlen(&DatasetSearchPath[0]))
    ==EBADWRITE)
   perror("acknowledge response write_to_socket");
  pause();
 } else {
  (void)sprintf(&buff[0],"%s %d %d",
   NetCDFFilename,get_netcdf_start_time(),get_netcdf_end_time());
   if (GetProtocolDebug())
    (void)fprintf(stderr,
     "WINPUT PROTOCOL: notify DISP_REQ_REPLY that dataset IS found\n");
  if (write_to_socket(DISP_REQ_REPLY,&buff[0],strlen(&buff[0]))==EBADWRITE)
   perror("acknowledge response write_to_socket");
 }
}

/****************************  GetShmemElapsedTime()  *************************/ 
int GetShmemElapsedTime()
{
 return *elapsed_time;
}

/****************************  SetShmemElapsedTime()  *************************/ 
void SetShmemElapsedTime(int elapsedtime)
{
 *elapsed_time=elapsedtime;
}

/****************************  GET_ARGS()  ******************************/

void get_args(int argc, char **argv) 
{
extern char *optarg; 
int c; 
/* 
set defaults 
*/ 
 SetControlHost(GetComputeHost());
 (void)sprintf(display_file,"\0"); 
 (void)sprintf(display_pnum,"\0"); 
 (void)sprintf(display_mode,"\0"); 
 Debug = FALSE;
 
 while ((c = getopt(argc, argv, CMDLINE_OPTIONS)) != -1) { 
  switch (c) { 
   case 'h': 
/* 
Control host -- if WINDS is called with no arguments, Control host is 
assumed to be the host on which winput is running.
*/ 
    SetControlHost(optarg); 
    (void)fprintf(stderr,"WINPUT control host: %s\n",(char *)GetControlHost()); 
    break;
   case 'd': 
/*
turn Debug on
*/ 
    Debug = atoi(optarg);
    (void)fprintf(stderr,"WINPUT: Debug = %d\n",Debug);
    break; 
   case 'm': 
/*
display mode
*/ 
    (void)strcpy(display_mode,optarg); 
    (void)fprintf(stderr,"WINPUT mode: %s\n",display_mode); 
    break; 
   case 'q': 
/*
turn on QC
*/
    QCenabled = TRUE;
    (void)fprintf(stderr,"WINPUT QC enabled.\n"); 
    break;
   case 'p': 
/* 
display project number 
*/ 
    (void)strcpy(display_pnum,optarg); 
    (void)fprintf(stderr,"WINPUT project: %s\n",display_pnum); 
    break; 
   case 'f': 
/* 
display dataset file descriptor 
*/ 
    (void)strcpy(display_file,optarg); 
    (void)fprintf(stderr,"WINPUT file: %s\n",display_file); 
    break; 
  } 
 }  
} 
