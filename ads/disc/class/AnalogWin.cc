/* AnalogWin.cc
   Creates and manages the analog control window.

   Original Author: Jerry V. Pelk 
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <AnalogWin.h>


/* -------------------------------------------------------------------- */
AnalogWin::AnalogWin (Widget parent, DsmMessage &dm, DsmHeaders &dh,
     void (*toggleButtonCallbackWrap)(Widget, XtPointer, 
                                      XmToggleButtonCallbackStruct*),
     void (*pushButtonCallbackWrap)(Widget, XtPointer, 
                                    XmPushButtonCallbackStruct*),
     void (*calCallbackWrap)(Widget, XtPointer, XtPointer)) :
     DsmConfig(), dsm_msg (dm), dsm_headers (dh)
{
  Widget	widget;
  XmString	string1, string2;
  Arg	args[5];
  char	str[6];
  int	stat;
  int	j, n;

// Create the window frame and form.
  n = 0;
  mainForm = XmCreateForm(parent, "analogForm", args, n);

  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); ++n;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); ++n;
  modeFrame = XmCreateFrame(mainForm, "modeFrame", args, n);

  n = 0;
  modeTitle = XmCreateLabel(modeFrame, "modeTitle", args, n);

  XtManageChild(modeFrame);
  XtManageChild(modeTitle);


// Create the analog control radio box.
  n = 0;
  modeRadioBox = XmCreateRadioBox(modeFrame, "modeRadioBox", NULL,0);
  XtManageChild(modeRadioBox);

  runButton	= XmCreateToggleButton(modeRadioBox, "runButt", args, n);
  calButton	= XmCreateToggleButton(modeRadioBox, "calButt", args, n);
  qcButton	= XmCreateToggleButton(modeRadioBox, "qcButt", args, n);
  diagButton	= XmCreateToggleButton(modeRadioBox, "diagButt", args, n);
  XtManageChild(runButton);
  XtManageChild(calButton);
  XtManageChild(qcButton);
  XtManageChild(diagButton);

  XmToggleButtonSetState(runButton, TRUE, FALSE);

 
  XtAddCallback(runButton, XmNvalueChangedCallback, 
                 (XtCallbackProc)toggleButtonCallbackWrap,
                 (XtPointer)ANALOG_RUN_MODE);

  XtAddCallback(calButton, XmNvalueChangedCallback, 
                 (XtCallbackProc)toggleButtonCallbackWrap,
                 (XtPointer)ANALOG_CAL_MODE);
 
  XtAddCallback(qcButton, XmNvalueChangedCallback, 
                 (XtCallbackProc)toggleButtonCallbackWrap,
                 (XtPointer)ANALOG_QC_MODE);
 
  XtAddCallback(diagButton, XmNvalueChangedCallback, 
                 (XtCallbackProc)toggleButtonCallbackWrap,
                 (XtPointer)ANALOG_DIAG_MODE);
 


// Create the diagnostics parameters frame and form within the radio box.
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); ++n;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); ++n;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); ++n;
  XtSetArg(args[n], XmNleftWidget, modeFrame); ++n;
  diagFrame = XmCreateFrame(mainForm, "diagFrame", args, n);

  n = 0;
  diagTitle = XmCreateLabel(diagFrame, "diagTitle", args, n);

  n = 0;
  diagRC = XmCreateRowColumn(diagFrame, "diagRC", args, n);

  XtManageChild(diagFrame);
  XtManageChild(diagTitle);
  XtManageChild(diagRC);


// Create the diagnostics dsm selection pull down menu.
  dsmMenu = XmCreatePulldownMenu(diagRC, "dsmMenu", NULL, 0);

  n = 0;
  XtSetArg(args[n], XmNsubMenuId, dsmMenu); ++n;
  dsmCascade = XmCreateOptionMenu(diagRC, "dsmCascade", args, n);
 
// The DSM button value is the index from the DsmConfig class.
  for (j = firstDsm(), diagLocn[0] = '\0'; j; j = nextDsm()) {
    if (isDsmLocation()) {
      n = 0;
      widget = XmCreatePushButton(dsmMenu, location(), args, n);
      XtManageChild(widget);
      XtAddCallback(widget, XmNactivateCallback, 
                 (XtCallbackProc)pushButtonCallbackWrap,
                     (XtPointer)(ANALOG_DSM_BUTTON_BASE + index()));

      if (!strlen(diagLocn))
        strcpy(diagLocn, location());	// init diag location
    }
  }
                                    

