/*
-------------------------------------------------------------------------
OBJECT NAME:	cb_time.c

FULL NAME:	Callbacks for TimeSlice Window

ENTRY POINTS:	CreateTimeSliceWindow()
		EditTimeSlices()
		ValidateTime()
		DismissTimeSliceWindow()
		ResetTimeSliceWindow()

STATIC FNS:		

DESCRIPTION:	

INPUT:		

OUTPUT:		

REFERENCES:	

REFERENCED BY:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/TextF.h>

#include "nimbus.h"
#include "gui.h"

Widget	ts_text[MAX_TIME_SLICES*2];


/* -------------------------------------------------------------------- */
void EditTimeSlices(Widget w, XtPointer client, XtPointer call)
{
  XtManageChild(TimeSliceWindow);
  XtPopup(XtParent(TimeSliceWindow), XtGrabNone);

}	/* END EDITTIMESLICES */

/* -------------------------------------------------------------------- */
void ValidateTime(Widget w, XtPointer client, XtPointer call)
{
  int	hour, min, sec;
  char	*p = XmTextFieldGetString(w);

  if (strlen(p) == 0)
    return;

  hour = min = sec = 0;

  if (strchr(p, ':') != NULL)
    sscanf(p, "%u:%u:%u", &hour, &min, &sec);
  else
    sscanf(p, "%02u%02u%02u", &hour, &min, &sec);

  if (hour < 0)		hour = 0;
  if (hour > 48)	hour = 48;
  if (min < 0)		min = 0;
  if (min > 59)		min = 59;
  if (sec < 0)		sec = 0;
  if (sec > 59)		sec = 59;

  sprintf(buffer, "%02d:%02d:%02d", hour, min, sec);
  XmTextFieldSetString(w, buffer);
  XtFree(p);

}	/* END VALIDATETIME */

/* -------------------------------------------------------------------- */
void ResetTimeSliceWindow(Widget w, XtPointer client, XtPointer call)
{
  int	i;

  for (i = 0; i < MAX_TIME_SLICES*2; ++i)
    XmTextFieldSetString(ts_text[i], "");

}	/* END RESETTIMESLICEWINDOW */

/* -------------------------------------------------------------------- */
void DismissTimeSliceWindow(Widget w, XtPointer client, XtPointer call)
{
  XtPopdown(XtParent(TimeSliceWindow));
  XtUnmanageChild(TimeSliceWindow);

}	/* END DISMISSTIMESLICEWINDOW */

/* -------------------------------------------------------------------- */
Widget CreateTimeSliceWindow(Widget parent)
{
  Arg			args[32];
  Cardinal	n;
  XmString	xmstr[32];
  Widget		tsForm, slForm, tsFrame, label;
  Widget		tsRC, b[2], drFrame, drRC;
  int			i;

  n = 0;
  tsForm = XmCreateForm(parent, "timeSliceForm", args, n);

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  tsFrame = XmCreateFrame(tsForm, "tsFrame", args, n);
  XtManageChild(tsFrame);

  n = 0;
  slForm = XmCreateForm(tsFrame, "slForm", args, n);
  XtManageChild(slForm);

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNalignment, XmALIGNMENT_BEGINNING); n++;
  XtSetArg(args[n], XmNlabelString, (xmstr[0]=XmStringCreateLtoR(
	"      Time Slices\n\
   Start      Finish", XmSTRING_DEFAULT_CHARSET))); n++;
  label = XmCreateLabel(slForm, "TSlabel", args, n);
  XmStringFree( xmstr[0] );
  XtManageChild(label);


  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, label); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  tsRC = XmCreateRowColumn(slForm, "timeSliceRC", args, n);
  XtManageChild(tsRC);


  for (i = 0; i < MAX_TIME_SLICES * 2; ++i)
    {
    n = 0;
    sprintf(buffer, "tsText%d", i);
    ts_text[i] = XmCreateTextField(tsRC, buffer, args, n);

    XtAddCallback(ts_text[i], XmNlosingFocusCallback, ValidateTime, NULL);
    XtAddCallback(ts_text[i], XmNactivateCallback,
		(XtCallbackProc)XmProcessTraversal,
		(XtPointer)XmTRAVERSE_NEXT_TAB_GROUP);
    }

  XtManageChildren(ts_text, MAX_TIME_SLICES * 2);

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, tsFrame); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  drFrame = XmCreateFrame(tsForm, "buttonFrame", args, n);
  XtManageChild(drFrame);

  n = 0;
  drRC = XmCreateRowColumn(drFrame, "tsRC", args, n);
  XtManageChild(drRC);


  n = 0;
  b[0] = XmCreatePushButton(drRC, "resetButton", args, n);
  XtAddCallback(b[0], XmNactivateCallback, ResetTimeSliceWindow, NULL);

  n = 0;
  b[1] = XmCreatePushButton(drRC, "dismissButton", args, n);
  XtAddCallback(b[1], XmNactivateCallback, DismissTimeSliceWindow, NULL);
  XtManageChildren(b, 2);


  return(tsForm);

}	/* END CREATETIMESLICEWINDOW */

/* END CB_TIME.C */
