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
  sprintf(logFileName, "%s.%d", tmp, getpid());

  if ((LogFile = fopen(logFileName, "w")) == NULL)
  {
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

    position = XmTextGetLastPosition(logText);
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

  strcpy(buffer, logFileName);
  char tmp[MAXPATHLEN], *p = strrchr(buffer, '.');

  sprintf(tmp, "_%s.%s",
          cfg.ProcessingRate() == Config::HighRate ? "HRT" : "LRT",
          cfg.FlightNumber().c_str());
  strcpy(p, tmp);

  if (rename(logFileName, buffer) == ERR)
  {
    char msg[512];
    sprintf(msg,	"\n>> Can't rename logFile: %s to %s.\n",
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
