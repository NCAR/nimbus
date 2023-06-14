/*
-------------------------------------------------------------------------
OBJECT NAME:	logfile.c

FULL NAME:	Log File

ENTRY POINTS:	OpenLogFile()
		CloseLogFile()
		CloseRemoveLogFile()

STATIC FNS:	none

DESCRIPTION:	Routines to support a logfile.

COPYRIGHT:	University Corporation for Atmospheric Research, 1997-2006
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include <unistd.h>
#include <sys/param.h>
#include <Xm/Text.h>

FILE	*LogFile = 0;
static char	logFileName[MAXPATHLEN];


/* -------------------------------------------------------------------- */
void OpenLogFile()
{
  char tmp[MAXPATHLEN];

  MakeProjectFileName(tmp, LOGFILE);
  snprintf(logFileName, MAXPATHLEN, "%s.%d", tmp, getpid());

  if ((LogFile = fopen(logFileName, "w")) == NULL)
  {
    snprintf(buffer, 8192, "Unable to create %s\n", logFileName);
    LogMessage(buffer);
  }

}	/* END OPENLOGFILE */

/* -------------------------------------------------------------------- */
void LogMessage(std::string msg)
{
  if (msg[msg.length()-1] != '\n')
    msg.append("\n");

  if (cfg.Interactive())
  {
    XmTextPosition      position;
    extern Widget       logText;

    position = XmTextGetLastPosition(logText);
    XmTextInsert(logText, position, (char*)msg.c_str());

    position += msg.length();
    XmTextShowPosition(logText, position);
    XmTextSetInsertionPosition(logText, position);
  }
  else
    fprintf(stderr, msg.c_str());

  if (LogFile)
    fprintf(LogFile, msg.c_str());

}       /* END LOGMESSAGE */

/* -------------------------------------------------------------------- */
void CloseLogFile()
{
  if (LogFile == NULL)
    return;

  fclose(LogFile);
  LogFile = NULL;

  strcpy(buffer, logFileName);
  char tmp[MAXPATHLEN], *p = strrchr(buffer, '.');

  snprintf(tmp, MAXPATHLEN, "_%s.%s",
          cfg.ProcessingRate() == Config::HighRate ? "HRT" : "LRT",
          cfg.FlightNumber().c_str());
  strcpy(p, tmp);

  if (rename(logFileName, buffer) == ERR)
  {
    char msg[512];
    snprintf(msg, 128, "\n>> Can't rename logFile: %s to %s.\n",
			logFileName, buffer);
    LogMessage(msg);
    fprintf(stderr, msg);
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
