/*
-------------------------------------------------------------------------
OBJECT NAME:	CanvasWindow.cc

FULL NAME:	

ENTRY POINTS:	CanvasWindow()

STATIC FNS:	

DESCRIPTION:	

NOTES:		

COPYRIGHT:	University Corporation for Atmospheric Research, 1997-9
-------------------------------------------------------------------------
*/

#include "CanvasWindow.h"

struct menu
	{
	char            *title;
	void            (*callback)(Widget, XtPointer, XtPointer);
	XtPointer       callData;
	} ;
 
static struct menu	fileMenu[] = {
	{ "quit", Quit, NULL, },
	{ NULL, NULL, NULL }};
 
static struct menu	unitMenu[] = {
	"Counts", ToggleUnits, (XtPointer)COUNTS,
	"Volts", ToggleUnits, (XtPointer)VOLTS,
	"Engineering", ToggleUnits, (XtPointer)ENGINEERING,
	{ NULL, NULL, NULL }};

static struct menu	viewMenu[] = {
	"autoScale", DoAutoScale, NULL,
	"toggleGrid", ToggleGrid, NULL,
	"toggleFlash", ToggleFlash, NULL,
	{ NULL, NULL, NULL }};

static struct
	{
	char		*title;
	struct menu	*sub;
	} main_menu[] = {
		{ "File", fileMenu, },
		{ "Units", unitMenu, },
		{ "Options", viewMenu, },
		{ NULL, NULL }};
 

/* -------------------------------------------------------------------- */
CanvasWindow::CanvasWindow(const Widget parent) : WinForm(parent, "canvas", Form)
{
  int		i, j;
  Cardinal	n;
  Arg		args[8];
  Widget	menubar, menu[5], bttn[16], menu_button[5], form, form1, frame,
		brc, freeze;
  XmString	item;

  n = 0;
  form = XmCreateForm(Window(), "mainForm", args, n);

  n = 0;
  menubar = XmCreateMenuBar(form, "menuBar", args, n);
  XtManageChild(menubar);

  for (i = 0; main_menu[i].title; ++i)
    {
    n = 0;
    menu[i] = XmCreatePulldownMenu(menubar, main_menu[i].title, args, n);
 
    n = 0;
    XtSetArg(args[n], XmNsubMenuId, menu[i]); ++n;
    menu_button[i] = XmCreateCascadeButton(menubar, main_menu[i].title, args,n);
 
    for (j = 0; main_menu[i].sub[j].title; ++j)
      {
      n = 0;
 
      if (main_menu[i].sub[j].callback == NULL)
        {
        bttn[j] = XmCreateSeparator(menu[i], main_menu[i].sub[j].title, args,n);
        continue;
        }
 
      bttn[j] = XmCreatePushButton(menu[i], main_menu[i].sub[j].title, args, n);
      XtAddCallback(bttn[j], XmNactivateCallback,
        main_menu[i].sub[j].callback, (void *)main_menu[i].sub[j].callData);
      }
 
    XtManageChildren(bttn, j);
    }
 
  XtManageChildren(menu_button, i);



  /* Create Graphics Canvas
   */
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  form1 = XmCreateForm(form, "varForm", args, n);
  XtManageChild(form1);

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNrightWidget, form1); n++;
  drawA = XmCreateDrawingArea(form, "canvas", args, n);
 
  XtAddCallback(drawA, XmNexposeCallback, (XtCallbackProc)PlotData, NULL);
  XtAddCallback(drawA, XmNresizeCallback, (XtCallbackProc)ResizePlotWindow, NULL);
  XtAddCallback(drawA, XmNresizeCallback, (XtCallbackProc)PlotData, NULL);

  XtManageChild(drawA);


  /* Page Fwd & Bkwd buttons.
   */
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  frame = XmCreateFrame(form1, "buttonFrame", args, n);
  XtManageChild(frame);

  n = 0;
  brc = XmCreateRowColumn(frame, "pgButtRC", args, n);
  XtManageChild(brc);

  n = 0;
  freeze = XmCreatePushButton(brc, "freeze", args, n);
  XtAddCallback(freeze, XmNactivateCallback, Freeze, NULL);
  XtManageChild(freeze);

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, frame); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  varList = XmCreateScrolledList(form1, "varList", args, n);
  XtAddCallback(varList, XmNbrowseSelectionCallback, ModifyActiveVars, NULL);
  XtManageChild(varList);

  for (i = 0; i < nsdi; ++i)
    {
    item = XmStringCreateLocalized(sdi[i]->name);
    XmListAddItem(varList, item, 0);
    XmStringFree(item);
    }

  for (i = 0; i < nraw; ++i)
    {
    item = XmStringCreateLocalized(raw[i]->name);
    XmListAddItem(varList, item, 0);
    XmStringFree(item);
    }

  XtManageChild(form);

}	/* END CONSTRUCTOR */

/* END CANVASWINDOW.CC */
