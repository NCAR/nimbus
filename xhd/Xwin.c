/*
-------------------------------------------------------------------------
OBJECT NAME:	Xwin.c

FULL NAME:	CreateX window

ENTRY POINTS:	CreateHDwindow()


STATIC FNS:	none

DESCRIPTION:	

REFERENCES:	

REFERENCED BY:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1996-2001
-------------------------------------------------------------------------
*/

#include <stdlib.h>

#include <Xm/CascadeB.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Separator.h>
#include <Xm/Text.h>

extern Widget	hdText, HDwindow;

void Save(Widget w, XtPointer client, XtPointer call);
void Print(Widget w, XtPointer client, XtPointer call);
void Quit(Widget w, XtPointer client, XtPointer call);
void HeaderDump(Widget w, XtPointer client, XtPointer call);
void DumpPMS(Widget w, XtPointer client, XtPointer call);
void LoadFile(Widget w, XtPointer client, XtPointer call);


/* -------------------------------------------------------------------- */
void CreateHDwindow(Widget parent)
{
  Arg         args[32];
  Cardinal    n;
  Widget      menuBar, pullDown[2], cascadeButton[2], b[64];

  n = 0;
  HDwindow = XmCreateForm(parent, "topLevelForm", args, n);

  /* Create Menus
   */
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  menuBar = XmCreateMenuBar(HDwindow, "menuBar", args, n);
  XtManageChild(menuBar);

  n = 0;
  pullDown[0] = XmCreatePulldownMenu(menuBar, "filePullDown", args, n);
  pullDown[1] = XmCreatePulldownMenu(menuBar, "viewPullDown", args, n);

  n = 0;
  XtSetArg(args[n], XmNsubMenuId, pullDown[0]); ++n;
  cascadeButton[0] = XmCreateCascadeButton(menuBar, "filePD_CB", args, n);

  n = 0;
  XtSetArg(args[n], XmNsubMenuId, pullDown[1]); ++n;
  cascadeButton[1] = XmCreateCascadeButton(menuBar, "viewPD_CB", args, n);

  XtManageChildren(cascadeButton, 2);
  XtManageChildren(pullDown, 2);


  n = 0;
  b[0] = XmCreatePushButton(pullDown[0], "reloadButton", args, n);
  b[1] = XmCreatePushButton(pullDown[0], "saveButton", args, n);
  b[2] = XmCreatePushButton(pullDown[0], "printButton", args, n);
  b[3] = XmCreatePushButton(pullDown[0], "quitButton", args, n);
  XtAddCallback(b[0], XmNactivateCallback, HeaderDump, (XtPointer)0);
  XtAddCallback(b[1], XmNactivateCallback, Save, NULL);
  XtAddCallback(b[2], XmNactivateCallback, Print, NULL);
  XtAddCallback(b[3], XmNactivateCallback, Quit, NULL);
  XtManageChildren(b, 4);

  n = 0;
  b[0] = XmCreatePushButton(pullDown[1], "viewRegular", args, n);
  b[1] = XmCreatePushButton(pullDown[1], "viewPinout", args, n);
  b[2] = XmCreatePushButton(pullDown[1], "viewPMS", args, n);
  b[3] = XmCreateSeparator(pullDown[1], "sep", args, n);
  b[4] = XmCreatePushButton(pullDown[1], "S100 - 20 - Water", args, n);
  b[5] = XmCreatePushButton(pullDown[1], "S100 - 20 - Glass", args, n);
  b[6] = XmCreatePushButton(pullDown[1], "S100 - 30 - Water", args, n);
  b[7] = XmCreatePushButton(pullDown[1], "S100 - 30 - Glass", args, n);
  b[8] = XmCreatePushButton(pullDown[1], "S100 - 40 - Water", args, n);
  b[9] = XmCreatePushButton(pullDown[1], "S100 - 40 - Glass", args, n);
  b[10] = XmCreateSeparator(pullDown[1], "sep", args, n);
  b[11] = XmCreatePushButton(pullDown[1], "S200 - 20 - PSL", args, n);
  b[12] = XmCreatePushButton(pullDown[1], "S200 - 30 - PSL", args, n);
  b[13] = XmCreatePushButton(pullDown[1], "S200 - 40 - PSL", args, n);
  b[14] = XmCreateSeparator(pullDown[1], "sep", args, n);
  b[15] = XmCreatePushButton(pullDown[1], "S300 - 20 - Water", args, n);
  b[16] = XmCreatePushButton(pullDown[1], "S300 - 20 - Glass", args, n);
  b[17] = XmCreatePushButton(pullDown[1], "S300 - 20 - PSL", args, n);
  b[18] = XmCreatePushButton(pullDown[1], "S300 - 30 - Water", args, n);
  b[19] = XmCreatePushButton(pullDown[1], "S300 - 30 - Glass", args, n);
  b[20] = XmCreatePushButton(pullDown[1], "S300 - 30 - PSL", args, n);
  b[21] = XmCreatePushButton(pullDown[1], "S300 - 40 - Water", args, n);
  b[22] = XmCreatePushButton(pullDown[1], "S300 - 40 - Glass", args, n);
  b[23] = XmCreatePushButton(pullDown[1], "S300 - 40 - PSL", args, n);
  XtAddCallback(b[0], XmNactivateCallback, HeaderDump, (XtPointer)0);
  XtAddCallback(b[1], XmNactivateCallback, HeaderDump, (XtPointer)1);
  XtAddCallback(b[2], XmNactivateCallback, DumpPMS, (XtPointer)0);
  XtAddCallback(b[4], XmNactivateCallback, LoadFile, "s100-20-water");
  XtAddCallback(b[5], XmNactivateCallback, LoadFile, "s100-20-glass");
  XtAddCallback(b[6], XmNactivateCallback, LoadFile, "s100-30-water");
  XtAddCallback(b[7], XmNactivateCallback, LoadFile, "s100-30-glass");
  XtAddCallback(b[8], XmNactivateCallback, LoadFile, "s100-40-water");
  XtAddCallback(b[9], XmNactivateCallback, LoadFile, "s100-40-glass");
  XtAddCallback(b[11], XmNactivateCallback, LoadFile, "s200-20-psl");
  XtAddCallback(b[12], XmNactivateCallback, LoadFile, "s200-30-psl");
  XtAddCallback(b[13], XmNactivateCallback, LoadFile, "s200-40-psl");
  XtAddCallback(b[15], XmNactivateCallback, LoadFile, "s300-20-water");
  XtAddCallback(b[16], XmNactivateCallback, LoadFile, "s300-20-glass");
  XtAddCallback(b[17], XmNactivateCallback, LoadFile, "s300-20-psl");
  XtAddCallback(b[18], XmNactivateCallback, LoadFile, "s300-30-water");
  XtAddCallback(b[19], XmNactivateCallback, LoadFile, "s300-30-glass");
  XtAddCallback(b[20], XmNactivateCallback, LoadFile, "s300-30-psl");
  XtAddCallback(b[21], XmNactivateCallback, LoadFile, "s300-40-water");
  XtAddCallback(b[22], XmNactivateCallback, LoadFile, "s300-40-glass");
  XtAddCallback(b[23], XmNactivateCallback, LoadFile, "s300-40-psl");
  XtManageChildren(b, 24);



  /* The Text Widget
   */
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, menuBar); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  hdText = XmCreateScrolledText(HDwindow, "hdText", args, n);
  XtManageChild(hdText);

}	/* END CREATEHDWINDOW */

/* END XWIN.C */
