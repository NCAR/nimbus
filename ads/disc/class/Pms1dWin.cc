/* Pms1dWin.cc
   Creates and manages the recording control window.

   Original Author: Jerry V. Pelk 
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <Pms1dWin.h>


/* -------------------------------------------------------------------- */
Pms1dWin::Pms1dWin(Widget parent, DsmMessage &dm) : dsm_msg (dm)
{
  Arg		args[8];
  Cardinal	n;

// Create the window frame and form.
  n = 0;
  pms1_frame = XmCreateFrame(parent, "pms1_frame", args, n);
  XtManageChild(pms1_frame);

  n = 0;
  pms1_title = XmCreateLabel(pms1_frame, "pms1_title", args, n);
  XtManageChild(pms1_title);
 
  n = 0;
  pms1_form = XmCreateRowColumn(pms1_frame, "pms1_form", args, n);
  XtManageChild(pms1_form);

  count = 0;
}

/* -------------------------------------------------------------------- */
void Pms1dWin::createFssp (char *name, char *probe_locn, char *dsm_locn,
       void (*callbackWrap)(Widget, XtPointer, XmToggleButtonCallbackStruct*))

// Creates an Fssp control box.
{
  XmString label;

// Create a pms 1d control radio box.
  pms1_radio_box[count] = XmCreateRadioBox (pms1_form, name, NULL, 0);

  if (!count)
    XtVaSetValues (pms1_radio_box[count],
//      XmNleftAttachment, XmATTACH_FORM,
//      XmNtopAttachment, XmATTACH_FORM,
      XmNorientation, XmHORIZONTAL,
      XmNisHomogeneous, FALSE,
      NULL);
  else
    XtVaSetValues (pms1_radio_box[count],
//      XmNleftAttachment, XmATTACH_FORM,
//      XmNtopAttachment, XmATTACH_WIDGET,
//      XmNtopWidget, pms1_radio_box[count - 1],
      XmNorientation, XmHORIZONTAL,
      XmNisHomogeneous, FALSE,
      NULL);

// Create the probe composite name from the name and location.  Save the probe 
// dsm location.
  makePms1Name (name, probe_locn);
  strcpy (location[count], dsm_locn);		// 

// Create the probe name label.
  label = XmStringCreateSimple (pms1_name[count]);
  pms1_label[count] = XtVaCreateManagedWidget (pms1_name[count],
    xmLabelWidgetClass, pms1_radio_box[count],
    XmNlabelString, label,
    NULL);
  XmStringFree (label);
 
  button0[count] = XtVaCreateManagedWidget ("Range 0 ",
    xmToggleButtonWidgetClass, pms1_radio_box[count], NULL);
 
  button1[count] = XtVaCreateManagedWidget ("Range 1 ",
    xmToggleButtonWidgetClass, pms1_radio_box[count], NULL);
 
  button2[count] = XtVaCreateManagedWidget ("Range 2 ",
    xmToggleButtonWidgetClass, pms1_radio_box[count], NULL);
 
  button3[count] = XtVaCreateManagedWidget ("Range 3 ",
    xmToggleButtonWidgetClass, pms1_radio_box[count], NULL);
 
  XtAddCallback (button0[count], XmNvalueChangedCallback, 
                 (XtCallbackProc)callbackWrap,
                 (XtPointer)(buttonOffset(count) + PMS1_BUTTON0_VALUE));
  XtAddCallback (button1[count], XmNvalueChangedCallback, 
                 (XtCallbackProc)callbackWrap,
                 (XtPointer)(buttonOffset(count) + PMS1_BUTTON1_VALUE));
  XtAddCallback (button2[count], XmNvalueChangedCallback, 
                 (XtCallbackProc)callbackWrap,
                 (XtPointer)(buttonOffset(count) + PMS1_BUTTON2_VALUE));
  XtAddCallback (button3[count], XmNvalueChangedCallback, 
                 (XtCallbackProc)callbackWrap,
                 (XtPointer)(buttonOffset(count) + PMS1_BUTTON3_VALUE));
 
  XtManageChild(pms1_radio_box[count]);
                                    
  count++;
}
 
/* -------------------------------------------------------------------- */
void Pms1dWin::createAsas (char *name, char *probe_locn, char *dsm_locn,
       void (*callbackWrap)(Widget, XtPointer, XmToggleButtonCallbackStruct*))
 
