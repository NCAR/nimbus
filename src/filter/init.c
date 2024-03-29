/*
-------------------------------------------------------------------------
OBJECT NAME:	init.c

FULL NAME:	Initialize

ENTRY POINTS:	Initialize()
		ProcessArgv()
		ReadBatchFile()

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2011
-------------------------------------------------------------------------
*/

#include <unistd.h>
#include <sys/types.h>
#include <Xm/TextF.h>

#include <nidas/util/Process.h>
#include <nidas/core/NidasApp.h>

#include "nimbus.h"
#include "decode.h"
#include "amlib.h"
#include "gui.h"

using nidas::core::NidasApp;
using nidas::core::ArgVector;

static void ReadBatchFile(const char *filename, Config::processingRate * rate);
static void usage();

using std::string;

void	Set_SetupFileName(char s[]);

void	RTinit_ADS2(), RTinit_ADS3();

/* -------------------------------------------------------------------- */
void Initialize()
{
  int	pos;
  char	*proj_dir;

  if ((proj_dir = (char *)getenv("PROJ_DIR")) == NULL)
    HandleFatalError("Environment variable PROJ_DIR not defined, this is fatal.");

  cfg.SetProjectDirectory(proj_dir);

  putenv((char *)"TZ=UTC");	// Perform all time calculations at UTC.

  LITTON51_present	= false;
  AVAPS			= false;
  PauseFlag		= false;
  PauseWhatToDo		= P_CONTINUE;
  FeedBack		= LOW_RATE_FEEDBACK;

  pos = XmTextFieldGetLastPosition(aDSdataText);
  XmTextFieldSetInsertionPosition(aDSdataText, pos);

  pos = XmTextFieldGetLastPosition(outputFileText);
  XmTextFieldSetInsertionPosition(outputFileText, pos);

  XtSetSensitive(goButton, true);
  XtSetSensitive(outputHRbutton, false);

  /* Check to see if user is 'nimbus'.
   */
  if (getuid() == 20000)
    cfg.SetProductionRun(true);

  if (cfg.ProductionRun())
    XtSetSensitive(outputFileText, false);

}	/* END INITIALIZE */

/* -------------------------------------------------------------------- */
bool nimbusIsAlreadyRunning()
{
  try {
    pid_t pid = nidas::util::Process::checkPidFile("/tmp/nimbus.pid");
    if (pid > 0) {
      fprintf(stderr, "nimbus process, pid=%d is already running.\n",pid);
      fprintf(stderr, "Or possibly nimbus terminated improperly \n");
      fprintf(stderr, " and /tmp/nimbus.pid file was left behind.\n");
      return true;
    }
  }
  catch(const nidas::util::IOException& e) {
    fprintf(stderr, "nimbus: %s\n", e.what());
    return true;
  }
  return false;
}

/* -------------------------------------------------------------------- */
void ProcessArgv(int argc, char **argv)
{
  int	i;
  int length;
  string p;
  Config::processingRate rate = Config::LowRate;

  cfg.SetInteractive(true);
  cfg.SetLoadProductionSetup(true);

  ArgVector args(argv+1, argv+argc);
  NidasApp& napp = *NidasApp::getApplicationInstance();

  args = napp.parseArgs(args);

  for (i = 0; i < (int)args.size(); ++i)
  {
    std::string arg = args[i];
    if (strstr(arg.c_str(), "help"))
      usage();

    if (arg.length() < 2 || arg[0] != '-')
    {
      fprintf(stderr, "Invalid option '%s', ignoring.\n", arg.c_str());
      continue;
    }

    switch (arg[1])
    {
      case 'h':
        usage();
        break;

      case 'b':	// Load batch file.
        cfg.SetInteractive(false);
        ReadBatchFile(args[++i].c_str(), &rate);
        break;
      case 'l':
	length = atoi(args[++i].c_str());
	if (length > 0)
	{
	  cfg.SetSorterLength(length);
	}
	else
	{
	  fprintf(stderr, "invalid sorter length: %s\n", args[i].c_str());
	  exit(1);
	}
        break;
      case 'r':	// -r is for raw data, -rt is for real-time mode.
        if (arg.substr(1, 2) == "rt" && nimbusIsAlreadyRunning())
	{
	  fprintf(stderr, "nimbus is already running in real-time mode, exiting.\n");
	  sleep(5);
          exit(1);
	}
        rate = Config::SampleRate;
        cfg.SetTimeShifting(false);
        cfg.SetDespiking(false);
        cfg.SetBlankoutVariables(false);
        cfg.SetOutputSQL(true);
        if (arg == "-rt")	/* RealTime ADS2 */
          RTinit_ADS2();
        else
        if (arg == "-rt3")	/* RealTime ADS3 */
          RTinit_ADS3();
        else
        {
          cfg.SetOutputSQL(false);	// Not yet.
          cfg.SetHoneyWellCleanup(false); // We want this in real-time.
          cfg.SetInertialShift(false);	// We want this in real-time.
        }
        break;

      case 'x':	// Transmit UDP to ground, with optional interval.
        if (cfg.ProcessingMode() == Config::RealTime)
          cfg.SetTransmitToGround(true);
        else
          fprintf(stderr, "Must be in RealTime mode to TransmitToGround, ignoring -x.\n");

        if (i+1 < (int)args.size() && isdigit(args[i+1][0]))
          cfg.SetGroundFeedDataRate(atoi(args[++i].c_str()));
        break;

      case 'y':
	cfg.SetEnableBroadcast(false);
	cfg.SetWarnTimeLags(false);
	break;

      case 'n':
        cfg.SetLoadProductionSetup(false);
        break;

      case 'p':	// Processing-rate.
        if (arg.substr(0, 3) == "-pr")
          usage();

        // Support '=' or white-space between arg and value.
        if (arg.find('=') != string::npos)
          p = arg.substr(arg.find('=') + 1);
        else
          if (i+1 < (int)args.size())
            p = args[++i];

        if (p.empty())
          usage();

        // Support 0/1/25 or s/l/h, sample-rate, low-rate, and high-rate respectively.
        if (p[0] == '0' || p[0] == 's')
          rate = Config::SampleRate;
        else
        if ((p[0] == '2' && p[1] == '5') || p[0] == 'h')
          rate = Config::HighRate;

        break;

      case 'q':
        cfg.SetQCenabled(true);
        break;

      default:
        fprintf(stderr, "Invalid option %s, ignoring.\n", arg.c_str());
    }
  }

  // Set the rate here, so that command line option will over-ride batch file.
  cfg.SetProcessingRate(rate);

}	/* END PROCESSARGV */

