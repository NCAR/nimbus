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

void ShowError(char msg[]);
void WarnUser(char msg[], void (*okCB)(Widget, XtPointer, XtPointer), void (*cancelCB)(Widget, XtPointer, XtPointer));

/* -------------------------------------------------------------------- */
void HandleError(char err_msg[])
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
void HandleWarning(char msg[], void (*callBack)(Widget, XtPointer, XtPointer))
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