// Create the diagnostics channel selection pull down menu.
  n = 0;
  XtSetArg(args[n], XmNpacking, XmPACK_COLUMN); ++n;
  XtSetArg(args[n], XmNnumColumns, 4); ++n;
  chanMenu = XmCreatePulldownMenu(diagRC, "chanMenu", args, n);

  n = 0;
  XtSetArg(args[n], XmNsubMenuId, chanMenu); ++n;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); ++n;
  XtSetArg(args[n], XmNleftWidget, dsmCascade); ++n;
  chanCascade = XmCreateOptionMenu(diagRC, "chanCascade", args, n);

// The channel button value is the channel number.
  for (j = 0; j < 64; j++) {
    sprintf(str, "%1x", j);
    n = 0;
    widget = XmCreatePushButton(chanMenu, str, args, n);
    XtManageChild(widget);
    XtAddCallback(widget, XmNactivateCallback, 
                 (XtCallbackProc)pushButtonCallbackWrap,
                   (XtPointer)(ANALOG_CHAN_BUTTON_BASE + j));
  }


// Create the diagnostics voltage selection pull down menu.
  voltMenu = XmCreatePulldownMenu (diagRC, "voltMenu", NULL, 0);

  n = 0;
  XtSetArg(args[n], XmNsubMenuId, voltMenu); ++n;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); ++n;
  XtSetArg(args[n], XmNleftWidget, chanCascade); ++n;
  voltCascade = XmCreateOptionMenu(diagRC, "voltCascade", args, n);
 
// Other callback button values are pulled from dsmMessageDefs.h.
  widget = XtVaCreateManagedWidget ("+10", xmPushButtonWidgetClass, 
                                    voltMenu, NULL);
  XtAddCallback (widget, XmNactivateCallback, 
                 (XtCallbackProc)pushButtonCallbackWrap,
                 (XtPointer)(ANALOG_VOLT_BUTTON_BASE + ANALOG_V10_SELECT));
  widget = XtVaCreateManagedWidget ("+5", xmPushButtonWidgetClass, 
                                    voltMenu, NULL);
  XtAddCallback (widget, XmNactivateCallback, 
                 (XtCallbackProc)pushButtonCallbackWrap,
                 (XtPointer)(ANALOG_VOLT_BUTTON_BASE + ANALOG_V5_SELECT));
  widget = XtVaCreateManagedWidget ("+2.5", xmPushButtonWidgetClass, 
                                    voltMenu, NULL);
  XtAddCallback (widget, XmNactivateCallback, 
                 (XtCallbackProc)pushButtonCallbackWrap,
                 (XtPointer)(ANALOG_VOLT_BUTTON_BASE + ANALOG_V2D5_SELECT));
  widget = XtVaCreateManagedWidget ("+1", xmPushButtonWidgetClass, 
                                    voltMenu, NULL);
  XtAddCallback (widget, XmNactivateCallback, 
                 (XtCallbackProc)pushButtonCallbackWrap,
                 (XtPointer)(ANALOG_VOLT_BUTTON_BASE + ANALOG_V1_SELECT));
  widget = XtVaCreateManagedWidget ("0", xmPushButtonWidgetClass, 
                                    voltMenu, NULL);
  XtAddCallback (widget, XmNactivateCallback, 
                 (XtCallbackProc)pushButtonCallbackWrap,
                 (XtPointer)(ANALOG_VOLT_BUTTON_BASE + ANALOG_V0_SELECT));
  widget = XtVaCreateManagedWidget ("-10", xmPushButtonWidgetClass, 
                                    voltMenu, NULL);
  XtAddCallback (widget, XmNactivateCallback, 
                 (XtCallbackProc)pushButtonCallbackWrap,
                 (XtPointer)(ANALOG_VOLT_BUTTON_BASE + ANALOG_V10N_SELECT));
                                    

