/* McrWin.cc
   Creates and manages the recording control window.

   Original Author: Jerry V. Pelk 
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <McrWin.h>

/******************************************************************************
** Public Functions
******************************************************************************/

McrWin::McrWin(Widget parent, DsmMessage &dm, char *dsm_loc,
	void(*toggleButtonCallbackWrap)
	(Widget, XtPointer, XmToggleButtonCallbackStruct*)) : dsm_msg (dm)
{
  XmString label;
  Arg	args[8];
  Cardinal n;

// Copy the dsm location string.
  strcpy (dsm_locn, dsm_loc);

// Create the window frame and form.
  n = 0;
  frame = XmCreateFrame(parent, "mcr_frame", args, n);
  XtManageChild(frame);

  n = 0;
  title = XmCreateLabel(frame, "mcr_title", args, n);
  XtManageChild(title);
 
  n = 0;
  form = XmCreateForm(frame, "mcr_form", args, n);
  XtManageChild(form);

// Create the selection check box.
  mcr_check_box = XmCreateSimpleCheckBox (form, "probe_check_box", NULL, 0);
  XtVaSetValues (mcr_check_box,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_FORM,
    XmNorientation, XmHORIZONTAL,
    XmNisHomogeneous, FALSE,
    NULL);
 
// Create the buttons.
  remote_button = XtVaCreateManagedWidget ("DSM Control Enable",
      xmToggleButtonWidgetClass, mcr_check_box, NULL);
      XtAddCallback(remote_button, XmNvalueChangedCallback, 
      (XtCallbackProc)toggleButtonCallbackWrap, (XtPointer)MCR_REMOTE_ENABLE);
     
  preamp_button = XtVaCreateManagedWidget ("Preamp Power",
      xmToggleButtonWidgetClass, mcr_check_box, NULL);
      XtAddCallback(preamp_button, XmNvalueChangedCallback,
      (XtCallbackProc)toggleButtonCallbackWrap, (XtPointer)MCR_PREAMP_ENABLE);
 
  door_enable_button = XtVaCreateManagedWidget ("Doors Enable" ,
      xmToggleButtonWidgetClass, mcr_check_box, NULL);
      XtAddCallback(door_enable_button, XmNvalueChangedCallback, 
      (XtCallbackProc)toggleButtonCallbackWrap, (XtPointer)MCR_DOOR_ENABLE);
 
  door_button = XtVaCreateManagedWidget ("Doors Open" ,
      xmToggleButtonWidgetClass, mcr_check_box, NULL);
      XtAddCallback(door_button, XmNvalueChangedCallback,
      (XtCallbackProc)toggleButtonCallbackWrap, (XtPointer)MCR_DOOR_SELECT);
 
  heater_button = XtVaCreateManagedWidget ("Heater Power",
      xmToggleButtonWidgetClass, mcr_check_box, NULL);
      XtAddCallback(heater_button, XmNvalueChangedCallback,
      (XtCallbackProc)toggleButtonCallbackWrap, (XtPointer)MCR_HEATER_SELECT);

// Set the initial button states to deselected.
  XmToggleButtonSetState (remote_button, FALSE, FALSE);
  XmToggleButtonSetState (preamp_button, FALSE, FALSE);
  XmToggleButtonSetState (door_enable_button, FALSE, FALSE);
  XmToggleButtonSetState (door_button, FALSE, FALSE);
  XmToggleButtonSetState (heater_button, FALSE, FALSE);

  XtManageChild (mcr_check_box);

// Create the status bit labels and text fields.
  label = XmStringCreateSimple ("DSM Active    ");
  dsm_active_label = XtVaCreateManagedWidget ("dsm_active_label",
    xmLabelWidgetClass, form,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, mcr_check_box,
    XmNleftAttachment, XmATTACH_POSITION,
    XmNleftPosition, 1,
    XmNlabelString, label,
    NULL);
  XmStringFree (label);
 
  dsm_active_text = XtVaCreateManagedWidget ("dsm_active_text",
    xmTextFieldWidgetClass, form,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, mcr_check_box,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, dsm_active_label,
    XmNcolumns, 1,
    XmNmarginHeight, 2,
    XmNvalue, "F",
    XmNeditable, FALSE,
    XmNcursorPositionVisible, FALSE,
    NULL);
 
  label = XmStringCreateSimple ("Control Enable");
  remote_enabled_label = XtVaCreateManagedWidget ("remote_enabled_label",
    xmLabelWidgetClass, form,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, dsm_active_label,
    XmNleftAttachment, XmATTACH_POSITION,
    XmNleftPosition, 1,
    XmNtopOffset, 10,
    XmNlabelString, label,
    NULL);
  XmStringFree (label);
 
  remote_enabled_text = XtVaCreateManagedWidget ("remote_enabled_text",
    xmTextFieldWidgetClass, form,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, dsm_active_text,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, remote_enabled_label,
    XmNcolumns, 1,
    XmNmarginHeight, 2,
    XmNvalue, "F",
    XmNeditable, FALSE,
    XmNcursorPositionVisible, FALSE,
    NULL);

  label = XmStringCreateSimple ("Preamp On     ");
  preamp_on_label = XtVaCreateManagedWidget ("preamp_on_label",
    xmLabelWidgetClass, form,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, mcr_check_box,
    XmNleftAttachment, XmATTACH_POSITION,
    XmNleftPosition, 21,
    XmNlabelString, label,
    NULL);
  XmStringFree (label);
 
  preamp_on_text = XtVaCreateManagedWidget ("preamp_on_text",
    xmTextFieldWidgetClass, form,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, mcr_check_box,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, preamp_on_label,
    XmNcolumns, 1,
    XmNmarginHeight, 2,
    XmNvalue, "F",
    XmNeditable, FALSE,
    XmNcursorPositionVisible, FALSE,
    NULL);

  label = XmStringCreateSimple ("Preamp Off    ");
  preamp_off_label = XtVaCreateManagedWidget ("preamp_off_label",
    xmLabelWidgetClass, form,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, preamp_on_label,
    XmNleftAttachment, XmATTACH_POSITION,
    XmNleftPosition, 21,
    XmNlabelString, label,
    XmNtopOffset, 10,
    NULL);
  XmStringFree (label);
 
  preamp_off_text = XtVaCreateManagedWidget ("preamp_off_text",
    xmTextFieldWidgetClass, form,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, preamp_on_text,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, preamp_off_label,
    XmNcolumns, 1,
    XmNmarginHeight, 2,
    XmNvalue, "F",
    XmNeditable, FALSE,
    XmNcursorPositionVisible, FALSE,
    NULL);

  label = XmStringCreateSimple ("R. Door Open  ");
  rdoor_open_label = XtVaCreateManagedWidget ("rdoor_open_label",
    xmLabelWidgetClass, form,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, mcr_check_box,
    XmNleftAttachment, XmATTACH_POSITION,
    XmNleftPosition, 41,
    XmNlabelString, label,
    NULL);
  XmStringFree (label);
 
  rdoor_open_text = XtVaCreateManagedWidget ("rdoor_open_text",
    xmTextFieldWidgetClass, form,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, mcr_check_box,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, rdoor_open_label,
    XmNcolumns, 1,
    XmNmarginHeight, 2,
    XmNvalue, "F",
    XmNeditable, FALSE,
    XmNcursorPositionVisible, FALSE,
    NULL);

  label = XmStringCreateSimple ("R. Door Closed");
  rdoor_closed_label = XtVaCreateManagedWidget ("rdoor_closed_label",
    xmLabelWidgetClass, form,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, rdoor_open_label,
    XmNleftAttachment, XmATTACH_POSITION,
    XmNleftPosition, 41,
    XmNtopOffset, 10,
    XmNlabelString, label,
    NULL);
  XmStringFree (label);
 
  rdoor_closed_text = XtVaCreateManagedWidget ("rdoor_closed_text",
    xmTextFieldWidgetClass, form,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, rdoor_open_text,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, rdoor_closed_label,
    XmNcolumns, 1,
    XmNmarginHeight, 2,
    XmNvalue, "F",
    XmNeditable, FALSE,
    XmNcursorPositionVisible, FALSE,
    NULL);

  label = XmStringCreateSimple ("L. Door Open  ");
  ldoor_open_label = XtVaCreateManagedWidget ("ldoor_open_label",
    xmLabelWidgetClass, form,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, mcr_check_box,
    XmNleftAttachment, XmATTACH_POSITION,
    XmNleftPosition, 61,
    XmNlabelString, label,
    NULL);
  XmStringFree (label);
 
  ldoor_open_text = XtVaCreateManagedWidget ("ldoor_open_text",
    xmTextFieldWidgetClass, form,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, mcr_check_box,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, ldoor_open_label,
    XmNcolumns, 1,
    XmNmarginHeight, 2,
    XmNvalue, "F",
    XmNeditable, FALSE,
    XmNcursorPositionVisible, FALSE,
    NULL);
 
  label = XmStringCreateSimple ("L. Door Closed");
  ldoor_closed_label = XtVaCreateManagedWidget ("ldoor_closed_label",
    xmLabelWidgetClass, form,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, ldoor_open_label,
    XmNleftAttachment, XmATTACH_POSITION,
    XmNleftPosition, 61,
    XmNtopOffset, 10,
    XmNlabelString, label,
    NULL);
  XmStringFree (label);
 
  ldoor_closed_text = XtVaCreateManagedWidget ("ldoor_closed_text",
    xmTextFieldWidgetClass, form,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, ldoor_open_text,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, ldoor_closed_label,
    XmNcolumns, 1,
    XmNmarginHeight, 2,
    XmNvalue, "F",
    XmNeditable, FALSE,
    XmNcursorPositionVisible, FALSE,
    NULL);

  label = XmStringCreateSimple ("Heaters On    ");
  heaters_on_label = XtVaCreateManagedWidget ("heaters_on_label",
    xmLabelWidgetClass, form,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, mcr_check_box,
    XmNleftAttachment, XmATTACH_POSITION,
    XmNleftPosition, 81,
    XmNlabelString, label,
    NULL);
  XmStringFree (label);
 
  heaters_on_text = XtVaCreateManagedWidget ("heaters_on_text",
    xmTextFieldWidgetClass, form,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, mcr_check_box,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, heaters_on_label,
    XmNcolumns, 1,
    XmNmarginHeight, 2,
    XmNvalue, "F",
    XmNeditable, FALSE,
    XmNcursorPositionVisible, FALSE,
    NULL);
 
  label = XmStringCreateSimple ("Heaters Off   ");
  heaters_off_label = XtVaCreateManagedWidget ("heaters_off_label",
    xmLabelWidgetClass, form,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, dsm_active_label,
    XmNleftAttachment, XmATTACH_POSITION,
    XmNleftPosition, 81,
    XmNtopOffset, 10,
    XmNlabelString, label,
    NULL);
  XmStringFree (label);
 
  heaters_off_text = XtVaCreateManagedWidget ("heaters_off_text",
    xmTextFieldWidgetClass, form,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, heaters_on_text,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, heaters_off_label,
    XmNcolumns, 1,
    XmNmarginHeight, 2,
    XmNvalue, "F",
    XmNeditable, FALSE,
    XmNcursorPositionVisible, FALSE,
    NULL);
}
/*****************************************************************************/

