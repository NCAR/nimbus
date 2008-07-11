/* errors.c -- respond to user input errors


Calling processes use the following:

 if process is NOT the WINDS display process: call NotifyWINDSProcess(message)
 if process IS the WINDS display process: 
  if it is safe to display message immediately, call RequestAlertNow(message)
  else, call RequestAlertWhenSafe(message); if using conditions as used in 
  function notify_user_error(condition), call set_notice_needed(condition).
  This will result in alert being shown at next time-out in catchalrm().
  Processes forked from WINDS should use LogMsgAndExit(desthost,label,message).

*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <xview/xview.h>
#include <xview/notice.h>

#include <errors.h>
#include <help.h>
#include <display.h>
#include <network.h>
#include <lengths.h>
#include <constants.h>
#include <proto.h>


/*********************  GLOBAL, DEFINED HERE ******************************/

char error_info[MAX_NOTICE_LINES*NOTICE_STR_LNGTH];  /* available for users to 
                      insert extra info into alert message */
int alert_up;			/* TRUE when alert is up to prevent confusion
				   on help images */
int exit_session=FALSE;		/* TRUE when WINDS session is to end */
int system_shutdown=FALSE;	/* TRUE when system is to be shutdown */
char errmsg[MAX_MSG_LNGTH];		/* utility string for writing 
					   error messages */

/*********************  GLOBAL WITHIN THIS FILE   *************************/

static int notice_needed=NONE;
static int verify_calib_quit=FALSE,	/* TRUE when CALIBRATION window is
                                           to end */
           shutdown_alert=FALSE,	/* TRUE when winds host requests an
					   end of session */
           yes_no=FALSE;		/* TRUE when a YES or NO response is
				 	   needed in alert */
static char yes_button[NOTICE_STR_LNGTH],  /* yes and no message buttons */
             no_button[NOTICE_STR_LNGTH];  

/*********************  NOTIFY_USER_ERROR()  *******************************/

