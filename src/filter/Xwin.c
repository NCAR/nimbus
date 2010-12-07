/*
-------------------------------------------------------------------------
OBJECT NAME:	Xwin.c

FULL NAME:	Create X window GUI

ENTRY POINTS:	CreateMainWindow()
		CreateSetupWindow()
		CreateProbeOutputMenu()
		CreateProbeDataQualityMenu()
		FlushXEvents()
		NextWidget()

STATIC FNS:	none

DESCRIPTION:	Create all or most widgets for the GUI.

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2005
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

Widget	menuBar, timeDisplayText, list1, goButton, logText, toggleProbeMenu,
	toggleDQMenu;
Widget	varNameLabel;
Widget	outputVarYes, outputVarNo;
Widget	outputLRbutton, outputSRbutton, outputHRbutton;
Widget	lagText, spikeText, synthConstText;
Widget	ev_text[16], slOpMenu,funcOpMenu;

extern void LoadSynthetic(Widget w, XtPointer client, XtPointer call);  //this may belong somewhere else
extern char *func[19];


/* -------------------------------------------------------------------- */
/* Not sure this is used anymore, other than maybe the widgets to store
 * file name data. (i.e. this was the original first window the user saw).
 */
Widget CreateMainWindow(Widget parent)
{
  Arg		args[32];
  Cardinal	n;
  Widget	topLevelForm;
  Widget	buttonFrame, buttonRC, quitButton;
  Widget	FNframe, fileNameRC, form[3];

  n = 0;
  topLevelForm = XmCreateForm(parent, (char *)"topLevelForm", args, n);

  /* Create Quit & Proceed buttons.
   */
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  buttonFrame = XmCreateFrame(topLevelForm, (char *)"buttonFrame", args, n);
  XtManageChild(buttonFrame);

  n = 0;
  buttonRC = XmCreateRowColumn(buttonFrame, (char *)"buttonRC", args, n);
  XtManageChild(buttonRC);

  n = 0;
  quitButton = XmCreatePushButton(buttonRC, (char *)"quitButton", args, n);
  XtAddCallback(quitButton, XmNactivateCallback, Quit, NULL);
  XtManageChild(quitButton);

  n = 0;
  readHeaderButton = XmCreatePushButton(buttonRC, (char *)"proceedButton", args, n);
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
  FNframe = XmCreateFrame(topLevelForm, (char *)"fileNameFrame", args, n);
  XtManageChild(FNframe);

  n = 0;
  fileNameRC = XmCreateRowColumn(FNframe, (char *)"fileNameRC", args, n);
  XtManageChild(fileNameRC);

  n = 0;
  form[0] = XmCreateRowColumn(fileNameRC, (char *)"fnRC", args, n);
  form[1] = XmCreateRowColumn(fileNameRC, (char *)"fnRC", args, n);

  n = 0;
  XtManageChild(XmCreateLabel(form[0], (char *)"FNlabel0", args, n));
  XtManageChild(XmCreateLabel(form[1], (char *)"FNlabel1", args, n));

  n = 0;
  aDSdataText = XmCreateTextField(form[0], (char *)"aDSdataText", args, n);
  XtAddCallback(aDSdataText, XmNactivateCallback,
                (XtCallbackProc)XmProcessTraversal,
                (XtPointer)XmTRAVERSE_NEXT_TAB_GROUP);
  XtManageChild(aDSdataText);

  n = 0;
  outputFileText = XmCreateTextField(form[1], (char *)"outputFileText", args, n);
  XtAddCallback(outputFileText, XmNactivateCallback,
                (XtCallbackProc)XmProcessTraversal,
                (XtPointer)XmTRAVERSE_NEXT_TAB_GROUP);
  XtManageChild(outputFileText);


  XtManageChildren(form, 2);

  return(topLevelForm);

}	/* END CREATEMAINWINDOW */

/* -------------------------------------------------------------------- */
/* This is the main window with menus and list widet, status text, etc.
 */
