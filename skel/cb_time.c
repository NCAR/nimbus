/*
-------------------------------------------------------------------------
OBJECT NAME:	cb_time.c

FULL NAME:	Callbacks for TimeSlice Window

ENTRY POINTS:	EditTimeSlices()
		ValidateTime()
		DismissTimeSliceWindow()
		ResetTimeSliceWindow()

STATIC FNS:		

DESCRIPTION:	

INPUT:		

OUTPUT:		

REFERENCES:	

REFERENCED BY:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include <Xm/TextF.h>

#include "define.h"


/* -------------------------------------------------------------------- */
/* ARGSUSED */
void
EditTimeSlices(w, client, call)
Widget		w;
XtPointer	client;
XtPointer	call;
{
	XtManageChild(TimeSliceWindow);
	XtPopup(XtParent(TimeSliceWindow), XtGrabNone);

}	/* END EDITTIMESLICES */

/* -------------------------------------------------------------------- */
/* ARGSUSED */
void
ValidateTime(w, client, call)
Widget		w;
XtPointer	client;
XtPointer	call;
{
	int		hour, min, sec;
	char	*p = XmTextFieldGetString(w);

	if (strlen(p) == 0)
		return;

	hour = min = sec = 0;

	if (strchr(p, ':') != NULL)
		sscanf(p, "%d:%d:%d", &hour, &min, &sec);
	else
		sscanf(p, "%02d%02d%02d", &hour, &min, &sec);


	sprintf(buffer, "%02d:%02d:%02d", hour, min, sec);
	XmTextFieldSetString(w, buffer);
	XtFree(p);

}	/* END VALIDATETIME */

/* -------------------------------------------------------------------- */
/* ARGSUSED */
void
DismissTimeSliceWindow(w, client, call)
Widget		w;
XtPointer	client;
XtPointer	call;
{
	XtPopdown(XtParent(TimeSliceWindow));
	XtUnmanageChild(TimeSliceWindow);
}

/* -------------------------------------------------------------------- */
/* ARGSUSED */
void
ResetTimeSliceWindow(Widget w, XtPointer client, XtPointer call)
{
    int     i;
 
    for (i = 0; i < MAX_TIME_SLICES*2; ++i)
        XmTextFieldSetString(ts_text[i], "");
 
}   /* END RESETTIMESLICEWINDOW */

/* END CB_TIME.C */
