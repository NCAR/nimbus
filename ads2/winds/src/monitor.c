/*****************************************************
monitor -- monitor and report on various conditions in the onboard WINDS
environment. Also execute WINDS' requested system halts, WINDS display
kills, etc.

There are two "classes" of monitors, each of which performs differing tasks.
The Master class runs on the WINDS server; any other monitor (running on
any other host) is a Client.

The monitor process requires environment variables WINDS and HOME. It is to 
be run on each host configured into onboard WINDS network. Here's what it does:

*********** STARTUP:   

All classes:

Catch core and exit signals, log message and exit with/without core dump
Run in verbose mode if -d argument supplied on command line
Log messages to standard error; open file for syslog to prepare for logging there
Kill and replace any monitor detected running 
Determine WINDS Server; if not gotten, exit with message
Set monitor "class" as function of whether this process is running on WINDS server
Determine names of all hosts configured into onboard WINDS network
Initialize time-stamp file whose age is used to indicate need for checks on
 new core files

Master class:

Establish names of all flag files used for communicating changes of status and
 requests for WINDS-process killing and/or halts, on onboard network
Initialize time-stamp files whose ages are used to indicate need for checks on
 disk usage and swap space left

*********** LOOP: do all tasks, sleep one second, repeat:   

Master class:

Check on full network WINDS status: if there is a flag file indicating WINDS on
 a given host is not updating, check if it's running or not; if not, check for
 core files. Report any change in status of WINDS running on each host.
 Move any core files found to save location indicated at startup.

Check on /home disk space usage. Report to WINDS when any shortfall is imminent.
Check on swap space left. Report to WINDS when any shortfall is imminent.
Check on any messages from all other monitors, report any to ongoing log.
Check on requests from WINDS for complete network system halts, execute the
 associated WINDS script to fulfill the request.
Check on requests from WINDS for killing of all WINDS processes, execute the
 associated WINDS script to fulfill the request.
Check on requests from WINDS for individual system halts, execute the
 associated WINDS script to fulfill the request.

Client class:

Check on whether this host is mounting any NFS directories from WINDS server,
 report any changes in status via flag file. Halts will be performed by
 Master only on those hosts that are currently indicating they are mounting.

All classes:
 
Check for any new messages in system log; if any, copy to monitor log and 
 empty system log file.
Check that WINDS session file exists and is updating on local host, report 
 any changes in status. Create flag file if not updating.
If WINDS is not updating, check on Open Windows running; report any changes in 
 status. Look for OW core files if not running. Move any found to save location 
 indicated at startup.
Check for any new core files under current directory (at designated interval).
 Move any found to save location indicated at startup.
{
For a listing of all functions with descriptions as extracted by getdoc, 
see ../doc/getdoc.c.doc. For new listing of this extraction, run

% cd $WINDS/bin; ./getdoc ../src/monitor.c >! ../doc/monitor.c.doc

The left bracket above is in place to cooperate with getdoc so as to exclude
this section from the documentation it extracts.
*/

/************************************************************************/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>

/************************************************************************/

#include <socket.h>
#include <files.h>
#include <constants.h>
#include <proto.h>

/************************************************************************/

/****************  Internal usage

whether to exit with an abort (dumping core) or not

maximum expected full pathname
*/
#define CORE 0
#define NOCORE 1
#define MaxFileNameLength 255

/****************** Default Status parms

Monitor classes, file with process id, name of host, verbose mode variable,
and name of WINDS server host. Also, string for setting GMT time zone.
*/
#define Master 0
#define Client 1
static int MonitorClass = Master;
static char PidFileName[MaxFileNameLength];
static char ThisHost[32];
static int debug = FALSE;
static int RemoveFlagFiles = TRUE;
static char *WindsServer;
static char TZstring[16];

/***************** Run-time Status 

whether OpenWindows is running or not (on THIS host)

status of WINDS running on all hosts

whether WINDS is updating or not on THIS host

whether this host is either the WINDS server or getting NFS service from the
WINDS server or not
*/
static int OWRunning = ERROR;
static int *WINDSRunning;
static int WINDSUpdating = ERROR; 
static int SessionFileExists = ERROR;
static int Connected = ERROR;

/****************** Flag Files

list of all NoUpdateFileName's to be checked by Master monitor, who then
verifies whether or not WINDS is actuall running

whether WINDS session file exists or not on THIS host

flag file for "connected" to NFS server 

filenames used as flags between monitor and winds processes for killing WINDS
and/or halting
*/
static char NoUpdateFileName[MaxFileNameLength] = {"notupdating"};
static char **NotUpdatingFileName;
static char SessionFileName[MaxFileNameLength];
static char *ConnectedFileName;
static char ExitAllFileName[MaxFileNameLength];
static char HaltFileName[MaxFileNameLength];
static char KillHostListFileName[MaxFileNameLength];

/***********************  Interval checks on various and sundry

how old the current WINDS session file gets before session deemed not-updating

how long between (general) checks on core files; flag filename for such checks

where to save core files when found 

how long between checks on disk/swap usage; flag filename for such checks

minimum swap and /home space before user is notified of possible shortfall

minimum size of files to be reported to user in event of space shortage on 
/home partition usage
*/
#define MaxUpdateInterval 5	
#define CoreFileChkInterval 300
#define CoreFileChkFile ".monitor-cfc-check"
static char *SaveDir; 
#define DiskCheckInterval 150
#define DiskSpaceCheckFileName ".monitor-disk-check"
#define MinimumSwapSpace 4000
#define MinimumHomeSpace 10000
#define Threshold (MinimumHomeSpace/10)

/***************** Message Logging

syslog file, logging of system boots, crashes, and all relevant info from these
monitors

utility strings for reporting messages
*/
static char SysLogFileName[MaxFileNameLength];
static char SysLogUpdateFile[MaxFileNameLength];
static char MonitorLogFileName[MaxFileNameLength];
static char LogMessageString[1024];
static char *SavedMsgs=0;

