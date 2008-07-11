/* StatusWin.cc
   Creates and manages the project information window.

   Original Author: Jerry V. Pelk 
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <time.h>
#include <StatusWin.h>

static char *drvNames[] = { "Tape 0 :", "Disk 0 :", "Disk 1 :" };

#define UNLD_DRV0_BUTTON_VALUE  3               // unload drive 0 button value


/* -------------------------------------------------------------------- */
StatusWin::StatusWin(Widget parent, DsmMessage &tm,
	void (quitWrap)(Widget, XtPointer, XmPushButtonCallbackStruct*),
	void (analogWrap)(Widget, XtPointer, XmPushButtonCallbackStruct*),
	void (instrumentWrap)(Widget, XtPointer, XmPushButtonCallbackStruct*),
	void (recordWrap)(Widget, XtPointer, XmPushButtonCallbackStruct*)
	) : DsmConfig(), tape_msg(tm)
{
  Arg      args[16];
  Cardinal n;

  n = 0;
  mainForm = XmCreateForm(parent, "topLevelForm", NULL, 0);

  CreateFlightInfoWin();
  CreateTimeStampWin();
  CreateNetworkStatusWin();
  CreateRecordStatusWin();
  CreateMessageWin();
  CreateButtonWin();

  XtAddCallback(quitButton, XmNactivateCallback, 
                (XtCallbackProc)quitWrap, NULL);
  XtAddCallback(analogButton, XmNactivateCallback, 
                (XtCallbackProc)analogWrap, NULL);
  XtAddCallback(instrumentButton, XmNactivateCallback, 
                (XtCallbackProc)instrumentWrap, NULL);

  XtAddCallback(unld_drv_button[0], XmNactivateCallback, 
                (XtCallbackProc)recordWrap,
		(XtPointer)UNLD_DRV0_BUTTON_VALUE);

  XtManageChild(mainForm);

}

/* -------------------------------------------------------------------- */
void StatusWin::CreateFlightInfoWin()
{
  Arg		args[16];
  Cardinal	n;
  Widget	projTitle, projRC, projNumLabel, fltNumLabel;

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); ++n;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); ++n;
  projFrame = XmCreateFrame(mainForm, "projFrame", args, n);
  XtManageChild(projFrame);

  n = 0;
  projTitle = XmCreateLabel(projFrame, "projTitle", args, n);
  projRC = XmCreateRowColumn(projFrame, "projRC", args, n);

  XtManageChild(projTitle);
  XtManageChild(projRC);


// Create project number label and text field.
  n = 0;
  projNumLabel = XmCreateLabel(projRC, "proj_num_label", args, n);

  n = 0;
  projNumText = XmCreateTextField(projRC, "proj_num_text", args, n);

  XtManageChild(projNumLabel);
  XtManageChild(projNumText);


// Create the flight number label and text field.
  n = 0;
  fltNumLabel = XmCreateLabel(projRC, "flt_num_label", args, n);

  n = 0;
  fltNumText = XmCreateTextField(projRC, "flt_num_text", args, n);

  XtManageChild(fltNumLabel);
  XtManageChild(fltNumText);

}  /* END CREATEFLIGHTINFOWIN */

/* -------------------------------------------------------------------- */
void StatusWin::CreateTimeStampWin()
{
  Arg		args[16];
  Cardinal	n;
  Widget	tsTitle, tsRC, dateLabel, timeLabel;

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); ++n;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); ++n;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); ++n;
  XtSetArg(args[n], XmNleftWidget, projFrame); ++n;
  tsFrame = XmCreateFrame(mainForm, "tsFrame", args, n);
  XtManageChild(tsFrame);

  n = 0;
  tsTitle = XmCreateLabel(tsFrame, "tsTitle", args, n);
  tsRC = XmCreateRowColumn(tsFrame, "tsRC", args, n);

  XtManageChild(tsTitle);
  XtManageChild(tsRC);


// Create the date label and text field.
  n = 0;
  dateLabel = XmCreateLabel(tsRC, "dateLabel", args, n);

  n = 0;
  dateText = XmCreateTextField(tsRC, "dateText", args, n);

  XtManageChild(dateLabel);
  XtManageChild(dateText);


// Create the time label and text field.
  n = 0;
  timeLabel = XmCreateLabel(tsRC, "timeLabel", args, n);

  n = 0;
  timeText = XmCreateTextField(tsRC, "timeText", args, n);

  XtManageChild(timeLabel);
  XtManageChild(timeText);

}  /* END CREATETIMESTAMPWIN */

