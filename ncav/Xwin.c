/*
-------------------------------------------------------------------------
OBJECT NAME:	Xwin.c

FULL NAME:	Create X window GUI

ENTRY POINTS:	CreateMainWindow()
		CreateSetupWindow()
		CreateEditWindow()
		CreateTimeSliceWindow()
		FlushXEvents()

STATIC FNS:		

DESCRIPTION:	

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	nimbus.c (main)

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include <Xm/CascadeB.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/List.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Separator.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/ToggleB.h>

#include "define.h"


/* Global widget declarations.
 */
Widget	readHeaderButton, inputFileText, outputFileText;

Widget	menuBar, timeDisplayText, list1, goButton, logText,
	lowRateButton, highRateButton;

Widget	ts_text[MAX_TIME_SLICES*2];


/* -------------------------------------------------------------------- */
Widget CreateMainWindow(parent)
Widget parent;
{
  Arg		args[32];
  Cardinal	n;
  Widget	topLevelForm;
  Widget	buttonFrame, buttonRC, quitButton;
  Widget	FNframe, fileNameRC, form[3];

  n = 0;
  topLevelForm = XmCreateForm(parent, "topLevelForm", args, n);

  /* Create Quit & Proceed buttons.
   */
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  buttonFrame = XmCreateFrame(topLevelForm, "buttonFrame", args, n);
  XtManageChild(buttonFrame);

  n = 0;
  buttonRC = XmCreateRowColumn(buttonFrame, "buttonRC", args, n);
  XtManageChild(buttonRC);

  n = 0;
  quitButton = XmCreatePushButton(buttonRC, "quitButton", args, n);
  XtAddCallback(quitButton, XmNactivateCallback, Quit, NULL);
  XtManageChild(quitButton);

  n = 0;
  readHeaderButton = XmCreatePushButton(buttonRC, "proceedButton", args, n);
  XtAddCallback(readHeaderButton, XmNactivateCallback, Proceed, NULL);
  XtManageChild(readHeaderButton);


  /* Create File Name entry fields.
   */
  n = 0;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, buttonFrame); n++;
  FNframe = XmCreateFrame(topLevelForm, "fileNameFrame", args, n);
  XtManageChild(FNframe);

  n = 0;
  fileNameRC = XmCreateRowColumn(FNframe, "fileNameRC", args, n);
  XtManageChild(fileNameRC);

  n = 0;
  form[0] = XmCreateRowColumn(fileNameRC, "fnRC", args, n);
  form[1] = XmCreateRowColumn(fileNameRC, "fnRC", args, n);

  n = 0;
  XtManageChild(XmCreateLabel(form[0], "FNlabel0", args, n));
  XtManageChild(XmCreateLabel(form[1], "FNlabel1", args, n));

  n = 0;
  inputFileText = XmCreateTextField(form[0], "inputFileText", args, n);
  XtAddCallback(inputFileText, XmNactivateCallback,
	XmProcessTraversal, (XtPointer)XmTRAVERSE_NEXT_TAB_GROUP);
  XtManageChild(inputFileText);

  n = 0;
  outputFileText = XmCreateTextField(form[1], "outputFileText", args, n);
  XtAddCallback(outputFileText, XmNactivateCallback,
	XmProcessTraversal, (XtPointer)XmTRAVERSE_NEXT_TAB_GROUP);
  XtManageChild(outputFileText);

  XtManageChildren(form, 2);

  return(topLevelForm);

}	/* END CREATEMAINWINDOW */

