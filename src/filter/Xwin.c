/*
-------------------------------------------------------------------------
OBJECT NAME:	Xwin.c

FULL NAME:	Create X window GUI

ENTRY POINTS:	CreateMainWindow()
		CreateSetupWindow()
		CreateEditWindow()
		CreateProbeMenu()
		FlushXEvents()

STATIC FNS:	NextWidget()

DESCRIPTION:	Create all or most widgets for the GUI.

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	nimbus.c (main)

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2000
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

#include "nimbus.h"
#include "gui.h"


/* Global widget declarations.
 */
Widget	readHeaderButton;
Widget	aDSdataText, outputFileText;

Widget	menuBar, timeDisplayText, list1, goButton, logText, pullRight,
	lowRateButton, highRateButton;

Widget	varNameLabel;
Widget	outputVarYes, outputVarNo;
Widget	outputLRbutton, outputSRbutton, outputHRbutton;
Widget	lagText, spikeText;
Widget	ev_text[16], slOpMenu;

extern char	*dataQuality[];

void	NextWidget(Widget w, int client, XtPointer call);

/* -------------------------------------------------------------------- */
Widget CreateMainWindow(Widget parent)
{
  Arg		args[32];
  Cardinal	n;
  Widget	topLevelForm;
  Widget	buttonFrame, buttonRC, quitButton;
  Widget	FNframe, fileNameRC, form[3];
  int		i;

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
  aDSdataText = XmCreateTextField(form[0], "aDSdataText", args, n);
  XtAddCallback(aDSdataText, XmNactivateCallback,
                (XtCallbackProc)XmProcessTraversal,
                (XtPointer)XmTRAVERSE_NEXT_TAB_GROUP);
  XtManageChild(aDSdataText);

  n = 0;
  outputFileText = XmCreateTextField(form[1], "outputFileText", args, n);
  XtAddCallback(outputFileText, XmNactivateCallback,
                (XtCallbackProc)XmProcessTraversal,
                (XtPointer)XmTRAVERSE_NEXT_TAB_GROUP);
  XtManageChild(outputFileText);


  XtManageChildren(form, 2);

  return(topLevelForm);

}	/* END CREATEMAINWINDOW */