// Create the diagnostics gain selection pull down menu.
  gainMenu = XmCreatePulldownMenu (diagRC, "gainMenu", NULL, 0);

  n = 0;
  XtSetArg(args[n], XmNsubMenuId, gainMenu); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, voltCascade); n++;
  gainCascade = XmCreateOptionMenu(diagRC, "gainCascade", args, n);
 
  widget = XtVaCreateManagedWidget ("1", xmPushButtonWidgetClass, 
                                    gainMenu, NULL);
  XtAddCallback (widget, XmNactivateCallback, 
                 (XtCallbackProc)pushButtonCallbackWrap,
                 (XtPointer)(ANALOG_GAIN_BUTTON_BASE + ANALOG_GAIN_1_SELECT));
  widget = XtVaCreateManagedWidget ("2", xmPushButtonWidgetClass, 
                                    gainMenu, NULL);
  XtAddCallback (widget, XmNactivateCallback, 
                 (XtCallbackProc)pushButtonCallbackWrap,
                 (XtPointer)(ANALOG_GAIN_BUTTON_BASE + ANALOG_GAIN_2_SELECT));
  widget = XtVaCreateManagedWidget ("4", xmPushButtonWidgetClass, 
                                    gainMenu, NULL);
  XtAddCallback (widget, XmNactivateCallback, 
                 (XtCallbackProc)pushButtonCallbackWrap,
                 (XtPointer)(ANALOG_GAIN_BUTTON_BASE + ANALOG_GAIN_4_SELECT));
  widget = XtVaCreateManagedWidget ("20", xmPushButtonWidgetClass, 
                                    gainMenu, NULL);
  XtAddCallback (widget, XmNactivateCallback, 
                 (XtCallbackProc)pushButtonCallbackWrap,
                 (XtPointer)(ANALOG_GAIN_BUTTON_BASE + ANALOG_GAIN_20_SELECT));
                              

// Create the diagnostics offset selection pull down menu.
  offsetMenu = XmCreatePulldownMenu (diagRC, "offsetMenu", NULL, 0);

  n = 0;
  XtSetArg(args[n], XmNsubMenuId, offsetMenu); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, gainCascade); n++;
  offsetCascade = XmCreateOptionMenu(diagRC, "offsetCascade", args, n);
 
  widget = XtVaCreateManagedWidget ("0", xmPushButtonWidgetClass, 
                                    offsetMenu, NULL);
  XtAddCallback (widget, XmNactivateCallback, 
                 (XtCallbackProc)pushButtonCallbackWrap,
                 (XtPointer)(ANALOG_OFFSET_BUTTON_BASE + ANALOG_OFFSET_0_SELECT));
  widget = XtVaCreateManagedWidget ("10", xmPushButtonWidgetClass, 
                                    offsetMenu, NULL);
  XtAddCallback (widget, XmNactivateCallback, 
                 (XtCallbackProc)pushButtonCallbackWrap,
               (XtPointer)(ANALOG_OFFSET_BUTTON_BASE + ANALOG_OFFSET_10_SELECT));

  XtManageChild (dsmCascade);
  XtManageChild (chanCascade);
  XtManageChild (voltCascade);
  XtManageChild (gainCascade);
  XtManageChild (offsetCascade);


// Create the warning dialog box.
  warningDialog = XmCreateQuestionDialog(modeRadioBox, 
                                            "ANALOG_WARNING", NULL, 0);
  string1 = XmStringCreateLocalized ("Yes");
  string2 = XmStringCreateLocalized ("No");
  XtVaSetValues (warningDialog, 
                 XmNokLabelString, string1,
                 XmNcancelLabelString, string2,
                 NULL);
  XtAddCallback (warningDialog, XmNokCallback, calCallbackWrap, 
                 (XtPointer)TRUE);
  XtAddCallback (warningDialog, XmNcancelCallback, calCallbackWrap, 
                 (XtPointer)FALSE);
  XmStringFree (string1);
  XmStringFree (string2);


// Create the message dialog box.
  messageDialog = XmCreateMessageDialog(modeRadioBox, "analogMessage", NULL, 0);



// Create the report dialog box.
  reportDialog = XmCreateDialogShell(modeRadioBox, "reportShell", NULL, 0);

  n = 0;
  reportForm = XmCreateRowColumn(reportDialog, "reportForm", args, n);

  n = 0;
  reportText = XmCreateScrolledText(reportForm, "reportText", args, n);
  XtManageChild(reportText);

  n = 0;
  reportFrame = XmCreateFrame(reportForm, "reportFrame", args, n);
  XtManageChild(reportFrame);

  n = 0;
  reportRC = XmCreateRowColumn(reportFrame, "reportRC", args, n);
  XtManageChild(reportRC);


  n = 0;
  report_ok_button = XmCreatePushButton(reportRC, "dismissButt", args, n);
  XtManageChild(report_ok_button);

  XtAddCallback(report_ok_button, XmNactivateCallback, 
                 (XtCallbackProc)pushButtonCallbackWrap,
                 (XtPointer)NULL);
                              


