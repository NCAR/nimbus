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

#include <Xm/Text.h>

FILE	*LogFile = 0;
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
void LogMessage(const char msg[])
{
  std::string messg(msg);

  if (messg[messg.length()-1] != '\n')
    messg.append("\n");

  if (cfg.Interactive())
    {
    XmTextPosition      position;
    extern Widget       logText;

    position = XmTextGetInsertionPosition(logText);
    XmTextInsert(logText, position, (char*)messg.c_str());

    position += messg.length();
    XmTextShowPosition(logText, position);
    XmTextSetInsertionPosition(logText, position);
    }
  else
    fprintf(stderr, messg.c_str());

  if (LogFile)
    fprintf(LogFile, messg.c_str());

}       /* END LOGMESSAGE */

/* -------------------------------------------------------------------- */
void CloseLogFile()
{
  if (LogFile == NULL)
    return;

  fclose(LogFile);
  LogFile = NULL;

  sprintf(buffer, "%s/%s/Production/logFile_%s.%s",
          ProjectDirectory, ProjectNumber,
          cfg.ProcessingRate() == Config::HighRate ? "HRT" : "LRT",
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
