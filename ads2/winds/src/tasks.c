/* tasks.c -- manage startup and disposal of exec'd / forked processes
*/

#include <stdio.h>
#include <xview/xview.h>

/****************************  WINDS INCLUDES  ******************************/

#include <tasks.h>
#include <lengths.h>
#include <constants.h>
#include <files.h>
#include <errors.h>
#include <socket.h>
#include <proto.h>
#include <printers.h>

/***************************  GLOBAL WITHIN FILE  ****************************/

static int input_started=FALSE;  /* whether input has been started already */
Notify_value bye_bye();
static int QCenabled=FALSE;

static int portserverpid[NUMSERIALPORTS]={ERROR,ERROR},
           OldPSPRINTpid=ERROR,
           psprintpid=ERROR;
/*
name the assumed device suffixes for serial ports on Sun machines
*/
static char *serialport[NUMSERIALPORTS]= { PORT1, PORT2 };


/**********************  start_winds_tasks()  ********************************/
void start_winds_tasks(void)
{
 if (WindsControlHost) {
  start_task(INPUT);
  if (connected_to_ads)
   start_task(WINDCOMM);
 }
/*
on startup, do NOT kill extant printer tasks; if any are running, allow
new process(es) to fail on socket binding address problem.
*/
 StartWindsPrinterTasks(FALSE); 
}

/**********************  kill_winds_tasks()  ********************************/

void kill_winds_tasks(void)
{
 if (WindsControlHost) {
/* 
only the winds-host deals with input and windComm process mgmt
*/
  kill_winds_task(INPUT);
  kill_winds_task(WINDCOMM);
 }
/*
no-one kills the printer tasks automatically on exit, since one host's
port server, e.g., may be serving for other hosts still on-line. If
server(s) still running on restart of winds, attempt to start new one
will fail on bad bind, but no problem, extant server will still suffice.
Only on explicit reset operation will printer tasks be killed to make
way for new servers.
*/
}

/******************* KillWindsPrinterTasks() *******************************/
void KillWindsPrinterTasks(void)
{
 if (WindsControlHost)
  kill_winds_task(POSTSCRIPTSERVER);
 kill_winds_task(PORTSERVER);
}

/*******************  StartWindsPrinterTasks()  *****************************/

StartWindsPrinterTasks(int reset) 
/*
if reset is true, kill extant printer servers. Either way, attempt to
start new ones up. If attempted kills fails or no kill attempted with
servers currently running, new servers will fail to bind and then exit.
If kills succeed or none needed, new servers will start.
*/
{
 if (reset) {
  KillWindsPrinterTasks();
 } 
 start_task(PORTSERVER);
 if (WindsControlHost)
/*
only one psprint server needed, and only the winds compute host deals with it 
*/
  start_task(POSTSCRIPTSERVER);
}

/****************************************************************************/

LogMsgAndExit(desthost,label,message)
char *desthost, *label, *message;
/*
all exits that indicate a condition unresolvable by simply re-starting the
process should use 0 as exitstatus, so that parent process doesn't go into
cardiac arrest trying to (ad nauseum) re-start process on detection of
previous exit
*/
{
 LogMsgToConsole(desthost,label,message);
 if (WindsControlHost) 
  RequestAlertNow(message);
 else
  NotifyWINDSProcess(message);
/*
Even though the man page tells you to use _exit(), NOT exit(), using
the former causes a break-down with notifier: the wait3 func doesn't
get the stated exit argument, thinks that the status implies a need to restart,
and that goes on forever if, e.g., the binary is missing. Use exit() instead.
*/
 exit(0);
}

/**************************  START_TASK()  **********************************/

SetQCenabled()
{
 (void)fprintf(stderr,"WINDS QC process requested enabled...\n");
 QCenabled=TRUE;
}

/**************************  START_TASK()  **********************************/

