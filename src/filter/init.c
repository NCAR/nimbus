/*
-------------------------------------------------------------------------
OBJECT NAME:	init.c

FULL NAME:	Initialize Global Variables

ENTRY POINTS:	Initialize()
		ProcessArgv()
		ReadBatchFile()

STATIC FNS:		

DESCRIPTION:	

INPUT:		none

OUTPUT:		none

REFERENCES:	none

REFERENCED BY:	nimbus.c, cb_main.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include <unistd.h>
#include <sys/types.h>
#include <Xm/TextF.h>

#include "nimbus.h"
#include "decode.h"
#include "amlib.h"
#include "gui.h"

static void ReadBatchFile(char *filename);

void	Set_SetupFileName(char s[]);
void	RTinit();

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

  Mode			= POST_PROCESSING;
  LITTON51_present	= false;
  AVAPS			= false;
  PauseFlag		= false;
  ProductionRun		= false;
  QCenabled		= false;
  AsyncFileEnabled	= false;
  RawData		= false;
  ProcessingRate	= LOW_RATE;
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
  if (getuid() == 130)
    ProductionRun = true;

  if (ProductionRun)
    XtSetSensitive(outputFileText, false);

}	/* END INITIALIZE */

/* -------------------------------------------------------------------- */
void ProcessArgv(int argc, char **argv)
{
  int	i;

  Interactive = LoadProductionSetupFile = true;

  for (i = 1; i < argc; ++i)
    {
    if (argv[i][0] != '-')
      {
      fprintf(stderr, "Invalid option %s, ignoring.\n", argv[i]);
      continue;
      }

    switch (argv[i][1])
      {
      case 'b':
        Interactive = false;
        ReadBatchFile(argv[++i]);
        break;

      case 'r':
        RawData = true;

        if (argv[i][2] == 't')	/* -rt	*/
          RTinit();

        break;

      case 'n':
        LoadProductionSetupFile = false;
        break;

      case 'q':
        QCenabled = true;
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
      if ((ProcessingRate = atoi(strtok(NULL, " \t\n"))) != HIGH_RATE)
        ProcessingRate = LOW_RATE;
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

/* END INIT.C */