/***************** Inter-process communications
*/
static int MasterSocket=0;
static int ClientSocket=0;

/*************** Project Number

current Project # as defined by file ProjectNumber

must declare ProjectDirectory and initialize it to use netconfig.c module:

$WINDS/hosts pathname
*/
static char *ProjectNumber;
static char WindsDirectory[MaxFileNameLength];
static char WINDSHostNamesPath[MaxFileNameLength];
char *ProjectDirectory;

/************* various declarations
*/
char *GetTimeStamp();
char *progName = "monitor:";
extern int errno;

/************************************************************************/

main(argc,argv)
int argc;
char **argv;
/*
the function names tell it all here...at least for me anyway
*/
{
 InitMonitor(argv,argc);

 ClassInitialize();

 LoopForever();
}

/************************************************************************/

InitMonitor(argv,argc)
int argc;
char **argv;
/*
basic initialization
*/
{
 (void)sprintf(TZstring,"%s","TZ=GMT");
 (void)putenv(TZstring);
 CatchSignals();
 GetArgs(argv,argc);
 SetHostName();
 SetComputeHost();
 SetFileNames();
 ReplaceCurrentMonitor();
 GetWINDSServerOrExit();
}

/************************************************************************/

SetHostName()
/*
set this host's name successfully or exit
*/
{
 char *p;

 if (GetHostName() == NULL) {
  LogMessage("WARNING: can't set host name\n",TRUE);
  CleanupAndExit(NOCORE,TRUE);
 } 

 strcpy(ThisHost, GetHostName());
 if ((p = strchr(ThisHost, '.')))
  *p = '\0';
}

/************************************************************************/

char *
ConstructConnectedFileName(host)
char *host;
/*
Given a host name, return the character string forming that host's
"Connected" filename, i.e. the file that exists if that host is
mounting an NFS directory from the WINDS Server, or if that host IS
the WINDS server. 
*/
{
static char connectedfilename[MaxFileNameLength];

 (void)sprintf(connectedfilename,"%s/%s/Connected",WINDSHostNamesPath,host);
 return connectedfilename;
}

/************************************************************************/

SetFileNames()
/*
set ProjectDirectory, WINDSHostNamesPath, MonitorLogFileName, and 
ConnectedFileName.  Open file MonitorLogFileName.
*/
{
 (void)sprintf(WindsDirectory,"%s",(char *)getenv("WINDS"));
 ProjectDirectory = (char *)getenv("PROJ_DIR");

/*
define pathname to proj-dir subdirectory that sits atop other subdirs for
all the hosts in the WINDS session network
*/
 (void)sprintf(WINDSHostNamesPath,"%s/hosts",ProjectDirectory);
/*
Monitor and system logging file names. System log filename must correspond to
that established in auto-install finish script stuff.
*/
 (void)sprintf(MonitorLogFileName,"%s/%s/MonitorLog",WINDSHostNamesPath,ThisHost);
 (void)sprintf(SysLogFileName,"/var/log/%s",ThisHost);
 (void)sprintf(SysLogUpdateFile,"%s/%s/.syslog-chk",WINDSHostNamesPath,ThisHost);
/*
NFS-connected to WINDS Server or not
*/
 ConnectedFileName=(char *)strdup(ConstructConnectedFileName(ThisHost));
printf("ConstructConnectedFileName: %s\n", ConnectedFileName);
}

/************************************************************************/

GetArgs(cmdline,numargs)
char **cmdline;
int numargs;
/*
look for -d option, if there turn on verbose mode
*/
{
 if (StringInList("-d",cmdline,numargs)) {
/*
output many details of run-time operations
*/
  debug=TRUE;
 }
 if (StringInList("-r",cmdline,numargs)) {
/*
output many details of run-time operations
*/
  RemoveFlagFiles=FALSE;
 }
}

/************************************************************************/

GetWINDSServerOrExit()
/*
determine which host is the so-called WINDS Server, i.e. the host from whom
WINDS sessions are run and from whom NFS directories are mounted
*/
{
 if ( (WindsServer=(char *)strdup((char *)GetWINDSServer()))==NULL) {
  (void)sprintf(LogMessageString,
  "No NFS server listed in Netconfig File. Monitor does NOT know name of WINDS Server, and will now exit; please contact an expert...\n");
  LogMessage(LogMessageString,TRUE);
  CleanupAndExit(NOCORE,TRUE);
 }
/*
finish up other initialization that depends on name of WINDS server
*/
 InitializeWindsServerDependents();
/*
don't log any message until WindsServer dependencies are finished
*/
 (void)sprintf(LogMessageString,"WINDS server is %s\n", 
  WindsServer);
 LogMessage(LogMessageString,TRUE);
}

/************************************************************************/

InitializeWindsServerDependents()
/*
initialization that depends on name of WINDS server
*/
{
 if (strcmp(ThisHost,WindsServer)) {
/*
this host is not the WINDS server; this monitor will perform differing 
tasks than the monitor there
*/
  MonitorClass = Client;
 } else {
/*
WINDS server is always considered "connected"
*/
  SetConnection(TRUE);
 }
}

#include <fmtmsg.h>
 
/************************************************************************/
void MsgToCtlHostAndLog(char *message)
{
 LogMsgSansTimeStamp(message);
 if (!strcmp((char *)GetControlHost(),ThisHost)) {
  fmtmsg(MM_CONSOLE,(char *)NULL,MM_NOSEV,message,
   (char *)NULL,(char *)NULL);
  LogMessage(message,TRUE);
 } else {
  if (write_to_socket(
   GetMonitorSocketNumber(get_session_number(GetControlHost())),
   message,strlen(message))==EBADWRITE) {
   perror((char *)strcat(" MONITOR socket-write on ",ThisHost));
   (void)fprintf(stderr,"%s",message);
  }
 }
}

/************************************************************************/
void MsgToLogAndConsole(char *msg)
/*
log message msg without a time stamp and also send it to standard error and
console of ControlHost
*/
{
 LogMsgSansTimeStamp(msg);
 LogMsgToConsole(GetControlHost(),(char *)NULL,msg);
}

