/*
-------------------------------------------------------------------------
OBJECT NAME:	Xwin.c

FULL NAME:	X window Stuff

TYPE:		X11/R5 Motif 1.2

DESCRIPTION:	This creates the initial X window, menus and all associated
		widgets.  Also calls CreateFile(), CreateError(),
		CreateQuery().

INPUT:		none

OUTPUT:		none

AUTHOR:		websterc@ncar
-------------------------------------------------------------------------
*/

#include "define.h"

#include <X11/cursorfont.h>
#include <X11/Xutil.h>
#include <Xm/CascadeB.h>
#include <Xm/DrawingA.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/List.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>

struct menu
	{
	char		*title;
	void		(*callback)();
	XtPointer	callData;
	} ;

static struct menu	file_menu[] = {
	"quit", Quit, NULL,
	NULL, NULL, NULL };

static struct menu	unit_menu[] = {
	"Counts", ToggleUnits, (XtPointer)COUNTS,
	"Volts", ToggleUnits, (XtPointer)VOLTS,
	"Engineering", ToggleUnits, (XtPointer)ENGINEERING,
	NULL, NULL, NULL };

static struct menu	legend_menu[] = {
	"autoScale", DoAutoScale, NULL,
	"toggleGrid", ToggleGrid, NULL,
	"toggleFlash", ToggleFlash, NULL,
	NULL, NULL, NULL };

static struct
	{
	char		*title;
	struct menu	*sub;
	} main_menu[] = {
		"File", file_menu,
		"Units", unit_menu,
		"Options", legend_menu,
		NULL, NULL };

void	Freeze();

/* -------------------------------------------------------------------- */
Widget CreateMainWindow(parent)
Widget	parent;
{
  Widget	menubar, form, form1, menu[5], bttn[50], menu_button[5], freeze,
		frame, brc;
  Arg		args[9];
  int		n, i, j;
  XmString	item;

  n = 0;
  form = XmCreateForm(parent, "mainForm", args, n);

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
      bttn[j] = XmCreatePushButton(menu[i], main_menu[i].sub[j].title, args, n);
      XtAddCallback(bttn[j], XmNactivateCallback,
			main_menu[i].sub[j].callback,
			main_menu[i].sub[j].callData);
      }

    XtManageChildren(bttn, j);
    }

  XtManageChildren(menu, i);
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
  canvas = XmCreateDrawingArea(form, "canvas", args, n);
  XtAddCallback(canvas, XmNexposeCallback, (XtCallbackProc)PlotData,
							(XtPointer)NULL);
  XtAddCallback(canvas, XmNresizeCallback, ResizePlotWindow, NULL);
  XtAddCallback(canvas, XmNresizeCallback, PlotData, NULL);

  XtManageChild(canvas);


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
  XtAddCallback(varList, XmNdefaultActionCallback, ModifyActiveVars, NULL);
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

  return(form);

}	/* END CREATEMAINWINDOW */

/* -------------------------------------------------------------------- */
void FlushXEvents()
{
  extern XtAppContext app_con;

  while (XtAppPending(app_con))
    XtAppProcessEvent(app_con, XtIMAll);

}	/* END FLUSHXEVENTS */

/* END XWIN.C */
