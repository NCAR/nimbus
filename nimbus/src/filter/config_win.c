/*
-------------------------------------------------------------------------
OBJECT NAME:	config_win.c

FULL NAME:	Processor configuration window.

ENTRY POINTS:	SetDefaultsValue()
		CreateFlightInfoWindow()
		EditFlightInfo()
		DismissFlightWindow()

STATIC FNS:	sMarkDirty()
		VerifyFlightInfo()
		ValidateTime()
		ResetFlightInfo()

DESCRIPTION:	Allow user to edit Flight Header Info (i.e. Date, Flight
		number, etc...).

INPUT:		none

OUTPUT:			

REFERENCES:	OpenProjectFile()

REFERENCED BY:	hdr_decode.c, cb_main(), various AMLIB fn's.

COPYRIGHT:	University Corporation for Atmospheric Research, 1996-2005
-------------------------------------------------------------------------
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/TextF.h>
#include <Xm/ToggleB.h>

#include "nimbus.h"
#include "decode.h"
#include "gui.h"
#include "ctape.h"

static const size_t nFlightInfo = 4;
static const size_t nTimeSliceInfo = 2;

static Widget	Shell005, ConfigWindow, flightText[nFlightInfo];

Widget ts_text[nTimeSliceInfo], lowRateButton, highRateButton;

struct flightInfo
  {
  bool	Dirty;
  char	*Name;
  char	originalValue[10];
  } FlightInfo[nFlightInfo], TimeSliceInfo[nTimeSliceInfo];

static void	VerifyFlightInfo(Widget w, int indx, XtPointer call);
static void	sMarkDirty(Widget w, int indx, XtPointer call);
static void	ResetFlightInfo(Widget w, XtPointer client, XtPointer call);


/* -------------------------------------------------------------------- */
/* Set a value being read in from a setup file */
void SetFlightValue(char target[], char new_value[])
{
  for (size_t i = 0; i < nFlightInfo; ++i)
    if (strcmp(FlightInfo[i].Name, target) == 0)
      {
      FlightInfo[i].Dirty = true;
      return;
      }

  sprintf(buffer, "Request to set non-existent default, %s.\n", target);
  LogMessage(buffer);

}	/* END SETDEFAULTSVALUE */

/* -------------------------------------------------------------------- */
void ResetFlightInfo(Widget w, XtPointer client, XtPointer call)
{
  for (size_t i = 0; i < nFlightInfo; ++i)
    {
    FlightInfo[i].Dirty = false;
    XmTextFieldSetString(flightText[i], FlightInfo[i].originalValue);
    }

  XmTextFieldSetString(ts_text[0], "");
  XmTextFieldSetString(ts_text[1], "");

}	/* END RESETFLIGHTINFO */

/* -------------------------------------------------------------------- */
void createFlightInfo(Widget parent)
{
  Arg		args[16];
  Cardinal	n;
  XmString	labelString;
  Widget	efiFrame, defRC, form[nFlightInfo], label;

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  efiFrame = XmCreateFrame(parent, "efiFrame", args, n);
  XtManageChild(efiFrame);


  n = 0;
  defRC = XmCreateRowColumn(efiFrame, "defRC", args, n);
  XtManageChild(defRC);

  for (size_t i = 0; i < nFlightInfo; ++i)
    {
    n = 0;
    form[i] = XmCreateRowColumn(defRC, "flightEntryRC", args, n);

    labelString = XmStringCreate(FlightInfo[i].Name, XmFONTLIST_DEFAULT_TAG);

    n = 0;
    XtSetArg(args[n], XmNlabelString, labelString); ++n;
    label = XmCreateLabel(form[i], "lbl", args, n);
    XtManageChild(label);
    XmStringFree(labelString);

    n = 0;
    XtSetArg(args[n], XmNcolumns, 10); n++;
    XtSetArg(args[n], XmNmaxLength, 10); n++;
    flightText[i] = XmCreateTextField(form[i], "txt", args, n);
    XtAddCallback(flightText[i], XmNlosingFocusCallback,
		(XtCallbackProc)VerifyFlightInfo, (XtPointer)i);
    XtAddCallback(flightText[i], XmNvalueChangedCallback,
		(XtCallbackProc)sMarkDirty, (XtPointer)i);
    XtManageChild(flightText[i]);
    }

  XtManageChildren(form, nFlightInfo);


}	/* END CREATEFLIGHTINFO */

