/*
-------------------------------------------------------------------------
OBJECT NAME:	nimbus.c

FULL NAME:	

ENTRY POINTS:	main()

STATIC FNS:	none

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2005
-------------------------------------------------------------------------
*/

#include <signal.h>

#include "nimbus.h"
#include "gui.h"
#include "fbr.h"
#include "svnInfo.h"

#define APP_CLASS	"XmNimbus"


Widget	AppShell;		/* The Main Application Shell */
Widget	Shell000, MainWindow;
Widget	Shell001, SetupWindow;

XtAppContext context;


Widget	CreateMainWindow(Widget parent);
Widget	CreateSetupWindow(Widget parent);
void	CreateEditWindow();
void	CreateConfigWindow();
void	CreatePauseWindows(Widget topLevel);

#ifdef __cplusplus
extern "C" {
#endif
void	CreateErrorBox(Widget parent),
	CreateFileSelectionBox(Widget parent),
	CreateQueryBox(Widget parent),
	CreateWarningBox(Widget parent);
#ifdef __cplusplus
}
#endif


/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  Arg		args[8];
  Cardinal	n;

  printf("%s\n", SVNREVISION);
  printf("%s\n", SVNLASTCHANGEDDATE);
  printf("%s\n\n", SVNURL);

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

  CreateEditWindow();
  CreateConfigWindow();

  CreatePauseWindows(AppShell);
  CreateErrorBox(AppShell);
  CreateQueryBox(AppShell);
  CreateWarningBox(AppShell);
  CreateFileSelectionBox(AppShell);

  Initialize();
  ProcessArgv(argc, argv);

  signal(SIGUSR2, (void (*) (int))Quit);

  if (cfg.Interactive())
  {
    char *p;

// This may still be applicable if .ads disk & .nc disk are the same.  But
// we've run out of big enough disks at RAF, so try new method below.
//    GetDataDirectory(buffer);

    // Set this environment variable to over-ride hard code below
    // (ground-stations, etc).
    if ( (p = getenv("RAW_DATA_DIR")) )
      strcpy(buffer, p);
    else
    {
      const std::string DefaultRawDataDirectory = "/scr/raf2/Raw_Data";

      strcpy(buffer, DefaultRawDataDirectory.c_str());

      fprintf(stderr, "Environment variable RAW_DATA_DIR not set, defaulting to %s.\n",
	DefaultRawDataDirectory.c_str());
    }

    strcat(buffer, "/*.ads");
    QueryFile("Enter ADS file name:", buffer, Proceed);

    XtAppMainLoop(context);
  }
  else
    Proceed(NULL, NULL, NULL);

  return(0);

}	/* END MAIN */

/* END NIMBUS.C */