/* -------------------------------------------------------------------- */
static char *processFileName(const char *in, char *out, size_t out_len)
{
  char *dollar, *p, *e, work[256];

  memset(out, 0, out_len);
  strcpy(work, in);
  dollar = strchr(work, '$');

  if (dollar)
  {
    if (dollar > work) strncpy(out, work, dollar-work);
    p = strtok(dollar, "${}");
    e = getenv(p);
    if (e) strcat(out, e);
    p = strtok(0, "${}");
    if (p) strcat(out, p);
  }
  else
    strcpy(out, in);

  return out;
}

/* -------------------------------------------------------------------- */
static void ReadBatchFile(const char *fileName, Config::processingRate *rate)
{
  FILE	*fp;
  char	*p;

  if ((fp = fopen(fileName, "r")) == NULL)
  {
    fprintf(stderr, "Can't open %s.\n", fileName);
    exit(1);
  }


  while (fgets(buffer, 512, fp))
  {
    if (buffer[0] == COMMENT)
      continue;

    if ((p = strtok(buffer, " \t=")) == NULL)
      continue;

    if (strcmp(p, "if") == 0)
    {
      char fname[256];
      processFileName(strtok(NULL, " \t\n"), fname, 256);
      XmTextFieldSetString(aDSdataText, fname);
    }
    else
    if (strcmp(p, "of") == 0)
    {
      char fname[256];
      processFileName(strtok(NULL, " \t\n"), fname, 256);
      XmTextFieldSetString(outputFileText, fname);
    }
    else
    if (strcmp(p, "sf") == 0)
    {
      char fname[256];
      processFileName(strtok(NULL, " \t\n"), fname, 256);
      Set_SetupFileName(fname);
    }
    else
    if (strcmp(p, "pr") == 0)
    {
      int value = atoi(strtok(NULL, " \t\n"));
      switch (value)
      {
        case Config::SampleRate:
          *rate = Config::SampleRate;
          break;
        case Config::LowRate:
          *rate = Config::LowRate;
          break;
        case 25:
        case 50:
          *rate = Config::HighRate;
          cfg.SetHRTRate((Config::hrtRate)value);
          break;
        default:
          fprintf(stderr, "ReadBatchFile: Unsupported processing rate specified, fatal.\n");
          exit(1);
      }
    }
    else
    if (strcmp(p, "ti") == 0)
      {
      char	*p;

      XmTextFieldSetString(ts_text[0], strtok(NULL, "- \t"));
      p = strtok(NULL, "- \t\n");

      XmTextFieldSetString(ts_text[MAX_TIME_SLICES], p);
      ValidateTime(ts_text[0], NULL, NULL);
      ValidateTime(ts_text[MAX_TIME_SLICES], NULL, NULL);
    }
  }

  fclose(fp);

}	/* END READBATCHFILE */

/* -------------------------------------------------------------------- */
void GetDataDirectory(char buff[])
{
  buff[0] = 0;

  char *p;

  if ( (p = getenv("DATA_FILTER")) )
    strcpy(buff, p);
  else
  if ( (p = getenv("DATA_DIR")) )
  {
    strcpy(buff, p);
    strcat(buff, "/");
  }
}

/* -------------------------------------------------------------------- */
static void usage()
{
  fprintf(stderr, "Usage: nimbus [-b batch_file] [-r] [-rt3 [-x [rate]]] [-pr s|l|h] [-n]\n\n\
 Post-processing options:\n\
  -b:	Loads a batch_file instead of going interactive.  File options\n\
	are (last 3 are optional):\n\
	if=input_file.ads\n\
	of=output_file.nc\n\
	sf=setup_file\n\
	pr=processing_rate [SampleRate=0, LowRate=1, HighRate=25]\n\
	ti=time_interval [xx:xx:xx-xx:xx:xx]\n\n\
  -n:	Do NOT load any existing production setup files.\n\
  -pr:	Set processing rate, options 0,1,25 or s,l,h for sample-rate, low-rate, or\n\
        high-rate respectively.  Default is low-rate.\n\
  -r:	Raw mode, no despiking, time shifting or Honeywell cleanup.\n\
	Default all variables to sample rate output.\n\n\
 Real-time options:\n\
  -rt:	Real-time for ADS2 (obsolete).\n\
  -rt3:	Real-time for ADS3.\n\
  -x:	Produce and transmit SQL statements to ground (see groundvars file), add an\n\
        optional frequency which to transmit the data.  Default is every 5 seconds.\n\
  -y:   When running real-time on old data: do not broadcast IWG1 packets,\n\
        do not warn about time lags.\n\
\n\
NIDAS-related options:\n");

  NidasApp& napp = *NidasApp::getApplicationInstance();
  fprintf(stderr, "%s", napp.usage().c_str());
  exit(0);
}

/* END INIT.C */