notify_user_error(condition)
int condition;
{
static char alert_message[MAX_NOTICE_LINES*NOTICE_STR_LNGTH];

  if (condition==NONE)
   return;

/* notify user via an alert that he goofed.  
*/
  switch (condition) {
   case GENERAL_MESSAGE:
    RequestAlertNow(error_info);
    break;
   case DELETE_LAST_TRACE:
    RequestAlertNow("An attempt has been made to delete the only trace being displayed.  At least one trace must be retained in the display window.");
    break;
   case TRACE_NOT_DISPLAYED:
    RequestAlertNow("A request has been made to delete a trace that is not displayed in this window. No action will be taken.");
    break;
   case VARIABLE_NOT_AVAILABLE:
   RequestAlertNow("A request has been made for a variable that is either unknown or is not available for some reason, possibly due to a range of zero.  No action will be taken. If you requested this variable from a menu, it is likely that a zero range is the problem. Check with a WINDS expert to find out how to remedy this. If you requested the variable by typing in its name, it is possible you mispelled it. If this alert is appearing at WINDS startup time before you've had a chance to do anything at all, it is likely due to a zero range. Examining the Attributes Menu is one way to confirm the variable name's existence, spelling, and range.");
    break;
   case UNKNOWN_VARIABLE:
    (void)sprintf(alert_message,"The following variable was not found: %s; ergo, the input process is not guaranteed to work and is quitting.  You should contact a software expert and remedy the situation.",error_info);
    RequestAlertNow(alert_message);
    break;
   case ONLY_ONE_VALUE_ENTERED:
    RequestAlertNow("Less than the expected number of values were entered, or non_numeric data was entered where numeric was expected.  The values are both left unchanged.  Redo the operation if this is not what you intend.");
    break;
   case ONLY_TWO_VALUES_ENTERED:
    RequestAlertNow("Less than the expected three values were entered, or non_numeric data was entered where numeric was expected.  The values are left unchanged.  Please redo the operation.");
    break;
   case NON_NUMERIC_VALUE_ENTERED:
    RequestAlertNow("Non-numeric data was entered where numeric value was expected.  The value is left unchanged.  Redo the operation if this is not what you intend.");
    break;
   case ZERO_UNREASONABLE:
    RequestAlertNow("The input value of zero doesn't seem reasonable.  The parameter in question is left unchanged.  Redo the operation if this is not what you intend.");
    break;
   case LOG_LESS_OR_EQUAL_ZERO:
    RequestAlertNow("A logarithm value entered was less than or equal to zero; the value is changed to 1.  Redo the operation if this is not what you intend.");
    break;
   case NO_LISTEES:
    RequestAlertNow(" A list window has been requested but none of its variables were available. If possible, the previous setup will be restored. You are advised to execute the Re-init Folder operation in the Display Setups Menu.");
    break;
   case NO_FIXEES:
    RequestAlertNow(" A fixed format window has been requested but none of its variables were available. If possible, the previous setup will be restored. You are advised to execute the Re-init Folder operation in the Display Setups Menu.");
    break;
   case NO_TRACES: 
    RequestAlertNow(" A display window has been requested but none of its traces were displayable. If possible, the previous setup will be restored. You are advised to execute the Re-init Folder operation in the Display Setups Menu.");
    break;
   case LESS_OR_EQUAL_ZERO: 
    RequestAlertNow("Value entered was less than or equal to zero; no can do. The parameter in question is left unchanged.  Redo the operation if this is not what you intend.");
    break;
   case TIME_LONGER_THAN_MAXSEX: 
    RequestAlertNow(" REMINDER -- With this range on the time axis, any changes to the current ranges or variables, or any autoscaling events, will result in the loss of displayed data.");
    break;
   case NO_SHMEM:
    RequestAlertNow(" FATAL -- failure attaching to shared memory.  Contact a software expert.");
    break;
   case PROBE_DISABLED:
    RequestAlertNow(" The requested probe is not enabled in the ADS.");
    break;
   case MIN_GT_MAX:
    RequestAlertNow(" Maximum must be greater than minimum.  Please redo the operation.");
    break;
   case INVALID_DME_ENTRY:
    RequestAlertNow(" WARNING: A DME frequency entry is invalid.  Please try again.");
    break;
   case INVALID_TIME_ENTRY:
    RequestAlertNow(" WARNING: Date/time entry is invalid.  Please try again.");
    break;
   case INVALID_GPS_MAG_ENTRY:
    RequestAlertNow(" WARNING: GPS initialization entry is invalid.  Please try again.");
    break;
   case INVALID_DIG_OUT_ENTRY:
    RequestAlertNow(" WARNING: Digital output initialization entry is invalid.  Please try again.");
    break;
   case ADS_CHANNEL_DEAD:
    RequestAlertNow("WARNING: Failure sending command to the ADS. Try resending the command.");
    break;
   case ADS_ERROR:
    RequestAlertNow(error_info);
    break;
   case INVALID_ADS_MSG:
    RequestAlertNow("WARNING: Invalid message type received from ADS.");
    break;
   case TAS_WARNING:
    RequestAlertNow("WARNING: Manual true air speed is selected while aircraft is in motion.");
    break;
   case ADS_INIT_ERROR:
    RequestAlertNow("WARNING: Failure initializing ADS.  Check the ADS before dismissing this alert.");
    break;
   case NO_HDR_FILE:
    RequestAlertNow("FATAL: Failure opening tape header file.");
    break;
   case NO_READ_HDR:
    RequestAlertNow("FATAL: Failure reading tape header file.");
    break;
   case NO_START_FILE:
    RequestAlertNow("WARNING: Failure opening ADS setup file.");
    break;
   case SAVE_HISTORY:
    yes_no=TRUE;
    (void)sprintf(yes_button,"Ready To Quit");
    (void)sprintf(no_button,"Do Not Quit");
    verify_calib_quit=TRUE;
     RequestAlertNow("You have requested termination of this WINDS session with a calibration session in progress.  Is this really what you want?  All work performed since the last time you updated the history file will be lost.  To save your work, click LEFT over 'Do Not Quit', move cursor over the Control Panel and click RIGHT over Update Hist button; else, click LEFT over 'Ready To Quit' (DEFAULT action). If you wish to shutdown this workstation (and that option is available), then select 'Do Not Quit', return to regular WINDS display (Dspys button in Control Panel), and click LEFT on the Control Panel Quit button.");
    break;
   case SLAVE_QUIT:
   case CTL_QUIT:
    shutdown_alert=TRUE;
    if (!cursor_in_ctl_panel && !cursor_in_control_panel && calib_window_type(current_quadrant))  
     RequestAlertNow("You have requested termination of this WINDS calibration session.  Is this really what you want?  All work performed since the last time you updated the history file will be lost.  Dismiss this alert with LEFT or <r> over the appropriate choice (DEFAULT action is to QUIT).");
    (void)sprintf(error_info,"%02d:%02d:%02d to ",
     start_hour,start_minute,start_second);
    (void)sprintf(&error_info[strlen(error_info)],"%02d:%02d:%02d",
     end_hour,end_minute,end_second);
    if (condition==CTL_QUIT) {
     switch (GetInputMode()) {
      case INIT_SETUP:
      case NETCDF:
      case DISK_DATA:
      case RAWFILE_NETCDF:
       (void)sprintf(alert_message,"You have requested termination of this WINDS session.  Click LEFT over the appropriate button for action desired.  Current flight duration: %s.",error_info);
       break;
      case ADS_DATA:
       (void)sprintf(alert_message,"You have requested termination of this WINDS session.  If this is NOT what you want, click LEFT over the 'Continue Displays' item.  If you wish to exit this display or ALL current WINDS displays, you may click LEFT on the corresponding button. If you wish to shut down ALL workstations, but have not stopped recording and unloaded both tape drives, then (1) click LEFT over the 'Continue Displays' item; (2) execute prescribed sequence to stop recording and unload tape drive(s), whether via the ADS button or from discWin; and (3) return to this message to shutdown.  If recording is stopped and drives unloaded, then click LEFT over the appropriate button for action desired; if this is \"Shutdown\", then you may power down AFTER all display windows disappear and either the \"ok\" or \">\" prompt appears.  This may take a minute or so. Current flight duration: %s.",error_info);
       break;
     }
    } else {
     (void)sprintf(alert_message,"You have requested termination of this WINDS session.  If this is NOT what you want, click LEFT over the 'Continue Displays' item.  If you DO wish to exit, then click LEFT over the 'Exit THIS Display Only' button.  If the 'Shutdown THIS Workstation Only' button appears, you may instead click LEFT over it and shutdown this workstation (only). Current flight duration: %s.",error_info);
    }
    RequestAlertNow(alert_message);
    break;
   case NO_GSI_VME : 
     RequestAlertNow ("Wave_track program: Failure opening the VMEbus.");
     break;
   case NO_VGME512 :
      RequestAlertNow(
     "Wave_track program: Failure mapping in the VGME512 graphics card.");
     break;
   case TOO_MANY_SETUPS:
    RequestAlertNow("There are more Plotspex setups than allowed for use within one WINDS session.  One or more of these setups will not be available for display.  Contact a WINDS expert for further assistance.");
    break;
   case REBOOT_ADS:
    RequestAlertNow("WARNING: The communication between the ADS and WINDS is being re-initialized.  Please check if the ADS is running properly and recording as expected.  IF NECESSARY, reboot the ADS, and manually unload any tapes with recorded data BEFORE dismissing this alert. The ADS will be re-initialized to its start up operating state upon the dismissal of this alert.");
    break;
   case INVALID_DESCRIPTION:
    RequestAlertNow(error_info);
    break;
   case NO_PMS2D_DATA:
   case NO_PMS1D_DATA:
    RequestAlertNow(error_info);
    break;
   case TOO_MANY_PLOTATTRS:
    RequestAlertNow(error_info);
    break;
   case NEEDPLOTSPEXFOLDERNAME:
    RequestAlertNow("You must specify a Display Setups Folder Name before you can save any setups. Do by calling up the Display Setups Menu, via the Control Panel Defaults-Disp buttons.");
    break;
   case REMOVEPLOTSPEXFOLDER:
    yes_no=TRUE;
    (void)sprintf(yes_button,"Delete");
    (void)sprintf(no_button,"Cancel");
    (void)sprintf(error_info,"You have requested to delete the current Plot Setup Folder \"%s\". Please indicate delete (DEFAULT action) or cancel:",(char *)GetCurrentPlotspexFolderName());
    RequestAlertNow(error_info);
    break;
   case REMOVE_PLOTSPEX_ENTRY:
    yes_no=TRUE;
    (void)sprintf(yes_button,"Delete");
    (void)sprintf(no_button,"Cancel");
    (void)sprintf(error_info,"You have requested to delete the Plot Setup entitled \"%s\". Please indicate delete (DEFAULT action) or cancel:",(char *)GetPendingDescription());
    RequestAlertNow(error_info);
    break;
   case DUPLICATE_PLOTSPEX_ENTRY:
    yes_no=TRUE;
    (void)sprintf(yes_button,"OVERWRITE OLD ENTRY");
    (void)sprintf(no_button,"CHANGE NEW DESCRIPTION");
    RequestAlertNow("WARNING: The description entered matches an entry already saved in the plot setup file.  You may overwrite that entry with this setup (DEFAULT action) or re-enter a unique description to save this setup as a distinct entry.");
    break;
   case INVALID_PLOTATTR:
    RequestAlertNow(error_info);
    break;
   }
}

