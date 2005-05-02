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

static Widget	ConfigShell = 0, ConfigWindow = 0, flightText[nFlightInfo],
		lowRateButton, hrt25Button, hrt50Button, despikeButton,
		lagButton, irsCleanupButton, inertialShiftButton, interpB[3];

Widget ts_text[nTimeSliceInfo];

struct flightInfo
  {
  bool	Dirty;
  char	*Name;
  char	originalValue[10];
  } FlightInfo[nFlightInfo], TimeSliceInfo[nTimeSliceInfo];

extern char OutputFileName[];

static void	VerifyFlightInfo(Widget w, int indx, XtPointer call);
static void	markFlightInfoDirty(Widget w, int indx, XtPointer call);
static void	markTimeSliceDirty(Widget w, int indx, XtPointer call);
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
void SetDespiking(Widget w, XtPointer client, XmToggleButtonCallbackStruct *call)
{
  cfg.SetDespiking(call->set);
}

/* -------------------------------------------------------------------- */
void SetTimeShifting(Widget w, XtPointer client, XmToggleButtonCallbackStruct *call)
{
  cfg.SetTimeShifting(call->set);
}

/* -------------------------------------------------------------------- */
void SetHWcleanup(Widget w, XtPointer client, XmToggleButtonCallbackStruct *call)
{
  cfg.SetHoneyWellCleanup(call->set);
}

/* -------------------------------------------------------------------- */
void SetInertialShift(Widget w, XtPointer client, XmToggleButtonCallbackStruct *call)
{
  cfg.SetInertialShift(call->set);
}

/* -------------------------------------------------------------------- */
void SetConfigWinFromConfig()
{
  if (cfg.ProcessingRate() == Config::LowRate)
    XmToggleButtonSetState(lowRateButton, true, true);
  else
  if (cfg.ProcessingRate() == Config::HighRate)
    XmToggleButtonSetState(hrt25Button, true, true);
  else
    XmToggleButtonSetState(hrt50Button, true, true);

  XmToggleButtonSetState(interpB[(int)cfg.InterpolationType()], true, true);

  XmToggleButtonSetState(despikeButton, cfg.Despiking(), false);
  XmToggleButtonSetState(lagButton, cfg.TimeShifting(), false);
  XmToggleButtonSetState(despikeButton, cfg.HoneyWellCleanup(), false);
  XmToggleButtonSetState(despikeButton, cfg.InertialShift(), false);

}

/* -------------------------------------------------------------------- */
void ResetFlightInfo(Widget w, XtPointer client, XtPointer call)
{
  for (size_t i = 0; i < nFlightInfo; ++i)
    {
    FlightInfo[i].Dirty = false;
    XmTextFieldSetString(flightText[i], FlightInfo[i].originalValue);
    }

  for (size_t i = 0; i < nTimeSliceInfo; ++i)
    {
    TimeSliceInfo[i].Dirty = false;
    XmTextFieldSetString(ts_text[i], TimeSliceInfo[i].originalValue);
    }

/** @todo
 * We need a Set & Reset function.  Don't want reset being called when the
 * user pops up this window after loading a setup.  Setup stuff gets whiped
 * out.
 */

}	/* END RESETFLIGHTINFO */

/* -------------------------------------------------------------------- */
void SetInterpType(Widget w, XtPointer client, XtPointer call)
{
  int it = (int)client;

  cfg.SetInterpolationType((Config::interpolationType)it);
}

/* -------------------------------------------------------------------- */
void SetLowRate(Widget w, XtPointer client, XmToggleButtonCallbackStruct *call)
{
  size_t        i;

  if (w == 0)
    {
    XmToggleButtonSetState(lowRateButton, true, true);
    return;
    }

  if (call->set == false)
    return;

  cfg.SetProcessingRate(Config::LowRate);

  for (i = 0; i < sdi.size(); ++i)
    sdi[i]->OutputRate = Config::LowRate;

  for (i = 0; i < raw.size(); ++i)
    raw[i]->OutputRate = Config::LowRate;

  for (i = 0; i < derived.size(); ++i)
    derived[i]->OutputRate = Config::LowRate;

  FillListWidget();
  XtSetSensitive(outputHRbutton, false);

  if (cfg.ProductionRun())
    {
    char *p = strstr(OutputFileName, "h.");

    if (p)
      {
      strcpy(p, ".nc");
      XmTextFieldSetString(outputFileText, OutputFileName);
      }
    }
}       /* END SETLOWRATE */

