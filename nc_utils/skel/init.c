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

#include <Xm/TextF.h>

#include "define.h"


char	*getenv();

static void ReadBatchFile();

/* -------------------------------------------------------------------- */
void Initialize()
{
	int		pos;

	PauseFlag	= FALSE;
	PauseWhatToDo	= P_CONTINUE;

	pos = XmTextFieldGetLastPosition(inputFileText);
	XmTextFieldSetInsertionPosition(inputFileText, pos);

	pos = XmTextFieldGetLastPosition(outputFileText);
	XmTextFieldSetInsertionPosition(outputFileText, pos);

	XtSetSensitive(goButton, TRUE);

}	/* END INITIALIZE */

/* -------------------------------------------------------------------- */
void ProcessArgv(argc, argv)
int		argc;
char	**argv;
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
			case 'b':
				Interactive = FALSE;
				ReadBatchFile(argv[++i]);
				break;

			default:
				fprintf(stderr, "Invalid option %s, ignoring.\n", argv[i]);
			}

		}

}	/* END PROCESSARGV */

/* -------------------------------------------------------------------- */
static void ReadBatchFile(fileName)
char	*fileName;
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
			XmTextFieldSetString(inputFileText, strtok(NULL, " \t\n"));
		else
		if (strcmp(p, "of") == 0)
			XmTextFieldSetString(outputFileText, strtok(NULL, " \t\n"));
		else
		if (strcmp(p, "ti") == 0)
			{
			XmTextFieldSetString(ts_text[0], strtok(NULL, "- \t"));
			XmTextFieldSetString(ts_text[MAX_TIME_SLICES],strtok(NULL," \t\n"));
			ValidateTime(ts_text[0], NULL, NULL);
			ValidateTime(ts_text[MAX_TIME_SLICES], NULL, NULL);
			}
		}

	fclose(fp);

}	/* END READBATCHFILE */

/* END INIT.C */
