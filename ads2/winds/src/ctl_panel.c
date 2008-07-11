/* ctl_panel.c  -- create and manage the operation panels associated with
   the control panel 
*/

#include <stdio.h>
#include <xview/xview.h>
#include <xview/panel.h>

/**************************  WINDS INCLUDES  *********************************/

#include <help.h>
#include <ctl_panel.h>
#include <display.h>
#include <tasks.h>
#include <errors.h>
#include <xview.h>
#include <lengths.h>
#include <printers.h>
#include <constants.h>
#include <socket.h>
#include <geom.h>
#include <shmem.h>
#include <ops.h>
#include <op_panels.h>
#include <network.h>
#include <panel_menus.h>
#include <colors.h>
#include <proto.h>


/************************  GLOBAL WITHIN MODULE  *****************************/

static Panel ControlPanel;
static Panel_item ControlButton[NUM_CTL_OPS];
static int ControlTextItem[NUM_CTL_OPS];
static int DisplayedParcel=SHOW_PARCEL1;
static int ParcelTrackingEnabled=FALSE;
static int UpdateInterval=1;
static int UpdateRate=REQ_SET_REALTIME;
static int UpdateRateButtonValue=RATE_REALTIME;
static int BroadCast=FALSE;
static int InputFrozen=FALSE;
static int VaropShowing=OP_CTL_DEFAULTS;
static long TimeToClearMessage;

/**********************  GET_BKGND_COLOR()  ******************************/

get_bkgnd_color()
{
 return (*GetColorMap[GetDrawType()])();
}

/**********************  SET_BKGND_COLOR()  ******************************/

set_bkgnd_color(color)
char *color;
{
int quadrant;

 (*SetColorMap[GetDrawType()])(color);
 (*SetupColors[GetDrawType()])();
 make_help();
 ShowTime();
 for (quadrant=0; quadrant<get_num_displays(); quadrant++) {
  if (refresh_window(INITIAL,quadrant))
   set_force_autoscale(quadrant,TRUE);
 }
}

/**********************  RECYCLE()  *********************************/

recycle()
/*
prototype func to change datasets in session, recycling state parms as needed
*/
{
 winds_cleanup(3);
}

/*******************  ShowFeedbackMessage()  *********************************/

static ShowFeedbackMessage(char msg[], int timeout)
/*
display message as indicated in control panel feedback area
*/
{
 XSetAttr(ControlButton[OP_CTL_THINKING],
	XV_SHOW,TRUE,
	PANEL_LABEL_STRING,msg,
 NULL);
 (*FlushDisplay[GetDrawType()])();
 SetTimeToClearMessage(time(NULL)+timeout);
}

/*****************  CheckForTimeToClearMessage()  **************************/

CheckForTimeToClearMessage() 
{
 if (time(NULL)>=TimeToClearMessage)
  ShowFeedbackMessage("", 1);
}

/******************  SetTimeToClearMessage()  ******************************/

SetTimeToClearMessage(seconds)
int seconds;
{
 TimeToClearMessage=seconds;
}

/**********************  ADSEVT_ITEM()  **************************************/

Panel_item
adsevt_item()
{
 return ControlButton[OP_CTL_EVENTS];
}

/***************************  INPUT_ACKNOWLEDGE()  ****************************/

input_acknowledge(notifyuser)
int notifyuser;
/* 
wait for input side acknowledgment on pending request
*/
{
int length,disp_reply;
char *s;
 length=MAX_SOCKET_BUFF_LNGTH;
 if (GetProtocolDebug())
  (void)fprintf(stderr,"WINDS PROTOCOL: waiting on input ack\n");
 s=(char *)read_from_socket(DISP_REQ_REPLY,&length,BLOCK_TIME_OUT);
 if (strcmp(s,EBADREAD)) {
  (void)sscanf(s,"%d",&disp_reply);
  if (disp_reply!=LISTENING) {
   if (GetProtocolDebug()) {
    (void)fprintf(stderr,"WINDS PROTOCOL: Unexpected response from WINPUT!\n");
   } else {
    time_to_log();
    (void)fprintf(stderr,
     "WINDS WARNING: Unexpected response from WINPUT process!\n");
   }
   return FALSE;
  } else {
   return TRUE;
  }
 } else {
  if (GetProtocolDebug()) {
   (void)fprintf(stderr,
    "WINDS PROTOCOL: NO response from WINPUT or BAD READ on socket!\n");
  } else {
   time_to_log();
   (void)fprintf(stderr,
    "WINDS WARNING: NO RESPONSE from WINPUT or BAD READ on socket!\n"); 
  }
  if (notifyuser)
   RequestAlertNow("WARNING: winput process has failed to respond in designated time-out period to a user request. This may indicate that is not running at all...contact a WINDS expert!");
  return FALSE; 
 }
}

/*******************  WAIT_FOR_SHMEM_REFILL()  *******************************/

wait_for_shmem_refill()
{
int length,disp_reply;
char *s;

 disp_reply=ERROR;
 length=MAX_SOCKET_BUFF_LNGTH;
 if (GetProtocolDebug())
  (void)fprintf(stderr,
   "WINDS PROTOCOL: waiting on input reply to shmem refilled...\n");
 s=(char *)read_from_socket(DISP_REQ_REPLY,&length,BLOCKING);
 if (strcmp(s,EBADREAD)) {
  (void)sscanf(s,"%d",&disp_reply);
  if (disp_reply==SHMEM_REFILLED) {
   int quadrant;
   if (GetProtocolDebug()) {
    (void)fprintf(stderr,"WINDS PROTOCOL: Shmem refilled OK\n");
   } else {
    time_to_log();
    (void)fprintf(stderr,"NOMINAL response from DISP_REQ_REPLY socket\n\n");
   }
   ShowFeedbackMessage("  Done.",5);
   for (quadrant=0; quadrant<get_num_displays(); quadrant++) {
    if (!frozen[quadrant] && 
     refresh_window(post_processing_mode()?INITIAL:START_NOW,quadrant))
     set_force_autoscale(quadrant,TRUE);
   }
   SetPrintMode(INITIAL);
   update_current_time=TRUE;
  } else if (disp_reply==NO_SHMEM_REFILLED) {
   if (GetProtocolDebug()) {
    (void)fprintf(stderr,"WINDS PROTOCOL: Shmem NOT refilled OK\n");
   } else {
    time_to_log();
    (void)fprintf(stderr,
    "WINDS: NO SHMEM REFILL response acknowledged from DISP_REQ_REPLY socket\n");
   }
   ShowFeedbackMessage("  Can't honor request...",5);
  } else {
   if (GetProtocolDebug()) {
    (void)fprintf(stderr,
     "WINDS PROTOCOL: Unexpected response on shmem-refilled.\n");
   } else {
    time_to_log();
    (void)fprintf(stderr,
     "WARNING: expecting SHMEM_REFILLED, got %s from DISP_REQ_REPLY\n\n",s);
   }
   ShowFeedbackMessage("  Protocol pblm...",5);
  }
 } else {
  if (GetProtocolDebug()) {
   (void)fprintf(stderr,"WINDS PROTOCOL: BAD READ on shmem refilled query.\n");
  } else {
   time_to_log();
   (void)fprintf(stderr,"WARNING: BAD READ from DISP_REQ_REPLY socket!\n\n");
  }
  RequestAlertNow("WARNING: winput process failed to indicate it has refilled shared memory properly. This may indicate that is not running at all...contact a WINDS expert!");
  ShowFeedbackMessage("  Comm'n pblm...",5);
 }
 return disp_reply;
}

/**************************  DISPLAY_REQUEST()  *******************************/

display_request(which,message)
int which;
char *message;
{
char buff[MAX_SOCKET_BUFF_LNGTH];

 (void)sprintf(buff,"%d",which);
 if (write_to_socket(DISP_REQ,&buff[0],strlen(&buff[0])) ==EBADWRITE) {
  perror(message);
 } else
  report(ERROR,message);
}

/***************************  SEND_INPUT_INFO()  ****************************/

send_input_info(which,value,message,waitforinput)
int which,value,waitforinput;
char *message;
/* 
request input process to adjust parameter which to value.  Send message to
standard error for feedback.  Wait for input process to respond, and if 
it does, wait also for shared memory to be refilled.
*/
{
 display_request(which,message);
 display_request(value,"send value for above op'n...");
/*
wait for input to acknowledge receipt of request
*/
 if (input_acknowledge(TRUE))
  {
  if (waitforinput)
/* 
wait for input process to respond that shared mem is adjusted 
*/
   return wait_for_shmem_refill();
  }
 else
  return NO_SHMEM_REFILLED;
}

/************************  GetParcelTrackingEnabled()  ***********************/

GetParcelTrackingEnabled()
{
 return ParcelTrackingEnabled || ParclDataToFlatPanel();
}

/************************  SetParcelTrackingEnabled()  ***********************/

SetParcelTrackingEnabled(enabled)
int enabled;
{
 ParcelTrackingEnabled=enabled;
}

/***************************  GET_CURRENT_PARCEL()  ****************************/

get_current_parcel()
{
 return DisplayedParcel;
}

/***************************  SET_CURRENT_PARCEL()  ****************************/

set_current_parcel(value)
int value;
{
 DisplayedParcel=value;
}

/***************************  SHOWCONTROLPANEl()  ***************************/

ShowControlPanel(which)
int which;
{
int op;

 switch (which) {
  case OPS_DISPLAYS:
   for (op=NUM_DISPLAY_OPS; op<NUM_CTL_OPS; op++) {
    if (ControlButton[op])
     {
     if (!ControlTextItem[op])
      XSetAttr(ControlButton[op],XV_SHOW,FALSE,NULL);
     else
      XSetAttr(ControlButton[op],XV_Y,CTL_VAROPS_Y+BUTTON_HEIGHT,NULL);
     }
   }
   show_ops(which);
   break;
  case OPS_CALIB:
   for (op=0; op<NUM_DISPLAY_OPS; op++) {
    if (ControlButton[op])
     {
     if (!ControlTextItem[op])
      XSetAttr(ControlButton[op],XV_SHOW,FALSE,NULL);
     else
      XSetAttr(ControlButton[op],XV_Y,CTL_VAROPS_Y+BUTTON_HEIGHT,NULL);
     }
   }
   show_ops(which);
   break;
 }
}

/**************************  SHOW_OPS()  ******************************/

show_ops(which)
int which;

