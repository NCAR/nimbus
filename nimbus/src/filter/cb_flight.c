/*
-------------------------------------------------------------------------
OBJECT NAME:	cb_flight.c

FULL NAME:	Edit Flight Info Window

ENTRY POINTS:	SetDefaultsValue()
		CreateFlightInfoWindow()
		EditFlightInfo()
		DismissFlightWindow()

STATIC FNS:	sMarkDirty()
		VerifyFlightInfo()
		ResetFlightInfo()

DESCRIPTION:	Allow user to edit Flight Header Info (i.e. Date, Flight
		number, etc...).

INPUT:		none

OUTPUT:			

REFERENCES:	OpenProjectFile()

REFERENCED BY:	hdr_decode.c, cb_main(), various AMLIB fn's.

COPYRIGHT:	University Corporation for Atmospheric Research, 1996
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

#include "nimbus.h"
#include "decode.h"
#include "gui.h"
#include "ctape.h"

#define nFlightInfo		4

static Widget		Shell005, FlightInfoWindow, flightText[nFlightInfo];

struct flightInfo
	{
	bool	Dirty;
	char	*Name;
	char	originalValue[10];
	} FlightInfo[nFlightInfo];

static void	VerifyFlightInfo(Widget w, int indx, XtPointer call);
static void	sMarkDirty(Widget w, int indx, XtPointer call);
static void	ResetFlightInfo(Widget w, XtPointer client, XtPointer call);


/* -------------------------------------------------------------------- */
/* Set a value being read in from a setup file */
void SetFlightValue(char target[], char new_value[])
{
  for (int i = 0; i < nFlightInfo; ++i)
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
  for (int i = 0; i < nFlightInfo; ++i)
    {
    FlightInfo[i].Dirty = false;
    XmTextFieldSetString(flightText[i], FlightInfo[i].originalValue);
    }

}	/* END RESETFLIGHTINFO */

/* -------------------------------------------------------------------- */
void CreateFlightInfoWindow()
{
  int		i;
  Arg		args[16];
  Cardinal	n;
  XmString	labelString;
  Widget	drFrame, drRC, efiFrame, b[2], defRC,
		form[nFlightInfo], label[nFlightInfo];

  extern Widget	AppShell;


  n = 0;
  XtSetArg(args[n], XmNtitle, "Edit Flight Info"); n++;
  XtSetArg(args[n], XmNtransientFor, Shell001); n++;
  Shell005 = XtCreatePopupShell("editFlightShell",
		transientShellWidgetClass, AppShell, args, n);

  n = 0;
  FlightInfoWindow = XmCreateForm(Shell005, "editFlightForm", args, n);


  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  efiFrame = XmCreateFrame(FlightInfoWindow, "efiFrame", args, n);
  XtManageChild(efiFrame);


  n = 0;
  defRC = XmCreateRowColumn(efiFrame, "defRC", args, n);
  XtManageChild(defRC);

  for (i = 0; i < nFlightInfo; ++i)
    {
    n = 0;
    form[i] = XmCreateRowColumn(defRC, "flightEntryRC", args, n);

    labelString = XmStringCreate(FlightInfo[i].Name,XmFONTLIST_DEFAULT_TAG);

    n = 0;
    XtSetArg(args[n], XmNlabelString, labelString); ++n;
    label[i] = XmCreateLabel(form[i], "lbl", args, n);
    XtManageChild(label[i]);
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


  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, defRC); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  drFrame = XmCreateFrame(FlightInfoWindow, "buttonFrame", args, n);
  XtManageChild(drFrame);

  n = 0;
  drRC = XmCreateRowColumn(drFrame, "efiRC", args, n);
  XtManageChild(drRC);


  n = 0;
  b[0] = XmCreatePushButton(drRC, "resetButton", args, n);
  XtAddCallback(b[0], XmNactivateCallback, ResetFlightInfo, NULL);

  n = 0;
  b[1] = XmCreatePushButton(drRC, "dismissButton", args, n);
  XtAddCallback(b[1], XmNactivateCallback, DismissFlightWindow, NULL);
  XtManageChildren(b, 2);

}	/* END CREATEFLIGHTINFOWINDOW */

/* -------------------------------------------------------------------- */
void EditFlightInfo(Widget w, XtPointer client, XtPointer call)
{
  static int	firstTime = true;

  if (firstTime)
    {
    char	*p;

    FlightInfo[0].Name = "   Project Number";
    FlightInfo[1].Name = "    Flight Number";
    FlightInfo[2].Name = "Date [MM/DD/YYYY]";
    FlightInfo[3].Name = "         Aircraft";

    CreateFlightInfoWindow();

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

    ResetFlightInfo(NULL, NULL, NULL);

    firstTime = false;
    }

  XtManageChild(FlightInfoWindow);
  XtPopup(XtParent(FlightInfoWindow), XtGrabNone);

}	/* END EDITFLIGHTINFO */

/* -------------------------------------------------------------------- */
void DismissFlightWindow(Widget w, XtPointer client, XtPointer call)
{
  XtPopdown(XtParent(FlightInfoWindow));
  XtUnmanageChild(FlightInfoWindow);

}	/* END DISMISSFLIGHTWINDOW */

/* -------------------------------------------------------------------- */
static void sMarkDirty(Widget w, int indx, XtPointer call)
{
  FlightInfo[indx].Dirty = true;

}	/* END MARKDIRTY */

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

/* END CB_FLIGHT.C */