/* -------------------------------------------------------------------- */
void SetHighRate(Widget w, XtPointer client, XmToggleButtonCallbackStruct *call)
{
  size_t        i;
  int	rate = (int)client;

  if (w == 0)
    {
    if (rate == (int)Config::HighRate)
      XmToggleButtonSetState(hrt25Button, true, true);
    else
    if (rate == (int)Config::HighRate50)
      XmToggleButtonSetState(hrt50Button, true, true);

    return;
    }

  if (call->set == false)
    return;

  if (rate == (int)Config::HighRate)
    cfg.SetProcessingRate(Config::HighRate);
  else
  if (rate == (int)Config::HighRate50)
    cfg.SetProcessingRate(Config::HighRate50);

  for (i = 0; i < sdi.size(); ++i)
    if (sdi[i]->SampleRate >= Config::HighRate)
      sdi[i]->OutputRate = (int)cfg.ProcessingRate();

  for (i = 0; i < raw.size(); ++i)
    {
    if (raw[i]->SampleRate >= Config::HighRate)
      raw[i]->OutputRate = (int)cfg.ProcessingRate();

    if (raw[i]->ProbeType & PROBE_PMS1D)
      raw[i]->OutputRate = raw[i]->SampleRate;
    }

  for (i = 0; i < derived.size(); ++i)
    if (derived[i]->Default_HR_OR == Config::LowRate)
      derived[i]->OutputRate = derived[i]->Default_HR_OR;
    else
      derived[i]->OutputRate = cfg.ProcessingRate();

  FillListWidget();
  XtSetSensitive(outputHRbutton, true);

  if (cfg.ProductionRun())
    {
    char *p = strchr(OutputFileName, '.');

    if (p)
      {
      strcpy(p, "h.nc");
      XmTextFieldSetString(outputFileText, OutputFileName);
      }
    }
}       /* END SETHIGHRATE */

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
  label = XmCreateLabel(efiFrame, "flightInfoTitle", args, n);
  XtManageChild(label);

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
		(XtCallbackProc)markFlightInfoDirty, (XtPointer)i);
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
  label = XmCreateLabel(tsFrame, "timeSliceTitle", args, n);
  XtManageChild(label);

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
                (XtCallbackProc)markTimeSliceDirty, (XtPointer)i);
    XtManageChild(ts_text[i]);
    }
}	/* END CREATETIMESLICE */

/* -------------------------------------------------------------------- */
void createProcessingRate(Widget parent)
{
  Arg args[8];
  Cardinal n;
  Widget rateFrame, rateRB, label;

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, menuBar); n++;
  rateFrame = XmCreateFrame(parent, "rateFrame", args, n);
  XtManageChild(rateFrame);

  n = 0;
  label = XmCreateLabel(rateFrame, "rateTitle", args, n);
  XtManageChild(label);

  n = 0;
  rateRB = XmCreateRadioBox(rateFrame, "rateRadioBox", args, n);
  XtManageChild(rateRB);

  n = 0;
  lowRateButton = XmCreateToggleButton(rateRB, "lowRateButton", args,n);
  XtAddCallback(lowRateButton, XmNvalueChangedCallback,
		(XtCallbackProc)SetLowRate, NULL);
  XtManageChild(lowRateButton);

  n = 0;
  hrt25Button = XmCreateToggleButton(rateRB, "hrt25Button", args, n);
  XtAddCallback(hrt25Button, XmNvalueChangedCallback,
		(XtCallbackProc)SetHighRate, (XtPointer)Config::HighRate);
  XtManageChild(hrt25Button);

  n = 0;
  hrt50Button = XmCreateToggleButton(rateRB, "hrt50Button", args, n);
  XtAddCallback(hrt50Button, XmNvalueChangedCallback,
		(XtCallbackProc)SetHighRate, (XtPointer)Config::HighRate50);
  XtManageChild(hrt50Button);

}	/* END CREATEPROCESSINGRATE */

