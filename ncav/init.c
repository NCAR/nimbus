/*
-------------------------------------------------------------------------
OBJECT NAME:	init.c

FULL NAME:	Initialize Global Variables

ENTRY POINTS:	Initialize()
		ProcessArgv()

STATIC FNS:		

DESCRIPTION:	

INPUT:		none

OUTPUT:		none

REFERENCES:	none

REFERENCED BY:	nimbus.c, cb_main.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-8
-------------------------------------------------------------------------
*/

#include <Xm/TextF.h>

#include "define.h"

char	*getenv();


/* -------------------------------------------------------------------- */
void Initialize()
{
  int	pos;
  char	*p;

  PauseFlag	= FALSE;
  PauseWhatToDo	= P_CONTINUE;
  AverageRate	= 10;	/* Default to 10 seconds	*/
  mvThreshold	= 100;

  if ((p = getenv("DATA_DIR")))
    {
    strcpy(buffer, p);
    strcat(buffer, "/");
    XmTextFieldSetString(inputFileText, buffer);
    XmTextFieldSetString(outputFileText, buffer);
    }

  pos = XmTextFieldGetLastPosition(inputFileText);
  XmTextFieldSetInsertionPosition(inputFileText, pos);

  pos = XmTextFieldGetLastPosition(outputFileText);
  XmTextFieldSetInsertionPosition(outputFileText, pos);

  XtSetSensitive(goButton, TRUE);

}	/* END INITIALIZE */

/* -------------------------------------------------------------------- */
void ProcessArgv(int argc, char **argv)
{
  int		i;

  Interactive = TRUE;

  for (i = 1; i < argc; ++i)
    {
    if (argv[i][0] != '-')
      {
      fprintf(stderr, "Invalid option %s, ignoring.\n", argv[i]);
      continue;
      }

    switch (argv[i][1])
      {
      case 'r':
        AverageRate = atoi(argv[++i]);
        break;

      case 'm':
        mvThreshold = atoi(argv[++i]);
        break;

      case 'b':
        Interactive = FALSE;
        ReadBatchFile(argv[++i]);
        break;

      default:
        fprintf(stderr, "Invalid option %s, ignoring.\n", argv[i]);
      }

    }

  printf("Using average ratio of %d:1.\n", AverageRate);
  printf("Using missing_value threshold of %d%%.\n", mvThreshold);

}	/* END PROCESSARGV */

/* -------------------------------------------------------------------- */
void ReadBatchFile(char file[])
{
  FILE	*fp;
  char	*p;
  int	i, indx;
  bool	varsFound = FALSE;
  static bool entryCnt = 0;
  static char fileName[256];


  /* This function gets called 3 times, 1st to save the fileName from the
   * command line, 2nd to get the file names, and 3rd to get time interval
   * and variable names.
   */
  if (++entryCnt == 1)
    {
    strcpy(fileName, file);
    return;
    }

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
      XmTextFieldSetString(inputFileText, strtok(NULL, " \t\n"));
    else
    if (strcmp(p, "of") == 0)
      XmTextFieldSetString(outputFileText, strtok(NULL, " \t\n"));
    else
    if (strcmp(p, "ti") == 0 && entryCnt > 2)
      {
      XmTextFieldSetString(ts_text[0], strtok(NULL, "- \t"));
      XmTextFieldSetString(ts_text[MAX_TIME_SLICES],strtok(NULL," \t\n"));
      ValidateTime(ts_text[0], NULL, NULL);
      ValidateTime(ts_text[MAX_TIME_SLICES], NULL, NULL);
      }
    else
    if (strcmp(p, "var") == 0 && entryCnt > 2)
      {
      p = strtok(NULL, " \t\n");

      if ((indx = SearchTable((char **)Variable, nVariables, p)) != ERR)
        {
        varsFound = TRUE;
        Variable[indx]->Output = TRUE;
        }
      else
        fprintf(stderr, "ncav: no such variable: %s.\n", p);
      }
    }

  fclose(fp);

  if (!varsFound && entryCnt > 2)
    for (i = 0; i < nVariables; ++i)
      Variable[i]->Output = TRUE;

}   /* END READBATCHFILE */

/* END INIT.C */