/* -------------------------------------------------------------------- */
void StatusWin::CreateNetworkStatusWin()
{
  Arg		args[16];
  Cardinal	n;
  int		stat, cnt;
  XmString	label;
  Widget	netTitle, netRC, netForm[MAX_DSM], netLabel[MAX_DSM];


  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); ++n;
  XtSetArg(args[n], XmNtopWidget, projFrame); ++n;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); ++n;
  netFrame = XmCreateFrame(mainForm, "netFrame", args, n);
 
  n = 0;
  netTitle = XmCreateLabel(netFrame, "netTitle", args, n);
  netRC = XmCreateRowColumn(netFrame, "netRC", args, n);
 
  XtManageChild(netFrame);
  XtManageChild(netTitle);
  XtManageChild(netRC);
 
// Read through the config file and create label and text widgets for each dsm.
// Attach the first two labels and text widgets to the top of the form.
  for (cnt = 0, stat = firstDsm(); stat; stat = nextDsm())
    {
    if (!strcmp (location(), CTL_LOCN_STR))
      continue;                         // no net for this ctl station
 
    n = 0;
    netForm[cnt] = XmCreateRowColumn(netRC, "netForm", args, n);
 
 
    sprintf (tstr, "%-8s at host: %-8s", location(), hostName());
    label = XmStringCreateSimple(tstr);
 
    n = 0;
    XtSetArg(args[n], XmNlabelString, label); ++n;
    netLabel[cnt] = XmCreateLabel(netForm[cnt], "netLabel", args, n);
    XmStringFree(label);
 
    n = 0;
    netText[cnt] = XmCreateTextField(netForm[cnt], "netText", args,n);
 
    XtManageChild(netText[cnt]);
    XtManageChild(netLabel[cnt]);
    ++cnt;
    }
 
  XtManageChildren(netForm, cnt);

}  /* END CREATENETWORKSTATUSWIN */

/* -------------------------------------------------------------------- */
void StatusWin::CreateRecordStatusWin()
{
  int		i;
  Arg		args[16];
  Cardinal	n;
  Widget	rcdTitle, rcdRC, drvRC[NUM_DRVS], drvLabel[NUM_DRVS];

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); ++n;
  XtSetArg(args[n], XmNtopWidget, tsFrame); ++n;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); ++n;
  XtSetArg(args[n], XmNleftWidget, netFrame); ++n;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); ++n;
  rcdFrame = XmCreateFrame(mainForm, "rcdFrame", args, n);
 
  n = 0;
  rcdTitle = XmCreateLabel(rcdFrame, "rcdTitle", args, n);
  rcdRC = XmCreateRowColumn(rcdFrame, "rcdRC", args, n);
  XtManageChild(rcdFrame);
  XtManageChild(rcdTitle);
  XtManageChild(rcdRC);
 
  n = 0;
  for (i = 0; i < NUM_DRVS; ++i)
    drvRC[i] = XmCreateRowColumn(rcdRC, "drvRC", args, n);
  XtManageChildren(drvRC, NUM_DRVS);
 
  n = 0;
  for (i = 0; i < NUM_DRVS; ++i)
    {
    drvLabel[i] = XmCreateLabel(drvRC[i], drvNames[i], args, n);
    XtManageChild(drvLabel[i]);
    }
 
  n = 0;
  for (i = 0; i < NUM_DRVS; ++i)
    {
    drvText[i] = XmCreateTextField(drvRC[i], "drvText", args, n);
    XtManageChild(drvText[i]);
    }

  n = 0;
  unld_drv_button[0] = XmCreatePushButton(drvRC[0], "drvUnload", args, n);
  XtManageChild(unld_drv_button[0]);

}  /* END CREATERECORDSTATUSWIN */

/* -------------------------------------------------------------------- */
void StatusWin::CreateMessageWin()
{
  Arg		args[16];
  Cardinal	n;

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); ++n;
  XtSetArg(args[n], XmNtopWidget, netFrame); ++n;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); ++n;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); ++n;
  msgFrame = XmCreateFrame(mainForm, "msgFrame", args, n);
  XtManageChild(msgFrame);

  n = 0;
  msgText = XmCreateScrolledText(msgFrame, "msgText", args, n);
  XtManageChild(msgText);

}  /* END CREATEMESSAGEWIN */

/* -------------------------------------------------------------------- */
void StatusWin::CreateButtonWin()
{
  Arg		args[16];
  Cardinal	n;
  Widget	buttonRC;

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); ++n;
  XtSetArg(args[n], XmNtopWidget, msgFrame); ++n;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); ++n;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); ++n;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); ++n;
  buttonFrame = XmCreateFrame(mainForm, "buttonFrame", args, n);
  XtManageChild(buttonFrame);
 
// Put a RowCol in the window.
  n = 0;
  buttonRC = XmCreateRowColumn(buttonFrame, "buttonRC", args, n);
  XtManageChild(buttonRC);
 
 