/* re-display subset of ctl-ops, either Display or Calib
*/
{
int op;
 
 if (which==OPS_DISPLAYS) {
  for (op=0; op<NUM_DISPLAY_OPS; op++) {
   if (!ControlButton[op])
    continue;
   switch (op) {
    case OP_CTL_EVT:
     XSetAttr(ControlButton[OP_CTL_EVT],XV_SHOW,TRUE,NULL); 
     if (VaropShowing==OP_CTL_EVT)
      XSetAttr(ControlButton[OP_CTL_EVENTS],XV_SHOW,TRUE,NULL); 
     break;
    case OP_CTL_DEFAULTS:
     XSetAttr(ControlButton[OP_CTL_DEFAULTS],XV_SHOW,TRUE,NULL); 
     if (VaropShowing==OP_CTL_DEFAULTS) {
      if (ControlButton[OP_CTL_HEX])
       XSetAttr(ControlButton[OP_CTL_HEX],XV_SHOW,TRUE,NULL); 
      XSetAttr(ControlButton[OP_CTL_PRINTING],XV_SHOW,TRUE,NULL); 
      XSetAttr(ControlButton[OP_CTL_BKGND_COLOR],XV_SHOW,TRUE,NULL); 
      XSetAttr(ControlButton[OP_CTL_ATTRS],XV_SHOW,TRUE,NULL); 
     }
     break;
    case OP_CTL_CONFIG:
     XSetAttr(ControlButton[OP_CTL_CONFIG],XV_SHOW,TRUE,NULL); 
     if (VaropShowing==OP_CTL_CONFIG) {
      XSetAttr(ControlButton[OP_CTL_PRINTCONFIG],XV_SHOW,TRUE,NULL); 
      XSetAttr(ControlButton[OP_CTL_SETUPS],XV_SHOW,TRUE,NULL); 
      XSetAttr(ControlButton[OP_CTL_FOLDERS],XV_SHOW,TRUE,NULL); 
     }
     break;
    case OP_CTL_PARCELS:
     if (!post_processing_mode() && GetParcelTrackingEnabled()) {
      XSetAttr(ControlButton[OP_CTL_PARCELS],XV_SHOW,TRUE,NULL); 
      if (VaropShowing==OP_CTL_PARCELS) {
       XSetAttr(ControlButton[OP_CTL_INIT_PARCEL],XV_SHOW,TRUE,NULL); 
       if (ParclDataToFlatPanel())
        XSetAttr(ControlButton[OP_CTL_SHOW_PARCEL],XV_SHOW,TRUE,NULL); 
      }
     }
     break;
    case OP_CTL_ETC:
     XSetAttr(ControlButton[OP_CTL_ETC],XV_SHOW,TRUE,NULL); 
     if (VaropShowing==OP_CTL_ETC) {
      XSetAttr(ControlButton[OP_CTL_SUBSET],XV_SHOW,TRUE,NULL); 
      if (ControlButton[OP_CTL_GEOM])
       XSetAttr(ControlButton[OP_CTL_GEOM],XV_SHOW,TRUE,NULL);
      if (ControlButton[OP_CTL_MULTIPRINT])
       XSetAttr(ControlButton[OP_CTL_MULTIPRINT],XV_SHOW,TRUE,NULL);
      if (WindsControlHost) {
       XSetAttr(ControlButton[OP_CTL_INPUT],XV_SHOW,TRUE,NULL); 
       if (ControlButton[OP_CTL_INPUT_BCAST] && 
            (GetInputMode() == ADS_DATA 
          || GetInputMode() == DISK_DATA 
          || GetInputMode() == RAWFILE_NETCDF 
       ))
        XSetAttr(ControlButton[OP_CTL_INPUT_BCAST],XV_SHOW,TRUE,NULL); 
      }
     }
     break;
    case OP_CTL_SELECT_PRINTSETUP:
    case OP_CTL_SELECT_PRINTTIME:
    case OP_CTL_CANCEL_MULTIPRINT:
    case OP_CTL_EXECUTE_MULTIPRINT:
     if (VaropShowing==OP_CTL_MULTIPRINT) {
      XSetAttr(ControlButton[op],XV_SHOW,TRUE,NULL);
     }
    case OP_CTL_FRZ_INPUT:
    case OP_CTL_RESET_INPUT:
     if (WindsControlHost && VaropShowing==OP_CTL_INPUT) {
      XSetAttr(ControlButton[op],XV_SHOW,TRUE,NULL);
     }
     break;
    case OP_CTL_START_TIME:
     if (WindsControlHost && VaropShowing==OP_CTL_INPUT_FRAME) {
      XSetAttr(ControlButton[op],XV_Y,CTL_VAROPS_Y,NULL);
      XSetAttr(ControlPanel,PANEL_CARET_ITEM,ControlButton[OP_CTL_START_TIME],NULL); 
     }
     break;
    case OP_CTL_FRAME_JUMP:
     if (WindsControlHost && VaropShowing==OP_CTL_INPUT_FRAME)
      XSetAttr(ControlButton[op],XV_SHOW,TRUE,NULL);
     break;
    case OP_CTL_UPDATE_INTRVL:
     if (ControlButton[op]) {  /* depending on input mode, these might not exist */
      if (WindsControlHost && VaropShowing==OP_CTL_INPUT && 
         (post_processing_mode() || GetInputMode()==DISK_DATA)) {
       XSetAttr(ControlButton[op],XV_Y,CTL_VAROPS_Y,NULL);
       XSetAttr(ControlPanel,PANEL_CARET_ITEM,ControlButton[OP_CTL_UPDATE_INTRVL],NULL); 
      }
     }
     break;
    case OP_CTL_INPUT_FRAME:
    case OP_CTL_UPDATE_RATE:
     if (ControlButton[op]) {  /* depending on input mode, these might not exist */
      if (WindsControlHost && VaropShowing==OP_CTL_INPUT && 
         (post_processing_mode() 
       || GetInputMode()==DISK_DATA
       || GetInputMode()==RAWFILE_NETCDF
      ))
       XSetAttr(ControlButton[op],XV_SHOW,TRUE,NULL);
     }
     break;
    case OP_CTL_STATS:
     XSetAttr(ControlButton[OP_CTL_STATS],XV_SHOW,TRUE,NULL); 
     break;
    case OP_CTL_QUIT:
    case OP_CTL_CALIB:
     if (ControlButton[op])
      XSetAttr(ControlButton[op],XV_SHOW,TRUE,NULL); 
     break;
   }
  }
 } else {
  for (op=NUM_DISPLAY_OPS; op<NUM_CTL_OPS; op++) {
   if (!ControlButton[op])
    continue;
   XSetAttr(ControlButton[op],XV_SHOW,TRUE,NULL); 
  }
 }
}

/***************************  SET_ADSCMD_BUTTON()  ***************************/

set_adscmd_button(value)
int value;
{
 XSetAttr(ControlButton[OP_CTL_ADSCMD],PANEL_VALUE,value,NULL);
}

/***************************  SET_INTRVL_BUTTON()  ****************************/

set_intrvl_button()
{
char rate[VALUELEN];

 (void)sprintf(rate,"%d",get_update_interval());
 XSetAttr(ControlButton[OP_CTL_UPDATE_INTRVL],
        PANEL_VALUE,rate,
	NULL);
}

/*********************  SetControlButtonInactive()  ***************************/

SetControlButtonInactive(which,value)
int which,value;
{
 if (ControlButton[which])
  XSetAttr(ControlButton[which],PANEL_INACTIVE,value,NULL);
}

/************************  SetControlButtonValue()  ***************************/

SetControlButtonValue(which,value)
int which,value;
{
 if (ControlButton[which])
  XSetAttr(ControlButton[which],PANEL_VALUE,value,NULL);
}

/**************************  SET_FREEZE_BUTTON()  *****************************/

set_freeze_button(value)
int value;
{
 XSetAttr(ControlButton[OP_CTL_FRZ_INPUT],PANEL_VALUE,value,
	NULL);
 set_input_frozen(value);
}

/**************************  RESET_RATE_BUTTON()  ******************************/

reset_rate_button()
{
 if (!ControlButton[OP_CTL_UPDATE_RATE]) 
  return;
 XSetAttr(ControlButton[OP_CTL_UPDATE_RATE],PANEL_VALUE,RATE_REALTIME,NULL);
}

/***************************  SET_UPDATE_RATE()  ***************************/

set_update_rate(value)
int value;
{
 UpdateRate=value;
}

/**********************  GET_UPDATE_RATE_BUTTON_VALUE()  *********************/

get_update_rate_button_value()
{
 return UpdateRateButtonValue;
}

/***************************  GET_UPDATE_RATE()  ***************************/

get_update_rate()
{
 return UpdateRate;
}

/***************************  SET_UPDATE_INTRVL()  ***************************/

set_update_interval(value)
int value;
{
 UpdateInterval=value;
}

/***************************  GET_UPDATE_INTRVL()  ***************************/

get_update_interval()
{
 return UpdateInterval;
}

/**************************  SET_INPUT_FROZEN()  *******************************/

set_input_frozen(value)
int value;
{
 InputFrozen=value;
}

/**************************  GET_INPUT_FROZEN()  *******************************/

get_input_frozen()
{
 return InputFrozen;
}

/**************************  SET_BROADCAST()  ********************************/

set_broadcast(value)
int value;
{
 BroadCast=value;
}

/**************************  GET_BROADCAST()  ********************************/

get_broadcast()
{
 return BroadCast;
}

/***************************  CHG_UPDATE_INTRVL()  *************************/

/*ARGSUSED*/
Panel_setting
chg_update_interval(item,event)
Panel_item item;
Event *event;
{
char kbd_input[MAX_DESC_LNGTH];  /* user keyboard input value(s)  */
int temp;

  (void)strncpy(kbd_input,(char *) XGetAttr(item,PANEL_VALUE),MAX_DESC_LNGTH-1);
  if (!(strlen(kbd_input)))  {

/* null string: take no action
*/
   set_intrvl_button();
   return PANEL_NONE;
  } else {
   if (sscanf(kbd_input,"%d",&temp) != 1) {
    set_notice_needed(NON_NUMERIC_VALUE_ENTERED);
    set_intrvl_button();
    return(PANEL_NONE);
   }
  }
  if (!temp){
   set_notice_needed(ZERO_UNREASONABLE);
   set_intrvl_button();
  } else {
   if (temp != get_update_interval()) {

/* don't take any action if value entered is same as previous; this happens
   when cursor is in control panel and user simply hits a <r>
*/
    set_update_interval(temp);
    ShowFeedbackMessage("  Chg'g updates...",60);
    send_input_info(REQ_SET_UPDATES,get_update_interval(),"chg update interval",
     TRUE);
   }
  }
  return(PANEL_NONE);
}

