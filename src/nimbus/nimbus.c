/*
-------------------------------------------------------------------------
OBJECT NAME:	nimbus.c

FULL NAME:	

ENTRY POINTS:	main()

STATIC FNS:	none

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2019
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "gui.h"
#include "fbr.h"
#include "gitInfo.h"

#include <nidas/core/NidasApp.h>

using nidas::util::Logger;
using nidas::util::LogConfig;

#include <csignal>

#define APP_CLASS	"XmNimbus"

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
  nidas::core::NidasApp napp("nimbus");
  napp.setApplicationInstance();
  napp.allowUnrecognized(true);
  napp.enableArguments(napp.XmlHeaderFile | napp.loggingArgs() |
		       napp.StartTime | napp.EndTime);
  // Require long flags to avoid confusion with nimbus flags like -x.
  napp.requireLongFlag(napp.XmlHeaderFile | napp.loggingArgs() |
		       napp.StartTime | napp.EndTime);

  Arg		args[8];
  Cardinal	n;

  printf("%s -> %s\n\n", REPO_URL, REPO_BRANCH);
  printf(" : %s\n", REPO_DATE);
  printf(" : %s\n", REPO_HASH);

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

  signal(SIGUSR2, (void (*) (int))Quit);

  // For now, always log to the console.  Previous to the sync_server
  // merge, NIDAS messages from the separate sync_server process were
  // interleaved with the NIMBUS output.  To keep that behavior, which
  // probably users will still expect, nimbus must send all NIDAS log
  // messages to the console.  If we really need to see NIDAS messages on
  // syslog instead, such as in real-time mode on the plane, then maybe
  // that becomes a command-line option.
  logger = Logger::createInstance(&std::cerr);
  // logger = Logger::createInstance("nimbus", LOG_CONS, LOG_LOCAL5);

  // Setup a default log scheme which can be overridden when log options
  // are parsed in ProcessArgv() below.
  nidas::util::LogScheme ls = logger->getScheme();
  if (cfg.ProcessingMode() == Config::PostProcessing)
  {
    // We do not need to see logging timestamps when post-processing.
    ls.setShowFields("message");
  }

  if (false)
  {
    ls.setParameter("trace_variables", "A1DC_LWOO,A1DC_LWO");
    ls.setParameter("trace_samples", "46,600-602");
    ls.setParameter("sync_warn_times_interval", "1");
    ls.addConfig(LogConfig("verbose,function=TraceVariables"));
    ls.addConfig(LogConfig("verbose,file=TwoD"));
    ls.addConfig(LogConfig("verbose,file=SamplePipeline"));
    ls.addConfig(LogConfig("verbose"));
  }

  // We still want at least info messages from everything else.
  ls.addConfig(LogConfig("level=info"));
  logger->setScheme(ls);

  ProcessArgv(argc, argv);

  if (cfg.Interactive())
  {
    char *p;

    // Set this environment variable to over-ride hard code below
    // (ground-stations, etc).
    if ( (p = getenv("RAW_DATA_DIR")) )
      strcpy(buffer, p);
    else
    {
      const std::string DefaultRawDataDirectory = "/scr/raf/Raw_Data";

      strcpy(buffer, DefaultRawDataDirectory.c_str());

      fprintf(stderr, "Environment variable RAW_DATA_DIR not set, defaulting to %s.\n",
	DefaultRawDataDirectory.c_str());
    }

    strcat(buffer, "/*.ads");
    QueryFile("Enter ADS file name:", buffer, Proceed);

    XtAppMainLoop(context);
  }
  else
  {
    Proceed(NULL, NULL, NULL);
    quit();
  }

  return(0);

}	/* END MAIN */

/* END NIMBUS.C */