/* -------------------------------------------------------------------- */
void createTimeSlice(Widget parent)
{
  Arg		args[16];
  Cardinal	n;
  XmString	labelString;
  Widget	tsFrame, tsRC, label;

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  tsFrame = XmCreateFrame(parent, "tsFrame", args, n);
  XtManageChild(tsFrame);

  n = 0;
  tsRC = XmCreateRowColumn(tsFrame, "flightEntryRC", args, n);
  XtManageChild(tsRC);

  for (size_t i = 0; i < nTimeSliceInfo; ++i)
    {
    labelString = XmStringCreate(TimeSliceInfo[i].Name, XmFONTLIST_DEFAULT_TAG);

    n = 0;
    XtSetArg(args[n], XmNlabelString, labelString); ++n;
    label = XmCreateLabel(tsRC, "lbl", args, n);
    XtManageChild(label);
    XmStringFree(labelString);

    n = 0;
    XtSetArg(args[n], XmNcolumns, 8); n++;
    XtSetArg(args[n], XmNmaxLength, 8); n++;
    ts_text[i] = XmCreateTextField(tsRC, "txt", args, n);
    XtAddCallback(ts_text[i], XmNlosingFocusCallback,
                (XtCallbackProc)ValidateTime, (XtPointer)i);
    XtAddCallback(ts_text[i], XmNvalueChangedCallback,
                (XtCallbackProc)sMarkDirty, (XtPointer)i);
    XtManageChild(ts_text[i]);
    }

}	/* END CREATETIMESLICE */

/* -------------------------------------------------------------------- */
void createProcessingRate(Widget parent)
{
  Arg args[8];
  Cardinal n;
  Widget rateFrame, rateRB;


  /* Create Low/High Rate toggle box.
   */
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, menuBar); n++;
  rateFrame = XmCreateFrame(parent, "rateFrame", args, n);
  XtManageChild(rateFrame);

  n = 0;
  rateRB = XmCreateRadioBox(rateFrame, "rateRadioBox", args, n);
  XtManageChild(rateRB);

  n = 0;
  lowRateButton = XmCreateToggleButton(rateRB, "lowRateButton", args,n);
  XtAddCallback(lowRateButton, XmNvalueChangedCallback,
                (XtCallbackProc)SetLowRate, NULL);
  XtManageChild(lowRateButton);

  n = 0;
  highRateButton = XmCreateToggleButton(rateRB, "highRateButton", args, n);
  XtAddCallback(highRateButton, XmNvalueChangedCallback,
                (XtCallbackProc)SetHighRate, NULL);
  XtManageChild(highRateButton);

}	/* END CREATEPROCESSINGRATE */

/* -------------------------------------------------------------------- */
void DismissConfigWindow(Widget w, XtPointer client, XtPointer call)
{
  XtPopdown(XtParent(ConfigWindow));
  XtUnmanageChild(ConfigWindow);

}	/* END DISMISSCONFIGWINDOW */

/* -------------------------------------------------------------------- */
static void sMarkDirty(Widget w, int indx, XtPointer call)
{
  FlightInfo[indx].Dirty = true;

}	/* END MARKDIRTY */

/* -------------------------------------------------------------------- */
void ValidateTime(Widget w, XtPointer client, XtPointer call)
{
  int   hour, min, sec;
  char  *p = XmTextFieldGetString(w);

  if (strlen(p) == 0)
    return;

  hour = min = sec = 0;

  if (strchr(p, ':') != NULL)
    sscanf(p, "%u:%u:%u", &hour, &min, &sec);
  else
    sscanf(p, "%02u%02u%02u", &hour, &min, &sec);

  if (hour < 0)         hour = 0;
  if (hour > 48)        hour = 48;
  if (min < 0)          min = 0;
  if (min > 59)         min = 59;
  if (sec < 0)          sec = 0;
  if (sec > 59)         sec = 59;

  sprintf(buffer, "%02d:%02d:%02d", hour, min, sec);
  XmTextFieldSetString(w, buffer);
  XtFree(p);

}       /* END VALIDATETIME */