/* -------------------------------------------------------------------- */
void createInterpType(Widget parent)
{
  Arg args[8];
  Cardinal n;
  Widget interpFrame, interpRB, label;

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, menuBar); n++;
  interpFrame = XmCreateFrame(parent, "interpFrame", args, n);
  XtManageChild(interpFrame);

  n = 0;
  label = XmCreateLabel(interpFrame, "interpTitle", args, n);
  XtManageChild(label);

  n = 0;
  interpRB = XmCreateRadioBox(interpFrame, "interpRadioBox", args, n);
  XtManageChild(interpRB);

  n = 0;
  interpB[0] = XmCreateToggleButton(interpRB, "linearButton", args,n);
  XtAddCallback(interpB[0], XmNvalueChangedCallback,
		(XtCallbackProc)SetInterpType, (XtPointer)Config::Linear);

  n = 0;
  interpB[1] = XmCreateToggleButton(interpRB, "csplineButton", args,n);
  XtAddCallback(interpB[1], XmNvalueChangedCallback,
		(XtCallbackProc)SetInterpType, (XtPointer)Config::CubicSpline);

  n = 0;
  interpB[2] = XmCreateToggleButton(interpRB, "akimaButton", args, n);
  XtAddCallback(interpB[2], XmNvalueChangedCallback,
		(XtCallbackProc)SetInterpType, (XtPointer)Config::AkimaSpline);

  XtManageChildren(interpB, 3);

}	/* END CREATEINTERPTYPE */

/* -------------------------------------------------------------------- */
void createOptions(Widget parent)
{
  Arg args[8];
  Cardinal n;
  Widget optFrame, optRC, label;

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, menuBar); n++;
  optFrame = XmCreateFrame(parent, "optFrame", args, n);
  XtManageChild(optFrame);

  n = 0;
  label = XmCreateLabel(optFrame, "optionsTitle", args, n);
  XtManageChild(label);

  n = 0;
  optRC = XmCreateRowColumn(optFrame, "optCheckBox", args, n);
  XtManageChild(optRC);

  n = 0;
  despikeButton = XmCreateToggleButton(optRC, "despikeButton", args,n);
  XtAddCallback(despikeButton, XmNvalueChangedCallback,
		(XtCallbackProc)SetDespiking, NULL);

  n = 0;
  lagButton = XmCreateToggleButton(optRC, "lagButton", args, n);
  XtAddCallback(lagButton, XmNvalueChangedCallback,
		(XtCallbackProc)SetTimeShifting, NULL);

  n = 0;
  irsCleanupButton = XmCreateToggleButton(optRC, "hwCleanButton", args, n);
  XtAddCallback(irsCleanupButton, XmNvalueChangedCallback,
		(XtCallbackProc)SetHWcleanup, NULL);

  n = 0;
  inertialShiftButton = XmCreateToggleButton(optRC, "inertialShift", args, n);
  XtAddCallback(inertialShiftButton, XmNvalueChangedCallback,
		(XtCallbackProc)SetInertialShift, NULL);

  XtManageChild(despikeButton);
  XtManageChild(lagButton);
  XtManageChild(irsCleanupButton);
  XtManageChild(inertialShiftButton);

}       /* END CREATEOPTIONS */

/* -------------------------------------------------------------------- */
void DismissConfigWindow(Widget w, XtPointer client, XtPointer call)
{
  if (ConfigWindow)
    {
    XtPopdown(XtParent(ConfigWindow));
    XtUnmanageChild(ConfigWindow);
    }
}	/* END DISMISSCONFIGWINDOW */

/* -------------------------------------------------------------------- */
static void markFlightInfoDirty(Widget w, int indx, XtPointer call)
{
  FlightInfo[indx].Dirty = true;

}	/* END MARKDIRTY */

/* -------------------------------------------------------------------- */
static void markTimeSliceDirty(Widget w, int indx, XtPointer call)
{
  TimeSliceInfo[indx].Dirty = true;

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

  extern Widget	Shell001, AppShell;

  FlightInfo[0].Name = "   Project Number";
  FlightInfo[1].Name = "    Flight Number";
  FlightInfo[2].Name = "Date [MM/DD/YYYY]";
  FlightInfo[3].Name = "         Aircraft";

  TimeSliceInfo[0].Name = "Start Time";
  TimeSliceInfo[1].Name = "End Time";


  n = 0;
  XtSetArg(args[n], XmNtitle, "Processor Configuration"); n++;
  XtSetArg(args[n], XmNtransientFor, Shell001); n++;
  ConfigShell = XtCreatePopupShell("editFlightShell",
		transientShellWidgetClass, AppShell, args, n);

  n = 0;
  ConfigWindow = XmCreateRowColumn(ConfigShell, "configWindow", args, n);

  createFlightInfo(ConfigWindow);
  createTimeSlice(ConfigWindow);
  createProcessingRate(ConfigWindow);
  createInterpType(ConfigWindow);
  createOptions(ConfigWindow);


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

  SetConfigWinFromConfig();
  XtManageChild(ConfigWindow);
  XtPopup(XtParent(ConfigWindow), XtGrabNone);

}	/* END EDITFLIGHTINFO */

/* END CONFIG_WIN.C */