void McrWin::toggleButtonCallbackProc (Widget w, XtPointer client,
	XmToggleButtonCallbackStruct *cbs, void (*turnOff)(int))

// Handles toggle button callback events.
{
  dsm_msg.sendMcrMsg((int)client, cbs->set, dsm_locn, "");

  if ((int)client == MCR_HEATER_SELECT && cbs->set)
    {
    signal(SIGALRM, turnOff);
    alarm(5 * 60);
    printf("discWin::McrWin: MCR heater OFF alarm is set for 6 minuets.\n");
    }
}

/*****************************************************************************/
void McrWin::sendState()
{
  XmToggleButtonCallbackStruct cbs;

  cbs.set = True;

  toggleButtonCallbackProc(remote_button, (XtPointer)MCR_REMOTE_ENABLE, &cbs, NULL);
  toggleButtonCallbackProc(preamp_button, (XtPointer)MCR_PREAMP_ENABLE, &cbs, NULL);
  toggleButtonCallbackProc(door_enable_button, (XtPointer)MCR_DOOR_ENABLE,&cbs, NULL);
  toggleButtonCallbackProc(door_button, (XtPointer)MCR_DOOR_SELECT, &cbs, NULL);

  // Force heater off.
  cbs.set = False;
  toggleButtonCallbackProc(heater_button, (XtPointer)MCR_HEATER_SELECT, &cbs, NULL);

}