start_task(which)
int which;
{
int remote,port,length,temp_pid;
char process_name[MAX_FLNM_LNGTH];
char *s;
char buffer[MAX_SOCKET_BUFF_LNGTH];

 switch (which) {
 case POSTSCRIPTSERVER:
  if (!WindsControlHost) {
/*
If a problem exists with the psprint server, the control host 
should do the reset. 
*/
   return;
  }
 
  switch (temp_pid=vfork()) {
   case ERROR:
    perror("vfork of POSTSCRIPTSERVER");
    LogMsgAndExit(GetDisplayHost(),"WINDS:Printing","WARNING: problem vforking psprint...contact WINDS expert with this information!");
    break;
   case 0:
    (void)sprintf(process_name,PSPRINT_PROCESS,winds_bin_path);
    (void)execl(process_name,process_name,NULL);
    perror("execl of psprint from ../bin directory");
    LogMsgAndExit(GetDisplayHost(),"WINDS:Printing","WARNING: Postscript printing process could not be started. This is likely due to a lack of a psprint binary. Contact a WINDS expert.");
    break;
   default:
    OldPSPRINTpid=psprintpid;
    psprintpid=temp_pid;
    time_to_log();
    (void)fprintf(stderr,"WINDS got PSPRINT pid: %d\n",temp_pid);
    notify_set_wait3_func(GetBaseFrame(),bye_bye,temp_pid);
    break;
  }
  break;
 case PORTSERVER:
/*
if compute host is not the same as port server host, so indicate the need for
remote request
*/
  remote=strcmp((char*)GetComputeHost(),(char *)get_port_server_host());
  for (port=0; port<NUMSERIALPORTS; port++) {
   FILE *pipe;
   (void)sprintf(process_name,PRTSRVR_PROCESS,winds_bin_path);
   (void)sprintf(buffer,"%s %s %s %s %s &",
    remote?"rsh ":"",remote?(char *)get_port_server_host():"", 
    process_name,serialport[port],GetComputeHost());
   pipe=popen(buffer,"r");
   pclose(pipe);
  }
  break;
 case INPUT:
  kill_winds_task(INPUT);
  if (!input_started) {  /* don't reset after data input has already begun */
   *elapsed_time=0;
   input_started=TRUE;
  }
  switch (temp_pid=vfork()) {
   case ERROR:
    perror("vfork of input");
    LogMsgAndExit(GetDisplayHost(),"WINDS:IPC","FATAL: problem vforking input...contact WINDS expert with this information!");
    exit (0);
    break;
   case 0:
    (void)sprintf(process_name,"%s/%s",winds_bin_path,
     (char *)GetWinputProgramName());
    if (post_processing_mode())
     (void)execl(process_name,process_name,
      "-h",(char *)GetControlHost(),
      "-m",GetDisplayMode(),
      "-p",get_proj_number(),
      "-f",get_pp_filename(),
      NULL);
    else
     (void)execl(process_name,process_name,
      "-h",(char *)GetControlHost(),
      "-m",GetDisplayMode(),
      "-p",get_proj_number(),
      QCenabled?"-q":"",
      NULL);
    perror("execl of input from ../bin directory");
    LogMsgAndExit(GetDisplayHost(),"WINDS:IPC","FATAL: WINDS Input process could not be started. This is likely due to a lack of a winput binary. Contact a WINDS expert.");
/*
this is the child, so don't bother checking for net messages from above, just
exit
*/
    break;
   default:
    notify_set_wait3_func(GetBaseFrame(),bye_bye,temp_pid);
    *input_pid=temp_pid;
    time_to_log();
    (void)fprintf(stderr,"WINDS got INPUT pid: %d\n",temp_pid);
    break;
  }
/* 
wait for input process to notify us that it is ready for dialogue
*/
  length=MAX_SOCKET_BUFF_LNGTH;
  if (GetProtocolDebug())
   (void)fprintf(stderr,"WINDS PROTOCOL: wait for input to start listening\n");
  s=(char *)read_from_socket(DISP_REQ_REPLY,&length,BLOCK_TIME_OUT);
  if (strcmp(s,EBADREAD)) {
   int disp_reply;
   (void)sscanf(s,"%d",&disp_reply);
   if (disp_reply!=LISTENING) {
    length=ERROR;
   } 
  } else {
   length=ERROR;
  }
  if (length==ERROR) {
   if (GetProtocolDebug())
    (void)fprintf(stderr,"WINDS PROTOCOL: ");
   else
    time_to_log();
   (void)fprintf(stderr,"WARNING: new input process not listening yet!!\n");
   CheckForNetworkMessages();
   if (!ConfirmWinputRunning(FALSE)) {
    RequestAlertNow("FATAL: input process is not responding and may have some problem running. Check your console for messages and contact a WINDS expert about this message...");
    winds_cleanup(ERROR);
   }
  } else {
   if (GetProtocolDebug()) {
    (void)fprintf(stderr,"WINDS PROTOCOL: Winput listening OK\n");
   }
  }
  break;

 case WINDCOMM:
  kill_winds_task(WINDCOMM);
  switch (temp_pid=vfork()) {
   case ERROR:
    perror("vfork of windComm");
    LogMsgAndExit(GetDisplayHost(),"WINDS:IPC","FATAL: problem vforking windComm...contact WINDS expert with this information!");
    exit (0);
    break;
   case 0:
    (void)sprintf(process_name,WINDCOMM_PROCESS,winds_bin_path);
    (void)execl(process_name,process_name,proj_number,NULL);
    perror("execl of windComm from ../bin directory");
    LogMsgAndExit(GetDisplayHost(),"WINDS:IPC","FATAL: windComm process could not be started. This is likely due to a lack of a windComm binary. Contact a WINDS expert.");
    exit (0);
    break;
   default:
    *windComm_pid=temp_pid;
    time_to_log();
    (void)fprintf(stderr,"WINDS got WINDCOMM pid: %d\n",temp_pid);
    notify_set_wait3_func(GetBaseFrame(),bye_bye,temp_pid);
    break;
  }
  break;
 }
}

