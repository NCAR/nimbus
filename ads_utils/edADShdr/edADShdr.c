/*
-------------------------------------------------------------------------
OBJECT NAME:	edADShdr.c

FULL NAME:	

ENTRY POINTS:	main()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		

OUTPUT:		

REFERENCES:	Xwin.c, init.c then enters XtAppMainLoop()

REFERENCED BY:	user

COPYRIGHT:	University Corporation for Atmospheric Research, 2000
-------------------------------------------------------------------------
*/

#include "define.h"
#include "fbr.h"


Widget	Shell000, MainWindow;


/* -------------------------------------------------------------------- */
main(int argc, char **argv)
{
  Widget	AppShell;	/* The Main Application Shell */
  XtAppContext	context;
  Arg		args[8];
  Cardinal	n;

  n = 0;
  AppShell = XtAppInitialize(&context, "XmEdADShdr", NULL, 0, &argc, argv,
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

/* END EDADSHDR.C */