/************************************************************************/

void LogMsgSansTimeStamp(char *msg)
/*
shortcut for logging a message without leading time-stamp. Useful as 
an argument in call to some other routine that needs a reporting facility;
can't use LogMessage() because it takes a 2nd argument that this other
routine does not know about.
*/
{
 LogMessage(msg,FALSE);
}

/************************************************************************/

void LogMessage(char *message, int AddTimeStamp)
/*
log a message, preceding it with a time stamp if so indicated by caller.
If this monitor is the Master, send message to standard error; else, 
send message via socket to the Master
*/
{
static char *FullMsg;

 if (!message)
  return;
 if (AddTimeStamp) {
  FullMsg=(char *)malloc((unsigned int)(StringLength(message)+
   StringLength(ThisHost)+StringLength((char *)GetTimeStamp())+10));
  (void)sprintf(FullMsg,"%s, %s: %s",ThisHost,GetTimeStamp(),message);
 } else {
  FullMsg=(char *)malloc((unsigned int)StringLength(message)+10);
  (void)sprintf(FullMsg,"%s",message);
 }
 if (MonitorClass==Master) {
  WriteToLog(FullMsg);
  if (message==SavedMsgs)
/*
the string SavedMsgs is used to store messages before the reporting socket
is created. Once the socket is created, that store is flushed here. Once
that is done, free up the storage space.
*/
   (void)free(SavedMsgs);
 } else {
/*
this is not the master; send message to master via socket
*/
  ReportToMaster(FullMsg);
 }
 (void)free(FullMsg);
}

/************************************************************************/

CheckForNewSysLogMsgs()
/*
check for any new messages recorded to system log since last time some 
were found. If any, copy to monitor log and remove system log file.
*/
{
FILE *SysLog;
char line[256];
char *sysloglines=(char *)NULL;
int index=0;

 if (FileExists(SysLogFileName)) {
  if (!FileExists(SysLogUpdateFile) ||
     (TimeSinceLastUpdate(SysLogFileName) <
     TimeSinceLastUpdate(SysLogUpdateFile))) {
   SysLog=fopen(SysLogFileName,"r");
/*
sometimes the /var/log/<hostname> file gets updated, apparently by OS, without
any content to file; ignore these instances
*/
   while (fgets(line,255,SysLog)!=NULL) {
    index=sysloglines?strlen(sysloglines):0;
    sysloglines=(char *)realloc((void *)sysloglines,
     (size_t)(index+strlen(line)+2));
    (void)sprintf(&sysloglines[index],"  %s",line);
   }
   if (sysloglines) {
    LogMessage("New system messages BEGIN: \n",TRUE);
    LogMessage(sysloglines,FALSE);
    LogMessage("New system messages END\n",TRUE);
    (void)free(sysloglines);
   }
   (void)fclose(SysLog);
   (void)unlink(SysLogFileName);
   CreateFile(SysLogFileName);
   CreateOrTouchFile(SysLogUpdateFile);
  }
 }
}

/************************************************************************/

WriteToLog(message)
char *message;
{
FILE *MonitorLog;
static int started=FALSE;

 MonitorLog=fopen(MonitorLogFileName,"a");
 if (!started) {
  started=TRUE;
  (void)fprintf(MonitorLog,"\n*-*-**-*--*-**--*--*-**--*--*-**--*--*-**--*\n");
 }
 (void)fprintf(MonitorLog,"%s",message);
 (void)fclose(MonitorLog);
}

/************************************************************************/

ClassInitialize()
/*
do all common initializing, plus any class-specific stuff
*/
{
 GetNetconfigHosts();
 DefineNoUpdateFiles();
 SetupCoreFileChecks();
 switch ( MonitorClass ) {
  case Master:
   MasterClassInit();
/*
Master also handles shutdown operations
*/
   InitShutdownFiles();
   break;
  case Client:
   break;
 }
}

/************************************************************************/
LoopForever()
{
 (void)nice(39);
 switch ( MonitorClass ) {
  case Master:
   LogMessage("Master monitor enabled\n",TRUE);
   break;
  case Client:
   LogMessage("Client monitor enabled\n",TRUE);
   break;
 }

fprintf(stderr, "MIONTOR: entering main loop\n");
 while (1) {
  CheckForNewSysLogMsgs();
  switch ( MonitorClass ) {
/*
class-specific tasks
*/
  case Master:
   MasterClassTasks();
   ShutdownsClassTasks();
   break;
  case Client:
   ClientClassTasks();
   break;
  }
/*
all monitors, Master and Client, do these
*/
  CheckOnWINDSUpdating();
  CheckOnOWRunning();
  CheckForCoreFiles();
/*
give it a rest so CPU doesn't get overworked
*/
  sleep (1);
 }
}

/************************************************************************/

CheckOnWINDSUpdating()
/*
check on the health of WINDS process: does it have a session file? if not,
create flag file that indicates problem with WINDS updating. if so, is it 
updating? if not, create flag file. Report any changes of status in updating.
*/
{
/*
check if winds-session file exists. This is a file that is updated once
every time WINDS goes into its time-out routine, with a string describing
the time frame encompassed thus far in this WINDS session. 
*/
 if (FileExists(SessionFileName)) {
  if (SessionFileExists!=TRUE) {
   (void)sprintf(LogMessageString,"WINDS sessionfile exists...\n");
   LogMessage(LogMessageString,TRUE);
   SessionFileExists  = TRUE; 
  }
/*
check that it has not been too long since winds-session file was last updated 
*/
  if ( TimeSinceLastUpdate(SessionFileName) < MaxUpdateInterval ) {
/*
WINDS is updating session file just fine...report this
*/
   if (WINDSUpdating!=TRUE) {
    (void)sprintf(LogMessageString,"WINDS session IS updating...\n");
    LogMessage(LogMessageString,TRUE);
    WINDSUpdating  = TRUE;
   }
  } else {
/*
session file not updating...report this
*/
   if (WINDSUpdating!=FALSE) {
    (void)sprintf(LogMessageString,"WINDS session is NOT updating...\n");
    LogMessage(LogMessageString,TRUE);
    WINDSUpdating  = FALSE;
/*
create a flag file that notifies the master monitor that there is some 
problem with WINDS updating
*/
    CreateFile(NotUpdatingFileName[get_session_number(ThisHost)]);
   }
  }
 } else {
/*
there is no session file...report this
*/
  if (SessionFileExists!=FALSE) {
   (void)sprintf(LogMessageString,"no WINDS sessionfile...\n");
   LogMessage(LogMessageString,TRUE);
   SessionFileExists  = FALSE; 
/*
create a flag file that notifies the master monitor that there is some 
problem with WINDS updating
*/
   CreateFile(NotUpdatingFileName[get_session_number(ThisHost)]);
  }
 }
}