// Set the default diagnostic parameters.
  mode = ANALOG_RUN_MODE;
  diagChan = 0;
  diagVolt = ANALOG_V10_SELECT;
  diagGain = ANALOG_GAIN_1_SELECT;
  diagOffset = ANALOG_OFFSET_0_SELECT;

// Initialize all dsms to be in the ANALOG_RUN_MODE.
  for (stat = firstDsm(); stat; stat = nextDsm()) {
    if (isDsmLocation())
      dsmMode[index()] = ANALOG_RUN_MODE;
  }
  msg[0] = '\0';


// Create buttons
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); ++n;
  XtSetArg(args[n], XmNtopWidget, modeFrame); ++n;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); ++n;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); ++n;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_WIDGET); ++n;
  XtSetArg(args[n], XmNrightWidget, diagFrame); ++n;
  buttonFrame = XmCreateFrame(mainForm, "buttonFrame", args, n);

  n = 0;
  buttonRC = XmCreateRowColumn(buttonFrame, "anaButtonRC", args, n);

  XtManageChild(buttonFrame);
  XtManageChild(buttonRC);

  n = 0;
  reportButt = XmCreatePushButton(buttonRC, "reportButt", args, n);
  dismissButt = XmCreatePushButton(buttonRC, "dismissButt", args, n);
  XtManageChild(dismissButt);
  XtManageChild(reportButt);

  XtAddCallback(dismissButt, XmNactivateCallback, 
                 (XtCallbackProc)pushButtonCallbackWrap, NULL);
  XtAddCallback(reportButt, XmNactivateCallback, 
                 (XtCallbackProc)pushButtonCallbackWrap, NULL);

  XtManageChild(mainForm);

}  /* END CONSTRUCTOR */
 
/* -------------------------------------------------------------------- */
void AnalogWin::toggleButtonCallbackProc (Widget w, XtPointer client,
                             XmToggleButtonCallbackStruct *cbs)
{
  int stat;

// Just return if this is not a button setting event.
  if (!cbs->set)
    return;

  switch ((int)client) {

    case ANALOG_RUN_MODE:
      if (!inCalMode()) {
        mode = ANALOG_RUN_MODE;			// set the mode

        // Send a message to each dsm to put it in run mode.
        for (stat = firstDsm(); stat; stat = nextDsm()) {
          if (isDsmLocation()) 
            dsm_msg.sendAnalogMsg (ANALOG_RUN_MODE, location(), 0,
                                   ANALOG_V10_SELECT, ANALOG_GAIN_1_SELECT, 
                                   ANALOG_OFFSET_0_SELECT, msg);
        }
      }
      // If a calibration is in progress, display the question dialog to get 
      // confirmation that the calibration is to be aborted. 
      else {
        confMode = ANALOG_RUN_MODE;		// set the confirmation mode
        displayWarningDialog ("An analog calibration is in progress. Abort the calibration?");
      }
      break;


    case ANALOG_QC_MODE:
      if (!inCalMode()) {
        mode = ANALOG_QC_MODE;			// set the mode

        // Send a message to each dsm to put it in run mode.
        for (stat = firstDsm(); stat; stat = nextDsm()) {
          if (isDsmLocation()) 
            dsm_msg.sendAnalogMsg (ANALOG_QC_MODE, location(), 0,
                                   ANALOG_V10_SELECT, ANALOG_GAIN_1_SELECT, 
                                   ANALOG_OFFSET_0_SELECT, msg);
        }
      }
      // If a calibration is in progress, display the question dialog to get 
      // confirmation that the calibration is to be aborted. 
      else {
        confMode = ANALOG_QC_MODE;		// set the confirmation mode
        displayWarningDialog ("An analog calibration is in progress. Abort the calibration?");
      }
      break;


    case ANALOG_CAL_MODE:
      // Display the question dialog for confirmation to do a calibration.
      confMode = ANALOG_CAL_MODE;		// set the confirmation mode
      if (!inCalMode()) 
        displayWarningDialog ("An Analog calibration may affect the engineering units calibrations. Continue?");
      else
        displayWarningDialog ("An analog calibration is already in progress. Start a new calibration?");
      break;


    case ANALOG_DIAG_MODE:
      // Send a message to the selected dsm to put it in diagnostic mode.
      if (!inCalMode()) {
        mode = ANALOG_DIAG_MODE;
        dsm_msg.sendAnalogMsg (ANALOG_DIAG_MODE, diagLocn, diagChan, 
                               diagVolt,diagGain, diagOffset, msg);
      }

      // If a calibration is in progress, display the question dialog for 
      // confirmation that the calibration is to be aborted. 
      else {
        confMode = ANALOG_DIAG_MODE;		// set the confirmation mode
        displayWarningDialog (
          "Analog calibration is in progress. Abort the calibration?");
      }
      break;
  }
}
 
