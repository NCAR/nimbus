/*
-------------------------------------------------------------------------
OBJECT NAME:	logfile.c

FULL NAME:	Log File

ENTRY POINTS:	OpenLogFile()
		CloseLogFile()
		CloseRemoveLogFile()

STATIC FNS:		

DESCRIPTION:	Allow user to edit Flight Header Info (i.e. Date, Flight
		number, etc...).

INPUT:		none

OUTPUT:			

REFERENCES:	none

REFERENCED BY:	cb_main.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1997
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include <unistd.h>

FILE		*LogFile = NULL;
static char	logFileName[256];

/* -------------------------------------------------------------------- */
void OpenLogFile()
{
  sprintf(logFileName, "%s/%s/Production/logFile.%d",
          ProjectDirectory, ProjectNumber, getpid());

  if ((LogFile = fopen(logFileName, "w")) == NULL) {
    sprintf(buffer, "Unable to create %s\n", logFileName);
    LogMessage(buffer);
    }

}	/* END OPENLOGFILE */

/* -------------------------------------------------------------------- */
void CloseLogFile()
{
  if (LogFile == NULL)
    return;

  fclose(LogFile);
  LogFile = NULL;

  sprintf(buffer, "%s/%s/Production/logFile_%s.%s",
          ProjectDirectory, ProjectNumber,
          cfg.ProcessingRate() == Config::LowRate ? "LRT" : "HRT",
          FlightNumber);

  if (rename(logFileName, buffer) == ERR)
    {
    strcpy(buffer, "\n>> Can't rename logFile.\n");
    LogMessage(buffer);
	fprintf(stderr, buffer);
    }

}	/* END CLOSELOGFILE */

/* -------------------------------------------------------------------- */
void CloseRemoveLogFile()
{
  if (LogFile == NULL)
    return;

  fclose(LogFile);
  LogFile = NULL;

  unlink(logFileName);

}	/* END CLOSEREMOVELOGFILE */

/* END LOGFILE.C */