/************************************************************************/

CheckOnOWRunning()
/*
check for Open Windows running (only if WINDS is not updating). Report 
changes of status. Look for core file if new status indicates it is NOT
running.
*/
{
 char filename[MaxFileNameLength];

 if (WINDSUpdating==FALSE) {
  (void)sprintf(filename,"%s/.xsun.%s:0",(char *)getenv("HOME"),ThisHost);
/*
perhaps a bad assumption being made here, on the relationhsip between
the .xsun.<host> file and the status of OW
*/
  if ( FileExists(filename)) {
   if ( OWRunning != TRUE ) {
    OWRunning = TRUE;
/*
openwin is running, thought at last check this was not the status...
*/
    LogMessage("OpenWindows IS running\n",TRUE);
   }
  } else {
   if ( OWRunning != FALSE ) {
    OWRunning = FALSE;
/*
OpenWindows is not running, though at last check this was not the status...
*/
    LogMessage("OpenWindows is NOT running\n",TRUE);
/*
check for OW core files, just in case that's why OW is not running... 
*/
    LogMessage("Check for OW crash core files...\n",TRUE);
    CoreFileCheck((char *)GetCurrentDir());
   }
  }
 }
}

/************************************************************************/

CheckForCoreFiles()
/*
check for core files under the process current directory whenever the
time stamp file is older than given interval; update time of said file
on completion. If file does not exist, create it.

Every now and then, perform a more complete check starting at top of $WINDS
heirarchy; less often, perform even more complete check starting at /home
heirarchy; at other times (or if not Master monitor) perform the normal check 
under home directory.
*/
{
 static int NumChecks=0;
 char checkdir[MaxFileNameLength];

 if ( FileExists(CoreFileChkFile)) {
  if ( TimeSinceLastUpdate(CoreFileChkFile) > CoreFileChkInterval ) {
   if ( debug ) {
    (void)sprintf(LogMessageString,"core file check under %s\n",
     GetCurrentDir());
    LogMessage(LogMessageString,TRUE);
   }
   NumChecks++;
   (void)sprintf(checkdir,"%s",(char *)GetCurrentDir());
   if (MonitorClass==Master) {
    if (!(NumChecks%12))
     (void)sprintf(checkdir,"/home");
    else if (!(NumChecks%3))
     (void)sprintf(checkdir,"%s",(char *)WindsDirectory);
   }
   CoreFileCheck(checkdir);
   TouchFile(CoreFileChkFile);
  }
 } else {
  CreateFile(CoreFileChkFile); 
 }
}

/************************************************************************/
void MasterClassInit()
/*
set up what's needed for core file checks and disk/swap space checks
*/
{
 SetupDiskChecks();
}

/************************************************************************/
void SetupCoreFileChecks()
/*
create or update the core check flag file; determine location of directory
in which to save any core files found.
*/
{
 CreateOrTouchFile(CoreFileChkFile);
 if (get_entry_by_locn("nfs")!=ERROR) {
  SaveDir=(char *)GetSaveDirectory();
 } else {
  LogMessage("WARNING: no NFS server found in Netconfig file\n",TRUE);
  SaveDir=(char *)NULL;
 }
}

/************************************************************************/
ReplaceCurrentMonitor()
/*
all monitors agree to leave their process id in PidFileName file. If, on
startup, this file exists, read its contents and send a kill message to
that process (which may not really be there). Regardless, leave this process
id in that file.
*/
{
 FILE *pidfile;
 int pid;

 (void)sprintf(PidFileName,"%s/%s/pidfile", WINDSHostNamesPath,ThisHost);
fprintf(stderr, "MONITOR: [%s]\n", PidFileName); fflush(stdout);

 if ((pidfile = fopen(PidFileName,"r"))) {
   (void)fscanf(pidfile,"%d",&pid);
   (void)fclose(pidfile);
   (void)kill(pid,SIGUSR1);
   }

 if ((pidfile=fopen(PidFileName,"w"))) {
   (void)fprintf(pidfile,"%d",getpid());
   (void)fclose(pidfile);
   }
}

/************************************************************************/