/**********************  NOTICE_REQUEST()  **********************************/
RequestAlertNow(char *string)
{
 int	j,len,num_strings,str_len,next_start;
 Rect	*rect;
 int	show_alert_x,show_alert_y;

 static char msg[NOTICE_STR_LNGTH*MAX_NOTICE_LINES];
 static char message[MAX_NOTICE_LINES][NOTICE_STR_LNGTH];
 static Attr_avlist attr_list=NULL;

#include <fmtmsg.h>

 time_to_log();
 if (!GetBaseFrame()) {
  (void)fprintf(stderr,
   "WINDS: can't show alert to user yet; message received:\n%s\n",string);
  LogMsgToConsole((char *)GetDisplayHost(),"WINDS:Alert",string);
  return;
 } else {
  (void)fprintf(stderr,"WINDS: Alert message received: \n%s\n",string);
 }


 if (!attr_list) {

/* set up quit alert button choices as function of input mode
*/
   switch (GetInputMode()) {
     case INIT_SETUP:
     case NETCDF:
      attr_list=attr_create_list(
		NOTICE_BUTTON,"Exit Display",EXIT_ALL,
         	NOTICE_BUTTON,"Continue Display",DO_NOT_EXIT,
      NULL);
      break;
     case DISK_DATA:
     case RAWFILE_NETCDF:
      attr_list=attr_create_list(
		NOTICE_BUTTON,"Exit ALL Displays",EXIT_ALL,
         	NOTICE_BUTTON,"Exit THIS Display Only",EXIT_HOST_ONLY,
         	NOTICE_BUTTON,"Continue Displays",DO_NOT_EXIT,
      NULL);
      break;
     case WINDS_SLAVE:
      if (IsWindsComputeHost(GetDisplayHost())) {
/*
offer shutdown individual workstation option on all except Control Host and
Compute Host. On former, shutdown option is for ALL.
*/
       attr_list=attr_create_list(
		NOTICE_BUTTON,"Exit THIS Display Only",EXIT_ALL,
         	NOTICE_BUTTON,"Continue Display",DO_NOT_EXIT,
      	NULL);
      } else {
       attr_list=attr_create_list(
		NOTICE_BUTTON,"Exit THIS Display Only",EXIT_ALL,
         	NOTICE_BUTTON,"Continue Display",DO_NOT_EXIT,
		NOTICE_BUTTON,"Shutdown THIS Workstation Only",SHUTDOWN,
      	NULL);
      }
      break;
     case ADS_DATA:
      attr_list=attr_create_list(
		NOTICE_BUTTON,"Exit ALL Displays",EXIT_ALL,
         	NOTICE_BUTTON,"Exit THIS Display Only",EXIT_HOST_ONLY,
         	NOTICE_BUTTON,"Continue Displays",DO_NOT_EXIT,
		NOTICE_BUTTON,"Shutdown ALL Workstations",SHUTDOWN,
      NULL);
      break;
   }
 }
/* determine current mouse position to determine both where to show alert and
   to facilitate resetting it after alert goes away
*/
 rect=(Rect *)XGetAttr(GetBaseFrame(),WIN_MOUSE_XY);
 show_alert_y=rect->r_top;
 if (rect->r_left<XGetAttr(GetBaseFrame(),XV_WIDTH)/2) 
/*
mouse is to left of midline; show alert at leftmost end
*/
  show_alert_x=0;
 else
/*
mouse is to right of midline; show alert at rightmost end
*/
  show_alert_x=XGetAttr(GetBaseFrame(),XV_WIDTH);

/* fill in the alert message */

  (void)strcpy(msg,string);
  len=strlen(msg);
  num_strings=0;
  next_start=0;
  for (j=0; j<MAX_NOTICE_LINES-2; j++) {
/*
save 2 lines for appended line feed and button prompter
*/
   if (len > next_start) {
    num_strings++;
    str_len=last_blank(&msg[next_start],NOTICE_STR_LNGTH-1);
    (void)strncpy(message[j],&msg[next_start],str_len+1);
    next_start += str_len+1;
    message[j][str_len]='\0';
   } else
    break;
  }
  if (len > next_start) {
   (void)sprintf(error_info,"WARNING!! Error message too long: %s",message[0]);
   report(current_quadrant,error_info);
  }

/* finish off the message with blank line, instructions, and NULL lines */

  (void)sprintf(message[num_strings++],
   "                ");
  (void)sprintf(message[num_strings++],
     (shutdown_alert? "      Click LEFT over desired action":
                      "  Click LEFT over button or enter <r> for DEFAULT action"));
  for (j=num_strings; j<MAX_NOTICE_LINES; j++) 
   (void)sprintf(message[j],"\0");

/* display appropriate alert
*/
  chg_tb_help_image(NOTICE_AREA);

/* postpone updates when an alert is up 
*/
  alert_up=TRUE;
  if (shutdown_alert) {
   switch (notice_prompt(GetBaseFrame(),(Event *)NULL,
	 ATTR_LIST,attr_list,
	 NOTICE_FONT, GetAlertFont(),
         NOTICE_FOCUS_XY, show_alert_x,show_alert_y,
         NOTICE_MESSAGE_STRINGS,
/* 
assuming MAX_NOTICE_LINES is defined as 18 
*/
          message[0], message[1], message[2], message[3], message[4],
          message[5], message[6], message[7], message[8], message[9],
          message[10],message[11],message[12],message[13],message[14],
          message[15], message[16], message[17],
          NULL,
	NULL)) {
    case EXIT_HOST_ONLY:
     exit_session=TRUE;
     break;
    case EXIT_ALL:
     exit_session=TRUE;
     WINDSCleanUp=TRUE;
     break;
    case SHUTDOWN:
     exit_session=TRUE;
     WINDSCleanUp=TRUE;
     system_shutdown=TRUE;
     break;
    case DO_NOT_EXIT:
     exit_session=FALSE;
     WINDSCleanUp=FALSE;
     system_shutdown=FALSE;
     break;
   }
   shutdown_alert=FALSE;
  } else if (yes_no) {
   if (notice_prompt(GetBaseFrame(),(Event *)NULL,
	 NOTICE_FONT, GetAlertFont(),
         NOTICE_FOCUS_XY, show_alert_x,show_alert_y,
         NOTICE_MESSAGE_STRINGS,
/* 
assuming MAX_NOTICE_LINES is defined as 18 
*/
          message[0], message[1], message[2], message[3], message[4],
          message[5], message[6], message[7], message[8], message[9],
          message[10],message[11],message[12],message[13],message[14],
          message[15], message[16], message[17],
          NULL,
         NOTICE_BUTTON_YES,yes_button,
         NOTICE_BUTTON_NO,no_button,
         NULL)==NOTICE_YES) {
    exit_session=TRUE;
    WINDSCleanUp=TRUE;
    overwrite=TRUE;
    if (verify_calib_quit) {
     verify_calib_quit=FALSE;
     print_history(FALSE);
    }
   } else {
    exit_session=FALSE;
    WINDSCleanUp=FALSE;
    overwrite=FALSE;
   }
  } else
  (void)notice_prompt(GetBaseFrame(),(Event *)NULL,
         NOTICE_FONT, GetAlertFont(),
         NOTICE_FOCUS_XY, show_alert_x,show_alert_y,
         NOTICE_MESSAGE_STRINGS,
/* 
assuming MAX_NOTICE_LINES is defined as 18 
*/
          message[0], message[1], message[2], message[3], message[4],
          message[5], message[6], message[7], message[8], message[9],
          message[10],message[11],message[12],message[13],message[14],
          message[15], message[16], message[17],
          NULL,
         NOTICE_BUTTON_YES,"ATTENTION!",
         NULL);
  
 yes_no=FALSE;
 alert_up=FALSE;
 (void)XSetAttr(GetBaseFrame(),WIN_MOUSE_XY,rect->r_left,rect->r_top,NULL);
 chg_tb_help_image(last_help_image());
 set_notice_needed(NONE);
 return;
}

/**************************  NotifyUserAndOrLog()  ****************************/

NotifyUserAndOrLog(message,exitstatus)
char *message;
int exitstatus;
{
 RequestAlertNow(message);
 if (GetBaseFrame()) {
/*
then RequestAlertNow() showed message to user but didn't write it to stderr. Do
so now.
*/
  (void)fprintf(stderr,"WINDS Alert: %s",message);
 }
 if (exitstatus) {
  winds_cleanup(ERROR);
 }
}

/**************************  SET_NOTICE_NEEDED()  ****************************/

set_notice_needed(which)
int which;
{
 notice_needed=which;
}

/**************************  GET_NOTICE_NEEDED()  ****************************/

get_notice_needed()
{
 return notice_needed;
}

/*****************************************************************************/

RequestAlertWhenSafe(msg)
char *msg;
{
 (void)sprintf(error_info,"%s",msg);
 set_notice_needed(GENERAL_MESSAGE);
}
