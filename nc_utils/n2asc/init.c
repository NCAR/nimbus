/*
-------------------------------------------------------------------------
OBJECT NAME:	init.c

FULL NAME:	Initialize Global Variables

ENTRY POINTS:	Initialize()
		ProcessArgv()
		ReadFormatFile()

STATIC FNS:		

DESCRIPTION:	

INPUT:		none

OUTPUT:		none

REFERENCES:	none

REFERENCED BY:	nimbus.c, cb_main.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
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

  XaxisType	= TIME;
  PauseFlag	= FALSE;
  PauseWhatToDo	= P_CONTINUE;

  strcpy(DefaultFormat, " %.3f");

  if ( (p = getenv("DATA_DIR")) )
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
void ProcessArgv(int argc, char *argv[])
{
  int	i;

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
      case 'a':
        AmesFormat = TRUE;
        XaxisType = UTS;
        break;

      case 'b':
        Interactive = FALSE;
        ReadBatchFile(argv[++i]);
        break;

      case 'f':	/* Default Format	*/
        strcpy(DefaultFormat, " ");
        strcat(DefaultFormat, argv[++i]);
        break;

      case 'n':	/* Change Xaxis to Colonless time stamp	*/
        XaxisType = COLONLESS;
        break;

      case 'c':	/* Change Xaxis to Running Counter	*/
        XaxisType = RUNCOUNT;
        break;

      default:
        fprintf(stderr, "Invalid option %s, ignoring.\n", argv[i]);
      }

    }

}	/* END PROCESSARGV */

/* -------------------------------------------------------------------- */
void ReadBatchFile(char file[])
{
  char		*p;
  FILE		*fp;
  int		indx;
  static bool	entryCnt = 0;
  static char	fileName[256];

  if (++entryCnt == 1)
    {
    strcpy(fileName, file);
    return;
    }


  if ((fp = fopen(fileName, "r")) == NULL)
    {
    fprintf(stderr, "Can't open batch file %s.\n", fileName);
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

      if ((indx = SearchTable(Variable, nVariables, p)) != ERR)
        Variable[indx]->Output = TRUE;
      else
        fprintf(stderr, "n2asc: no such variable: %s.\n", p);
      }
    }

  fclose(fp);

}	/* END READBATCHFILE */

/* -------------------------------------------------------------------- */
void ReadFormatFile()
{
  FILE	*fp;
  char	*var, *format, *home, target[16];
  int	indx;

  home = getenv("HOME");

  strcpy(buffer, home);
  strcat(buffer, "/nimbus/.n2asc");

  if ((fp = fopen(buffer, "r")) == NULL)
    return;


  while (fgets(buffer, 512, fp))
    {
    if (buffer[0] == COMMENT)
      continue;

    if ((var = strtok(buffer, " \t=")) == NULL)
      continue;

    if ((indx = SearchTableSansLocation(Variable, nVariables, var)) == ERR)
      continue;

    format = strtok(NULL, "\n");
    strcpy(target, var);
    strcat(target, "_");

    do
      {
      strcpy(Variable[indx]->Format, " ");
      strcat(Variable[indx]->Format, format);
      }
    while (strncmp((char *)Variable[++indx], target, strlen(target)) == 0);
    }

  fclose(fp);

}	/* END READFORMATFILE */

/* END INIT.C */