Widget CreateSetupWindow(Widget parent)
{
  Arg		args[16];
  Cardinal	n;
  XmString	xmstr[1];
  Widget	setupForm;
  Widget	pullDown[2], cascadeButton[2], b[16]; 
  Widget	listRC, listTitle, separ1, separ2;


  n = 0;
  setupForm = XmCreateForm(parent, (char *)"setupForm", args, n);


  /* Create Menus
   */
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  menuBar = XmCreateMenuBar(setupForm, (char *)"menuBar", args, n);
  XtManageChild(menuBar);

  n = 0;
  pullDown[0] = XmCreatePulldownMenu(menuBar, (char *)"setupPullDown", args, n);
  pullDown[1] = XmCreatePulldownMenu(menuBar, (char *)"editPullDown", args, n);

  n = 0;
  XtSetArg(args[n], XmNsubMenuId, pullDown[0]); ++n;
  cascadeButton[0] = XmCreateCascadeButton(menuBar, (char *)"File", args, n);

  n = 0;
  XtSetArg(args[n], XmNsubMenuId, pullDown[1]); ++n;
  cascadeButton[1] = XmCreateCascadeButton(menuBar, (char *)"Edit", args, n);

  XtManageChildren(cascadeButton, 2);
  XtManageChildren(pullDown, 2);

  n = 0;
  b[0] = XmCreatePushButton(pullDown[0], "loadSetupButton", args, n);
  b[1] = XmCreatePushButton(pullDown[0], "saveSetupButton", args, n);
  b[2] = XmCreateSeparator(pullDown[0], "setupSeparator", args, n);
  b[3] = XmCreatePushButton(pullDown[0], "printButton", args, n);
  b[4] = XmCreateSeparator(pullDown[0], "setupSeparator", args, n);
  b[5]=  XmCreatePushButton(pullDown[0], "synthButton", args, n);
  b[6] = XmCreateSeparator(pullDown[0], "setupSeparator", args, n);
  b[7] = XmCreatePushButton(pullDown[0], "cancelSetupButton", args, n);

  XtAddCallback(b[0], XmNactivateCallback, LoadSetup, NULL);
  XtAddCallback(b[1], XmNactivateCallback, SaveSetup, NULL);
  XtAddCallback(b[3], XmNactivateCallback, PrintSetup, NULL);
  XtAddCallback(b[5], XmNactivateCallback, LoadSynthetic, NULL);
  XtAddCallback(b[7], XmNactivateCallback, Quit, NULL);
  
  XtManageChildren(b, 8);

  n = 0;
  XtSetArg(args[n], XmNtearOffModel, XmTEAR_OFF_ENABLED); ++n;
  toggleProbeMenu = XmCreatePulldownMenu(pullDown[1], "editPR_CB", args, n);
  toggleDQMenu = XmCreatePulldownMenu(pullDown[1], "editDQ_CB", args, n);

  n = 0;
  b[0] = XmCreatePushButton(pullDown[1], "editConfiguration", args, n);
  b[1] = XmCreatePushButton(pullDown[1], "editDefaultsButton", args, n);
  b[2] = XmCreatePushButton(pullDown[1], "toggleOutput", args, n);
  b[3] = XmCreatePushButton(pullDown[1], "toggleRate", args, n);
  XtSetArg(args[n], XmNsubMenuId, toggleProbeMenu); ++n;
  b[4] = XmCreateCascadeButton(pullDown[1], "Toggle Probe", args, n);
  XtSetArg(args[n], XmNsubMenuId, toggleDQMenu); ++n;
  b[5] = XmCreateCascadeButton(pullDown[1], "Toggle Probe Data Quality", args, n);
  XtAddCallback(b[0], XmNactivateCallback, EditConfiguration, NULL);
  XtAddCallback(b[1], XmNactivateCallback, EditDefaultsFile, NULL);
  XtAddCallback(b[2], XmNactivateCallback, ToggleOutput, NULL);
  XtAddCallback(b[3], XmNactivateCallback, ToggleRate, NULL);
  XtManageChildren(b, 6);


  /* Go Button
   */
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, menuBar); n++;
  goButton = XmCreatePushButton(setupForm, "goButton", args, n);
  XtAddCallback(goButton, XmNactivateCallback, QueryOutputFile, NULL);
  XtManageChild(goButton);

  /* Time Display textfield
   */
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
//  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
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
/* This is the edit variable window.  User can edit cal coes, spike slopes
 * output rate, etc.
 */
