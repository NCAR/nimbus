/*
-------------------------------------------------------------------------
OBJECT NAME:	cb_pause.c

FULL NAME:	Command CallBacks

ENTRY POINTS:	PauseProcessing()

STATIC FNS:	none

DESCRIPTION:	Contains callbacks for the nimbus GUI pause related stuff.

INPUT:			

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	XtAppMainLoop()

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include <Xm/Form.h>
#include <Xm/PushB.h>

#include "define.h"

static Widget	PauseShell;

/* -------------------------------------------------------------------- */
/* ARGSUSED */
void
PauseProcessing(Widget w, XtPointer client, XtPointer call)
{
	PauseFlag = TRUE;
	XtSetSensitive(goButton, FALSE);

	XtManageChild(PauseShell);
	XtPopup(XtParent(PauseShell), XtGrabNonexclusive);

}	/* END PAUSEPROCESSING */

/* -------------------------------------------------------------------- */
/* ARGSUSED */
void
PauseStop(Widget w, XtPointer client, XtPointer call)
{
	PauseFlag = FALSE;
	PauseWhatToDo = P_QUIT;

	XtPopdown(XtParent(PauseShell));
	XtUnmanageChild(PauseShell);

}	/* END PAUSEPROCESSING */

/* -------------------------------------------------------------------- */
/* ARGSUSED */
void
PauseContinue(Widget w, XtPointer client, XtPointer call)
{
	PauseFlag = FALSE;
	PauseWhatToDo = P_CONTINUE;
	XtSetSensitive(goButton, TRUE);

	XtPopdown(XtParent(PauseShell));
	XtUnmanageChild(PauseShell);

}	/* END PAUSECONTINUE */

/* -------------------------------------------------------------------- */
/* ARGSUSED */
void
PauseXTV(Widget w, XtPointer client, XtPointer call)
{
/*
	XtPopup(PauseXTVBox);
*/
}	/* END PAUSEXTV */

/* -------------------------------------------------------------------- */
void CreatePauseWindows(topLevel)
Widget	topLevel;
{
	Widget	b[3];
	Arg		args[5];
	int		n;

	n = 0;
	XtSetArg(args[n], XmNtitle, "Pause Choices"); n++;
	PauseShell = XmCreateFormDialog(topLevel, "pauseForm", args, n);

	n = 0;
	XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
	XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
	XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
	b[0] = XmCreatePushButton(PauseShell, "pauseStop", args, n);
	XtAddCallback(b[0], XmNactivateCallback, PauseStop, NULL);

	n = 0;
	XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
	XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
	XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
	XtSetArg(args[n], XmNleftWidget, b[0]); n++;
	b[1] = XmCreatePushButton(PauseShell, "pauseCont", args, n);
	XtAddCallback(b[1], XmNactivateCallback, PauseContinue, NULL);

	XtManageChildren(b, 2);

}	/* END CREATEPAUSEWINDOWS */

/* END CB_PAUSE.C */