/***************************  CTL_VAROP_PROC()  *******************************/
ctl_varop_proc(Panel_item item, int value, Event *event)
{
  int show;

 show = XGetAttr(item,PANEL_CLIENT_DATA);

 if (show != VaropShowing) {
   /* show is NOT the button currently hilited and whose sub-panels are being
    * shown.  Make adjustments; start by turning off the one that is hilited:
    */
   XSetAttr(ControlButton[VaropShowing],PANEL_VALUE,0,NULL);
   switch (VaropShowing) {
/*
next, hide any of its sub-panels
*/
   case OP_CTL_EVT:
    XSetAttr(ControlButton[OP_CTL_EVENTS],XV_SHOW,FALSE,NULL);
    break;
   case OP_CTL_STATS:
    break;
   case OP_CTL_DEFAULTS:
    if (ControlButton[OP_CTL_HEX])
     XSetAttr(ControlButton[OP_CTL_HEX],XV_SHOW,FALSE,NULL); 
    XSetAttr(ControlButton[OP_CTL_PRINTING],XV_SHOW,FALSE,NULL); 
    XSetAttr(ControlButton[OP_CTL_BKGND_COLOR],XV_SHOW,FALSE,NULL); 
    XSetAttr(ControlButton[OP_CTL_ATTRS],XV_SHOW,FALSE,NULL); 
    break;
   case OP_CTL_CONFIG:
    XSetAttr(ControlButton[OP_CTL_PRINTCONFIG],XV_SHOW,FALSE,NULL); 
    XSetAttr(ControlButton[OP_CTL_SETUPS],XV_SHOW,FALSE,NULL); 
    XSetAttr(ControlButton[OP_CTL_FOLDERS],XV_SHOW,FALSE,NULL); 
    break;
   case OP_CTL_PARCELS:
    XSetAttr(ControlButton[OP_CTL_INIT_PARCEL],XV_SHOW,FALSE,NULL); 
    if (ParclDataToFlatPanel())
     XSetAttr(ControlButton[OP_CTL_SHOW_PARCEL],XV_SHOW,FALSE,NULL); 
    break;
   case OP_CTL_ETC:
    XSetAttr(ControlButton[OP_CTL_SUBSET],XV_SHOW,FALSE,NULL); 
    if (ControlButton[OP_CTL_GEOM])
     XSetAttr(ControlButton[OP_CTL_GEOM],XV_SHOW,FALSE,NULL); 
    if (ControlButton[OP_CTL_MULTIPRINT])
     XSetAttr(ControlButton[OP_CTL_MULTIPRINT],XV_SHOW,FALSE,NULL); 
    if (WindsControlHost) {
     XSetAttr(ControlButton[OP_CTL_INPUT],XV_SHOW,FALSE,NULL); 
     if (ControlButton[OP_CTL_INPUT_BCAST] && 
         (GetInputMode() == ADS_DATA 
       || GetInputMode() == DISK_DATA 
       || GetInputMode() == RAWFILE_NETCDF 
     ))
      XSetAttr(ControlButton[OP_CTL_INPUT_BCAST],XV_SHOW,FALSE,NULL); 
    }
    break;
   case OP_CTL_INPUT_FRAME:
    if (GetInputMode()==RAWFILE_NETCDF || post_processing_mode()) {
     XSetAttr(ControlButton[OP_CTL_START_TIME],XV_Y,CTL_VAROPS_Y+BUTTON_HEIGHT,NULL); 
     XSetAttr(ControlButton[OP_CTL_FRAME_JUMP],XV_SHOW,FALSE,NULL); 
    }
    break;
   case OP_CTL_MULTIPRINT:
    XSetAttr(ControlButton[OP_CTL_SELECT_PRINTSETUP],XV_SHOW,FALSE,NULL); 
    XSetAttr(ControlButton[OP_CTL_SELECT_PRINTTIME],XV_SHOW,FALSE,NULL); 
    XSetAttr(ControlButton[OP_CTL_CANCEL_MULTIPRINT],XV_SHOW,FALSE,NULL); 
    XSetAttr(ControlButton[OP_CTL_EXECUTE_MULTIPRINT],XV_SHOW,FALSE,NULL); 
    break;
   case OP_CTL_INPUT:
    if (WindsControlHost) {
     XSetAttr(ControlButton[OP_CTL_FRZ_INPUT],XV_SHOW,FALSE,NULL); 
     XSetAttr(ControlButton[OP_CTL_RESET_INPUT],XV_SHOW,FALSE,NULL); 
     if (post_processing_mode() 
      || GetInputMode()==DISK_DATA
      || GetInputMode()==RAWFILE_NETCDF
     ) {
      XSetAttr(ControlButton[OP_CTL_UPDATE_RATE],XV_SHOW,FALSE,NULL); 
     }
     if (post_processing_mode() 
      || GetInputMode()==DISK_DATA
     ) {
      XSetAttr(ControlButton[OP_CTL_UPDATE_INTRVL],
       XV_Y,CTL_VAROPS_Y+BUTTON_HEIGHT,NULL); 
     }
     if (GetInputMode()==RAWFILE_NETCDF || post_processing_mode()) 
      XSetAttr(ControlButton[OP_CTL_INPUT_FRAME],XV_SHOW,FALSE,NULL); 
    }
    break;
  }


  switch (show) {
   case OP_CTL_EVT:
    XSetAttr(ControlButton[OP_CTL_EVENTS],XV_SHOW,TRUE,NULL);
    break;
   case OP_CTL_DEFAULTS:
    if (ControlButton[OP_CTL_HEX])
     XSetAttr(ControlButton[OP_CTL_HEX],XV_SHOW,TRUE,NULL); 
    XSetAttr(ControlButton[OP_CTL_PRINTING],XV_SHOW,TRUE,NULL); 
    XSetAttr(ControlButton[OP_CTL_BKGND_COLOR],XV_SHOW,TRUE,NULL); 
    XSetAttr(ControlButton[OP_CTL_ATTRS],XV_SHOW,TRUE,NULL); 
    break;
   case OP_CTL_CONFIG:
    XSetAttr(ControlButton[OP_CTL_CONFIG],XV_SHOW,TRUE,NULL); 
    XSetAttr(ControlButton[OP_CTL_PRINTCONFIG],XV_SHOW,TRUE,NULL); 
    XSetAttr(ControlButton[OP_CTL_SETUPS],XV_SHOW,TRUE,NULL); 
    XSetAttr(ControlButton[OP_CTL_FOLDERS],XV_SHOW,TRUE,NULL); 
    break;
   case OP_CTL_PARCELS:
    XSetAttr(ControlButton[OP_CTL_INIT_PARCEL],XV_SHOW,TRUE,NULL); 
    if (ParclDataToFlatPanel())
     XSetAttr(ControlButton[OP_CTL_SHOW_PARCEL],XV_SHOW,TRUE,NULL); 
    break;
   case OP_CTL_MULTIPRINT:
    XSetAttr(ControlButton[OP_CTL_SELECT_PRINTSETUP],XV_SHOW,TRUE,NULL); 
    XSetAttr(ControlButton[OP_CTL_SELECT_PRINTTIME],XV_SHOW,TRUE,NULL); 
    XSetAttr(ControlButton[OP_CTL_CANCEL_MULTIPRINT],XV_SHOW,TRUE,NULL); 
    XSetAttr(ControlButton[OP_CTL_EXECUTE_MULTIPRINT],XV_SHOW,TRUE,NULL); 
    break;
   case OP_CTL_ETC:
    XSetAttr(ControlButton[OP_CTL_SUBSET],XV_SHOW,TRUE,NULL); 
    if (ControlButton[OP_CTL_GEOM])
     XSetAttr(ControlButton[OP_CTL_GEOM],XV_SHOW,TRUE,NULL); 
    if (ControlButton[OP_CTL_MULTIPRINT])
     XSetAttr(ControlButton[OP_CTL_MULTIPRINT],XV_SHOW,TRUE,NULL); 
    if (WindsControlHost) {
     XSetAttr(ControlButton[OP_CTL_INPUT],XV_SHOW,TRUE,NULL); 
     if (ControlButton[OP_CTL_INPUT_BCAST] && 
         (GetInputMode() == ADS_DATA 
       || GetInputMode() == DISK_DATA 
       || GetInputMode() == RAWFILE_NETCDF 
     ))
      XSetAttr(ControlButton[OP_CTL_INPUT_BCAST],XV_SHOW,TRUE,NULL); 
    }
    break;
   case OP_CTL_INPUT_FRAME:
    if (GetInputMode()==RAWFILE_NETCDF || post_processing_mode()) {
     XSetAttr(ControlButton[OP_CTL_START_TIME],XV_Y,CTL_VAROPS_Y,NULL); 
     XSetAttr(ControlPanel,PANEL_CARET_ITEM,ControlButton[OP_CTL_START_TIME],NULL); 
     XSetAttr(ControlButton[OP_CTL_FRAME_JUMP],XV_SHOW,TRUE,NULL); 
    }
    break;
   case OP_CTL_INPUT:
    if (WindsControlHost) {
     XSetAttr(ControlButton[OP_CTL_FRZ_INPUT],XV_SHOW,TRUE,NULL); 
     XSetAttr(ControlButton[OP_CTL_RESET_INPUT],XV_SHOW,TRUE,NULL); 
     if (post_processing_mode() 
      || GetInputMode()==DISK_DATA
      || GetInputMode()==RAWFILE_NETCDF
      ) {
      XSetAttr(ControlButton[OP_CTL_UPDATE_RATE],XV_SHOW,TRUE,NULL); 
      XSetAttr(ControlPanel,PANEL_CARET_ITEM,ControlButton[OP_CTL_UPDATE_INTRVL],NULL); 
     }
     if (post_processing_mode() 
      || GetInputMode()==DISK_DATA
      ) {
      XSetAttr(ControlButton[OP_CTL_UPDATE_INTRVL],XV_Y,CTL_VAROPS_Y,NULL); 
     }
     if (GetInputMode()==RAWFILE_NETCDF || post_processing_mode()) 
      XSetAttr(ControlButton[OP_CTL_INPUT_FRAME],XV_SHOW,TRUE,NULL); 
    }
    break;
  }
  VaropShowing=show;
 } else
/*
show is the button currently hilited and whose sub-panels are being shown. 
Simply set its value.
*/
  XSetAttr(ControlButton[VaropShowing],PANEL_VALUE,1,NULL); 

}

