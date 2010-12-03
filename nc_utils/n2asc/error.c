/*
-------------------------------------------------------------------------
OBJECT NAME:	error.c

FULL NAME:	Error Handler

ENTRY POINTS:	HandleError()
		HandleWarning()

STATIC FNS:	none

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2005
-------------------------------------------------------------------------
*/

#include "define.h"

void ShowError(const char msg[]);
void WarnUser(const char msg[], void (*okCB)(Widget, XtPointer, XtPointer), void (*cancelCB)(Widget, XtPointer, XtPointer));

/* -------------------------------------------------------------------- */
void HandleError(const char err_msg[])
{
  if (Interactive)
    ShowError(err_msg);
  else
    {
    fprintf(stderr, "%s\n", err_msg);
    exit(1);
    }

}	/* END HANDLEERROR */

/* -------------------------------------------------------------------- */
void HandleWarning(const char msg[], void (*callBack)(Widget, XtPointer, XtPointer))
{
  if (Interactive)
    WarnUser(msg, callBack, NULL);
  else
    {
    fprintf(stderr, "%s\n", msg);
    (*callBack)(NULL, NULL, NULL);
    }

}	/* END HANDLEWARNING */

/* END ERROR.C */