/* -------------------------------------------------------------------- */
void AnalogWin::pushButtonCallbackProc(Widget w, XtPointer client,
                             XmPushButtonCallbackStruct *cbs)
{

  if (w == dismissButt) {
    XtPopdown(XtParent(mainForm));
    XtUnmanageChild(mainForm);
    return;
    }

  if (w == reportButt) {
    XtManageChild(reportForm);
    XtPopup(XtParent(reportForm), XtGrabNone);
    return;
    }

  if (w == report_ok_button) {
    XtUnmanageChild(reportForm);
    XtPopdown(XtParent(reportForm));
    return;
    }

  switch ((int)client & ~ANALOG_BUTTON_VALUE_MASK) {
    case ANALOG_DSM_BUTTON_BASE:
      selectByIndex ((int)client & ANALOG_BUTTON_VALUE_MASK);
      strcpy (diagLocn, location());		// update current diag location
      break;

    case ANALOG_CHAN_BUTTON_BASE:
      diagChan = (int)client & ANALOG_BUTTON_VALUE_MASK;
      break;

    case ANALOG_VOLT_BUTTON_BASE:
      diagVolt = (int)client & ANALOG_BUTTON_VALUE_MASK;
      break;

    case ANALOG_GAIN_BUTTON_BASE:
      diagGain = (int)client & ANALOG_BUTTON_VALUE_MASK;
      break;

    case ANALOG_OFFSET_BUTTON_BASE:
      diagOffset = (int)client & ANALOG_BUTTON_VALUE_MASK;
      break;
  }

// If the diagnostic mode is currently selected, send a message for the change.
  if (inDiagMode())
    dsm_msg.sendAnalogMsg (ANALOG_DIAG_MODE, diagLocn, diagChan, 
                           diagVolt, diagGain, diagOffset, msg);
                          
}

/* -------------------------------------------------------------------- */
void AnalogWin::warningCallbackProc (Widget button, XtPointer client_data, 
                                 XtPointer call_data)
{
  int stat;

// If cancel was selected, don't change the current state.
  if (!(int)client_data) {
    setToggleButtonStates();
    return;
  }

// Otherwise switch to the new confirmed mode.
  switch (confMode) {

    case ANALOG_RUN_MODE:

// Send a message to each dsm to put it in run mode.
      for (stat = firstDsm(); stat; stat = nextDsm()) {
        if (isDsmLocation()) 
          dsm_msg.sendAnalogMsg(ANALOG_RUN_MODE, location(), 0,
                                ANALOG_V10_SELECT, ANALOG_GAIN_1_SELECT, 
                                ANALOG_OFFSET_0_SELECT, msg);
      }
      break;

    case ANALOG_CAL_MODE:

// Send a message to each dsm to start the calibration sequence.
      for (stat = firstDsm(); stat; stat = nextDsm()) {
        if (isDsmLocation()) 
          dsm_msg.sendAnalogMsg(ANALOG_CAL_MODE, location(), 0,
                                ANALOG_V10_SELECT, ANALOG_GAIN_1_SELECT, 
                                ANALOG_OFFSET_0_SELECT, msg);
      }
      break;

    case ANALOG_DIAG_MODE:

// Send a message to the selected dsm to put it in diagnostic mode.
      dsm_msg.sendAnalogMsg (ANALOG_DIAG_MODE, diagLocn, diagChan, 
                             diagVolt,diagGain, diagOffset, msg);
      break;
  }
  mode = confMode;
}