/***************************  INIT_PARCEL()  *********************************/

/*ARGSUSED*/
init_parcel(item,value,event)
Panel_item item;
int value;
Event *event;
{

 display_request(RESET_PARCEL,"reset parcel");
 display_request(value,"send value for reset parcel"); 

 /* Only Control Host has the socket connection to ack input's response.
  */
 if (WindsControlHost && !input_acknowledge(TRUE))
  return;

}

/***************************  SHOW_PARCEL()  *********************************/

/*ARGSUSED*/
show_parcel(item,value,event)
Panel_item item;
int value;
Event *event;
{
 set_current_parcel(value); 
}

/***************************  CHG_UPDATE_RATE()  ******************************/

/*ARGSUSED*/
chg_update_rate(item,value)
Panel_item item;
int value;
{
 if (value==get_update_rate_button_value())
  return;

/*** we want to re-construct the choice strings here so that the fonts don't
get confused (somehow, they get larger, but I can't reproduce the problem at
will), but here is not the place; need to do it when the abbreviated menu
button is clicked, before displaying the choices. Defer until I know how to
do this (4/10/95).
 MakeUpdateRateChoices()
***/
 change_update_rate(value);
}

/***************************************************************************/

MakeUpdateRateChoices()
{
 ClearChoiceStrings(ControlButton[OP_CTL_UPDATE_RATE]);
 XSetAttr(ControlButton[OP_CTL_UPDATE_RATE],
  PANEL_CHOICE_STRINGS,"1x","2x","4x","10x",NULL,
  NULL);
}

/*************************  CHANGE_UPDATE_RATE()  ******************************/

change_update_rate(rate)
int rate;
{

 UpdateRateButtonValue=rate;
 switch (rate) {
  case RATE_REALTIME:
   set_update_rate(REQ_SET_REALTIME);
   break;
  case RATE_WARP2:
   set_update_rate(REQ_SET_WARP2);
   break;
  case RATE_WARP4:
   set_update_rate(REQ_SET_WARP4);
   break;
  case RATE_WARP10:
   set_update_rate(REQ_SET_WARP10);
   break;
 }
 
 send_input_info(REQ_SET_RATE,get_update_rate(),"chg update rate",FALSE);
}

/***************************  FrameForward()  *********************************/

FrameForward()
{
 return ChgTimeFrame(FRAME_FWD);
}

/***************************  CHG_SUBSET()  *********************************/

/*ARGSUSED*/
chg_subset(item,value,event)
Panel_item item;
int value;
Event *event;

/* cycle subsets of 4-at-at-time display setups. Manage it so that any given
   setup always appears in same quadrant, regardless of forward or backward
   cycling
*/
{
int num_displays;
 
 num_displays=get_num_displays();
/*
adjust current value of setup to start with, upper left quadrant
*/
 fill_displays(value==VAROP_FWD?num_displays:-num_displays);
 XSetAttr(ControlButton[OP_CTL_SUBSET],PANEL_VALUE,0,NULL);
}

/***************************  RESET_GEOM()  *********************************/

/*ARGSUSED*/
reset_geom(item,value,event)
Panel_item item;
int value;
Event *event;
{
int window;

 for (window=0; window<get_num_displays(); window++) {
  set_geom(window,get_default_geom());
  SetDisplayFont(window);
 }

 if (ControlButton[OP_CTL_GEOM])
  XSetAttr(ControlButton[OP_CTL_GEOM],PANEL_VALUE,0,NULL);
}

/***************************  INVERT_BKGND_COLOR()  **************************/

/*ARGSUSED*/
invert_bkgnd_color(item,value,event)
Panel_item item;
int value;
Event *event;
{
 switch (value) {
  case BKGND_BLUE:
   set_bkgnd_color(LTBLU_STRING);
   break;
  case BKGND_BLACK:
   set_bkgnd_color(BLACK_STRING);
   break;
 }
}

/***************************  SET_PRINTING_ATTRS()  **************************/

/*ARGSUSED*/
set_printing_attrs(item,value,event)
Panel_item item;
int value;
Event *event;
{
 if (!XGetAttr(ControlButton[OP_CTL_PRINTING],PANEL_VALUE)) {
  SetControlButtonValue(OP_CTL_PRINTING,TRUE);
  return;
 }
 show_panel_menu(PRINTINGMENU);
}

/***************************  ShowPrintConfigMenu()  **************************/

/*ARGSUSED*/
ShowPrintConfigMenu(item,value,event)
Panel_item item;
int value;
Event *event;
{
 if (!XGetAttr(ControlButton[OP_CTL_PRINTCONFIG],PANEL_VALUE)) {
/*
user has clicked on the button while it's menu was already showing -- simply
ignore redundant request
*/
  SetControlButtonValue(OP_CTL_PRINTCONFIG,TRUE);
  return;
 }
 show_panel_menu(PRINTCONFIGMENU);
}


/***************************  ShowFoldersMenu()  **************************/

/*ARGSUSED*/
ShowFoldersMenu(item,value,event)
Panel_item item;
int value;
Event *event;
{
 if (!XGetAttr(ControlButton[OP_CTL_FOLDERS],PANEL_VALUE)) {
/*
user has clicked on the button while it's menu was already showing -- simply
ignore redundant request
*/
  SetControlButtonValue(OP_CTL_FOLDERS,TRUE);
  return;
 }
 show_panel_menu(FOLDERSMENU);
}

/***************************  ShowPlotspexMenu()  **************************/

/*ARGSUSED*/
ShowPlotspexMenu(item,value,event)
Panel_item item;
int value;
Event *event;
{
 if (!XGetAttr(ControlButton[OP_CTL_SETUPS],PANEL_VALUE)) {
/*
user has clicked on the button while it's menu was already showing -- simply
ignore redundant request
*/
  SetControlButtonValue(OP_CTL_SETUPS,TRUE);
  return;
 }
 show_panel_menu(PLOTSPEXMENU);
}

/***************************  CTL_ATTRIBUTES()  **************************/

/*ARGSUSED*/
chg_attributes(item,value,event)
Panel_item item;
int value;
Event *event;
{
 if (!XGetAttr(ControlButton[OP_CTL_ATTRS],PANEL_VALUE)) {
  SetControlButtonValue(OP_CTL_ATTRS,TRUE);
  return;
 }
 ShowAttrsMenu(ATTRSMENU,event);
}

/***************************  CHG_NUMBER_BASE()  *******************************/

/*ARGSUSED*/
chg_number_base(item,value,event)
Panel_item item;
int value;
Event *event;
{
 if (value==VAROP_HEX)
  set_hex(TRUE);
 else
  set_hex(FALSE);
}

/*****************************************************************************/

ShowCalibDisplay()
{
  XSetAttr(ControlButton[OP_CTL_CALIB],PANEL_VALUE,0,NULL);
  if (!GetWINDSElapsedTime()) {
/*
don't start up calib yet...data has not yet started
*/
   RequestAlertNow("Sorry...data flow has not yet begun. Please try again after displays have initialized!");
   return;
  }
  report(TRUE,"startup calibration process");
  calib_running=TRUE;
  hide_quadrants();
  init_calib_parms();
  chg_plot_attrs(CALIBRATION);
  show_calib();
  chg_tb_help_image(CTL_AREA);
  ShowControlPanel(OPS_CALIB);
}

/*****************************************************************************/

ShowNormalDisplay()
{
  report(TRUE,"done with calibration process");
  calib_running=FALSE;
  if (GetCalibWindowsMade())
   HideCalibDisplay();
  chg_plot_attrs(TIME_SERIES);
  show_quadrants();
  chg_tb_help_image(CTL_AREA);
  ShowControlPanel(OPS_DISPLAYS);
}

/***************************  CALIB_WINDOW_PROC()  ***************************/

static calib_window_proc(int w, int v, int e)
{
 if (!calib_running) {
  ShowCalibDisplay();
 } else {
  ShowNormalDisplay();
 }
}

/*****************************************************************************/

ShowStatisticsMenu(item,value,event)
Panel_item item;
int value;
Event *event;
{
 SetStatsIntervalButtonValue(stats.interval);
 if (!value)  {
/*
stats menu is already up, but user clicked again on stats button (turning it
off). Turn it back on and be done with it.
*/
  SetStatsButtonValue(TRUE);
  return;
 }
 ShowStatsMenu(STATSMENU,event);
}

/**************************************************************************/

SetStatsButtonValue(value)
int value;
{
 SetControlButtonValue(OP_CTL_STATS,value);
}

/**************************************************************************/

/*ARGSUSED*/
Panel_setting
ChgStartTime(item,value,event)
Panel_item item;
int value;
Event *event;
{
char kbd_input[MAX_DESC_LNGTH];  /* user keyboard input value(s)  */
int new_start_hr,new_start_minute,new_start_sec;

 (void)strncpy(kbd_input,(char *) XGetAttr(item,PANEL_VALUE),MAX_DESC_LNGTH-1);
 if (!(strlen(kbd_input)))  {
/* 
null string: take no action
*/
  return PANEL_NONE;
 } else {
/*
assume zeroes for non-entered values
*/
  switch (sscanf(kbd_input,"%d %d %d",&new_start_hr,&new_start_minute,&new_start_sec)) {
   case 1:
    new_start_minute=0;
   case 2:
    new_start_sec=0;
    break;
  }
 }
 ShowFeedbackMessage("  Chg'g start time...",60);
 if (send_input_info(REQ_SET_START_TIME,
   (new_start_hr*3600+new_start_minute*60+new_start_sec)-
   (start_hour*3600+start_minute*60+start_second),
   "start time request",TRUE)==SHMEM_REFILLED) {
  SetNeedStatsInit(TRUE);
 }
 return PANEL_NONE;
}

/**************************************************************************/

/*ARGSUSED*/
FrameJump(item,value,event)
Panel_item item;
int value;
Event *event;
{
 ShowFeedbackMessage("  Chg'g start time...",60);

/* ask input process to reset time frame 
*/
 if (ChgTimeFrame(value)==NO_SHMEM_REFILLED)
  ShowFeedbackMessage(" Invalid start time?",5);
 else {
  SetNeedStatsInit(TRUE);
  ShowFeedbackMessage("  Done.",5);
 }

 XSetAttr(item,PANEL_VALUE,0,NULL);
}

