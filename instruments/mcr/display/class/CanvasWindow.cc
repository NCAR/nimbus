/*
-------------------------------------------------------------------------
OBJECT NAME:	CanvasWindow.cc

FULL NAME:	

ENTRY POINTS:	CanvasWindow()

STATIC FNS:	

DESCRIPTION:	

NOTES:		

COPYRIGHT:	University Corporation for Atmospheric Research, 1997
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
	{ "print", PrintSave, NULL, },
	{ "quit", Quit, NULL, },
	{ NULL, NULL, NULL }};

static struct menu	recordMenu[] = {
	{ "Record 10 sex", Record, (XtPointer)true, },
	{ NULL, NULL, NULL }};

static struct menu	channelMenu[] = {
	{ "Channel 1", SetChannel, (XtPointer)0, },
	{ "Channel 2", SetChannel, (XtPointer)1, },
	{ "Channel 3", SetChannel, (XtPointer)2, },
	{ "Channel 4", SetChannel, (XtPointer)3, },
	{ "Channel 5", SetChannel, (XtPointer)4, },
	{ "Channel 6", SetChannel, (XtPointer)5, },
	{ "Channel 7", SetChannel, (XtPointer)6, },
	{ NULL, NULL, NULL }};
 
static struct
	{
	char		*title;
	struct menu	*sub;
	} main_menu[] = {
		{ "File", fileMenu, },
		{ "Record", recordMenu, },
		{ "Channel", channelMenu, },
		{ NULL, NULL }};
 

/* -------------------------------------------------------------------- */
void CanvasWindow::SetStartVolt(float v)
{
  char	tmp[24];

  sprintf(tmp, "%.2f", v);
  XmTextFieldSetString(startVoltW, tmp);

}

/* -------------------------------------------------------------------- */
void CanvasWindow::SetEndVolt(float v)
{
  char	tmp[24];

  sprintf(tmp, "%.2f", v);
  XmTextFieldSetString(endVoltW, tmp);

}

/* -------------------------------------------------------------------- */
float CanvasWindow::GetStartVolt()
{
  char	*p;
  float	v;

  p = XmTextFieldGetString(startVoltW);
  v = atof(p); XtFree(p);
  return(v);

}

/* -------------------------------------------------------------------- */
float CanvasWindow::GetEndVolt()
{
  char	*p;
  float	v;

  p = XmTextFieldGetString(endVoltW);
  v = atof(p); XtFree(p);
  return(v);

}

/* -------------------------------------------------------------------- */
CanvasWindow::CanvasWindow(const Widget parent) : WinForm(parent, "canvas", Form)
{
  int		i, j;
  Cardinal	n;
  Arg		args[8];
  Widget	menubar, menu[5], bttn[16], menu_button[5], freeze;


  n = 0;
  menubar = XmCreateMenuBar(Window(), "menuBar", args, n);
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


  n = 0;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  freeze = XmCreatePushButton(Window(), "Freeze", args, n);
  XtAddCallback(freeze, XmNactivateCallback, Freeze, NULL);
  XtManageChild(freeze);

  n = 0;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNrightWidget, freeze); n++;
  endVoltW = XmCreateTextField(Window(), "endVolt", args, n);
  XtAddCallback(endVoltW, XmNactivateCallback, SetEndVoltage, NULL);
  XtAddCallback(endVoltW, XmNlosingFocusCallback, SetEndVoltage, NULL);
  XtManageChild(endVoltW);

  n = 0;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNrightWidget, endVoltW); n++;
  startVoltW = XmCreateTextField(Window(), "startVolt", args, n);
  XtAddCallback(startVoltW, XmNactivateCallback, SetStartVoltage, NULL);
  XtAddCallback(startVoltW, XmNlosingFocusCallback, SetStartVoltage, NULL);
  XtManageChild(startVoltW);



  /* Create Graphics Canvas
   */
  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  drawA = XmCreateDrawingArea(Window(), "canvas", args, n);
 
  XtAddCallback(drawA, XmNexposeCallback, (XtCallbackProc)CanvasExpose, NULL);
//  XtAddCallback(drawA, XmNinputCallback, (XtCallbackProc)CanvasInput, NULL);
  XtAddCallback(drawA, XmNresizeCallback, (XtCallbackProc)CanvasResize, NULL);

  XtManageChild(drawA);

}	/* END CONSTRUCTOR */

/* END CANVASWINDOW.CC */