/* -------------------------------------------------------------------- */
void AnalogWin::displayState(int action, char *locn)

// Sets the analog state of the dsm at the specified location.
{

  selectByLocn(locn);
  dsmMode[index()] = action;		// set the dsm mode

  switch (action) {
    case ANALOG_RUN_MODE:
      if (inCalMode() && calComplete()) {	// check if a cal has completed
        mergeHeaders();
        if (dsm_headers.mainHeader()->writeFile(mainHeaderName()) == ERROR)
          displayMessageDialog("Permission denied opening header, calibrate failed.");
        else
          displayMessageDialog("Analog calibration is complete.  WINDS must be reset for the new analog coefficients to be in effect.");

        mode = ANALOG_RUN_MODE;
        setToggleButtonStates();
      }

      if (inQCmode() && QCcomplete()) {
        writeQCreport();
        mode = ANALOG_RUN_MODE;
        setToggleButtonStates();
      }

      break;

    case ANALOG_CAL_MODE:
    case ANALOG_DIAG_MODE:
      break;
  }
}

/* -------------------------------------------------------------------- */
void AnalogWin::popUp(Widget w, XtPointer client,
			XmPushButtonCallbackStruct *cbs)
{
  XtManageChild(mainForm);
  XtPopup(XtParent(mainForm), XtGrabNone);

}

/* -------------------------------------------------------------------- */
int AnalogWin::QCcomplete ()
{
  int stat;

  if (!inQCmode())
    return FALSE;

// Check if all dsms are back in the run mode.
  for (stat = firstDsm(); stat; stat = nextDsm()) {
    if (isDsmLocation() && (dsmMode[index()] != ANALOG_RUN_MODE))
      return FALSE;
  }
  return TRUE;
}

/* -------------------------------------------------------------------- */
int AnalogWin::calComplete ()
{
  int stat;

  if (!inCalMode())
    return FALSE;

// Check if all dsms are back in the run mode.
  for (stat = firstDsm(); stat; stat = nextDsm()) {
    if (isDsmLocation() && (dsmMode[index()] != ANALOG_RUN_MODE))
      return FALSE;
  }
  return TRUE;
}

/* -------------------------------------------------------------------- */
void AnalogWin::mergeHeaders ()

// Merges analog conversion factors from the dsm headers into the main header.
{
  int stat;
  int convert_offset;			// temp convert_offset var
  float convert;			// temp convert var
  char convert_error;
  char convert_offset_error;

  // Clear report text window.
  XmTextSetString(reportText, "");

  // Write the report header.
  writeReportMessage (
    "Analog Calibration Report.\n");
  writeReportMessage (
    "* indicates channel exceeds specification for this parameter.\n\n");
  writeReportMessage (
    "DSM       NAME      CHANNEL    CONVERT   CONVERT_OFFSET\n");

  // Read in the updated dsm headers.
  dsm_headers.readHeaders();

  // Parse the main header.
  for (stat = dsm_headers.mainHeader()->firstDesc(); stat;
       stat = dsm_headers.mainHeader()->nextDesc()) {

    if (dsm_headers.mainHeader()->analogType() &&
        strcmp(dsm_headers.mainHeader()->name(), "DUMMY")) {

// Select the correct dsm header.
      dsm_headers.selectHeader (dsm_headers.mainHeader()->dsm_locn());

// Select the matching dsm header descriptor.
      if (!dsm_headers.curHeader()->selectShDesc (
          dsm_headers.mainHeader()->name())) {
        fprintf (stderr, "AnalogWin: Failure selecting Sh descriptor %s.\n",
                 dsm_headers.mainHeader()->name());
        exit (ERROR);
      }

// Update the convert and convert_offset fields.
      dsm_headers.mainHeader()->setShConvert (
        convert = dsm_headers.curHeader()->convert());
           
      dsm_headers.mainHeader()->setShConvertOffset (
        convert_offset = dsm_headers.curHeader()->getShConvertOffset());
                                             
// Check the limits.
      if ((convert < ANALOG_CONVERT_MIN) || (convert > ANALOG_CONVERT_MAX)) 
        convert_error = '*';
      else
        convert_error = ' ';
          
      if ((convert_offset < ANALOG_CONVERT_OFFSET_MIN) ||
          (convert_offset > ANALOG_CONVERT_OFFSET_MAX))
        convert_offset_error = '*';
      else
        convert_offset_error = ' ';

// Write results to the report dialog.
      sprintf (msg, "%-8s  %-8s  %2x        %8.6f%c    %4d%c\n",
               dsm_headers.mainHeader()->dsm_locn(),
               dsm_headers.curHeader()->name(), 
               dsm_headers.curHeader()->adsaddr(), 
               dsm_headers.curHeader()->convert(), 
               convert_error,
               dsm_headers.curHeader()->getShConvertOffset(),
               convert_offset_error);

      writeReportMessage(msg);
    }
  }
}