/***************************  ChgTimeFrame()  ***************************/

ChgTimeFrame(value)
int value;
{
 display_request(value==FRAME_FWD?REQ_SEARCH_FWD:REQ_SEARCH_BACK,
  "frame jump request");

 if (input_acknowledge(TRUE)) {
/* 
wait for input process to respond that time frame change is complete
*/
  return wait_for_shmem_refill();
 }
 return NO_SHMEM_REFILLED;
}

/***************************  RESET_INPUT_PROC()  ***************************/

/*ARGSUSED*/
ResetInputProc()
{
  int quadrant;

  ShowFeedbackMessage("  Resetting input...",60);

  if (GetInputMode()==RAWFILE_NETCDF || post_processing_mode()) {
    report(TRUE,"reset input to start of file");

    /* ask input process to go back to start of file, reset date
     */
    send_input_info(REQ_SET_START_TIME,0,"start time request",TRUE);
    }
  else {
    /* kill current input process, if running; restart a new input process,
     * retaining broadcast status and update interval/rate;
     */
    report(TRUE,"restart new input");
    start_task(INPUT);

    if (GetInputMode()==DISK_DATA || GetInputMode()==RAWFILE_NETCDF)
      WaitOnDatasetFound();

    if (connected_to_ads) {
      start_task(WINDCOMM);
      } 

    /* retain broadcast status
     */
    if ( GetInputMode() == ADS_DATA || GetInputMode() == DISK_DATA  
	|| GetInputMode() == RAWFILE_NETCDF)
      broadcast_proc((Panel_item)NULL,get_broadcast());

      /* retain update interval; defaulted to 1
       */
      if (get_update_interval()!=1) {
     send_input_info(REQ_SET_UPDATES,get_update_interval(),"chg update interval",TRUE);
     }

    /* retain update rate; defaulted to REQ_SET_REALTIME
     */
    if (get_update_rate()!=REQ_SET_REALTIME) {
      change_update_rate(get_update_rate());
      }

    /* reset in DISK_DATA mode means restart disk file; ergo, reset date as
     * per start.
     */
    if (GetInputMode()==DISK_DATA || GetInputMode() == RAWFILE_NETCDF)
      set_date_string();

    /* re-init statistics times, etc.
     */
    ResetCurrentTime();

    /* unfreeze input 
     */
    set_freeze_button(FALSE);
    ShowFeedbackMessage("  Done.",5);
    }

  for (quadrant=0; quadrant<get_num_displays(); quadrant++) {
    if (!frozen[quadrant] && refresh_window( INITIAL, quadrant))
      set_force_autoscale(quadrant,TRUE);
    }

  SetNeedStatsInit(TRUE);
  XSetAttr(ControlButton[OP_CTL_RESET_INPUT],PANEL_VALUE,0,NULL);
  SetPrintMode(INITIAL);
  update_current_time=TRUE;
}

/*******************************  BROADCAST_PROC()  ****************************/

/*ARGSUSED*/
broadcast_proc(item,value)
Panel_item item;
int value;
{
char string[MAX_FLNM_LNGTH];

 if (WindsControlHost) {
  display_request(REQ_BCAST,string);
  display_request(value,"send value for above op'n");
  if (!input_acknowledge(TRUE))
   return;
  set_broadcast(value);
 }
}

/***************************  FREEZE_PROC()  ***************************/

/*ARGSUSED*/
freeze_proc(item,value,event)
Panel_item item;
int value;
Event *event;
{

 if (WindsControlHost) {
  if (!get_input_frozen()) {
   display_request(REQ_FREEZE,"freeze input request");
   set_input_frozen(TRUE);
  } else {
   display_request(REQ_UNFREEZE,"unfreeze input request");
   set_input_frozen(FALSE);
  }
  if (value!=get_input_frozen())
   set_freeze_button(get_input_frozen());

  if (!input_acknowledge(TRUE))
   return;
 } 
}

/***************************  QUIT_PROC()  ****************************/

/*ARGSUSED*/
quit_proc(item,value,event)
Panel_item item;
int value;
Event *event;
{
 report(TRUE,"request to quit WINDS display process");
 XSetAttr(ControlButton[OP_CTL_QUIT],PANEL_VALUE,0,NULL);
 winds_cleanup(OK);
}

/***************************  CALIBOPS_PROC()  ****************************/

calibops_proc(item,value,event)
Panel_item item;
int value;
Event *event;
{

if (XGetAttr(item,PANEL_CLIENT_DATA)==OP_CTL_CALIBOPS0) {
 switch (value) {
  case OP_CALIB_DISPLAYS:
   calib_window_proc(item,value,event);
   break;
  case OP_CALIB_SENSOR_IN:
   report(TRUE,"initiate sensor input");
   set_set_point(); /* just in case user has not hit <r> after entry */
   start_calib_curve();
   break;
  case OP_CALIB_REGRESS:
   regress_stats();
   break;
  case OP_CALIB_REDO:
   redo_last_entry();
   need_rehilite[STATS]=TRUE;
   break;
  case OP_CALIB_SCROLL_UP:
   if (scroll_up()) {
    need_rehilite[STATS]=TRUE;
   }
   break;
  case OP_CALIB_SCROLL_DOWN:
   (void)scroll_down();
   need_rehilite[STATS]=TRUE;
   break;
 }
} else {
 switch (value) {
  case OP_CALIB_PRINT_STATS:
   if (get_lstat_port()!=ERROR) {
    report(TRUE,"print calibration entry");
    print_entry();
   } else { 
    RequestAlertNow("WINDS Network Configuration has not indicated a printer is available to the Calibration windows...Contact a WINDS expert if this needs attention."); 
   }
   break;
  case OP_CALIB_PRINT_PLOT:
   if (get_lstat_port()!=ERROR) {
    report(TRUE,"calib plot snapshot");

/* disregard color vs. b/w setting; plot only b/w for calib plots, since only
   one trace per window is allowed anyway -- save color ink, technician time.
*/
    take_snapshot[PLOTS]=TRUE;
   } else { 
    RequestAlertNow("WINDS Network Configuration has not indicated a printer is available to the Calibration windows...Contact a WINDS expert if this needs attention."); 
   }
   break;
  case OP_CALIB_PRINT_HIST:
   if (get_lstat_port()!=ERROR) {
    report(TRUE,"print calib history");
    print_history(FALSE);
   } else {
    RequestAlertNow("WINDS Network Configuration has not indicated a printer is available to the Calibration windows...Contact a WINDS expert if this needs attention.");
   }
   break;
  case OP_CALIB_UPDATE_HIST:
   update_histfile();
   break;
  case OP_CALIB_QUIT:
   report(TRUE,"request to quit WINDS calibration process");
   winds_cleanup(OK);
   break;
 }
}
/* 
clear all hilited buttons
*/
XSetAttr(item,PANEL_VALUE,0,NULL);
}

static int MultiPrintTime;
static int MultiPrintSetup;

/***************************  SelectPrintSetup()  ****************************/

SelectPrintSetup(item,value,event)
Panel_item item;
int value;
Event *event;
{
 MultiPrintSetup=value;
}

/***************************  SelectPrintTime()  ****************************/

SelectPrintTime(item,value,event)
Panel_item item;
int value;
Event *event;
{
 MultiPrintTime=value;
}

/***************************  CancelMultiPrint()  ****************************/

CancelMultiPrint(item,value,event)
Panel_item item;
int value;
Event *event;
{
 TurnMultiPrintOff();
 XSetAttr(item,PANEL_VALUE,0,NULL);
}

/************************  ActivateControlPanel()  ****************************/

ActivateControlPanel(active)
int active;
/*
activate entire control panel if active is true, else inactivate entire 
control panel, making exceptions as noted
*/
{
int button;

 for (button=0; button<NUM_DISPLAY_OPS; button++)
  if (ControlButton[button] && XGetAttr(ControlButton[button],XV_SHOW))
/*
the use of XV_SHOW is useful in avoiding unnecessary work when active is FALSE,
since buttons not showing won't be able to be brought up anyway once the top
level buttons, which are the way to get to them, are made inactive. When active
is TRUE, all of them will be worked on. C'est la vie.
*/
   SetControlButtonInactive(button,!active);
/*
cancel button always opposite of others
*/
 SetControlButtonInactive(OP_CTL_CANCEL_MULTIPRINT,active);
/*
exceptions on return to active status
*/
 if (active) {
  if (!GetInputMode()==RAWFILE_NETCDF && !post_processing_mode()) {
   SetControlButtonInactive(OP_CTL_SELECT_PRINTTIME,TRUE);
  }
  SetControlButtonInactive(OP_CTL_ADSCMD,GetInputMode()==DISK_DATA);
  SetControlButtonInactive(OP_CTL_CALIB,TRUE);
  SetControlButtonInactive(OP_CTL_PRINTCONFIG,TRUE);
 }
}

/***************************  ExecuteMultiPrint()  ****************************/

ExecuteMultiPrint(item,value,event)
Panel_item item;
int value;
Event *event;
{
 ActivateOpPanels(FALSE);
 ActivateControlPanel(FALSE);
 MultiPrint(MultiPrintSetup,MultiPrintTime);
 XSetAttr(item,PANEL_VALUE,0,NULL);
}

/*****************************************************************************/

SetControlPanelDims()
{
  XSetAttr(ControlPanel,
         XV_X,GetTimeWindowWidth(),
  	 XV_Y,display_height,
         XV_HEIGHT,BUTTON_HEIGHT,
         XV_WIDTH, GetBaseFrameWidth()-
          GetTimeWindowWidth()
           -NUM_BUTTONS*BUTTON_WIDTH,
   	NULL);
}

/*****************************************************************************/

RefreshControlPanel()
{
  XSetAttr(ControlPanel,
   XV_SHOW,FALSE,
   NULL);
  XSetAttr(ControlPanel,
   XV_SHOW,TRUE,
   NULL);
}

/*****************************************************************************/

ResizeControlPanel()
{
 xv_destroy_safe(ControlPanel);
 MakeControlPanel();
}

/***************************  MakeControlPanel()  ****************************/

