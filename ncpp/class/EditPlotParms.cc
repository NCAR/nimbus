/*
-------------------------------------------------------------------------
OBJECT NAME:	EditPlotParms.cc

COPYRIGHT:	University Corporation for Atmospheric Research, 1998-9
-------------------------------------------------------------------------
*/

#include "EditPlotParms.h"

#include <Xm/Frame.h>
#include <Xm/RowColumn.h>
#include <Xm/PushB.h>
#include <Xm/Text.h>


/* -------------------------------------------------------------------- */
EditPlotParms::EditPlotParms(const Widget parent, PlotType plotType, const char name[], int np, bool za) : WinForm(parent, name, RowColumn)
{
  Cardinal	n;
  Arg		args[8];
  Widget	drFrame, drRC, b[3];

  nPanels = np;
  zaxis = za;
  yright = false;

  RC[0] = createTitles(Window());
  RC[1] = createLabels(Window());
  RC[2] = createMinMax(Window());
  RC[3] = createNtics(Window());
  RC[4] = createLogLin(Window());
  RC[5] = createInvert(RC[4]);

  XtManageChild(RC[0]);
  XtManageChild(RC[1]);
  XtManageChild(RC[2]);
  XtManageChild(RC[3]);
  XtManageChild(RC[4]);
  XtManageChild(RC[5]);

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  drFrame = XmCreateFrame(Window(), (char*)"buttonFrame", args, n);
  XtManageChild(drFrame);
 
  n = 0;
  drRC = XmCreateRowColumn(drFrame, (char*)"buttonRC", args, n);
  XtManageChild(drRC);
 
  n = 0;
  b[0] = XmCreatePushButton(drRC, (char*)"applyButton", args, n);
 
  n = 0;
  b[1] = XmCreatePushButton(drRC, (char*)"resetButton", args, n);
 
  n = 0;
  b[2] = XmCreatePushButton(drRC, (char*)"dismissButton", args, n);
 
  XtManageChildren(b, 3);
 
 
  XtAddCallback(b[0],XmNactivateCallback,ApplyParmsWindow, (XtPointer)plotType);
  XtAddCallback(b[1],XmNactivateCallback,SetParmsWindow, (XtPointer)plotType);
  XtAddCallback(b[2],XmNactivateCallback,DismissWindow, (XtPointer)Window());

  XtAddCallback(autoTitles, XmNvalueChangedCallback, AutoTitle_CB,
						(XtPointer)plotType);

  XtAddCallback(autoLabels, XmNvalueChangedCallback, AutoLabel_CB,
						(XtPointer)plotType);

  XtAddCallback(autoScale, XmNvalueChangedCallback, AutoScale_CB,
						(XtPointer)plotType);

  XtAddCallback(autoTics, XmNvalueChangedCallback, AutoTic_CB,
						(XtPointer)plotType);

  if (nPanels > 1)
    for (int i = 0; i < nPanels; ++i)
      XtAddCallback(panelB[i], XmNvalueChangedCallback, SetParmsWindow, (XtPointer)plotType);

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
int EditPlotParms::CurrentPanel()
{
  if (nPanels > 1)
    for (int i = 0; i < nPanels; ++i)
      if (XmToggleButtonGetState(panelB[i]))
        return(i);

  return(0);

}	/* END CURRENTPANEL */

/* -------------------------------------------------------------------- */
void EditPlotParms::SetActiveNumberPanels(int n)
{
  if (nPanels == 1)
    return;

  for (int i = 0; i < nPanels; ++i)
    XtSetSensitive(panelB[i], (i < n));

  if (CurrentPanel() >= n)
    {
    XmToggleButtonSetState(panelB[CurrentPanel()], false, false);
    XmToggleButtonSetState(panelB[n-1], true, true);
    }

}	/* END SETACTIVENUMBERPANELS */

/* -------------------------------------------------------------------- */
Widget EditPlotParms::createLineItem(Widget parent, const char lbl[], const char txt[])
{
  Widget        label, text;

  tmpRC = XmCreateRowColumn(parent, (char *)"plRC", NULL, 0);
  label = XmCreateLabel(tmpRC, (char *)lbl, NULL, 0);
  text = XmCreateTextField(tmpRC, (char *)txt, NULL, 0);

  XtManageChild(label);
  XtManageChild(text);
  XtManageChild(tmpRC);

  return(text);

}	/* END CREATELINEITEM */

/* -------------------------------------------------------------------- */
Widget EditPlotParms::addToLineItem(const char lbl[], const char txt[])
{
  Widget        label, text;

  label = XmCreateLabel(tmpRC, (char *)lbl, NULL, 0);
  text = XmCreateTextField(tmpRC, (char *)txt, NULL, 0);

  XtManageChild(label);
  XtManageChild(text);

  return(text);

}       /* END ADDTOLINEITEM */

/* -------------------------------------------------------------------- */
void EditPlotParms::addPanelButtons(Widget parent)
{
  int		i;
  Widget	plRC, label;

  /* Panel stuff.
   */
  plRC = XmCreateRowColumn(parent, (char*)"plRC", NULL, 0);
  XtManageChild(plRC);

  label = XmCreateLabel(plRC, (char*)"Panel", NULL, 0);
  plRC = XmCreateRadioBox(plRC, (char*)"pnRC", NULL, 0);
  XtManageChild(label);
  XtManageChild(plRC);

  for (i = 0; i < nPanels; ++i)
    {
    sprintf(buffer, "%d", i+1);
    panelB[i] = XmCreateToggleButton(plRC, buffer, NULL, 0);
    }

  XtManageChildren(panelB, nPanels);
  XmToggleButtonSetState(panelB[0], true, false);

}       /* END ADDPANELBUTTONS */

/* -------------------------------------------------------------------- */
Widget EditPlotParms::createTitles(Widget parent)
{
  int           n;
  Arg           args[3];
  Widget        frame, RC;

  n = 0;
  frame = XmCreateFrame(parent, (char*)"titlesFrame", args, n);
  XtManageChild(frame);

  n = 0;
  RC = XmCreateRowColumn(frame, (char*)"titlesRC", args, n);

  autoTitles = XmCreateToggleButton(RC, (char*)"Auto-titles", args, n);
  XtManageChild(autoTitles);
  XmToggleButtonSetState(autoTitles, true, false);

  title = createLineItem(RC, "Title", "titleTxt");
  subTitle = createLineItem(RC, "Subtitle", "subTitleTxt");

  if (nPanels > 1)
    addPanelButtons(RC);

  return(RC);   /* In case we want to add more stuff    */

}       /* END CREATETITLES */

/* -------------------------------------------------------------------- */
Widget EditPlotParms::createLabels(Widget parent)
{
  int           n;
  Arg           args[3];
  Widget        frame, RC;

  n = 0;
  frame = XmCreateFrame(parent, (char*)"labelsFrame", args, n);
  XtManageChild(frame);

  n = 0;
  RC = XmCreateRowColumn(frame, (char*)"labelsRC", args, n);

  autoLabels = XmCreateToggleButton(RC, (char*)"Auto-labels", args, n);
  XtManageChild(autoLabels);

  xAxis.label = createLineItem(RC, "X Label", "labelTxt");
  yAxis[0].label = createLineItem(RC, "Y Label (left)", "labelTxt");
  XtManageChild(xAxis.label);
  XtManageChild(yAxis[0].label);

  if (yRight())
    {
    yAxis[1].label = createLineItem(RC, "Y Label (right)", "labelTxt");
    XtManageChild(yAxis[1].label);
    }


  if (isZaxis())
    {
    zAxis.label = createLineItem(RC, "Z Label", "labelTxt");
    XtManageChild(zAxis.label);
    }

  return(RC);   /* In case we want to add more stuff    */

}       /* END CREATELABELS */

/* -------------------------------------------------------------------- */
Widget EditPlotParms::createMinMax(Widget parent)
{
  int           n;
  Arg           args[3];
  Widget        frame, RC;

  n = 0;
  frame = XmCreateFrame(parent, (char*)"minMaxFrame", args, n);
  XtManageChild(frame);

  n = 0;
  RC = XmCreateRowColumn(frame, (char*)"minMaxRC", args, n);

  autoScale = XmCreateToggleButton(RC, (char*)"Auto-scaling", args, n);
  XtManageChild(autoScale);

  xAxis.min = createLineItem(RC, "X Min", "minMaxTxt");
  xAxis.max = addToLineItem("X Max", "minMaxTxt");
  yAxis[0].min = createLineItem(RC, "Y Min (left)", "minMaxTxt");
  yAxis[0].max = addToLineItem("Y Max (left)", "minMaxTxt");
  XtManageChild(xAxis.min); XtManageChild(xAxis.max);
  XtManageChild(yAxis[0].min); XtManageChild(yAxis[0].max);

  if (yRight())
    {
    yAxis[1].min = createLineItem(RC, "Y Min (right)", "minMaxTxt");
    yAxis[1].max = addToLineItem("Y Max (right)", "minMaxTxt");
    XtManageChild(yAxis[1].min); XtManageChild(yAxis[1].max);
    }

  if (isZaxis())
    {
    zAxis.min = createLineItem(RC, "Z Min", "minMaxTxt");
    zAxis.max = addToLineItem("Z Max", "minMaxTxt");
    XtManageChild(zAxis.min); XtManageChild(zAxis.max);
    }

  return(RC);

}	/* END CREATEMINMAX */

/* -------------------------------------------------------------------- */
Widget EditPlotParms::createNtics(Widget parent)
{
  int           n;
  Arg           args[3];
  Widget        frame, RC;

  n = 0;
  frame = XmCreateFrame(parent, (char *)"ticsFrame", args, n);
  XtManageChild(frame);

  n = 0;
  RC = XmCreateRowColumn(frame, (char *)"ticsRC", args, n);

  autoTics = XmCreateToggleButton(RC, (char *)"Auto-tic marks", args, n);
  XtManageChild(autoTics);

  xAxis.nMajorTics = createLineItem(RC, "# X tics", "ticTxt");
  xAxis.nMinorTics = addToLineItem("# minor", "ticTxt");
  yAxis[0].nMajorTics = createLineItem(RC, "# Y tics (left)", "ticTxt");
  yAxis[0].nMinorTics = addToLineItem("# minor (left)", "ticTxt");
  XtManageChild(xAxis.nMajorTics); XtManageChild(xAxis.nMinorTics);
  XtManageChild(yAxis[0].nMajorTics); XtManageChild(yAxis[0].nMinorTics);

  if (yRight())
    {
    yAxis[1].nMajorTics = createLineItem(RC, "# Y tics (right)", "ticTxt");
    yAxis[1].nMinorTics = addToLineItem("# minor (right)", "ticTxt");
    XtManageChild(yAxis[1].nMajorTics); XtManageChild(yAxis[1].nMinorTics);
    }

  if (isZaxis())
    {
    zAxis.nMajorTics = createLineItem(RC, "# Z tics", "ticTxt");
    zAxis.nMinorTics = addToLineItem("# minor", "ticTxt");
    XtManageChild(zAxis.nMajorTics); XtManageChild(zAxis.nMinorTics);
    }

  return(RC);

}	/* END CREATENTICS */

/* -------------------------------------------------------------------- */
Widget EditPlotParms::createLogLin(Widget parent)
{
  int           n;
  Arg           args[3];
  Widget        frame, RC, llRC, label;

  n = 0;
  frame = XmCreateFrame(parent, (char *)"logLinFrame", args, n);
  XtManageChild(frame);

  n = 0;
  RC = XmCreateRowColumn(frame, (char *)"logLinRC", args, n);
  XtManageChild(RC);

  n = 0;
  llRC = XmCreateRowColumn(RC, (char *)"plRC", args, n);
  XtManageChild(llRC);

  n = 0;
  label = XmCreateLabel(llRC, (char *)"Log scale :", args, n);
  XtManageChild(label);

  xAxis.logScale = XmCreateToggleButton(llRC, (char *)"X axis", args, n);
  yAxis[0].logScale = XmCreateToggleButton(llRC, (char *)"Y axis", args, n);
  XtManageChild(xAxis.logScale);
  XtManageChild(yAxis[0].logScale);
  XmToggleButtonSetState(xAxis.logScale, true, false);
  XmToggleButtonSetState(yAxis[0].logScale, true, false);

  if (yRight())
    {
    yAxis[1].logScale = XmCreateToggleButton(llRC, (char *)"Y (right)", args, n);
    XtManageChild(yAxis[1].logScale);
    XmToggleButtonSetState(yAxis[1].logScale, true, false);
    }

  if (isZaxis())
    {
    zAxis.logScale = XmCreateToggleButton(llRC, (char *)"Z axis", args, n);
    XtManageChild(zAxis.logScale);
    XmToggleButtonSetState(zAxis.logScale, true, false);
    }

  return(RC);

}	/* END CREATELOGLIN */

/* -------------------------------------------------------------------- */
Widget EditPlotParms::createInvert(Widget parent)
{
  int           n;
  Arg           args[3];
  Widget        RC, label;

  n = 0;
  RC = XmCreateRowColumn(parent, (char *)"plRC", args, n);
  XtManageChild(RC);

  label = XmCreateLabel(RC, (char *)"Invert scale :", args, n);
  xAxis.invertAxis = XmCreateToggleButton(RC, (char *)"X axis", args, n);
  yAxis[0].invertAxis = XmCreateToggleButton(RC, (char *)"Y axis", args, n);
  XtManageChild(label);
  XtManageChild(xAxis.invertAxis);
  XtManageChild(yAxis[0].invertAxis);

  if (yRight())
    {
    yAxis[1].invertAxis = XmCreateToggleButton(RC, (char *)"Y (right)", args, n);
    XtManageChild(yAxis[1].invertAxis);
    }

  if (isZaxis())
    {
    zAxis.invertAxis = XmCreateToggleButton(RC, (char *)"Z axis", args, n);
    XtManageChild(zAxis.invertAxis);
    }

  return(RC);

}	/* END CREATEINVERT */

/* END EDITPLOTPARMS.CC */
