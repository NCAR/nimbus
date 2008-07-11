/* Pms2dWin.cc
   Creates and manages the recording control window.

   Original Author: Jerry V. Pelk 
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <Pms2dWin.h>

/******************************************************************************
** Public Functions
******************************************************************************/
Pms2dWin::Pms2dWin (Widget parent, DsmMessage &dm, 
    void(*tasCallbackWrap)(Widget, XtPointer, XmToggleButtonCallbackStruct*),
    void(*rateCallbackWrap)(Widget, XtPointer, XmToggleButtonCallbackStruct*)) :
    dsm_msg (dm)

// Constructor.
{
  XmString label;
  int tas_button_values[PMS2_TAS_NUM_BUTTONS] = PMS2_TAS_BUTTON_VALUES;
  int rate_button_values[PMS2_RATE_NUM_BUTTONS] = PMS2_RATE_BUTTON_VALUES;
  int j;
  Arg args[8];
  Cardinal n;

// Create the window frame and form.
  n = 0;
  frame = XmCreateFrame(parent, "pms2_frame", args, n);
  XtManageChild(frame);

  n = 0;
  title = XmCreateLabel(frame, "pms2_title", args, n);
  XtManageChild(title);
 
  n = 0;
  form = XmCreateForm(frame, "pms2_form", args, n);
  XtManageChild(form);


// Create the tas selection radio box.
  tas_radio_box = XmCreateRadioBox (form, "tas_radio_box", NULL, 0);
  XtVaSetValues (tas_radio_box,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_FORM,
    XmNorientation, XmHORIZONTAL,
    XmNisHomogeneous, FALSE,
    NULL);

// Create the tas selection label.
  label = XmStringCreateSimple ("TAS Select  ");
  tas_label = XtVaCreateManagedWidget ("tas_label",
    xmLabelWidgetClass, tas_radio_box,
    XmNlabelString, label,
    NULL);
  XmStringFree (label);

// Create the tas selection buttons.
  for (j = 0; j < PMS2_TAS_NUM_BUTTONS; j++) {
    if (tas_button_values[j] == TAS2D_AUTO)
      sprintf (name[0], " Auto ");
    else
      sprintf (name[0], " %2d   ", tas_button_values[j]);

    tas_button[j] = XtVaCreateManagedWidget (name[0],
      xmToggleButtonWidgetClass, tas_radio_box, NULL);

    XtAddCallback (tas_button[j], XmNvalueChangedCallback, 
                 (XtCallbackProc)tasCallbackWrap,
                 (XtPointer)j);
  }
// Set the initial tas toggle state to TAS2D_AUTO.
  XmToggleButtonSetState (tas_button[PMS2_TAS_NUM_BUTTONS - 1], 
                                TRUE, FALSE);

  XtManageChild (tas_radio_box);

// Create the record rate radio box.
  rate_radio_box = XmCreateRadioBox (form, "rate_radio_box", NULL, 0);
  XtVaSetValues (rate_radio_box,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, tas_radio_box,
    XmNorientation, XmHORIZONTAL,
    XmNisHomogeneous, FALSE,
    NULL);
 
// Create the record rate label.
  label = XmStringCreateSimple ("Record Rate ");
  rate_label = XtVaCreateManagedWidget ("rate_label",
    xmLabelWidgetClass, rate_radio_box,
    XmNlabelString, label,
    NULL);
  XmStringFree (label);

// Create the rate selection buttons.
  for (j = 0; j < PMS2_RATE_NUM_BUTTONS; j++) {
    if (rate_button_values[j] == RATE2D_OFF)
      sprintf (name[0], " Off  ");
    else if (rate_button_values[j] == RATE2D_MAX)
      sprintf (name[0], " Max  ");
    else
      sprintf (name[0], " %2d   ", rate_button_values[j]);
 
    rate_button[j] = XtVaCreateManagedWidget (name[0],
      xmToggleButtonWidgetClass, rate_radio_box, NULL);
 
    XtAddCallback(rate_button[j], XmNvalueChangedCallback, 
                 (XtCallbackProc)rateCallbackWrap,
                  (XtPointer)j);
  }
// Set the initial rate toggle state to RATE2D_MAX.
  XmToggleButtonSetState (rate_button[PMS2_RATE_NUM_BUTTONS - 1], 
                                TRUE, FALSE);
  XtManageChild (rate_radio_box);

// Create the probe selection check box.
  probe_check_box = XmCreateSimpleCheckBox (form, "probe_check_box", NULL, 0);
  XtVaSetValues (probe_check_box,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, rate_radio_box,
    XmNorientation, XmHORIZONTAL,
    XmNisHomogeneous, FALSE,
    NULL);
 
// Create the probe selection label.
  label = XmStringCreateSimple ("Probe Select");
  probe_label = XtVaCreateManagedWidget ("probe_label",
    xmLabelWidgetClass, probe_check_box,
    XmNlabelString, label,
    NULL);
  XmStringFree (label);

  XtManageChild (probe_check_box);

  p_cnt = 0;
  c_cnt = 0;
  g_cnt = 0;
  h_cnt = 0;
  tot_cnt = 0;
}