/* -------------------------------------------------------------------- */
static void VerifyFlightInfo(Widget w, int indx, XtPointer call)
{
  int	i;
  char	*p, *p1;
  struct Fl   *fp = get_header();

  extern int  Aircraft;


  if (FlightInfo[indx].Dirty == false)
    return;


  p = XmTextFieldGetString(flightText[indx]);

  /* Verify values
   */
  switch (indx)
    {
    case 0:	/* Proj #	*/
      i = atoi(p);
      sprintf(buffer, "%d", i);
      buffer[7] = '\0';
      XmTextFieldSetString(flightText[indx], buffer);
      /* Need to set ProjectNumber variable.	*/
      break;

    case 1:	/* Flight #	*/
      strcpy(buffer, p);
      buffer[7] = '\0';
      strlower(buffer);
      XmTextFieldSetString(flightText[indx], buffer);
      strcpy(fp->fltnum, buffer);

      for (p1 = buffer; *p1 && !isdigit(*p1); ++p1)
        ;

      strcpy(FlightNumber, buffer);
      FlightNumberInt = atoi(p1);
      break;

    case 2:	/* Date		*/
      strcpy(buffer, p);
      buffer[10] = '\0';

      XmTextFieldSetString(flightText[indx], buffer);
      strcpy(fp->date, buffer);
      break;

    case 3:	/* Aircraft	*/
      strcpy(buffer, p);
      buffer[7] = '\0';
      strupr(buffer);
      XmTextFieldSetString(flightText[indx], buffer);
      strcpy(fp->acraft, buffer);

      for (p1 = buffer; *p1 && !isdigit(*p1); ++p1)
        ;

      Aircraft = atoi(p1);
      break;
    }

  XtFree(p);

}	/* END VERIFYFLIGHTINFO */

/* -------------------------------------------------------------------- */
void CreateConfigWindow()
{
  Arg		args[16];
  Cardinal	n;

  extern Widget	AppShell;


  n = 0;
  XtSetArg(args[n], XmNtitle, "Processor Configuration"); n++;
  XtSetArg(args[n], XmNtransientFor, Shell001); n++;
  Shell005 = XtCreatePopupShell("editFlightShell",
		transientShellWidgetClass, AppShell, args, n);

  n = 0;
  ConfigWindow = XmCreateRowColumn(Shell005, "configWindow", args, n);

  createFlightInfo(ConfigWindow);
  createTimeSlice(ConfigWindow);
  createProcessingRate(ConfigWindow);
//  createOptions(ConfigWindow);


  Widget	drFrame, drRC, b[2];

  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  drFrame = XmCreateFrame(ConfigWindow, "buttonFrame", args, n);
  XtManageChild(drFrame);

  n = 0;
  drRC = XmCreateRowColumn(drFrame, "efiRC", args, n);
  XtManageChild(drRC);


  n = 0;
  b[0] = XmCreatePushButton(drRC, "resetButton", args, n);
  XtAddCallback(b[0], XmNactivateCallback, ResetFlightInfo, NULL);

  n = 0;
  b[1] = XmCreatePushButton(drRC, "dismissButton", args, n);
  XtAddCallback(b[1], XmNactivateCallback, DismissConfigWindow, NULL);
  XtManageChildren(b, 2);

}	/* END CREATECONFIGWINDOW */

/* -------------------------------------------------------------------- */
void EditConfiguration(Widget w, XtPointer client, XtPointer call)
{
  static int	firstTime = true;

  if (firstTime)
    {
    char	*p;

    FlightInfo[0].Name = "   Project Number";
    FlightInfo[1].Name = "    Flight Number";
    FlightInfo[2].Name = "Date [MM/DD/YYYY]";
    FlightInfo[3].Name = "         Aircraft";

    TimeSliceInfo[0].Name = "Start Time";
    TimeSliceInfo[1].Name = "End Time";

    CreateConfigWindow();

    strcpy(FlightInfo[0].originalValue, ProjectNumber);
    XtSetSensitive(flightText[0], false);

    GetFlightNumber(&p);
    strcpy(FlightInfo[1].originalValue, p);

    GetHeaderDate(&p);
    strcpy(FlightInfo[2].originalValue, p);

    GetAircraft(&p);
    strcpy(FlightInfo[3].originalValue, p);
    if (strlen(p))
      XtSetSensitive(flightText[3], false);

/** @todo Time Slice should be saved in setup files.
 */
    ResetFlightInfo(NULL, NULL, NULL);

    firstTime = false;
    }

  XtManageChild(ConfigWindow);
  XtPopup(XtParent(ConfigWindow), XtGrabNone);

}	/* END EDITFLIGHTINFO */

/* END CONFIG_WIN.C */
