/*
-------------------------------------------------------------------------
OBJECT NAME:	skel.c

FULL NAME:	

ENTRY POINTS:	main()

STATIC FNS:	none

DESCRIPTION:	

REFERENCES:	Xwin.c, init.c then enters XtAppMainLoop()

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2005
-------------------------------------------------------------------------
*/

#include "define.h"
#include "fbr.h"

#define APP_CLASS	"XmSkeleton"


Widget	AppShell;				/* The Main Application Shell */
Widget	Shell000, MainWindow;
Widget	Shell001, SetupWindow;
Widget	Shell003, TimeSliceWindow;

XtAppContext context;


Widget CreateMainWindow(Widget);
Widget CreateSetupWindow(Widget);
Widget CreateEditWindow(Widget);
Widget CreateTimeSliceWindow(Widget);

void CreatePauseWindows(Widget);
void CreateErrorBox(Widget);
void CreateQueryBox(Widget);
void CreateWarningBox(Widget);
void CreateFileSelectionBox(Widget);

void ProcessArgv(int argc, char *argv[]);

/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  Arg		args[8];
  Cardinal	n;

  n = 0;
  AppShell = XtAppInitialize(&context, APP_CLASS, NULL, 0, &argc, argv,
			fallback_resources, NULL, 0);

  n = 0;
  Shell000 = XtCreatePopupShell("topLevelShell",
			topLevelShellWidgetClass, AppShell, args, n);

  MainWindow = CreateMainWindow(Shell000);

  n = 0;
  XtSetArg(args[n], XmNiconName, "Nimbus"); n++;
  Shell001 = XtCreatePopupShell("setupShell",
			topLevelShellWidgetClass, AppShell, args, n);

  SetupWindow = CreateSetupWindow(Shell001);

  n = 0;
  XtSetArg(args[n], XmNtitle, "Edit Time Slices"); n++;
  Shell003 = XtCreatePopupShell("timeSliceShell",
			topLevelShellWidgetClass, AppShell, args, n);

  TimeSliceWindow = CreateTimeSliceWindow(Shell003);


  CreatePauseWindows(AppShell);
  CreateErrorBox(AppShell);
  CreateQueryBox(AppShell);
  CreateWarningBox(AppShell);
  CreateFileSelectionBox(AppShell);

  Initialize();
  ProcessArgv(argc, argv);

  if (Interactive)
    {
    XtManageChild(MainWindow);
    XtPopup(XtParent(MainWindow), XtGrabNone);

    XtAppMainLoop(context);
    }
  else
    {
    Proceed(NULL, NULL, NULL);
    StartProcessing(NULL, NULL, NULL);
    }

  return(0);

}	/* END MAIN */

/* END SKEL.C */
