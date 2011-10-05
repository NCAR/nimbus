/*
-------------------------------------------------------------------------
OBJECT NAME:	Xwin.c

FULL NAME:	Create X window GUI

ENTRY POINTS:	CreateMainWindow()

STATIC FNS:	none

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2011
-------------------------------------------------------------------------
*/

#include <stdio.h>

#include <Xm/CascadeB.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/List.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Separator.h>
#include <Xm/TextF.h>
#include <Xm/ToggleB.h>

#include "define.h"
#include <raf/vardb.h>


static const int nForms = 13;

/* Global widget declarations.
 */
Widget	EFtext[16], catMenu, stdNameMenu, catXx, stdNameXx;
Widget	list, analogButton, referenceButton;

extern char	buffer[];


/* -------------------------------------------------------------------- */
Widget CreateMainWindow(Widget parent)
{
  int		i, textCnt;
  Arg		args[32];
  Cardinal	n;
  Widget	topLevelForm;
  Widget	buttonFrame, buttonRC;
  Widget	menuBar, pullDown[3], cascadeButton[2];
  Widget	b[10], acceptButton;
  Widget	separ;
  Widget	editFieldRC, EFform[nForms], EFlabel[nForms];

  n = 0;
  topLevelForm = XmCreateForm(parent, (char *)"topLevelForm", args, n);


  /* Create Menus
   */
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  menuBar = XmCreateMenuBar(topLevelForm, (char *)"menuBar", args, n);
  XtManageChild(menuBar);

  n = 0;
  pullDown[0] = XmCreatePulldownMenu(menuBar, (char *)"filePullDown", args, n);
  pullDown[1] = XmCreatePulldownMenu(menuBar, (char *)"editPullDown", args, n);

  n = 0;
  XtSetArg(args[n], XmNsubMenuId, pullDown[0]); ++n;
  cascadeButton[0] = XmCreateCascadeButton(menuBar, (char *)"filePD_CB", args, n);

  n = 0;
  XtSetArg(args[n], XmNsubMenuId, pullDown[1]); ++n;
  cascadeButton[1] = XmCreateCascadeButton(menuBar, (char *)"editPD_CB", args, n);

  XtManageChildren(cascadeButton, 2);
  XtManageChildren(pullDown, 2);

  n = 0;
  b[0] = XmCreatePushButton(pullDown[0], (char *)"openNewFile", args, n);
  b[1] = XmCreatePushButton(pullDown[0], (char *)"saveFile", args, n);
  b[2] = XmCreatePushButton(pullDown[0], (char *)"saveFileAs", args, n);
  b[3] = XmCreatePushButton(pullDown[0], (char *)"quit", args, n);
  XtAddCallback(b[0], XmNactivateCallback, OpenNewFile, NULL);
  XtAddCallback(b[1], XmNactivateCallback, SaveFile, NULL);
  XtAddCallback(b[2], XmNactivateCallback, SaveFileAs, NULL);
  XtAddCallback(b[3], XmNactivateCallback, Quit, NULL);
  XtManageChildren(b, 4);

  n = 0;
  b[0] = XmCreatePushButton(pullDown[1], (char *)"clearVar", args, n);
  b[1] = XmCreatePushButton(pullDown[1], (char *)"deleteVar", args, n);
  b[2] = XmCreatePushButton(pullDown[1], (char *)"resetVar", args, n);
  XtAddCallback(b[0], XmNactivateCallback, Clear, NULL);
  XtAddCallback(b[1], XmNactivateCallback, Delete, NULL);
  XtAddCallback(b[2], XmNactivateCallback, (XtCallbackProc)EditVariable, NULL);
  XtManageChildren(b, 3);



  /* Separator
   */
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, menuBar); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  separ = XmCreateSeparator(topLevelForm, (char *)"separ1", args, n);
  XtManageChild(separ);


  /* Create Form & Label widgets.
   */
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, separ); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  editFieldRC = XmCreateRowColumn(topLevelForm, (char *)"editFieldRC", args, n);
  XtManageChild(editFieldRC);

  /* Create 11 Form and 10 Label widgets
   */
  for (i = 0; i < nForms; ++i)
    {
    n = 0;
    sprintf(buffer, "EFform%d", i);
    EFform[i] = XmCreateForm(editFieldRC, buffer, args, n);

    if (i < 10)
      {
      sprintf(buffer, "EFlabel%d", i);

      n = 0;
      XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
      XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
      XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
      EFlabel[i] = XmCreateLabel(EFform[i], buffer, args, n);
      XtManageChild(EFlabel[i]);
      }
    }

  XtManageChildren(EFform, nForms);

  n = 0;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  buttonFrame = XmCreateFrame(EFform[12], (char *)"buttonFrame", args, n);
  XtManageChild(buttonFrame);


  /* Add first 4 TextField widgets
   */
  for (i = 0; i < 4; ++i)
    {
    n = 0;
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
    XtSetArg(args[n], XmNleftWidget, EFlabel[i]); n++;
    sprintf(buffer, "EFtext%d", i);
    EFtext[i] = XmCreateTextField(EFform[i], buffer, args, n);
    XtAddCallback(EFtext[i], XmNactivateCallback,
		(XtCallbackProc)XmProcessTraversal,
		(XtPointer)XmTRAVERSE_NEXT_TAB_GROUP);
    XtManageChild(EFtext[i]);
    }


  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, EFlabel[4]); n++;
  analogButton = XmCreateToggleButton(EFform[4], (char *)"analogButton", args, n);
  XtManageChild(analogButton);


  for (i = 5, textCnt = 4; i < 10; ++i)
    {
    n = 0;
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
    XtSetArg(args[n], XmNleftWidget, EFlabel[i]); n++;


    sprintf(buffer, "EFtext%d", textCnt);
    EFtext[textCnt] = XmCreateTextField(EFform[i], buffer, args, n);
    XtAddCallback(EFtext[textCnt], XmNactivateCallback,
    		(XtCallbackProc)XmProcessTraversal,
		(XtPointer)XmTRAVERSE_NEXT_TAB_GROUP);
    XtManageChild(EFtext[textCnt]);
    ++textCnt;

    if (i == 5 || i == 9)	/* Add second text widget	*/
      {
      n = 0;
      XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
      XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM);n++;
      XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
      XtSetArg(args[n], XmNleftWidget, EFtext[textCnt-1]); n++;

      sprintf(buffer, "EFtext%d", textCnt);
      EFtext[textCnt] = XmCreateTextField(EFform[i], buffer, args, n);
      XtAddCallback(EFtext[textCnt], XmNactivateCallback,
		(XtCallbackProc)XmProcessTraversal,
		(XtPointer)XmTRAVERSE_NEXT_TAB_GROUP);
      XtManageChild(EFtext[textCnt]);
      ++textCnt;
      }
    }


  /* Create category option menu
   */
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightWidget, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNrightWidget, buttonFrame); n++;
  catMenu = XmCreatePulldownMenu(EFform[10], (char *)"optionMenu", args, n);

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightWidget, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNrightWidget, buttonFrame); n++;
  XtSetArg(args[n], XmNsubMenuId, catMenu); ++n;
  catXx = XmCreateOptionMenu(EFform[10], (char *)"catMenu", args, n);

  XtManageChild(catXx);

  /* Create standard_name option menu
   */
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, catMenu); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  stdNameMenu = XmCreatePulldownMenu(EFform[11], (char *)"optionMenu", args, n);

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightWidget, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNrightWidget, buttonFrame); n++;
  XtSetArg(args[n], XmNsubMenuId, stdNameMenu); ++n;
  stdNameXx = XmCreateOptionMenu(EFform[11], (char *)"stdNameMenu", args, n);

  XtManageChild(stdNameXx);

  n = 0;
  referenceButton = XmCreateToggleButton(EFform[12], (char *)"referenceButton", args, n);
  XtManageChild(referenceButton);

  /* Scrolled List of variable names
   */
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, separ); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, editFieldRC); n++;
  list = XmCreateScrolledList(topLevelForm, (char *)"varList", args, n);
  XtAddCallback(list, XmNbrowseSelectionCallback, (XtCallbackProc)EditVariable, NULL);
  XtManageChild(list);

  /* Create accept button
   */
  n = 0;
  buttonRC = XmCreateRowColumn(buttonFrame, (char *)"buttonRC", args, n);
  XtManageChild(buttonRC);

  n = 0;
  acceptButton = XmCreatePushButton(buttonRC, (char *)"acceptButton", args, n);
  XtAddCallback(acceptButton, XmNactivateCallback, Accept, (XtPointer)0);
  XtManageChild(acceptButton);


  return(topLevelForm);

}	/* END CREATEMAINWINDOW */

/* END XWIN.C */