/**********************  KILL_WINDS_TASK()  ********************************/
void kill_winds_task(int which)
{
switch (which) {

 case PORTSERVER:
/*
as many as two from the following may be the socket(s) that the PORTSERVER
is bound to. Kill 'em all just to be sure.
*/
  NotifyPrinter(ERROR,PJETSOCKET,PARTYISOVER,"none");
  NotifyPrinter(ERROR,EPSONSOCKET,PARTYISOVER,"none");
  NotifyPrinter(ERROR,ASCIIVALUES,PARTYISOVER,"none");
  NotifyPrinter(ERROR,FLAT_PANEL_DATA,PARTYISOVER,"none");
  break;
 case POSTSCRIPTSERVER:
  if (WindsControlHost)
/*
only compute-host deals with psprint server mgmt; others, on reset operation,
issue a request via socket to control-host ONLY for PORTSERVER resets.
*/
   NotifyPrinter(ERROR,POSTSCRIPT,PARTYISOVER,"none");
  break;
 case INPUT:
fprintf(stderr, "killing windCom\n");
 if (input_pid && *input_pid) {

fprintf(stderr, "  passed if test\n");
/* input process may be in stopped mode, in which case SIGUSR2 doesn't seem
   to have any effect. Insure it is not stopped first.
*/
  kill(*input_pid,SIGCONT);
  if (kill(*input_pid,SIGUSR2) == ERROR) {
   time_to_log();
   (void)fprintf(stderr,"can't kill input, pid %d (not there?)\n",*input_pid);
  }
  *input_pid=0;
 }
 break;

 case WINDCOMM:
fprintf(stderr, "killing windCom\n");
 if (windComm_pid && *windComm_pid) {
fprintf(stderr, "  passed if test\n");
  kill(*windComm_pid, SIGCONT);
  if (kill(*windComm_pid, SIGUSR2) == ERROR) {
   time_to_log();
   fprintf(stderr, "can't kill windComm, pid %d (not there?)\n", *windComm_pid);
  }
  *windComm_pid=0;
 }
 break;
}
}

/****************************** GetProcessName()  ***************************/
char *GetProcessName(int pid)
{
int port;

 for (port=0; port<NUMSERIALPORTS; port++) {
  if (pid==portserverpid[port]) {
   return "PRT_SRVR";
  }
 }
 if (pid==psprintpid ) {
  return "PSPRINT";
 }
 if ( pid==OldPSPRINTpid) {
  return "Old PSPRINT";
 }
 if (pid==*input_pid) {
  return "WINPUT";
 }
 if (pid==*windComm_pid) {
  return "WINDCOMM";
 }
 return "UNKNOWN";
}

/****************************   BYE_BYE()  **********************************/

/*ARGSUSED*/
Notify_value
bye_bye(me,pid,status,rusage)
Notify_client me;
int pid;
int *status;
struct rusage *rusage;
{
int restart;

 restart=FALSE;
 time_to_log();
 if (WIFEXITED(*status)) {
  restart=*status;
  (void)fprintf(stderr,"%s process (%d) called exit() with status %d\n",
   GetProcessName(pid),pid,restart);
 } else if (WIFSIGNALED(*status)) {
  restart=*status;
  (void)fprintf(stderr,"%s process (%d) exited due to signal %d\n",
   GetProcessName(pid),pid,restart);
 } else if (WIFSTOPPED(*status)) {
  (void)fprintf(stderr,"%s process (%d) stopped due to signal\n",
   GetProcessName(pid),pid);
 } else {
  (void)fprintf(stderr,"%s process (%d) died w/o status\n",
  GetProcessName(pid),pid);
 }
 if (restart)
  RestartProcess(pid);
 return(NOTIFY_DONE);
}

/****************************** RestartProcess()  ***************************/

RestartProcess(pid)
int pid;
{
int port;

 for (port=0; port<NUMSERIALPORTS; port++) {
  if (pid==portserverpid[port]) {
/*
actually, since portservers are started up via popen() instead of
forking, their exits are NOT detected, but leave this code in place anyway
just for symmetry's sake, and in case things change later
*/
   (void)fprintf(stderr,"restarting portserver processes\n");
   start_task(PORTSERVER);
   break;
  }
 } 
 if (pid==psprintpid) {
  (void)fprintf(stderr,"restarting psprint process\n");
  start_task(POSTSCRIPTSERVER);
 } 
 if (WindsControlHost) {
  if (pid==*input_pid) {
   (void)fprintf(stderr,"restarting input process\n");
   start_task(INPUT);
  } else if (pid==*windComm_pid) {
   if (connected_to_ads) {
    (void)fprintf(stderr,"restarting windComm process\n");
    start_task(WINDCOMM);
    set_notice_needed(REBOOT_ADS);
   } 
  }
 }
}