/* -------------------------------------------------------------------- */
Widget CreateSetupWindow(Widget parent)
{
  Arg		args[16];
  Cardinal	n;
  XmString	xmstr[1];
  Widget	setupForm;
  Widget	rateFrame, rateRB;
  Widget	pullDown[2], cascadeButton[2], b[16];
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
  b[0] = XmCreatePushButton(pullDown[0], "loadSetupButton", args, n);
  b[1] = XmCreatePushButton(pullDown[0], "saveSetupButton", args, n);
  b[2] = XmCreateSeparator(pullDown[0], "setupSeparator", args, n);
  b[3] = XmCreatePushButton(pullDown[0], "printButton", args, n);
  b[4] = XmCreateSeparator(pullDown[0], "setupSeparator", args, n);
  b[5] = XmCreatePushButton(pullDown[0], "cancelSetupButton", args, n);
  XtAddCallback(b[0], XmNactivateCallback, LoadSetup, NULL);
  XtAddCallback(b[1], XmNactivateCallback, SaveSetup, NULL);
  XtAddCallback(b[3], XmNactivateCallback, PrintSetup, NULL);
  XtAddCallback(b[5], XmNactivateCallback, Quit, NULL);
  XtManageChildren(b, 6);

  n = 0;
  XtSetArg(args[n], XmNtearOffModel, XmTEAR_OFF_ENABLED); ++n;
  pullRight = XmCreatePulldownMenu(pullDown[1], "editPR_CB", args, n);

  n = 0;
  b[0] = XmCreatePushButton(pullDown[1], "editDefaultsButton", args, n);
  b[1] = XmCreatePushButton(pullDown[1], "editTimeSlices", args, n);
  b[2] = XmCreatePushButton(pullDown[1], "editFlightInfo", args, n);
  b[3] = XmCreatePushButton(pullDown[1], "toggleOutput", args, n);
  b[4] = XmCreatePushButton(pullDown[1], "toggleRate", args, n);
  XtSetArg(args[n], XmNsubMenuId, pullRight); ++n;
  b[5] = XmCreateCascadeButton(pullDown[1], "toggleProbe", args, n);
  XtAddCallback(b[0], XmNactivateCallback, EditDefaultsFile, NULL);
  XtAddCallback(b[1], XmNactivateCallback, EditTimeSlices, NULL);
  XtAddCallback(b[2], XmNactivateCallback, EditFlightInfo, NULL);
  XtAddCallback(b[3], XmNactivateCallback, ToggleOutput, NULL);
  XtAddCallback(b[4], XmNactivateCallback, ToggleRate, NULL);
  XtManageChildren(b, 6);


  /* Create Low/High Rate toggle box.
   */
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, menuBar); n++;
  rateFrame = XmCreateFrame(setupForm, "rateFrame", args, n);
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


  /* Go Button
   */
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, rateFrame); n++;
  goButton = XmCreatePushButton(setupForm, "goButton", args, n);
  XtAddCallback(goButton, XmNactivateCallback, QueryOutputFile, NULL);
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
  XtSetArg(args[n], XmNlabelString, (xmstr[0]=XmStringCreateLtoR("                  Sample Output Static Spike   Data    Cal Coes/\n  Name     Output  Rate   Rate   Lag   Slope   Qual  Dependencies", XmSTRING_DEFAULT_CHARSET))); n++;
  listTitle = XmCreateLabel(listRC, "listTitle", args, n);
  XtManageChild(listTitle);
  XmStringFree(xmstr[0]);

  n = 0;
  list1 = XmCreateScrolledList(listRC, "varList", args, n);
  XtAddCallback(list1, XmNdefaultActionCallback,
                (XtCallbackProc)EditVariable, NULL);
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
Widget CreateEditWindow(Widget parent)
{
  Arg		args[16];
  Cardinal	n;
  Widget	editForm;
  Widget	frame[10];
  Widget	obRC, radioBox;
  Widget	form, label;
  Widget	cbRC, b[3];
  Widget	evRC[2];
  Widget	slPD, slButts[15];
  XmString	name;
  int		i;

  n = 0;
  editForm = XmCreateForm(parent, "editForm", args, n);

  /* Create all frame's at once.
   */
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  frame[0] = XmCreateFrame(editForm, "varNameFrame", args, n);
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopWidget, frame[0]); n++;
  frame[1] = XmCreateFrame(editForm, "outputFrame", args, n);
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopWidget, frame[1]); n++;
  frame[2] = XmCreateFrame(editForm, "outRateFrame", args, n);
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopWidget, frame[2]); n++;
  frame[3] = XmCreateFrame(editForm, "lagFrame", args, n);
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopWidget, frame[3]); n++;
  frame[4] = XmCreateFrame(editForm, "spikeFrame", args, n);
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopWidget, frame[4]); n++;
  frame[5] = XmCreateFrame(editForm, "buttonFrame", args, n);
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, frame[0]); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, frame[1]); n++;
  frame[6] = XmCreateFrame(editForm, "evTextFrame1", args, n);
  n = 0;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, frame[6]); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNbottomWidget, frame[5]); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, frame[4]); n++;
  frame[7] = XmCreateFrame(editForm, "dqFrame", args, n);

  XtManageChildren(frame, 8);

  /* Var Name TF.
   */
  n = 0;
  varNameLabel = XmCreateLabel(frame[0], "varNameLabel", args, n);
  XtManageChild(varNameLabel);


  /* Output Box.
   */
  n = 0;
  obRC = XmCreateRowColumn(frame[1], "outputRC", args, n);
  XtManageChild(obRC);

  n = 0;
  label = XmCreateLabel(obRC, "outputLabel", args, n);
  XtManageChild(label);

  n = 0;
  radioBox = XmCreateRadioBox(obRC, "outputRB", args, n);
  XtManageChild(radioBox);

  n = 0;
  outputVarYes = XmCreateToggleButton(radioBox, "outputVarYes", args,n);
  XtManageChild(outputVarYes);

  n = 0;
  outputVarNo = XmCreateToggleButton(radioBox, "outputVarNo", args, n);
  XtManageChild(outputVarNo);


  /* OutputRate Box.
   */
  n = 0;
  obRC = XmCreateRowColumn(frame[2], "outRateRC", args, n);
  XtManageChild(obRC);

  n = 0;
  label = XmCreateLabel(obRC, "outRateLabel", args, n);
  XtManageChild(label);

  n = 0;
  radioBox = XmCreateRadioBox(obRC, "outRateRB", args, n);
  XtManageChild(radioBox);

  n = 0;
  outputLRbutton = XmCreateToggleButton(radioBox, "outputLR", args, n);
  XtManageChild(outputLRbutton);

  n = 0;
  outputSRbutton = XmCreateToggleButton(radioBox, "outputSR", args, n);
  XtManageChild(outputSRbutton);

  n = 0;
  outputHRbutton = XmCreateToggleButton(radioBox, "outputHR", args, n);
  XtManageChild(outputHRbutton);


  /* Lag Text.
   */
  n = 0;
  form = XmCreateForm(frame[3], "lagForm", args, n);
  XtManageChild(form);

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  label = XmCreateLabel(form, "lagLabel", args, n);
  XtManageChild(label);

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, label); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  lagText = XmCreateTextField(form, "lagText", args, n);
  XtManageChild(lagText);


  /* Spike Slope Text.
   */
  n = 0;
  form = XmCreateForm(frame[4], "spikeForm", args, n);
  XtManageChild(form);

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  label = XmCreateLabel(form, "spikeLabel", args, n);
  XtManageChild(label);

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, label); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  spikeText = XmCreateTextField(form, "spikeText", args, n);
  XtManageChild(spikeText);


  /* Command buttons.
   */
  n = 0;
  cbRC = XmCreateRowColumn(frame[5], "buttonRC", args, n);
  XtManageChild(cbRC);

  n = 0;
  b[0] = XmCreatePushButton(cbRC, "applyButton", args, n);
  b[1] = XmCreatePushButton(cbRC, "resetButton", args, n);
  b[2] = XmCreatePushButton(cbRC, "dismissButton", args, n);
  XtAddCallback(b[0], XmNactivateCallback, ApplyVariableMods, NULL);
  XtAddCallback(b[1], XmNactivateCallback, (XtCallbackProc)EditVariable,NULL);
  XtAddCallback(b[2], XmNactivateCallback, DismissEditWindow, NULL);
  XtManageChildren(b, 3);
  

  n = 0;
  label = XmCreateLabel(frame[6], "evTextTitle", args, n);
  XtManageChild(label);

  n = 0;
  evRC[0] = XmCreateRowColumn(frame[6], "evTextRC1", args, n);
  XtManageChild(evRC[0]);

  n = 0;

  for (i = 0; i < MAXDEPEND; ++i)
    {
    sprintf(buffer, "ev_text%d", i);
    ev_text[i] = XmCreateTextField(evRC[0], buffer, args, n);

    XtAddCallback(ev_text[i], XmNlosingFocusCallback, MapCalCof, NULL);
    XtAddCallback(ev_text[i], XmNactivateCallback, (XtCallbackProc)NextWidget,(XtPointer)i);
    }
  
  XtManageChildren(ev_text, MAXDEPEND);


  /* Data Quality Op Menu.
   */
  n = 0;
  slPD = XmCreatePulldownMenu(frame[7], "slPullDown", args, n);

  name = XmStringCreateLocalized("Data Quality");
  n = 0;
  XtSetArg(args[n], XmNlabelString, name); ++n;
  XtSetArg(args[n], XmNsubMenuId, slPD); ++n;
  slOpMenu = XmCreateOptionMenu(frame[7], "slOpMenu", args, n);
  XtManageChild(slOpMenu);
  XmStringFree(name);


  for (i = 0; dataQuality[i]; ++i)
    {
    name = XmStringCreateLocalized(dataQuality[i]);

    n = 0;
    XtSetArg(args[n], XmNlabelString, name); ++n;
    slButts[i] = XmCreatePushButton(slPD, "opMenB", args, n);
/*
    XtAddCallback(slButts[i], XmNactivateCallback, SetDataQuality,
                  (XtPointer)dataQuality[i]);
*/
    XmStringFree(name);
    }

  XtManageChildren(slButts, i);


  return(editForm);

}	/* END CREATEEDITWINDOW */

/* -------------------------------------------------------------------- */
void CreateProbeMenu()
{
  Widget	b[32];
  int		i;
  char		**probeNames;

  probeNames = GetProbeList();

  for (i = 0; probeNames[i] != NULL; ++i)
    {
    b[i] = XmCreatePushButton(pullRight, probeNames[i], NULL, 0);
    XtAddCallback(b[i], XmNactivateCallback, ToggleProbe,
			(XtPointer)GetProbeType(probeNames[i]));
    }

  XtManageChildren(b, i);

}	/* END CREATEPROBEMENU */

/* -------------------------------------------------------------------- */
void FlushXEvents()
{
  extern XtAppContext context;

  if (Interactive)
    while (XtAppPending(context))
      XtAppProcessEvent(context, XtIMAll);

}	/* END FLUSHXEVENTS */

/* -------------------------------------------------------------------- */
void NextWidget(Widget w, int client, XtPointer call)
{
  Widget	next;

  if (client >= 13)
    next = ev_text[0];
  else
    next = ev_text[client+1];

  XmProcessTraversal(next, XmTRAVERSE_CURRENT);

}	/* END NEXTWIDGET */

/* END XWIN.C */
