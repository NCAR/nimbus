
#include "InstrumentWin.h"


/* -------------------------------------------------------------------- */
InstrumentWin::InstrumentWin(Widget parent)
{
  Arg		args[8];
  Cardinal	n;

  n = 0;
  instrumentForm = XmCreateForm(parent, "instrumentForm", args, n);

  n = 0;
  instrumentFrame = XmCreateFrame(instrumentForm, "instrumentFrame", args, n);
  XtManageChild(instrumentFrame);

  n = 0;
  instrumentRC = XmCreateRowColumn(instrumentFrame, "instrumentRC", args, n);
  XtManageChild(instrumentRC);

}

/* -------------------------------------------------------------------- */
void InstrumentWin::addDismiss(
	void (dissMissWrap)(Widget, XtPointer, XmPushButtonCallbackStruct*))
{
  Widget frame, rc, db;

  frame = XmCreateFrame(instrumentRC, "buttonFrame", NULL, 0);
  rc = XmCreateRowColumn(frame, "buttonRC", NULL, 0);
  XtManageChild(frame);
  XtManageChild(rc);

  db = XmCreatePushButton(rc, "dismissButt", NULL, 0);
  XtManageChild(db);
  XtAddCallback(db, XmNactivateCallback, 
                (XtCallbackProc)dissMissWrap, NULL);

}

/* -------------------------------------------------------------------- */
void InstrumentWin::popUp(Widget w, XtPointer client,
				XmPushButtonCallbackStruct *cbs)
{
  XtManageChild(instrumentForm);
  XtPopup(XtParent(instrumentForm), XtGrabNone);

}

/* -------------------------------------------------------------------- */
void InstrumentWin::popDown(Widget w, XtPointer client,
				XmPushButtonCallbackStruct *cbs)
{
  XtPopdown(XtParent(instrumentForm));
  XtUnmanageChild(instrumentForm);

}

/* END INSTRUMENTWIN.CC */