MakeControlPanel()
{
int op;

  ControlPanel=xv_create(GetBaseFrame(),PANEL,
         PANEL_BACKGROUND_PROC, panel_background_proc,
         PANEL_EVENT_PROC, panel_event_proc,
	 PANEL_REPAINT_PROC, panel_repaint,
         PANEL_ACCEPT_KEYSTROKE, TRUE,
	 XV_HELP_DATA,"WINDS:CTL_PANEL_HELP",
/*
we got a problem: here, though not with the calib-dialogue panel, using
XSetAttr to create and set a font results in the font not getting
set as requested. Turns out that doing it with xv_create sans XV_FONT and then
XSetAttr (...,XV_FONT,...) does same thing (here, anyway). So, we'll use
xv_create here until we figure out just how to get around this, since when
XV_FONT is set in the xv_create argument list, things comply as requested.
*/
         XV_FONT, GetControlPanelFont(),
         XV_X,GetTimeWindowWidth(),XV_Y,display_height,
         XV_HEIGHT,BUTTON_HEIGHT,
         XV_WIDTH, GetBaseFrameWidth()-
          GetTimeWindowWidth()-NUM_BUTTONS*BUTTON_WIDTH,
         PANEL_ITEM_X_GAP, CTL_ITEMS_XGAP,
	 NULL);
  paint_window[CTL_PANEL]=canvas_paint_window(ControlPanel);
  XSetAttr(paint_window[CTL_PANEL],
          PANEL_CLIENT_DATA,WTYPE_CONTROLPANEL,
#include <event_attr.h>
          WIN_EVENT_PROC, event_proc,
          WIN_IGNORE_EVENTS,WIN_TOP_KEYS,WIN_RIGHT_KEYS,WIN_LEFT_KEYS,NULL,
         NULL);
  for (op=0; op<NUM_CTL_OPS; op++)
   ControlTextItem[op]=FALSE;
  MakeControlButtons();
  VaropShowing=OP_CTL_DEFAULTS;
}

/****************************************************************************/

ToggleCalibAccess()
{
 if (!ControlButton[OP_CTL_CALIB])
  return;
 SetItemInactive(ControlButton[OP_CTL_CALIB],
  GetButtonIsActive(ControlButton[OP_CTL_CALIB]));
}

/****************************************************************************/

ToggleExpertAccess()
{
 if (!ControlButton[OP_CTL_PRINTCONFIG])
  return;
 SetItemInactive(ControlButton[OP_CTL_PRINTCONFIG],
  GetButtonIsActive(ControlButton[OP_CTL_PRINTCONFIG]));
}

/************************  MAKECONTROLBUTTONS()  ****************************/

