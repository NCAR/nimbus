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

COPYRIGHT:	University Corporation for Atmospheric Research, 1994
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
	int		i, hour, min, sec;
	char	*p = XmTextFieldGetString(w);
	bool	inSeg;

	static bool	firstTime = TRUE;
	static int	nTimInt = 0;
	static char	*timInt[40];

	if (firstTime)
		{
		int		len = strlen(TimeInterval);

		nTimInt = len / 17;

		for (i = 0; i < nTimInt; ++i)
			{
			timInt[i*2] = GetMemory(9);
			timInt[i*2+1] = GetMemory(9);

			strcpy(timInt[i*2], &TimeInterval[i * 19]);
			strcpy(timInt[i*2+1], &TimeInterval[(i * 19) + 9]);

			if (atoi(timInt[i*2]) > atoi(timInt[i*2+1]))
				{
				sprintf(buffer, "%02d", atoi(timInt[i*2+1]) + 24);
				memcpy(timInt[i*2+1], buffer, 2);
				}
			}

		firstTime = FALSE;
		}

	if (strlen(p) == 0)
		return;

	hour = min = sec = 0;

	if (strchr(p, ':') != NULL)
		sscanf(p, "%d:%d:%d", &hour, &min, &sec);
	else
		sscanf(p, "%02d%02d%02d", &hour, &min, &sec);


	sprintf(buffer, "%02d:%02d:%02d", hour, min, sec);

	inSeg = FALSE;

	for (i = 0; i < nTimInt; ++i)
		{
		if (memcmp(timInt[i*2], buffer, 8) <= 0 &&
			memcmp(buffer, timInt[i*2+1], 8) <= 0)
			{
			inSeg = TRUE;
			break;
			}
		}

	if (inSeg == FALSE)
		{
		buffer[0] = '\0';
		}

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