/* -------------------------------------------------------------------- */
void AnalogWin::writeQCreport()
{
  FILE	*fp;
  int	stat;
  char	fileName[256], buff[80], host[32];

  selectByLocn(CTL_LOCN_STR);
  strcpy(host, hostName());

  writeReportMessage("\n\nDSM       Channel   Addr    Low      High\n");
  writeReportMessage("---------------------------------------------\n");

  for (stat = (int)firstDsm(); stat; stat = (int)nextDsm()) {
    if (!isDsmLocation())
      continue;

    sprintf(fileName,"%s/proj/hosts/%s/qc.%s",getenv("LOCAL"),host,location());

    if ((fp = fopen(fileName, "r")) == NULL) {
      sprintf(buff, "\nNo report from %s DSM.\n", location());
      writeReportMessage(buff);
      continue;
    }

    while (fgets(buff, 256, fp) != NULL)
      writeReportMessage(buff);

    fclose(fp);
    unlink(fileName);
  }

  writeReportMessage("\n");
}

/* -------------------------------------------------------------------- */
void AnalogWin::setToggleButtonStates()

// Sets the button states to match the current mode.
{
  switch (mode) {
    case ANALOG_RUN_MODE:
      XmToggleButtonSetState(runButton, TRUE, FALSE);
      XmToggleButtonSetState(calButton, FALSE, FALSE);
      XmToggleButtonSetState(diagButton, FALSE, FALSE);
      XmToggleButtonSetState(qcButton, FALSE, FALSE);
      break;

    case ANALOG_CAL_MODE:
      XmToggleButtonSetState(runButton, FALSE, FALSE);
      XmToggleButtonSetState(calButton, TRUE, FALSE);
      XmToggleButtonSetState(diagButton, FALSE, FALSE);
      XmToggleButtonSetState(qcButton, FALSE, FALSE);
      break;

    case ANALOG_DIAG_MODE:
      XmToggleButtonSetState(runButton, FALSE, FALSE);
      XmToggleButtonSetState(calButton, FALSE, FALSE);
      XmToggleButtonSetState(diagButton, TRUE, FALSE);
      XmToggleButtonSetState(qcButton, FALSE, FALSE);

    case ANALOG_QC_MODE:
      XmToggleButtonSetState(runButton, FALSE, FALSE);
      XmToggleButtonSetState(calButton, FALSE, FALSE);
      XmToggleButtonSetState(diagButton, FALSE, FALSE);
      XmToggleButtonSetState(qcButton, TRUE, FALSE);
  }
}

/* -------------------------------------------------------------------- */
void AnalogWin::displayWarningDialog (char *message)

// Displays the warning dialog popup with the specified message.
{
  XmString string;

  string = XmStringCreateLocalized (message);
  XtVaSetValues (warningDialog, 
                 XmNmessageString, string,
                 NULL);
  XmStringFree (string);
  XtManageChild (warningDialog);
  XtPopup(XtParent(warningDialog), XtGrabNone);
}
 
/* -------------------------------------------------------------------- */
void AnalogWin::displayMessageDialog (char *message)
 
// Displays the message dialog popup with the specified message.
{
  XmString string;
 
  string = XmStringCreateLocalized(message);
  XtVaSetValues (messageDialog,
                 XmNmessageString, string,
                 NULL);
  XmStringFree(string);
  XtManageChild(messageDialog);
  XtPopup(XtParent(messageDialog), XtGrabNone);
}
 
/* -------------------------------------------------------------------- */
void AnalogWin::writeReportMessage(char *message)
 
// Writes a message for display in the report dialog.
{
  int last_pos;
 
  last_pos = XmTextGetLastPosition(reportText);
  XmTextInsert(reportText,last_pos, message);
  XmTextShowPosition(reportText, last_pos);
}

/* END ANALOGWIN.CC */