/*****************************************************************************/
void Pms2dWin::createProbe (char *probe_name, char *probe_locn, char *dsm_locn,
    void (*probeCallbackWrap)(Widget, XtPointer, XmToggleButtonCallbackStruct*))
 
// Creates a 2d probe selection button.
{
  str12 label;

// Save the dsm location.
  strcpy (location[tot_cnt], dsm_locn);

// Build the probe name.
  if (!strcmp (probe_name, P2D_P_STR)) 
    sprintf (name[tot_cnt], "P%1d", ++p_cnt);
  else if (!strcmp (probe_name, P2D_C_STR)) 
    sprintf (name[tot_cnt], "C%1d", ++c_cnt);
  else if (!strcmp (probe_name, P2D_G_STR)) 
    sprintf (name[tot_cnt], "G%1d", ++g_cnt);
  else if (!strcmp (probe_name, P2D_H_STR)) 
    sprintf (name[tot_cnt], "H%1d", ++h_cnt);
  else {
    fprintf (stderr, "Pms2dWin: Unknown probe type, %s.\n", probe_name);
    exit (ERROR);
  }

// Create the button.
  sprintf (label, " %s   ", name[tot_cnt]);
  probe_button[tot_cnt] = XtVaCreateManagedWidget (label,
      xmToggleButtonWidgetClass, probe_check_box, NULL);
  XtAddCallback(probe_button[tot_cnt], XmNvalueChangedCallback, 
                (XtCallbackProc)probeCallbackWrap, (XtPointer)tot_cnt);
               

// Set the initial probe state to selected.
  XmToggleButtonSetState (probe_button[tot_cnt], TRUE, FALSE);
  tot_cnt++;
  XtManageChild (probe_check_box);
}

/*****************************************************************************/
void Pms2dWin::sendState()
{
  int	i;
  XmToggleButtonCallbackStruct cbs;

  cbs.set = True;

  for (i = 0; i < PMS2_TAS_NUM_BUTTONS; ++i)
    if (XmToggleButtonGetState(tas_button[i]))
      tasCallbackProc(tas_button[i], (XtPointer)i, &cbs);

  for (i = 0; i < PMS2_RATE_NUM_BUTTONS; ++i)
    if (XmToggleButtonGetState(rate_button[i]))
      rateCallbackProc(rate_button[i], (XtPointer)i, &cbs);

  for (i = 0; i < tot_cnt; ++i)
    if (XmToggleButtonGetState(probe_button[i]))
      probeCallbackProc(probe_button[i], (XtPointer)i, &cbs);

}

/*****************************************************************************/
void Pms2dWin::tasCallbackProc (Widget w, XtPointer client,
                             XmToggleButtonCallbackStruct *cbs)

// Tas event call back procedure.
{
  static int tas_button_values[PMS2_TAS_NUM_BUTTONS] = PMS2_TAS_BUTTON_VALUES;
  int j;

// Just return if this is not a button setting event.
  if (!cbs->set)
    return;

// Send a tas selection message to the dsm for each probe.
  for (j = 0; j < tot_cnt; j++) {
    dsm_msg.sendPms2Msg (PMS2_TAS_SELECT, 
                         tas_button_values[(int)client],
                         name[j],
                         location[j], "");
  }
}

/*****************************************************************************/
void Pms2dWin::rateCallbackProc (Widget w, XtPointer client,
                             XmToggleButtonCallbackStruct *cbs)
 
// Record rate event call back procedure.
{
  static int rate_button_values[PMS2_RATE_NUM_BUTTONS] =PMS2_RATE_BUTTON_VALUES;
  int j;

// Just return if this is not a button setting event.
  if (!cbs->set)
    return;
 
// Send a rate selection message to the dsm for each probe.
  for (j = 0; j < tot_cnt; j++) {
    dsm_msg.sendPms2Msg (PMS2_RATE_SELECT, 
                         rate_button_values[(int)client],
                         name[j],
                         location[j], "");
  }
}

/*****************************************************************************/
void Pms2dWin::probeCallbackProc (Widget w, XtPointer client,
                             XmToggleButtonCallbackStruct *cbs)
 
// Probe selection event call back procedure.
{
  dsm_msg.sendPms2Msg (PMS2_PROBE_SELECT, 
                       cbs->set,
                       name[(int)client],
                       location[(int)client], "");
}