GetNetconfigHosts()
/*
determine names of hosts (and how many) configured into this session via
the Network Configuration file
*/
{
int stringlen,host;
/*
get project # from the ProjectNumber file, which is set up during
onboard proj_setup operation
*/
 if ( (ProjectNumber=(char *)GetProjectNumberFromFile())==NULL) {
  LogMessage("FATAL: no Project # file. Run setup_server_init <pnum>.\n",TRUE);
  CleanupAndExit(NOCORE,TRUE);
 }
/*
set names of hosts intending to have sessions
*/
 set_winds_sessions("MONITOR",ProjectNumber,WindsServer,ThisHost);
/*
determine the Control Host, to whose console shutdown messages are to go
*/
 if (get_entry_by_locn("ctl")!=ERROR) {
  set_proj_no();
  FindAndSetControlHost();
 } else {
  SetControlHost(ThisHost);
 }
/*
now that we know how many sessions there are, we can determine the
addressing for IPC socket
*/
 SetupIPC();
/*
now that IPC is set up, we can flush any messages saved to this point,
due to lack of a socket 
*/
 LogMessage(SavedMsgs,FALSE);
/*
now that we know how many sessions there are, we can allocate the array
that tracks status of WINDS running
*/
 WINDSRunning = (int *)malloc((unsigned int)
  get_num_winds_sessions()*sizeof(int));
 stringlen=0;
 for (host=0; host<get_num_winds_sessions(); host++) {
  WINDSRunning[host] = ERROR;
/*
report names of hosts
*/
  if (host==0) {
   (void)sprintf(LogMessageString,"Netconfig hosts: \n");
   stringlen=StringLength(LogMessageString);
  }
  (void)sprintf(&LogMessageString[stringlen],"  %s ",get_display_host(host));
  stringlen=StringLength(LogMessageString);
/*
confirm each host is actually alive via ping
*/
  if (HostIsAlive(get_display_host(host))) {
   (void)sprintf(&LogMessageString[stringlen],"(responsive)\n");
  } else {
   (void)sprintf(&LogMessageString[stringlen],"(no response)\n");
  }
  stringlen=StringLength(LogMessageString);
 }
 LogMessage(LogMessageString,FALSE);
}

/************************************************************************/

DefineNoUpdateFiles()
/*
define the names of all files that flag a WINDS session that is not
updating, one for each host in session network

each local-host monitor creates NoUpdateFileName when it detects that a session 
file exists but is not being updated. This could mean that WINDS process is
not timing out into it's one-second routine, or that a previous WINDS
session did not remove this file on exit
*/
{
int host;

 (void)sprintf(SessionFileName,SESSIONFILE,WindsDirectory,ThisHost);
 NotUpdatingFileName = (char **)malloc((unsigned int)
  (get_num_winds_sessions())*sizeof(char *));
 for (host=0; host<get_num_winds_sessions(); host++) {
  NotUpdatingFileName[host] = (char *)malloc((unsigned int)MaxFileNameLength);
  (void)sprintf(NotUpdatingFileName[host],"%s/%s/%s",
   WINDSHostNamesPath,get_display_host(host),NoUpdateFileName);
/*
remove any stragglers from previous sessions
*/
  (void)unlink(NotUpdatingFileName);
 }
}

/************************************************************************/

void
CatchCoreSignal(signal)
int signal;
/*
catch any signals that call for generating a core file
*/
{
 (void)sprintf(LogMessageString,
  "%s monitor caught %s-dumping core\n",
  MonitorClass==Master?"MASTER":"LOCAL-HOST",GetCoreSignalName(signal));
 LogMessage(LogMessageString,TRUE);
/*
leave a core file on exit
*/
 CleanupAndExit(CORE,TRUE);
}

/************************************************************************/

void
CatchExitSignal(signal)
int signal;
/*
catch any signals that do NOT call for generating a core file
*/
{
 (void)sprintf(LogMessageString,
  "%s monitor caught %s -exiting w/o dump\n",
  MonitorClass==Master?"MASTER":"LOCAL-HOST",GetExitSignalName(signal));
 LogMessage(LogMessageString,TRUE);
/*
exit w/o leaving a core

SIGUSR1 is issued in routine ReplaceCurrentMonitor() by another monitor
who is starting up, as a signal to this monitor that it's time to go. If
this monitor removes the PID file on exit, such a removal can happen AFTER
the new monitor has written it's own PID to that file. Don't do it.
*/
 CleanupAndExit(NOCORE,(signal!=SIGUSR1));
}

/************************************************************************/

CleanupAndExit(status,removepid)
int status,removepid;
/*
perform any cleanup, and exit with or without a core as per status
*/
{
fprintf(stderr, "MONITOR: CleanupAndExit\n");
 if (removepid)
/*
remove file that implies the monitor is still running, unless calling
party indicates not to do so.
*/
  (void)unlink(PidFileName);
/*
close off sockets, open files, etc.
*/
 if (MasterSocket)
  close_socket(MasterSocket);
 if (ClientSocket) {
  close_socket(ClientSocket);
 }
 if (status==CORE) {
/*
leave a core
*/
  abort();
 } else {
/*
just go
*/
  exit (status);
 }
}

/************************************************************************/

CatchSignals()
/*
establish signal handling, distinguishing between those that call for
leaving a core file and those that do not
*/
{
 int signal;
return;
 for (signal=0; signal < GetNumberCoreSignals(); signal++ )
  if ((int)sigset(GetCoreSignal(signal),CatchCoreSignal) < 0) 
   perror("sigset");

 for (signal=0; signal < GetNumberExitSignals(); signal++ )
  if ((int)sigset(GetExitSignal(signal),CatchExitSignal) < 0) 
   perror("sigset");
}

/************************************************************************/

SetupIPC()
/*
define address of socket for inter-process communiques, and initialize
connections to it for listening and talking

set up reporting to and listening from master/client via sockets

set up reporting to WINDS via socket, and listening to other monitors via
socket
*/
{
 int session;

 MasterSocket=GetMonitorSocketNumber(get_session_number(GetWINDSServer()));
 if (MonitorClass==Master) {
  ListenOnSocket(MasterSocket);
  TalkToSocket(GENERAL_LISTENING,WindsServer);
  for (session=0; session<get_num_winds_sessions(); session++) {
   TalkToSocket(GetMonitorSocketNumber(session),get_display_host(session));
  }
 } else {
  TalkToSocket(MasterSocket,WindsServer);
  ClientSocket=GetMonitorSocketNumber(get_session_number(ThisHost));
  ListenOnSocket(ClientSocket);
 }
}

/************************************************************************/

GetMonitorSocketNumber(session)
{
 return INTERMONITOR_LISTEN+session;
}

/************************************************************************/