/* -------------------------------------------------------------------- */
Widget CreateSetupWindow(parent)
Widget	parent;
{
  Arg		args[16];
  Cardinal	n;
  XmString	xmstr[1];
  Widget	setupForm;
  Widget	pullDown[2], cascadeButton[2], b[6];
  Widget	listRC, listTitle, separ1, separ2;


  n = 0;
  setupForm = XmCreateForm(parent, "setupForm", args, n);


  /* Create Menus
   */
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  menuBar = XmCreateMenuBar(setupForm, "menuBar", args, n);
  XtManageChild(menuBar);

  n = 0;
  pullDown[0] = XmCreatePulldownMenu(menuBar, "setupPullDown", args, n);
  pullDown[1] = XmCreatePulldownMenu(menuBar, "editPullDown", args, n);

  n = 0;
  XtSetArg(args[n], XmNsubMenuId, pullDown[0]); ++n;
  cascadeButton[0] = XmCreateCascadeButton(menuBar, "setupPD_CB", args, n);

  n = 0;
  XtSetArg(args[n], XmNsubMenuId, pullDown[1]); ++n;
  cascadeButton[1] = XmCreateCascadeButton(menuBar, "editPD_CB", args, n);

  XtManageChildren(cascadeButton, 2);
  XtManageChildren(pullDown, 2);


  n = 0;
  b[0] = XmCreatePushButton(pullDown[0], "cancelSetupButton", args, n);
  XtAddCallback(b[0], XmNactivateCallback, CancelSetup, NULL);
  XtManageChild(b[0]);

  n = 0;
  b[0] = XmCreatePushButton(pullDown[1], "editTimeSlices", args, n);
  b[1] = XmCreatePushButton(pullDown[1], "toggleOutput", args, n);
  XtAddCallback(b[0], XmNactivateCallback, EditTimeSlices, NULL);
  XtAddCallback(b[1], XmNactivateCallback, ToggleOutput, NULL);
  XtManageChildren(b, 2);



  /* Go Button
   */
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, menuBar); n++;
  goButton = XmCreatePushButton(setupForm, "goButton", args, n);
  XtAddCallback(goButton, XmNactivateCallback, StartProcessing, NULL);
  XtManageChild(goButton);

  /* Time Display textfield
   */
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, goButton); n++;
  timeDisplayText = XmCreateTextField(setupForm, "timeDisplayText", args, n);
  XtManageChild(timeDisplayText);


  /* Create Separator.
   */
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, menuBar); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  separ1 = XmCreateSeparator(setupForm, "separ1", args, n);
  XtManageChild(separ1);


  /* Scrolled List
   */
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, separ1); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  listRC = XmCreateRowColumn(setupForm, "listRC", args, n);
  XtManageChild(listRC);

  n = 0;
  XtSetArg(args[n], XmNlabelString, (xmstr[0]=XmStringCreateLtoR(
	"  Name      Output  SR(hz)  OR(hz)", XmSTRING_DEFAULT_CHARSET))); n++;
  listTitle = XmCreateLabel(listRC, "listTitle", args, n);
  XtManageChild(listTitle);
  XmStringFree(xmstr[0]);

  n = 0;
  list1 = XmCreateScrolledList(listRC, "varList", args, n);
  XtAddCallback(list1, XmNdefaultActionCallback, ToggleOutput, NULL);
  XtManageChild(list1);


  /* Create Separator.
   */
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, listRC); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  separ2 = XmCreateSeparator(setupForm, "separ2", args, n);
  XtManageChild(separ2);


  /* Log Message Text
   */
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, separ2); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  logText = XmCreateScrolledText(setupForm, "logText", args, n);
  XtManageChild(logText);


  return(setupForm);

}	/* END CREATESETUPWINDOW */

/* -------------------------------------------------------------------- */
Widget CreateTimeSliceWindow(parent)
Widget	parent;
{
  Arg		args[512];
  Cardinal	n;
  XmString	xmstr[32];
  Widget	tsForm, tsFrame, slForm, label;
  Widget	tsRC, b[2], drFrame, drRC;
  int		i;

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
		XmProcessTraversal, (XtPointer)XmTRAVERSE_NEXT_TAB_GROUP);
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

/* -------------------------------------------------------------------- */
void FlushXEvents()
{
  extern XtAppContext context;

  if (Interactive)
    while (XtAppPending(context))
      XtAppProcessEvent(context, XtIMAll);

}	/* END FLUSHXEVENTS */

/* END XWIN.C */