Widget CreateEditWindow(Widget parent)
{
  Arg		args[16];
  Cardinal	n;
  Widget	editForm;
  Widget	frame[12];
  Widget	obRC, radioBox;
  Widget	form, label;
  Widget	cbRC, b[3];
  Widget	evRC[2];
  Widget	slPD, slButts[15],funcPD,funcButts[19];
  XmString	name;
  size_t	i;

  //**************Definition of FuncPD******************//
  func[0]="none";
  func[1]="abs";
  func[2]="acos";
  func[3]="asin";
  func[4]="atan";
  func[5]="ceil";
  func[6]="cos";
  func[7]="cosh";
  func[8]="exp";
  func[9]="fabs";
  func[10]="floor";
  func[11]="ln";
  func[12]="log";
  func[13]="pow";
  func[14]="sin";
  func[15]="sinh";
  func[16]="sqrt";
  func[17]="tan";
  func[18]="tanh";
 //***************************************************//



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
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopWidget, frame[4]); n++;
  frame[8] = XmCreateFrame(editForm, "synthConstFrame", args, n);
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopWidget, frame[8]); n++;
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
  XtSetArg(args[n], XmNbottomWidget, frame[8]); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, frame[4]); n++;
  frame[7] = XmCreateFrame(editForm, "dqFrame", args, n);
   n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget,frame[8]); n++;
  XtSetArg(args[n], XmNtopWidget, frame[4]); n++;
  frame[9] = XmCreateFrame(editForm, "funcmenuFrame", args, n);  

  XtManageChildren(frame, 10);

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


  /*Synthetic Constant Text
   */

  n = 0;
  form = XmCreateForm(frame[8], "synthConstForm", args, n);
  XtManageChild(form);

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  label = XmCreateLabel(form, "Constant Value", args, n);
  XtManageChild(label);

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, label); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  synthConstText = XmCreateTextField(form, "synthConstText", args, n);
  XtManageChild(synthConstText);


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
  XtManageChildren(b,3);
  

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
    name = XmStringCreateLocalized((char *)dataQuality[i]);

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


  /* Functions menu*/
  
  n=0;
  funcPD=XmCreatePulldownMenu(frame[9],"funcPD",args,n);

  name=XmStringCreateLocalized("Functions");
  n=0;
  XtSetArg(args[n],XmNlabelString,name);++n;
  XtSetArg(args[n],XmNsubMenuId,funcPD);++n;
  funcOpMenu=XmCreateOptionMenu(frame[9],"Function Menu",args,n);
  XtManageChild(funcOpMenu);
  XmStringFree(name);
  
  for (i = 0; i < 18; ++i)
  {
    name=XmStringCreateLocalized(func[i]);

    n=0;
    XtSetArg(args[n],XmNlabelString,name);++n;
    funcButts[i]=XmCreatePushButton(funcPD,(char *)"Function Menu",args,n);

    XmStringFree(name);
  }

  XtManageChildren(funcButts,i);
 
 //end functions menu

  return(editForm);

}	/* END CREATEEDITWINDOW */

/* -------------------------------------------------------------------- */
void CreateProbeOutputMenu()
{
  Widget b;
  std::vector<Widget>	btts;
  extern std::vector<struct probelist> probeList;

  b = XmCreatePushButton(toggleProbeMenu, (char *)"All Variables Off", NULL, 0);
  XtAddCallback(b, XmNactivateCallback, ToggleAllOff, NULL);
  btts.push_back(b);

  b = XmCreatePushButton(toggleProbeMenu, (char *)"All Variables On", NULL, 0);
  XtAddCallback(b, XmNactivateCallback, ToggleAllOn, NULL);
  btts.push_back(b);

  b = XmCreatePushButton(toggleProbeMenu, (char *)"All Derived Off", NULL, 0);
  XtAddCallback(b, XmNactivateCallback, ToggleAllDerivedOff, NULL);
  btts.push_back(b);

  for (size_t i = 0; i < probeList.size(); ++i)
    {
    b = XmCreatePushButton(toggleProbeMenu, probeList[i].label, NULL, 0);
    XtAddCallback(b, XmNactivateCallback, ToggleProbeOutput,
			(XtPointer)probeList[i].suffix);
    btts.push_back(b);
    }

  XtManageChildren(&btts[0], btts.size());

}	/* END CREATEPROBEOUTPUTMENU */

/* -------------------------------------------------------------------- */
void CreateProbeDataQualityMenu()
{
  Widget b;
  std::vector<Widget>	btts;
  extern std::vector<struct probelist> probeList;

  for (size_t i = 0; i < probeList.size(); ++i)
    {
    b = XmCreatePushButton(toggleDQMenu, probeList[i].label, NULL, 0);
    XtAddCallback(b, XmNactivateCallback, ToggleProbeDataQuality,
			(XtPointer)probeList[i].suffix);
    btts.push_back(b);
    }

  XtManageChildren(&btts[0], btts.size());

}	/* END CREATEPROBEDATAQUALLLITYMENU */

/* -------------------------------------------------------------------- */
void FlushXEvents()
{
  extern XtAppContext context;

  if (cfg.Interactive())
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