// Create the quit button.
  quitButton = XmCreatePushButton(buttonRC, "quitButton", args, n);
  analogButton = XmCreatePushButton(buttonRC, "analogButton", args, n);
  instrumentButton = XmCreatePushButton(buttonRC, "instrumentButton", args, n);
 
  XtManageChild(quitButton);
  XtManageChild(analogButton);
  XtManageChild(instrumentButton);

}  /* END CREATEBUTTONWIN */

/* -------------------------------------------------------------------- */
void StatusWin::displayTime (int year, int month, int day, 
                              int hour, int minute, int second)
{
  // Set and update the date and time text fields.
  updateDate(year, month, day);
  updateTime(hour, minute, second);
}
 
/* -------------------------------------------------------------------- */
void StatusWin::updateDate (int year, int month, int day)
{
  time_t	ct;
  struct tm	*cts;
/*
  ct = time(NULL);
  cts = gmtime(&ct);

  if (year != cts->tm_year || month != cts->tm_mon + 1 || day != cts->tm_mday) {
    year = cts->tm_year;
    month = cts->tm_mon+1;
    day = cts->tm_mday;
  }
*/
  // Display the date in the date text field.
//  sprintf(tstr, "%02d/%02d/%02d", month, day, year-100);
  sprintf(tstr, "%02d/%02d/%02d", month, day, year);
  XmTextFieldSetString(dateText, tstr);
}
 
/* -------------------------------------------------------------------- */
void StatusWin::updateTime (int hour, int minute, int second)
{
  // display the time in the time text field.
  sprintf (tstr, "%02d:%02d:%02d", hour, minute, second);
  XmTextFieldSetString(timeText, tstr);
}

/* -------------------------------------------------------------------- */
void StatusWin::displayNetState(char *net_name, int state)
{
  if (!selectByLocn(net_name)) {
    fprintf (stderr, "StatusWin: Unknown net name --> %s.\n", net_name);
    exit (ERROR);
  }
 
  switch (state) {
    case NET_STATUS:
      break;
 
    case NET_DISCONNECTED:
        XmTextFieldSetString(netText[index()], "DISCONNECTED");
      break;
 
    case NET_CONNECTED:
        XmTextFieldSetString(netText[index()], "CONNECTED");
      break;
 
    default:;
      fprintf (stderr, "StatusWin: Unknown state --> %d.\n", state);
      exit (ERROR);
  }

}

/* -------------------------------------------------------------------- */
void StatusWin::displayDSMTime(char *net_name, int hour, int minute, int second)
{
  int	stat, cnt;
  char	tstr[32];

  if (!selectByLocn(net_name)) {
    fprintf (stderr, "StatusWin: Unknown net name --> %s.\n", net_name);
    exit (ERROR);
  }

  sprintf(tstr, "%02d:%02d:%02d", hour, minute, second);
  XmTextFieldSetString(netText[index()], tstr);

}

/* -------------------------------------------------------------------- */
void StatusWin::displayDriveState (int drive, int state)
{
  Widget *text_w;
 
  text_w = &drvText[drive];
 
  switch (state) {
    case UNLOADED:
        XmTextFieldSetString (*text_w, "UNLOADED");
      break;
    case LOADING:
        XmTextFieldSetString (*text_w, "LOADING");
      break;
    case LOADED:
        XmTextFieldSetString (*text_w, "LOADED");
      break;
    case RECORDING:
        XmTextFieldSetString (*text_w, "RECORDING");
      break;
    case UNLOADING:
        XmTextFieldSetString (*text_w, "UNLOADING");
      break;
    case NOT_PRES:
        XmTextFieldSetString (*text_w, "NOT PRESENT");
      break;
    case FAILED:
        XmTextFieldSetString (*text_w, "FAILED");
      break;
    default:
      fprintf (stderr, "RecordWin: Unknown state = %d, received.\n", state);
  }
}

/* -------------------------------------------------------------------- */
void StatusWin::recordCB(Widget w, XtPointer client,
                              XmPushButtonCallbackStruct *cbs)
 
// Event call back procedure.
{

// Process the button selected.
  switch ((int)client) {
    case UNLD_DRV0_BUTTON_VALUE:
      tape_msg.sendTapeMsg(TAPE_UNLOAD, DRV_0, "");
      break;
    default:
      return;                           // unknown
  }
}

/* -------------------------------------------------------------------- */
void StatusWin::displayMsg(char *msg)
 
// Writes a message to the Text widget.
{
  int last_pos;
 
  XtPopup(XtParent(mainForm), XtGrabNone);
printf("Popup was performed [%s].\n", msg);
  last_pos = XmTextGetLastPosition(msgText);
  XmTextInsert(msgText, last_pos, msg);
  XmTextShowPosition(msgText, last_pos);
}

/* -------------------------------------------------------------------- */
void StatusWin::Quit(Widget w, XtPointer clientData, XtPointer callData)
{
  exit(0);
}

/* END STATUSWIN.CC */
