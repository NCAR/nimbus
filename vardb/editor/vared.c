/*
-------------------------------------------------------------------------
OBJECT NAME:	setup.c

FULL NAME:	

ENTRY POINTS:	main()

STATIC FNS:	none

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2005
-------------------------------------------------------------------------
*/

#include <Xm/Xm.h>

#include "define.h"
#include "fbr.h"


Widget	Shell000, MainWindow;

void CreateErrorBox(Widget);
void CreateWarningBox(Widget);
void CreateFileSelectionBox(Widget);


/* -------------------------------------------------------------------- */
int main(int argc, char **argv)
{
  Widget	AppShell;	/* The Main Application Shell */
  XtAppContext	context;
  Arg		args[8];
  Cardinal	n;

  n = 0;
  AppShell = XtAppInitialize(&context, "XmVared", NULL, 0, &argc, argv,
				fallback_resources, NULL, 0);

  n = 0;
  Shell000 = XtCreatePopupShell("topLevelShell", 
				topLevelShellWidgetClass, AppShell, args, n);

  MainWindow = CreateMainWindow(Shell000);

  CreateErrorBox(AppShell);
  CreateWarningBox(AppShell);
  CreateFileSelectionBox(AppShell);

  Initialize(argc, argv);

  XtManageChild(MainWindow);
  XtPopup(XtParent(MainWindow), XtGrabNone);

  XtAppMainLoop(context);

  return(0);

}	/* END MAIN */

/* END VARED.C */
