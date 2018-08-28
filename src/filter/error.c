/*
-------------------------------------------------------------------------
OBJECT NAME:	error.c

FULL NAME:	Error Handler

ENTRY POINTS:	HandleError()
		HandleWarning()

STATIC FNS:	none

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "gui.h"

#include <csignal>

void ShutdownSyncServer();

/* -------------------------------------------------------------------- */
void HandleFatalError(const char err_msg[])
{
  fprintf(stderr, "%s\n", err_msg);
  ShutdownSyncServer();
  kill(0, SIGTERM);
  exit(1);

}	/* END HANDLEFATALERROR */

/* -------------------------------------------------------------------- */
void HandleError(const char err_msg[])
{
  if (cfg.Interactive())
    ShowError(err_msg);
  else
    HandleFatalError(err_msg);

}	/* END HANDLEERROR */

/* -------------------------------------------------------------------- */
void HandleWarning(const char msg[], XtCallbackProc okCB, XtCallbackProc cancelCB)
{
  if (cfg.Interactive())
    WarnUser(msg, okCB, cancelCB);
  else
    {
    fprintf(stderr, "%s\n", msg);

    if (okCB)
      (*okCB)(NULL, NULL, NULL);
    }

}	/* END HANDLEWARNING */

/* END ERROR.C */