/*****************************************************************************/
void McrWin::displayState (int action, int value)

// Sets the state of the output control buttons and input status bits based
// upon the passed in values.
{
  switch (action) {
    case MCR_REMOTE_ENABLE:
      XmToggleButtonSetState (remote_button, value, FALSE);
      break;
    case MCR_PREAMP_ENABLE:
      XmToggleButtonSetState (preamp_button, value, FALSE);
      break;
    case MCR_DOOR_ENABLE:
      XmToggleButtonSetState (door_enable_button, value, FALSE);
      break;
    case MCR_DOOR_SELECT:
      XmToggleButtonSetState (door_button, value, FALSE);
      break;
    case MCR_HEATER_SELECT:
      XmToggleButtonSetState (heater_button, value, FALSE);
      break;
    case MCR_STATUS:
      displayStatus (value);
      break;
    default:;
  }
}
/******************************************************************************
** Private Functions
******************************************************************************/

void McrWin::displayStatus (int status)

// Displays the current status word bit values.
{
  if (status & MCR_PREAMP_ON)
    XmTextSetString (preamp_on_text, "T");
  else
    XmTextSetString (preamp_on_text, "F");

  if (status & MCR_PREAMP_OFF)
    XmTextSetString (preamp_off_text, "T");
  else
    XmTextSetString (preamp_off_text, "F");

  if (status & MCR_RIGHT_DOOR_OPEN)
    XmTextSetString (rdoor_open_text, "T");
  else
    XmTextSetString (rdoor_open_text, "F");

  if (status & MCR_RIGHT_DOOR_CLOSED)
    XmTextSetString (rdoor_closed_text, "T");
  else
    XmTextSetString (rdoor_closed_text, "F");

  if (status & MCR_LEFT_DOOR_OPEN)
    XmTextSetString (ldoor_open_text, "T");
  else
    XmTextSetString (ldoor_open_text, "F");

  if (status & MCR_LEFT_DOOR_CLOSED)
    XmTextSetString (ldoor_closed_text, "T");
  else
    XmTextSetString (ldoor_closed_text, "F");

  if (status & MCR_REMOTE_ACTIVE)
    XmTextSetString (dsm_active_text, "T");
  else
    XmTextSetString (dsm_active_text, "F");

  if (status & MCR_REMOTE_ENABLED)
    XmTextSetString (remote_enabled_text, "T");
  else
    XmTextSetString (remote_enabled_text, "F");

  if (status & MCR_HEATERS_ON)
    XmTextSetString (heaters_on_text, "T");
  else
    XmTextSetString (heaters_on_text, "F");

  if (status & MCR_HEATERS_OFF)
    XmTextSetString (heaters_off_text, "T");
  else
    XmTextSetString (heaters_off_text, "F");
}
/*****************************************************************************/

