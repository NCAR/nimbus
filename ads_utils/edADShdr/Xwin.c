/*
-------------------------------------------------------------------------
OBJECT NAME:	Xwin.c

FULL NAME:	Create X window GUI

ENTRY POINTS:	CreateMainWindow()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	edADShdr.c (main)

COPYRIGHT:	University Corporation for Atmospheric Research, 2000
-------------------------------------------------------------------------
*/

#include <Xm/CascadeB.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Separator.h>
#include <Xm/TextF.h>

#include "define.h"


/* Global widget declarations.
 */
Widget	EFtext[16], opMenu, x;
Widget	list, fixedButton, floatButton;

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
  Widget	b[10], acceptButton, clearButton;
  Widget	separ, typeRB;
  Widget	editFieldRC, EFform[16], EFlabel[16];

  n = 0;
  topLevelForm = XmCreateForm(parent, "topLevelForm", args, n);


  /* Create Menus
   */
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  menuBar = XmCreateMenuBar(topLevelForm, "menuBar", args, n);
  XtManageChild(menuBar);

  n = 0;
  pullDown[0] = XmCreatePulldownMenu(menuBar, "filePullDown", args, n);
  pullDown[1] = XmCreatePulldownMenu(menuBar, "editPullDown", args, n);

  n = 0;
  XtSetArg(args[n], XmNsubMenuId, pullDown[0]); ++n;
  cascadeButton[0] = XmCreateCascadeButton(menuBar, "filePD_CB", args, n);

  n = 0;
  XtSetArg(args[n], XmNsubMenuId, pullDown[1]); ++n;
  cascadeButton[1] = XmCreateCascadeButton(menuBar, "editPD_CB", args, n);

  XtManageChildren(cascadeButton, 1);
  XtManageChildren(pullDown, 1);

  n = 0;
  b[0] = XmCreatePushButton(pullDown[0], "openNewFile", args, n);
  b[1] = XmCreatePushButton(pullDown[0], "saveFile", args, n);
  b[2] = XmCreatePushButton(pullDown[0], "quit", args, n);
  XtAddCallback(b[0], XmNactivateCallback, OpenNewFile, NULL);
  XtAddCallback(b[1], XmNactivateCallback, SaveFile, NULL);
  XtAddCallback(b[2], XmNactivateCallback, Quit, NULL);
  XtManageChildren(b, 3);
/*
  n = 0;
  b[0] = XmCreatePushButton(pullDown[1], "clearVar", args, n);
  b[1] = XmCreatePushButton(pullDown[1], "deleteVar", args, n);
  b[2] = XmCreatePushButton(pullDown[1], "resetVar", args, n);
  XtAddCallback(b[0], XmNactivateCallback, Clear, NULL);
  XtAddCallback(b[1], XmNactivateCallback, Delete, NULL);
  XtAddCallback(b[2], XmNactivateCallback, EditVariable, NULL);
  XtManageChildren(b, 3);
*/


  /* Separator
   */
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, menuBar); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  separ = XmCreateSeparator(topLevelForm, "separ1", args, n);
  XtManageChild(separ);


  /* Create Form & Label widgets.
   */
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, separ); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  editFieldRC = XmCreateRowColumn(topLevelForm, "editFieldRC", args, n);
  XtManageChild(editFieldRC);

  for (i = 0; i < 6; ++i)
    {
    n = 0;
    sprintf(buffer, "EFform%d", i);
    EFform[i] = XmCreateForm(editFieldRC, buffer, args, n);

    sprintf(buffer, "EFlabel%d", i);

    n = 0;
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
    EFlabel[i] = XmCreateLabel(EFform[i], buffer, args, n);
    XtManageChild(EFlabel[i]);
    }

  XtManageChildren(EFform, 6);


  /* Add TextField widgets
   */
  for (i = 0; i < 6; ++i)
    {
    n = 0;
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
    XtSetArg(args[n], XmNleftWidget, EFlabel[i]); n++;
    sprintf(buffer, "EFtext%d", i);
    EFtext[i] = XmCreateTextField(EFform[i], buffer, args, n);
    XtAddCallback(EFtext[i], XmNactivateCallback,
    	XmProcessTraversal, (XtPointer)XmTRAVERSE_NEXT_TAB_GROUP);
    XtManageChild(EFtext[i]);
    }

  return(topLevelForm);

}	/* END CREATEMAINWINDOW */

/* END XWIN.C */
