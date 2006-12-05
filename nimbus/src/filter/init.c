/*
-------------------------------------------------------------------------
OBJECT NAME:	init.c

FULL NAME:	Initialize

ENTRY POINTS:	Initialize()
		ProcessArgv()
		ReadBatchFile()

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-05
-------------------------------------------------------------------------
*/

#include <unistd.h>
#include <sys/types.h>
#include <Xm/TextF.h>

#include "nimbus.h"
#include "decode.h"
#include "amlib.h"
#include "gui.h"

static void ReadBatchFile(char *filename), usage();
static const std::string NIMBUS_RUNNING_CMD =
	"ps ax | grep \"nimbus -r\" | grep -v grep";

void	Set_SetupFileName(char s[]);

void	RTinit_ADS2(), RTinit_ADS3();

/* -------------------------------------------------------------------- */
void Initialize()
{
  int	pos;

  if ((ProjectDirectory = (char *)getenv("PROJ_DIR")) == NULL)
    {
    fprintf(stderr,
      "Environment variable PROJ_DIR not defined, this is fatal.\n");
    exit(1);
    }

  putenv("TZ=UTC");	// Perform all time calculations at UTC.

  LITTON51_present	= false;
  AVAPS			= false;
  PauseFlag		= false;
  PauseWhatToDo		= P_CONTINUE;
  FeedBack		= LOW_RATE_FEEDBACK;
  sync_server_pipe[0]	= '\0';

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
  int runCnt = 0;
  FILE *fp = popen(NIMBUS_RUNNING_CMD.c_str(), "r");

  while (fgets(buffer, 256, fp) != 0)
  {
    printf(buffer);
    runCnt++;
  }

  pclose(fp);
  return runCnt > 1 ? true : false;
}

/* -------------------------------------------------------------------- */
void ProcessArgv(int argc, char **argv)
{
  int	i;

  cfg.SetInteractive(true);
  cfg.SetLoadProductionSetup(true);

  for (i = 1; i < argc; ++i)
  {
    if (strstr(argv[i], "help"))
      usage();

    if (argv[i][0] != '-')
    {
      fprintf(stderr, "Invalid option %s, ignoring.\n", argv[i]);
      continue;
    }

    switch (argv[i][1])
    {
      case 'h':
        usage();
        break;
      case 'b':
        cfg.SetInteractive(false);
        ReadBatchFile(argv[++i]);
        break;
      case 'r':
        if (nimbusIsAlreadyRunning())
	{
	  fprintf(stderr, "nimbus is already running in real-time mode, exiting.\n");
	  sleep(5);
          exit(1);
	}
        cfg.SetProcessingRate(Config::SampleRate);
        cfg.SetTimeShifting(false);
        cfg.SetDespiking(false);
        cfg.SetOutputSQL(true);
        if (strcmp(argv[i], "-rt") == 0)	/* RealTime ADS2 */
          RTinit_ADS2();
        else
        if (strcmp(argv[i], "-rt3") == 0)	/* RealTime ADS3 */
          RTinit_ADS3();
        else
        {
          cfg.SetOutputSQL(false);	// Not yet.
          cfg.SetHoneyWellCleanup(false); // We want this in real-time.
          cfg.SetInertialShift(false);	// We want this in real-time.
        }
        break;
      case 'x':
        if (cfg.ProcessingMode() == Config::RealTime)
          cfg.SetTransmitToGround(true);
        else
          fprintf(stderr, "Must be in RealTime mode to TransmitToGround, ignoring -x.\n");
        break;
      case 'n':
        cfg.SetLoadProductionSetup(false);
        break;
      case 'q':
        cfg.SetQCenabled(true);
        break;
      default:
        fprintf(stderr, "Invalid option %s, ignoring.\n", argv[i]);
    }
  }
}	/* END PROCESSARGV */

/* -------------------------------------------------------------------- */
static void ReadBatchFile(char *fileName)
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
      XmTextFieldSetString(aDSdataText, strtok(NULL, " \t\n"));
    else
    if (strcmp(p, "of") == 0)
      XmTextFieldSetString(outputFileText, strtok(NULL, " \t\n"));
    else
    if (strcmp(p, "sf") == 0)
      Set_SetupFileName(strtok(NULL, " \t\n"));
    else
    if (strcmp(p, "pr") == 0)
      {
      int value = atoi(strtok(NULL, " \t\n"));
      switch (value)
        {
        case Config::SampleRate:
          cfg.SetProcessingRate(Config::SampleRate);
          break;
        case Config::LowRate:
          cfg.SetProcessingRate(Config::LowRate);
          break;
        case 25:
        case 50:
          cfg.SetProcessingRate(Config::HighRate);
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
  fprintf(stderr, "Usage: nimbus [-b batch_file] [[-r[t[3]] -x] [-n]\n\n\
  -b:	Loads a batch_file instead of going interactive.  File options\n\
	are (last 3 are optional):\n\
	if=input_file.ads\n\
	of=output_file.nc\n\
	sf=setup_file\n\
	pr=processing_rate [SampleRate=0, LowRate=1, HighRate=25]\n\
	ti=time_interval [xx:xx:xx-xx:xx:xx]\n\n\
  -r:	Raw mode, no despiking, time shifting or Honeywell cleanup.\n\
	Default all variables to sample rate output.\n\
  -rt:	Real-time for ADS2.\n\
  -rt3:	Real-time for ADS3.\n\
  -x:	Produce and transmit SQL statements to ground (see groundvars file and LDM pqinsert(1)).\n\
  -n:	Do NOT load any existing production setup files.\n");

  exit(0);
}

/* END INIT.C */