// Creates an Asas of Pcas control box.
{
  XmString label;
 
// Create a pms 1d control radio box.
  pms1_radio_box[count] = XmCreateRadioBox (pms1_form, name, NULL, 0);
 
  if (!count)
    XtVaSetValues (pms1_radio_box[count],
//      XmNleftAttachment, XmATTACH_FORM,
//      XmNtopAttachment, XmATTACH_FORM,
      XmNorientation, XmHORIZONTAL,
      XmNisHomogeneous, FALSE,
      NULL);
  else
    XtVaSetValues (pms1_radio_box[count],
//      XmNleftAttachment, XmATTACH_FORM,
//      XmNtopAttachment, XmATTACH_WIDGET,
//      XmNtopWidget, pms1_radio_box[count - 1],
      XmNorientation, XmHORIZONTAL,
      XmNisHomogeneous, FALSE,
      NULL);
 
// Create the probe composite name from the name and location.
  makePms1Name (name, probe_locn);
  strcpy (location[count], dsm_locn);		// 
 
// Create the probe name label.
  label = XmStringCreateSimple (pms1_name[count]);
  pms1_label[count] = XtVaCreateManagedWidget (pms1_name[count],
    xmLabelWidgetClass, pms1_radio_box[count],
    XmNlabelString, label,
    NULL);
  XmStringFree (label);

// Make the probe pump off and on buttons.
  button0[count] = XtVaCreateManagedWidget ("Pump Off",
    xmToggleButtonWidgetClass, pms1_radio_box[count], NULL);

  button1[count] = XtVaCreateManagedWidget ("Pump On ",
    xmToggleButtonWidgetClass, pms1_radio_box[count], NULL);
 
  XtAddCallback (button0[count], XmNvalueChangedCallback, 
                 (XtCallbackProc)callbackWrap,
                 (XtPointer)(buttonOffset(count) + PMS1_BUTTON0_VALUE));
  XtAddCallback (button1[count], XmNvalueChangedCallback, 
                 (XtCallbackProc)callbackWrap,
                 (XtPointer)(buttonOffset(count) + PMS1_BUTTON1_VALUE));
 
  XtManageChild(pms1_radio_box[count]);
 
  count++;
}
 
/* -------------------------------------------------------------------- */
void Pms1dWin::callbackProc (Widget w, XtPointer client,
                             XmToggleButtonCallbackStruct *cbs)
{
// Just return if this is not a button setting event.
  if (!cbs->set)
    return;

  dsm_msg.sendPms1Msg (buttonValue ((int)client), 
                       pms1_name[buttonIndex((int)client)], 
                       location[buttonIndex((int)client)], "");
}

/* -------------------------------------------------------------------- */
void Pms1dWin::sendState()
{
  XmToggleButtonCallbackStruct	cbs;

  cbs.set = True;

  for (int i = 0; i < count; ++i)
    {
    if (strncmp(pms1_name[i], "FSSP", 4) == 0)
      {
      if (XmToggleButtonGetState(button0[i]))
        callbackProc(button0[i], (XtPointer)(buttonOffset(i) + PMS1_BUTTON0_VALUE), &cbs);

      if (XmToggleButtonGetState(button1[i]))
        callbackProc(button1[i], (XtPointer)(buttonOffset(i) + PMS1_BUTTON1_VALUE), &cbs);

      if (XmToggleButtonGetState(button2[i]))
        callbackProc(button2[i], (XtPointer)(buttonOffset(i) + PMS1_BUTTON2_VALUE), &cbs);

      if (XmToggleButtonGetState(button3[i]))
        callbackProc(button3[i], (XtPointer)(buttonOffset(i) + PMS1_BUTTON3_VALUE), &cbs);
      }
    else // PCASP
      {
      if (XmToggleButtonGetState(button0[i]))
        callbackProc(button0[i], (XtPointer)(buttonOffset(i) + PMS1_BUTTON0_VALUE), &cbs);

      if (XmToggleButtonGetState(button1[i]))
        callbackProc(button1[i], (XtPointer)(buttonOffset(i) + PMS1_BUTTON1_VALUE), &cbs);
      }
    }

}

/* -------------------------------------------------------------------- */
void Pms1dWin::makePms1Name (char *name, char *locn)

// Create the probe composite name from the name and location.
{
  strcpy (pms1_name[count], name);
  strcat (pms1_name[count], "_");
  strcat (pms1_name[count], locn);
}

/* END PMS1DWIN.CC */
