#include <stdio.h>
#include <sys/param.h>
#include <netdb.h>
#include <socket.h>
#include <display.h>
#include <printers.h>
#include <constants.h>
#include <lengths.h>
#include <errors.h>
#include <proto.h>

/* 
ipcdisp.c -- handle IPC requests for display side actions
*/
static int msglength;
static char *message;

/********************  CheckForNetworkMessages()  ****************************/

CheckForNetworkMessages()
{

 msglength=MAX_SOCKET_BUFF_LNGTH;
 message=(char *)read_from_socket(INTERWINDS_LISTEN+get_session_number(GetDisplayHost()),
  &msglength,NON_BLOCKING);
 if (msglength>0) {
  ProcessWindsMessage("INTERWINDS_LISTEN");
 }
/*
check also the socket provided for various status queries from processes 
that do not know the INTERWINDS_LISTEN address
*/
 msglength=MAX_SOCKET_BUFF_LNGTH;
 message=(char *)read_from_socket(GENERAL_LISTENING,&msglength,NON_BLOCKING);
 if (msglength>0) {
  ProcessWindsMessage("GENERAL_LISTENING");
 }
}

/************************  ProcessWindsMessage()  ****************************/

ProcessWindsMessage(socket)
char *socket;
{
int which,window;
char hostname[MAXHOSTNAMELEN];
char filename[MAX_FLNM_LNGTH];
char fltno[MAX_FLNM_LNGTH];
int notify,session;

 (void)sscanf(message,"%d",&which);
 switch (which) {
  case ARE_YOU_LISTENING:
/*
query from some new WINDS process seeking an available cpu, i.e. one that
does not have a WINDS session on it. Tell it that this one is busy.
*/
   (void)sscanf(message,"%d %s",&which,hostname);
   talk_to_socket("WINDS-GENERAL_RESPONSE",
    GENERAL_RESPONSE,GENERAL_RESPONSE,hostname);
   (void)sprintf(message,"%d",LISTENING);
   write_to_socket(GENERAL_RESPONSE,message,strlen(message)); 
   close_socket(GENERAL_RESPONSE);
   break;
  case PRINTRESPONSE:
   (void)sscanf(message,"%d %d",&which,&window);
/*
more info expected
*/
   msglength=MAX_SOCKET_BUFF_LNGTH;
   message=(char *)read_from_socket
    (INTERWINDS_LISTEN+get_session_number(GetDisplayHost()),&msglength,BLOCK_TIME_OUT);
   if (msglength>0) {
    switch (window) {
     case TEXTFILEPRINTED:
      time_to_log();
/*
on message received, \n is already appended; no need to add another
*/
      (void)fprintf(stderr,"%s",message);
      break;
     case BYEBYE:
      time_to_log();
      (void)fprintf(stderr,"%s exiting on request over socket\n",message);
      break;
     case PREAMBLE:
      break;
     case UPPER_LEFT:
     case UPPER_RIGHT:
     case LOWER_RIGHT:
     case LOWER_LEFT:
      time_to_log();
      (void)fprintf(stderr,"%s",message);
      (void)sprintf(message,"Print request received by %s",
       (char *)GetCurrentImagePrinterName());
      show_op_msg(window,message);
      break;
    }
   } 
   break;
  case REQ_CHG_DATE:
   time_to_log();
   (void)fprintf(stderr,"WINDS received INPUT Request to CHG DATE\n");
   msglength=MAX_SOCKET_BUFF_LNGTH;
   message=(char *)read_from_socket
    (INTERWINDS_LISTEN+get_session_number(GetDisplayHost()),&msglength,BLOCK_TIME_OUT);
   if (msglength>0) {
    SetCurrentDate(message);
   } else {
    (void)fprintf(stderr,
     "WARNING: got notice to chg date but no date string\n");
   }
   break;
 case REQ_USER_ALERT:
  time_to_log();
  (void)fprintf(stderr,"WINDS receives request for USER ALERT\n");
  (void)sscanf(message,"%d",&which);
  msglength=MAX_SOCKET_BUFF_LNGTH;
  message=(char *)read_from_socket(GENERAL_LISTENING,&msglength,BLOCK_TIME_OUT);
  if (msglength>0) {
   RequestAlertNow(message);
  } else {
   (void)fprintf(stderr,
    "WARNING: got notice to give alert to user but no msg followed\n");
  }
  break;
 case PLOTSPEXFOLDERREMOVED:
  (void)sscanf(message,"%d %s %s",&which,hostname,filename);
  if (!(strcmp(filename,(char *)GetCurrentPlotspexFolderName()))) {
/*
this is the same folder as the one that was removed: go ahead and process
*/
   time_to_log();
   (void)fprintf(stderr,"%s acknowledges that %s has deleted folder '%s'\n",
    (char *)GetDisplayHost(),hostname,filename);
   SetCurrentFolderDeleted(TRUE);  
  }
  break;
 case UPDATEFLIGHTNUM:
  (void)sscanf(message,"%d %d %s",&which,&notify,fltno);
  if (GotFlightNumber() && !strcmp(fltno,(char *)GetFlightNum()))
/*
same as before ... perhaps sender of message wants to be sure we
have it right
*/
   return;
  time_to_log();
  (void)fprintf(stderr,
   "%s updating flight # to %s as per net request\n",
    (char *)GetDisplayHost(),fltno);
  set_fltno(fltno);
  DrawControlPanelFltno();
  if (post_processing_mode()) {
   return;
  }
  if (notify) {
/*
this is real-time and sender wishes message to propagate --
notify any other displays to update their flight # displays as well, sans
the request to update everyone else again, ad nauseum
*/
   (void)sprintf(message,"%d %d %s",UPDATEFLIGHTNUM,FALSE,fltno);
/*
set flag to remind yourself that you're the one sending this request; hence
there is no need to update your own flight # again
*/
   SetResponseToNetRequest(TRUE);
   for (session=0; session<get_num_winds_sessions(); session++) {
    if (strcmp((char *)GetDisplayHost(),(char *)get_display_host(session))) {
     (void)fprintf(stderr,"%s notifies %s with UPDATEFLIGHTNUM\n",
     (char *)GetDisplayHost(),(char *)get_display_host(session));
     if (write_to_socket(INTERWINDS_TALK+session,message,strlen(message))
      ==EBADWRITE)   
      perror("UPDATEFLIGHTNUM write_to_socket");
    }
   }
  }
  break;
 case INITPLOTSPEXFILE:
  (void)sscanf(message,"%d %s %s",&which,hostname,filename);
  if (strcmp(hostname,(char *)GetDisplayHost())) {
/*
this is not the same host as the one sending the message: go ahead and process
*/
   if (!(strcmp(filename,(char *)GetPlotspexFilename()))) {
    time_to_log();
    (void)fprintf(stderr,
     "%s acknowledges %s request for re-init of folder '%s'\n",
     (char *)GetDisplayHost(),hostname,filename);
/*
set flags that the plotspex file needs re-initialization and that this is being
done in response to some other host's actions. The latter precludes this host
sending out notices to other hosts to update THEIR plotspex files, which would
result in a rather annoying feedback loop.
*/
    SetNeedInitPlotspex(TRUE);
    SetResponseToNetRequest(TRUE);
   }
  } else {
/*
a refresh of panel list may be needed, since it may have just now gotten
destroyed within a callback 
*/
   RefreshFolderList();
  } 
  break;
 default:
  (void)fprintf(stderr,
   "WARNING: unexpected message received on socket %s: %s",socket,message);
  break;
 }
 return;
}