void TalkToSocket(int port, char *host)
/*
we're using this interface instead of the talk_to_socket() in socket.c because
the latter does an exit on failure; here we want to do our own cleanup before
exiting. The talk_to_socket() routine should eventually be modified to
accommodate calling party's preference on this, if not accept a function
name in arg list to be called for exiting purposes...
*/
{
int retval;

 retval=make_client_socket(port,port,host);
 switch (retval) {
  case EBADOPEN:
   perror("opening stream socket");
   (void)sprintf(LogMessageString,"FATAL: Monitor process is exiting due to error opening stream socket...contact a WINDS expert about this problem.\n",TRUE);
   LogMessage(LogMessageString,TRUE);
   CleanupAndExit(NOCORE,TRUE);
  case EBADHOST:
   (void)sprintf(LogMessageString,"FATAL: Monitor process is exiting because an unknown host was requested for socket connection...contact a WINDS expert about this problem.\n",TRUE);
   LogMessage(LogMessageString,TRUE);
   CleanupAndExit(NOCORE,TRUE);
  case CLIENT_OK:
   break;
  default:
   (void)sprintf(LogMessageString,
    "WARNING: UNK status on writes to socket on %s: port %d, addr %d\n",
    host,port,port);
    LogMessage(LogMessageString,TRUE);
   break;
 }
}

/************************************************************************/

ListenOnSocket(port)
int port;
/*
setup listening on given socket port
*/
{
int retval;

 retval=listen_on_socket(ThisHost,port,port);
 switch (retval) {
 case OK:
  break;
 case EBADBIND:
/*
socket is already bound. 
*/
  (void)sprintf(LogMessageString,
   "FATAL: Socket already bound. monitor exiting.\n");
  LogMessage(LogMessageString,TRUE);
  CleanupAndExit(NOCORE,TRUE);
 default:
  (void)sprintf(LogMessageString,
   "FATAL: problem (# %d) on attempt to bind socket. Monitor exiting.\n",
   retval);
  LogMessage(LogMessageString,TRUE);
  CleanupAndExit(NOCORE,TRUE);
 }
}

/************************************************************************/

ReportToMaster(message)
char *message;
/*
Clients call this to report any message to master monitor
*/
{
int msglength,savedlength;

 if (!(msglength=StringLength(message)))
  exit;

 if (MasterSocket) {
  if (write_to_socket(MasterSocket,message,msglength)==EBADWRITE) {
   perror((char *)strcat(" MONITOR socket-write on ",ThisHost));
   (void)fprintf(stderr,"%s",message);
  }
  if (message==SavedMsgs)
/*
Once the socket is created, store of saved messages is flushed here. Once
that is done, free up the storage space.
*/
   (void)free(SavedMsgs);
 } else {
/*
the string SavedMsgs is used to store messages before the reporting socket
is created. 
*/
  savedlength=StringLength(SavedMsgs);
  SavedMsgs=(char *)realloc((void *)SavedMsgs,(size_t)(savedlength+msglength+4));
  (void)sprintf(&SavedMsgs[savedlength],"%s",message);
 }
}

/************************************************************************/

CheckForMessages()
/*
Master monitor checks for any waiting messages from clients on listen socket,
flushes to log; client monitors wait for messages from Master, flush to
their console.
*/
{
int length;
char *s;

 length=MAX_SOCKET_BUFF_LNGTH;
 if (MonitorClass==Master) {
  s=(char *)read_from_socket(MasterSocket,&length,NON_BLOCKING);
  if (strcmp(s,NODATA)) {
/*
flush message to log
*/
   LogMessage(s,FALSE);
  }
 } else {
/*
receiving message from Master: flush to console
*/
  s=(char *)read_from_socket(ClientSocket,&length,NON_BLOCKING);
  if (strcmp(s,NODATA)) {
   fmtmsg(MM_CONSOLE,(char *)NULL,MM_NOSEV,s,
    (char *)NULL,(char *)NULL); 
  }
 }
}

/************************************************************************/

SetupDiskChecks()
/*
do what needs to be done to setup disk/swap space checks, including an
initial report of space left
*/
{
 int space;

 CreateOrTouchFile(DiskSpaceCheckFileName);
 if ( (space=DiskSpaceLeft("/home"))!=ERROR) {
  (void)sprintf(LogMessageString,"Initial disk space chk, /home: %d Kb\n",
   space);
  LogMessage(LogMessageString,TRUE);
 }
}

/************************************************************************/

InitShutdownFiles()
/*
define names of all flag files that communicate a shutdown or kill of some
type; remove any stragglers from previous session
*/
{
/*
flag to kill all WINDS windows
*/
 (void)sprintf(ExitAllFileName,EXITALLDISPLAYS,WindsDirectory,ThisHost);
/*
flag to halt all onboard workstations
*/
 (void)sprintf(HaltFileName,HALT_FILENAME, WindsDirectory,ThisHost,"winds");
/*
file with names listed of hosts to be halted
*/
 (void)sprintf(KillHostListFileName,KILLHOSTSLISTFILE,WindsDirectory);
/*
remove any left overs unless command line arg specifies otherwise
*/
 if (RemoveFlagFiles) {
  (void)unlink(ExitAllFileName);
  (void)unlink(HaltFileName);
  (void)unlink(KillHostListFileName);
 } else {
  LogMessage("startup WITHOUT removing flag files: last monitor died?",TRUE);
 }
 (void)sprintf(LogMessageString,
  "Setting Control Host to %s\n",(char *)GetControlHost());
 LogMessage(LogMessageString,TRUE);
}

/************************************************************************/

MasterClassTasks()
/*
Check WINDS status, disk/swap space, and pending messages from other
monitors
*/
{
 CheckWINDSstatus();
 CheckDiskandSwapSpace();
 CheckForMessages();
}

/************************************************************************/