MakeControlButtons()
{
/* 
Defaults
*/
   ControlButton[OP_CTL_DEFAULTS]=XSetAttr(ControlPanel,PANEL_TOGGLE,
        PANEL_VALUE,1,
        PANEL_CLIENT_DATA,OP_CTL_DEFAULTS,
        PANEL_CHOICE_STRINGS,"Defaults",NULL,
        PANEL_EVENT_PROC, panel_event_proc,
	XV_HELP_DATA,"WINDS:DEFAULTS_HELP",
        PANEL_NOTIFY_PROC,ctl_varop_proc,
	XV_Y,CTL_TOPOPS_Y,
        NULL);
  
   ControlButton[OP_CTL_CONFIG]=XSetAttr(ControlPanel,PANEL_TOGGLE,
        PANEL_VALUE,0,
        PANEL_CLIENT_DATA,OP_CTL_CONFIG,
        PANEL_CHOICE_STRINGS,"Config",NULL,
        PANEL_EVENT_PROC, panel_event_proc,
	XV_HELP_DATA,"WINDS:CONFIG_HELP",
        PANEL_NOTIFY_PROC,ctl_varop_proc,
	XV_Y,CTL_TOPOPS_Y,
        NULL);
/* 
Statistics
*/
 ControlButton[OP_CTL_STATS]=XSetAttr(ControlPanel,PANEL_TOGGLE,
        PANEL_VALUE,0,
        PANEL_CLIENT_DATA,OP_CTL_STATS,
        PANEL_CHOICE_STRINGS,"Stats",NULL,
        PANEL_EVENT_PROC, panel_event_proc,
        PANEL_NOTIFY_PROC,ShowStatisticsMenu,
	XV_Y,CTL_TOPOPS_Y,
        NULL);
/*  Calibration window
*/
 if (!post_processing_mode()) {
  ControlButton[OP_CTL_CALIB]=XSetAttr(ControlPanel,PANEL_TOGGLE,
        PANEL_VALUE,0,
        PANEL_CHOICE_STRINGS,"Calib",NULL,
        PANEL_EVENT_PROC, panel_event_proc,
        PANEL_NOTIFY_PROC,calib_window_proc,
	XV_Y,CTL_TOPOPS_Y,
        NULL);
  ToggleCalibAccess();

/* Parcel tracking, etc., to flat panel display
*/
  if (!post_processing_mode() && GetParcelTrackingEnabled()) 
   ControlButton[OP_CTL_PARCELS]=XSetAttr(ControlPanel,PANEL_TOGGLE,
        PANEL_CLIENT_DATA,OP_CTL_PARCELS,
	PANEL_CHOICE_STRINGS,"Prcls",NULL,
        PANEL_EVENT_PROC, panel_event_proc,
        PANEL_NOTIFY_PROC,ctl_varop_proc,
	XV_Y,CTL_TOPOPS_Y,
        NULL);

/* Events
*/
  ControlButton[OP_CTL_EVT]=XSetAttr(ControlPanel,PANEL_TOGGLE,
        PANEL_CLIENT_DATA,OP_CTL_EVT,
	PANEL_CHOICE_STRINGS,"Evts",NULL,
        PANEL_EVENT_PROC, panel_event_proc,
        PANEL_NOTIFY_PROC,ctl_varop_proc,
	PANEL_VALUE,0,
	XV_Y,CTL_TOPOPS_Y,
        NULL);
 }

/* Etc
*/
 ControlButton[OP_CTL_ETC]=XSetAttr(ControlPanel,PANEL_TOGGLE,
        PANEL_CLIENT_DATA,OP_CTL_ETC,
	PANEL_CHOICE_STRINGS,"Etc",NULL,
        PANEL_EVENT_PROC, panel_event_proc,
        PANEL_NOTIFY_PROC,ctl_varop_proc,
	XV_Y,CTL_TOPOPS_Y,
        NULL);

/* Quit control
*/
 ControlButton[OP_CTL_QUIT]=XSetAttr(ControlPanel,PANEL_TOGGLE,
        PANEL_VALUE,0,
        PANEL_CHOICE_STRINGS,"Quit",NULL,
        PANEL_EVENT_PROC, panel_event_proc,
        PANEL_NOTIFY_PROC,quit_proc,
	XV_Y,CTL_TOPOPS_Y,
        NULL);

/* Message for feedback during extended length operations
*/
 ControlButton[OP_CTL_THINKING]=XSetAttr(ControlPanel,PANEL_MESSAGE,
	XV_SHOW,FALSE,
	PANEL_LABEL_STRING,"  Thinking...",
	XV_Y,CTL_TOPOPS_Y,
	NULL);

/****************  1st Level Sub-panels  ******************/

/* 
Parcel tracking, etc., to flat panel display 
*/
 if (!post_processing_mode() && GetParcelTrackingEnabled()) {
  ControlButton[OP_CTL_INIT_PARCEL]=XSetAttr(ControlPanel,PANEL_CHOICE,
	XV_SHOW,FALSE,
        PANEL_VALUE,0,
        PANEL_LABEL_STRING,"Reset:",
 	PANEL_DISPLAY_LEVEL,PANEL_CURRENT,
 	PANEL_CHOICE_STRINGS,
/*
what we have here is a hard-wire, assuming # of sets of parcel tracking
parameters under each of these pre-determined methods. WINDS notifies winput 
process with value of item selected from Xview menu, the rest is up to winput.
Obviously WINDS and winput should agree on the ordering here and winput
should take appropriate action, including none, on receipt of request.
*/
 	 "Lagrang'n1","Lagrang'n2","Lagrang'n3",
 	 "DeadReckn1","DeadReckn2","DeadReckn3",
  	NULL,
        PANEL_EVENT_PROC, panel_event_proc,
	XV_X,CTL_VAROPS_X,
	XV_Y,CTL_VAROPS_Y,
        PANEL_NOTIFY_PROC,init_parcel,
        NULL);
  if (ParclDataToFlatPanel())
   ControlButton[OP_CTL_SHOW_PARCEL]=XSetAttr(ControlPanel,PANEL_CHOICE,
	XV_SHOW,FALSE,
        PANEL_VALUE,SHOW_PARCEL1,
        PANEL_LABEL_STRING,"Show Parcel:",
	PANEL_CHOICE_STRINGS,"1","2",NULL,
        PANEL_EVENT_PROC, panel_event_proc,
	XV_X,XGetAttr(ControlButton[OP_CTL_INIT_PARCEL],XV_X)+
             XGetAttr(ControlButton[OP_CTL_INIT_PARCEL],XV_WIDTH)+
             CTL_ITEMS_XGAP,
	XV_Y,CTL_VAROPS_Y,
        PANEL_NOTIFY_PROC,show_parcel,
        NULL);
 }

/* Events 
*/
 if (!post_processing_mode())
  ControlButton[OP_CTL_EVENTS]=XSetAttr(ControlPanel,PANEL_TOGGLE,
        PANEL_VALUE,0,
        PANEL_CHOOSE_NONE, TRUE,
        PANEL_LABEL_STRING,"Pick Event",
	PANEL_CHOICE_STRINGS,"1","2","3","4","5","6","7","8",NULL,
        PANEL_EVENT_PROC, panel_event_proc,
	XV_SHOW,FALSE,
	XV_X,CTL_VAROPS_X,
	XV_Y,CTL_VAROPS_Y,
        PANEL_NOTIFY_PROC,set_adsevt_proc,
        NULL);

/* Defaults 
*/
 ControlButton[OP_CTL_ATTRS]=XSetAttr(ControlPanel,PANEL_TOGGLE,
	XV_SHOW,TRUE,
        PANEL_VALUE,0,
        PANEL_CHOICE_STRINGS,"Attr's",NULL,
        PANEL_EVENT_PROC, panel_event_proc,
        PANEL_NOTIFY_PROC,chg_attributes,
	XV_X,CTL_VAROPS_X, 
	XV_Y,CTL_VAROPS_Y, 
        NULL);
 ControlButton[OP_CTL_PRINTING]=XSetAttr(ControlPanel,PANEL_TOGGLE,
	XV_SHOW,TRUE,
        PANEL_VALUE,0,
	PANEL_CHOICE_STRINGS,"Printing",NULL,
        PANEL_EVENT_PROC, panel_event_proc,
        PANEL_NOTIFY_PROC,set_printing_attrs,
	XV_X, XGetAttr(ControlButton[OP_CTL_ATTRS],XV_X)+
              XGetAttr(ControlButton[OP_CTL_ATTRS],XV_WIDTH)+
             CTL_ITEMS_XGAP,
	XV_Y,CTL_VAROPS_Y,
        NULL);

 ControlButton[OP_CTL_BKGND_COLOR]=XSetAttr(ControlPanel,PANEL_TOGGLE,
	XV_SHOW,TRUE,
        PANEL_LABEL_STRING,"Bkgnd:",
	PANEL_CHOICE_STRINGS,"Invrt",NULL,
        PANEL_EVENT_PROC, panel_event_proc,
	XV_X, XGetAttr(ControlButton[OP_CTL_PRINTING],XV_X)+
              XGetAttr(ControlButton[OP_CTL_PRINTING],XV_WIDTH)+
             CTL_ITEMS_XGAP,
	XV_Y,CTL_VAROPS_Y,
        PANEL_NOTIFY_PROC,invert_bkgnd_color,
        PANEL_VALUE,get_bkgnd_color(),
        NULL);
 
 if (!post_processing_mode())
  ControlButton[OP_CTL_HEX]=XSetAttr(ControlPanel,PANEL_CHOICE,
        PANEL_LABEL_STRING,"Cnts:",
        PANEL_CHOICE_STRINGS,"Hex","Octal",NULL,
        PANEL_EVENT_PROC, panel_event_proc,
        PANEL_NOTIFY_PROC,chg_number_base,
	XV_SHOW,TRUE,
	XV_X,XGetAttr(ControlButton[OP_CTL_BKGND_COLOR],XV_X)+
             XGetAttr(ControlButton[OP_CTL_BKGND_COLOR],XV_WIDTH)+
             CTL_ITEMS_XGAP,
	XV_Y,CTL_VAROPS_Y,
        NULL);
/*
Config
*/
 ControlButton[OP_CTL_PRINTCONFIG]=XSetAttr(ControlPanel,PANEL_TOGGLE,
	XV_SHOW,FALSE,
        PANEL_VALUE,0,
	PANEL_CHOICE_STRINGS,"Printers",NULL,
	XV_HELP_DATA,"WINDS:PRINTERS_HELP",
        PANEL_EVENT_PROC, panel_event_proc,
        PANEL_NOTIFY_PROC,ShowPrintConfigMenu,
	XV_X,CTL_VAROPS_X,
	XV_Y,CTL_VAROPS_Y,
        NULL);
 ControlButton[OP_CTL_SETUPS]=XSetAttr(ControlPanel,PANEL_TOGGLE,
	XV_SHOW,FALSE,
        PANEL_VALUE,0,
	PANEL_LABEL_STRING,"Folders:",
	PANEL_CHOICE_STRINGS,"Edit",NULL,
	XV_HELP_DATA,"WINDS:DISPSETUPS_HELP",
        PANEL_EVENT_PROC, panel_event_proc,
        PANEL_NOTIFY_PROC,ShowPlotspexMenu,
	XV_X,XGetAttr(ControlButton[OP_CTL_PRINTCONFIG],XV_X)+
             XGetAttr(ControlButton[OP_CTL_PRINTCONFIG],XV_WIDTH)+
             CTL_ITEMS_XGAP,
	XV_Y,CTL_VAROPS_Y,
        NULL);
 ControlButton[OP_CTL_FOLDERS]=XSetAttr(ControlPanel,PANEL_TOGGLE,
	XV_SHOW,FALSE,
        PANEL_VALUE,0,
	PANEL_CHOICE_STRINGS,"Create",NULL,
	XV_HELP_DATA,"WINDS:DISPFOLDERS_HELP",
        PANEL_EVENT_PROC, panel_event_proc,
        PANEL_NOTIFY_PROC,ShowFoldersMenu,
	XV_X,XGetAttr(ControlButton[OP_CTL_SETUPS],XV_X)+
             XGetAttr(ControlButton[OP_CTL_SETUPS],XV_WIDTH)+
             CTL_ITEMS_XGAP,
	XV_Y,CTL_VAROPS_Y,
        NULL);

/* Etc 
*/
 ControlButton[OP_CTL_SUBSET]=XSetAttr(ControlPanel,PANEL_TOGGLE,
        PANEL_VALUE,0,
        PANEL_LABEL_STRING,"Cycle:",
        PANEL_CHOICE_STRINGS,"Fwd","Back",NULL,
        PANEL_EVENT_PROC, panel_event_proc,
        PANEL_NOTIFY_PROC,chg_subset,
	XV_SHOW,FALSE,
	XV_X,CTL_VAROPS_X,
	XV_Y,CTL_VAROPS_Y, 
        NULL);
 if (WindsControlHost) {
  ControlButton[OP_CTL_INPUT]=XSetAttr(ControlPanel,PANEL_TOGGLE,
        PANEL_CLIENT_DATA,OP_CTL_INPUT,
        PANEL_VALUE,0,
        PANEL_CHOICE_STRINGS,"Input",NULL,
        PANEL_EVENT_PROC, panel_event_proc,
        PANEL_NOTIFY_PROC,ctl_varop_proc,
	XV_SHOW,FALSE,
	XV_Y,CTL_VAROPS_Y, 
        NULL);
/****************  2nd Level Sub-panels  ******************/

/* 
Input Controls
*/
  ControlButton[OP_CTL_FRZ_INPUT]=XSetAttr(ControlPanel,PANEL_TOGGLE,
        PANEL_VALUE,0,
        PANEL_CHOICE_STRINGS,"Frz",NULL,
        PANEL_EVENT_PROC, panel_event_proc,
        PANEL_NOTIFY_PROC,freeze_proc,
        XV_X,CTL_VAROPS_X,
	XV_Y,CTL_VAROPS_Y, 
	XV_SHOW,FALSE,
        NULL);
  XSetAttr(ControlButton[OP_CTL_FRZ_INPUT],
        PANEL_LABEL_STRING,"Input:",
 	NULL);
  ControlButton[OP_CTL_RESET_INPUT]=XSetAttr(ControlPanel,PANEL_TOGGLE,
        PANEL_VALUE,0,
        PANEL_CHOICE_STRINGS,"Reset",NULL,
        PANEL_EVENT_PROC, panel_event_proc,
        PANEL_NOTIFY_PROC,ResetInputProc,
	XV_SHOW,FALSE,
        XV_X,XGetAttr(ControlButton[OP_CTL_FRZ_INPUT],XV_X)+
             XGetAttr(ControlButton[OP_CTL_FRZ_INPUT],XV_WIDTH)+
	     CTL_ITEMS_XGAP,
	XV_Y,CTL_VAROPS_Y, 
        NULL);

  if ( post_processing_mode()  
    || GetInputMode() == DISK_DATA
    || GetInputMode() == RAWFILE_NETCDF
  ) {
/* 
then input is from disk file; offer input rate changes
*/
   ControlButton[OP_CTL_UPDATE_RATE]=XSetAttr(ControlPanel,PANEL_CHOICE,
        PANEL_VALUE,RATE_REALTIME,
    	PANEL_LABEL_STRING,"Rate:",
	PANEL_DISPLAY_LEVEL,PANEL_CURRENT,
        PANEL_EVENT_PROC, panel_event_proc,
        PANEL_NOTIFY_PROC,chg_update_rate,
	XV_SHOW,FALSE,
        XV_X,XGetAttr(ControlButton[OP_CTL_RESET_INPUT],XV_X)+
             XGetAttr(ControlButton[OP_CTL_RESET_INPUT],XV_WIDTH)+
	     CTL_ITEMS_XGAP,
	XV_Y,CTL_VAROPS_Y, 
        NULL);
   MakeUpdateRateChoices();
/*
disallow Update Rate changes in Real-time mode to ensure chk_ads_stuff() will
see change in value of *elapsed_time every second
*/
   ControlButton[OP_CTL_UPDATE_INTRVL]=XSetAttr(ControlPanel,PANEL_TEXT,
 	PANEL_NOTIFY_LEVEL, PANEL_SPECIFIED,
	PANEL_VALUE_DISPLAY_LENGTH, STATS_INTRVL_LNGTH,
	PANEL_VALUE_STORED_LENGTH, STATS_INTRVL_LNGTH,
        PANEL_NOTIFY_STRING,NOTIFY_STRING,
	PANEL_LABEL_STRING,"Updates:",
        PANEL_EVENT_PROC, panel_event_proc,
        PANEL_NOTIFY_PROC,chg_update_interval,
/*
if mode is disk-data or post_processing, position interval button to right 
of rate button; else, to right of reset input button
*/
        XV_X,XGetAttr(ControlButton[
         (ControlButton[OP_CTL_UPDATE_RATE]&& (GetInputMode()==DISK_DATA || 
					post_processing_mode()))?
           OP_CTL_UPDATE_RATE:OP_CTL_RESET_INPUT],XV_X)+
         XGetAttr(ControlButton[
         (ControlButton[OP_CTL_UPDATE_RATE]&& (GetInputMode()==DISK_DATA || 
					post_processing_mode()))?
           OP_CTL_UPDATE_RATE:OP_CTL_RESET_INPUT],XV_WIDTH)
	     +CTL_ITEMS_XGAP,
	XV_Y,CTL_VAROPS_Y+BUTTON_HEIGHT,
        NULL);
   ControlTextItem[OP_CTL_UPDATE_INTRVL]=TRUE;
   set_intrvl_button();
  }
  if (GetInputMode()==RAWFILE_NETCDF || post_processing_mode()) {
/* 
time frame changes
*/
   ControlButton[OP_CTL_INPUT_FRAME]=XSetAttr(ControlPanel,PANEL_TOGGLE,
        PANEL_VALUE,0,
        PANEL_CLIENT_DATA,OP_CTL_INPUT_FRAME,
        PANEL_CHOICE_STRINGS,"Frame",NULL,
        PANEL_EVENT_PROC, panel_event_proc,
        PANEL_NOTIFY_PROC,ctl_varop_proc,
	XV_SHOW,FALSE,
        XV_X,XGetAttr(ControlButton[OP_CTL_UPDATE_INTRVL],XV_X)+
             XGetAttr(ControlButton[OP_CTL_UPDATE_INTRVL],XV_WIDTH)+
	     CTL_ITEMS_XGAP,
	XV_Y,CTL_VAROPS_Y, 
        NULL);
   ControlButton[OP_CTL_FRAME_JUMP]=XSetAttr(ControlPanel,PANEL_TOGGLE,
        PANEL_VALUE,0,
    	PANEL_LABEL_STRING,"Frame:",
        PANEL_CHOICE_STRINGS,"Fwd","Back",NULL,
        PANEL_EVENT_PROC, panel_event_proc,
        PANEL_NOTIFY_PROC,FrameJump,
	XV_SHOW,FALSE,
        XV_X,CTL_VAROPS_X,
	XV_Y,CTL_VAROPS_Y, 
        NULL);
   ControlButton[OP_CTL_START_TIME]=XSetAttr(ControlPanel,PANEL_TEXT,
 	PANEL_NOTIFY_LEVEL, PANEL_SPECIFIED,
	PANEL_VALUE_DISPLAY_LENGTH, STATS_INTRVL_LNGTH*2,
	PANEL_VALUE_STORED_LENGTH, STATS_INTRVL_LNGTH*2,
        PANEL_NOTIFY_STRING,NOTIFY_STRING,
	PANEL_LABEL_STRING,"Start <hh [mm [ss]]>:",
        PANEL_EVENT_PROC, panel_event_proc,
        PANEL_NOTIFY_PROC,ChgStartTime,
        XV_X,XGetAttr(ControlButton[OP_CTL_FRAME_JUMP],XV_X)+
             XGetAttr(ControlButton[OP_CTL_FRAME_JUMP],XV_WIDTH)
	     +CTL_ITEMS_XGAP,
	XV_Y,CTL_VAROPS_Y+BUTTON_HEIGHT,
        NULL);
   ControlTextItem[OP_CTL_START_TIME]=TRUE;
   set_freeze_button(TRUE);
  }
 }
 ControlButton[OP_CTL_GEOM]=XSetAttr(ControlPanel,PANEL_TOGGLE,
        PANEL_VALUE,0,
        PANEL_CHOICE_STRINGS,"DefGeom",NULL,
        PANEL_EVENT_PROC, panel_event_proc,
        PANEL_NOTIFY_PROC,reset_geom,
	XV_SHOW,FALSE,
	XV_X,XGetAttr(ControlButton[OP_CTL_SUBSET],XV_X)+
             XGetAttr(ControlButton[OP_CTL_SUBSET],XV_WIDTH)+
	     CTL_ITEMS_XGAP,
	XV_Y,CTL_VAROPS_Y, 
        NULL);
/* 
Following label-string attaches to first "appropriate" button
...also set x-position, etc. based on type of display (PC or SUN)
*/
  XSetAttr(ControlButton[OP_CTL_GEOM],
        PANEL_LABEL_STRING,"Other:",
	NULL);
  if (WindsControlHost) {
   XSetAttr(ControlButton[OP_CTL_INPUT],
	XV_X,XGetAttr(ControlButton[OP_CTL_GEOM],XV_X)+
             XGetAttr(ControlButton[OP_CTL_GEOM],XV_WIDTH)+
             CTL_ITEMS_XGAP,
	NULL);
  }
/*
Multi-print control
*/
 ControlButton[OP_CTL_MULTIPRINT]=XSetAttr(ControlPanel,PANEL_TOGGLE,
        PANEL_CLIENT_DATA,OP_CTL_MULTIPRINT,
        PANEL_VALUE,0,
        PANEL_CHOICE_STRINGS,"MultiPrt",NULL,
        PANEL_EVENT_PROC, panel_event_proc,
        PANEL_NOTIFY_PROC,ctl_varop_proc,
	XV_SHOW,FALSE,
	XV_Y,CTL_VAROPS_Y, 
        NULL);
 {
/*
set x-position of button as function of which display size and whether
input control buttons is present
*/
 int last_item;
 if (WindsControlHost) 
  last_item=OP_CTL_INPUT;
 else
  last_item=OP_CTL_GEOM;
 XSetAttr(ControlButton[OP_CTL_MULTIPRINT],
	XV_X,XGetAttr(ControlButton[last_item],XV_X)+
             XGetAttr(ControlButton[last_item],XV_WIDTH)+
             CTL_ITEMS_XGAP,
	NULL);
 }
 ControlButton[OP_CTL_SELECT_PRINTSETUP]=XSetAttr(ControlPanel,PANEL_CHOICE,
        PANEL_VALUE,0,
        PANEL_CHOICE_STRINGS,
	 "Upper Left Setup", "Upper Right Setup", 
         "Lower Right Setup", "Lower Left Setup",
         "All Setups","Entire Folder",
	NULL,
	PANEL_DISPLAY_LEVEL,PANEL_CURRENT,
        PANEL_EVENT_PROC, panel_event_proc,
        PANEL_NOTIFY_PROC,SelectPrintSetup,
        XV_X,CTL_VAROPS_X,
	XV_Y,CTL_VAROPS_Y, 
	XV_SHOW,FALSE,
        NULL);
 ControlButton[OP_CTL_SELECT_PRINTTIME]=XSetAttr(ControlPanel,PANEL_CHOICE,
        PANEL_VALUE,0,
	PANEL_DISPLAY_LEVEL,PANEL_CURRENT,
        PANEL_EVENT_PROC, panel_event_proc,
        PANEL_NOTIFY_PROC,SelectPrintTime,
        XV_X,XGetAttr(ControlButton[OP_CTL_SELECT_PRINTSETUP],XV_X)+
             XGetAttr(ControlButton[OP_CTL_SELECT_PRINTSETUP],XV_WIDTH)+
	     CTL_ITEMS_XGAP,
	XV_Y,CTL_VAROPS_Y, 
	XV_SHOW,FALSE,
        NULL);
 if (post_processing_mode())
/*
time frames other than the current one are not printable except in 
post-processing mode
*/
  XSetAttr (ControlButton[OP_CTL_SELECT_PRINTTIME],
        PANEL_CHOICE_STRINGS,
         "Current Frame Only",
         "Current Frame To EOF",
         "All Frames",
	NULL,
  NULL);
 else
  XSetAttr (ControlButton[OP_CTL_SELECT_PRINTTIME],
        PANEL_CHOICE_STRINGS,
         "Current Frame Only",
	NULL,
  NULL);
 ControlButton[OP_CTL_CANCEL_MULTIPRINT]=XSetAttr(ControlPanel,PANEL_TOGGLE,
        PANEL_VALUE,0,
        PANEL_CHOICE_STRINGS,"Cancel",NULL,
        PANEL_EVENT_PROC, panel_event_proc,
        PANEL_NOTIFY_PROC,CancelMultiPrint,
        XV_X,XGetAttr(ControlButton[OP_CTL_SELECT_PRINTTIME],XV_X)+
             XGetAttr(ControlButton[OP_CTL_SELECT_PRINTTIME],XV_WIDTH)+
	     CTL_ITEMS_XGAP,
	XV_Y,CTL_VAROPS_Y, 
	XV_SHOW,FALSE,
        NULL);
 ControlButton[OP_CTL_EXECUTE_MULTIPRINT]=XSetAttr(ControlPanel,PANEL_TOGGLE,
        PANEL_VALUE,0,
        PANEL_CHOICE_STRINGS,"Print",NULL,
        PANEL_EVENT_PROC, panel_event_proc,
        PANEL_NOTIFY_PROC,ExecuteMultiPrint,
        XV_X,XGetAttr(ControlButton[OP_CTL_CANCEL_MULTIPRINT],XV_X)+
             XGetAttr(ControlButton[OP_CTL_CANCEL_MULTIPRINT],XV_WIDTH)+
	     CTL_ITEMS_XGAP,
	XV_Y,CTL_VAROPS_Y, 
	XV_SHOW,FALSE,
        NULL);
/*
broadcast data to network, toggle on or off.  Disallow on pc-sized display due
to lack of space in Control Panel.
*/
 if ( (GetInputMode() == ADS_DATA 
   || GetInputMode() == DISK_DATA
   || GetInputMode() == RAWFILE_NETCDF
   ) )
  ControlButton[OP_CTL_INPUT_BCAST]=XSetAttr(ControlPanel,PANEL_TOGGLE,
        PANEL_VALUE,0,
        PANEL_LABEL_STRING,"Net:",
        PANEL_CHOICE_STRINGS,"Bcast",NULL,
        PANEL_EVENT_PROC, panel_event_proc,
        PANEL_NOTIFY_PROC,broadcast_proc,
	XV_SHOW,FALSE,
        XV_X,XGetAttr(ControlButton[OP_CTL_MULTIPRINT],XV_X)+
             XGetAttr(ControlButton[OP_CTL_MULTIPRINT],XV_WIDTH)+
	     CTL_ITEMS_XGAP,
	XV_Y,CTL_VAROPS_Y, 
        NULL);
/* 
calib operations
*/

 if (!post_processing_mode()) {
  ControlButton[OP_CTL_CALIBOPS0]=XSetAttr(ControlPanel,PANEL_TOGGLE,
        PANEL_VALUE,0,
        PANEL_CHOICE_STRINGS,
         "Dspys", "Snsr In", "Regrss", "Redo",
         "Scroll Up", "Scroll Dn", 
        NULL,
        PANEL_EVENT_PROC, panel_event_proc,
        PANEL_NOTIFY_PROC,calibops_proc,
	PANEL_CLIENT_DATA,OP_CTL_CALIBOPS0,
	XV_X,CTL_TOPOPS_X,
	XV_Y,CTL_TOPOPS_Y,
	XV_SHOW,FALSE,
        NULL);
  ControlButton[OP_CTL_CALIBOPS1]=XSetAttr(ControlPanel,PANEL_TOGGLE,
        PANEL_VALUE,0,
        PANEL_CHOICE_STRINGS,
         "Prt Stats", "Prt Plot", "Prt Hist", "Update Hist", "Quit",
        NULL,
        PANEL_EVENT_PROC, panel_event_proc,
        PANEL_NOTIFY_PROC,calibops_proc,
	PANEL_CLIENT_DATA,OP_CTL_CALIBOPS1,
	XV_X,CTL_TOPOPS_X,
	XV_Y,CTL_VAROPS_Y,
	XV_SHOW,FALSE,
        NULL);
 }
 ActivateControlPanel(TRUE);
}
