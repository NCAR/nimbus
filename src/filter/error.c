/*
-------------------------------------------------------------------------
OBJECT NAME:	error.c

FULL NAME:	Error Handler

ENTRY POINTS:	HandleError()
		HandleWarning()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		

OUTPUT:		

REFERENCES:	

REFERENCED BY:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "gui.h"


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
void HandleWarning(char msg[], XtCallbackProc okCB, XtCallbackProc cancelCB)
{
  if (Interactive)
    WarnUser(msg, okCB, cancelCB);
  else
    {
    fprintf(stderr, "%s\n", msg);

    if (okCB)
      (*okCB)(NULL, NULL, NULL);
    }

}	/* END HANDLEWARNING */

/* END ERROR.C */