CheckWINDSstatus()
/*
for each host in current WINDS session, check for a flag indicating that
session is not updating; if there, check whether WINDS is actually running
or not. If it is, the problem must be with updating; else, check for any
core file from the WINDS session. Regardless of that flag existing or not,
if current status shows WINDS not running (which it will be at startup), 
verify that this is indeed the case.
*/
{
 int host;

 for (host=0; host<get_num_winds_sessions(); host++) {
  if (FileExists(NotUpdatingFileName[host])) {
/*
not updating flag is there; remove it
*/
   (void)unlink(NotUpdatingFileName[host]);
/*
investigate for an actual problem with WINDS
*/
   if ( (WINDSRunning[host] = WINDSisBlowing(host)) ) {
/*
WINDS is running OK; it may be that it is not going to its time-out routine
*/
    (void)sprintf(LogMessageString,
     "not-updating flag detected (%s), but WINDS running OK: not timing out?\n",
     get_display_host(host));
    LogMessage(LogMessageString,TRUE);
   } else {
/*
WINDS is not running, and the flag file is there: check for a crash
*/
    char chkdir[MaxFileNameLength];
    (void)sprintf(chkdir,"%s/%s",WINDSHostNamesPath,get_display_host(host));
    (void)sprintf(LogMessageString,
     "WINDS is NOT running (%s); checking for crash under %s\n",
     get_display_host(host),chkdir);
    LogMessage(LogMessageString,TRUE);
/*
core file check
*/
    CoreFileCheck(chkdir);
/*
check also for host alive
*/
    if (HostIsAlive(get_display_host(host))) {
     (void)sprintf(LogMessageString,"%s is alive...\n",get_display_host(host));
    } else {
     (void)sprintf(&LogMessageString[StringLength(LogMessageString)],"%s is NOT alive...\n",
      get_display_host(host));
    } 
    LogMessage(LogMessageString,FALSE);
   }
  }
  if (WINDSRunning[host]!=TRUE) {
/*
at last check, WINDS was not seen as OK; check again
*/
   if ( (WINDSRunning[host] = WINDSisBlowing(host)) ) {
    (void)sprintf(LogMessageString,"WINDS session IS running on %s\n",
      get_display_host(host));
    LogMessage(LogMessageString,TRUE);
   } 
  } 
 }
}

/************************************************************************/
void ClassifyAndMove(char *CoreFile)
/*
get the date of creation, name of process creator, stack info for CoreFile.
If possible, move it to designated save location.
*/
{
char newfile[MaxFileNameLength];
char coretype[MaxFileNameLength];
/*
date and type
*/
 (void)sprintf(LogMessageString,"%s %s\n",
  GetFileDate(CoreFile),GetFileType(CoreFile));
 LogMessage(LogMessageString,FALSE);
 (void)sprintf(coretype,"%s",(char *)GetCoreType(CoreFile));
 if (!strcmp(coretype,"'winds'")) {
/*
stack info, via gdb
*/
  (void)sprintf(LogMessageString,"\nWINDS Stack Info for %s: \n\n%s\n\n",
   CoreFile,(char *)ShowWINDSStack(CoreFile));
  LogMessage(LogMessageString,FALSE);
 }
 if (!SaveDir) 
  return;
/*
relocation
*/
 (void)sprintf(newfile,"%s/%s-%s",
  SaveDir,(char *)GetCoreType(CoreFile),(char *)GetFileDate(CoreFile));
 (void)sprintf(LogMessageString,"Moving core file to %s\n",newfile);
 LogMessage(LogMessageString,TRUE);
 MoveFile(CoreFile,newfile);
 if ( FileExists(CoreFile)) {
/*
the relocation failed; remove file instead
*/
  LogMessage("WARNING: failure moving core file; removing it instead",TRUE);
  (void)unlink(CoreFile);
 }
}

/************************************************************************/
void CoreFileCheck(char *directory)
/*
check for any files named "core" under directory. Each one found is to 
be examined and relocated
*/
{
 if ( debug ) {
  LogMessage("WINDS core file check\n",TRUE);
  (void)sprintf(LogMessageString,"Core files under %s: \n\n",directory);
  LogMessage(LogMessageString,TRUE);
 }
 FindFiles("core", directory, ClassifyAndMove, (void(*)(char *))LogMessage, FALSE);
}

/************************************************************************/
CheckDiskandSwapSpace()
/*
check on disk and swap space whenever it's been longer than designated
interval for such checks. Report to WINDS process if there is a shortfall.
*/
{
char message[255];
int swap_left,home_left;


 if ( FileExists(DiskSpaceCheckFileName)) {
  if ( TimeSinceLastUpdate(DiskSpaceCheckFileName) > DiskCheckInterval ) {
   if (debug ) {
    (void)sprintf(LogMessageString,"checking disk/swap space...\n");
    LogMessage(LogMessageString,TRUE);
/*
swap partition space
*/  }
   swap_left = SwapSpaceLeft();
   if ( swap_left < MinimumSwapSpace ) {
    (void)sprintf(message,
     "RESOURCE WARNING, %s: %d Kb swap space on %s; BELOW MINIMUM OF %d.",
     GetTimeStamp(),swap_left,ThisHost,MinimumSwapSpace);
/*
send notice to WINDS process, which is to be displayed as an alert to user
*/
    NotifyWINDSProcess(message);
   }
/*
/home partition space
*/
   home_left= DiskSpaceLeft("/home");
   if ( home_left < MinimumHomeSpace ) {
    (void)sprintf(message,
     "RESOURCE WARNING, %s: %d Kb home space on %s; BELOW MINIMUM OF %d. Please take immediate corrective action and/or contact an expert!!! ",
     GetTimeStamp(),home_left,ThisHost,MinimumHomeSpace);
/*
send notice to WINDS process, which is to be displayed as an alert to user
*/
    NotifyWINDSProcess(message);
/*
list files under /home that are larger than a significant percentage of the 
space left
*/
    LogMessage((char *)strcat(message,"\n\n'Large' Files: \n\n"),TRUE);
    LogMessage((char *)ListFilesLargerThan(Threshold,"/home"),FALSE);
   }
   TouchFile(DiskSpaceCheckFileName);
  }
 } else {
  CreateFile(DiskSpaceCheckFileName);
 }
}

/************************************************************************/

SetConnection(connected)
int connected;
/*
set status of "connection" to NFS server, both internally and with flag
file. Report whenever status changes.
*/
{
 if (connected) {
  if (Connected!=TRUE) {
   Connected=TRUE;
   CreateOrTouchFile(ConnectedFileName);
   (void)sprintf(LogMessageString,"%s is connected to monitor network\n",
    ThisHost);
   LogMessage(LogMessageString,TRUE);
  }
 } else {
  if (Connected!=FALSE) {
   Connected=FALSE;
   (void)unlink(ConnectedFileName);
   (void)sprintf(LogMessageString,"%s is NOT connected to monitor network\n",
    ThisHost);
   LogMessage(LogMessageString,TRUE);
  }
 }
}

/************************************************************************/

ClientClassTasks()
/*
set the status of "connection" to NFS server
*/
{
 if (!strcmp(ThisHost,WindsServer) || MountingFromNFSServer(WindsServer)) {
  SetConnection(TRUE);
 } else {
  SetConnection(FALSE);
 }
 CheckForMessages();
}

/************************************************************************/

ShutdownsClassTasks()
/*
check for overall halt, individual halt, and WINDS process kill requests
*/
{
/*
if the file HaltFileName exists ($WINDS/proj/hosts/WindsServer/halt.winds),
then halt all hosts that are connected
*/
 ExecuteShutdownScript("Check for halt-all flag...\n",
  HaltFileName,"halt -",FALSE,TRUE);
/*
if file ExitAllFileName exists ($WINDS/proj/hosts/WindsServer/ExitAllDisplays),
then kill winds on all hosts
*/
 ExecuteShutdownScript("check for exit all winds",
  ExitAllFileName,"kill-winds",FALSE,FALSE);
/*
if the file KillHostListFileName exists ($WINDS/proj/hosts/halt.winds),
then halt the hostnames listed therein
*/
 ExecuteShutdownScript("check for local-host requests to halt...\n",
  KillHostListFileName,"sudo -x",TRUE,FALSE);
}

/************************************************************************/
 
ExecuteShutdownScript(message,flagfilename,scriptname,argsfromfile,chkconnection)
char *message;
char *flagfilename;
char *scriptname;
int argsfromfile,chkconnection;
/*
report message to log; if flagfilename exists, execute the script named
scriptname under the $WINDS/scripts directory. If argsfromfile is TRUE,
read flagfilename for list of arguments to send to scriptname; else, send
names of ALL network session hosts. If chkconnection is TRUE, though, first
send only the names of hosts that are still "connected" to NFS server.
*/
{
int host,index;
char *command;
char hostname[128];
char string[256];
FILE *arglist;
char process_name[MaxFileNameLength];

 if ( debug )
  LogMessage(message,TRUE);
 if (FileExists( flagfilename)) {
  (void)sprintf(process_name,"%s/scripts/%s",WindsDirectory,scriptname);
  command=(char *)strdup(process_name);
  index=0;
  if (argsfromfile) {
/*
get argument list from file 
*/
   arglist=fopen(flagfilename,"r");
   while (fscanf(arglist,"%s",hostname)!=EOF) {
    index=StringLength(command);
    command = (char *)realloc((void *)command,
     (size_t)(index+StringLength(hostname)+2));
    (void)sprintf(&command[index]," %s",hostname);
   } 
   (void)fclose(arglist);
  } else {
/*
send all hostnames as argument list...
*/
   for (host=0; host<get_num_winds_sessions(); host++) {
    if (!chkconnection || CheckConnection((char *)get_display_host(host))) {
/*
...that is, if the host is "connected" or there is no need to check connectivity
*/
     index=StringLength(command);
     command = (char *)realloc((void *)command,
      index+StringLength((char *)get_display_host(host))+4);
     (void)sprintf(&command[index]," %s",get_display_host(host));
    }
   }
  }
/*
remove flag before proceeding with any shutdowns
*/
  (void)unlink(flagfilename);
  if (index) {
   (void)sprintf(LogMessageString,"executing following: %s\n",command);
   LogMessage(LogMessageString,TRUE);
/*
execute the script, getting its output as it happens; report to log without
a time stamp
*/
   (void)sprintf(string,"WINDS Shutdown, %s:",GetTimeStamp());
   MsgToCtlHostAndLog(string);
   (void)GetShellOutput(command, MsgToCtlHostAndLog);
  }
  (void)free(command);
 } 
}

/************************************************************************/

CheckConnection(host)
char *host;
/*
construct the "connected" flag filename from hostname host, return TRUE
if file exists, else return FALSE
*/
{
char *filename;

 filename=(char *)ConstructConnectedFileName(host);
 return FileExists(filename); 
}

/************************************************************************/

WINDSisBlowing(host)
int host;
/*
determine whether WINDS session on host is running or not; return TRUE if
so, else return FALSE
*/
{
int port,addr;

 port=INTERWINDS_LISTEN+get_session_number(get_display_host(host));
 addr=NUM_PORTS+get_session_number(get_display_host(host));
/*
use the fact that a socket already bound can't be bound again successfully
as the test for WINDS running. Note that this assumes that only hosts
listed in Network Config file are actually running on the WINDS Server.
Any others will have a "session number" of 0, which corresponds to the
first Network Config session host listed; thus, it will appear that WINDS
is running on behalf of this first host, when it's actually running for
some other host.
*/
 if (bind_socket(port,addr)==EBADBIND && errno==EADDRINUSE) {
  if ( debug ) {
   (void)sprintf(LogMessageString,"WINDS IS running on %s\n",
    get_display_host(host));
   LogMessage(LogMessageString,FALSE);
  }
  return TRUE;
 } else {
/*
bind was successful; WINDS is not running. Detach from socket.
*/
  close_socket(port);
  if ( debug ) {
   (void)sprintf(LogMessageString,"WINDS is NOT running on %s\n",
    get_display_host(host));
   LogMessage(LogMessageString,FALSE);
  }
  return FALSE;
 }
}
