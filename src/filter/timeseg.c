/*
-------------------------------------------------------------------------
OBJECT NAME:	timeseg.c

FULL NAME:	Time Segment Routines

ENTRY POINTS:	GetUserTimeIntervals()		Used by StartProcesing()
		NextTimeInterval()		Used by StartProcesing()
		CheckForTimeGap()		lrloop.c & hrloop.c
		UpdateTime()			lrloop.c & hrloop.c
		FormatTimeSegmentsForOutputFile()	netcdf.c

STATIC FNS:	none

DESCRIPTION:	Functions should be used in the order shown above.
		User specified time intervals may not correspond to
		what is actually output (e.g. User requests 00:00:00 -
		15:00:00, and ADS actually started recording at 13:00:00).

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-05
-------------------------------------------------------------------------
*/

#include <netinet/in.h>

#include <Xm/TextF.h>

#include "nimbus.h"
#include "decode.h"
#include "gui.h"
#include "header.h"

#define NEW_SEG		(-1)

static int	nTimeIntervals;

/* User specified time intervals, stored as int's	*/
static long	prevTime,
		UserBtim[MAX_TIME_SLICES*4],
		UserEtim[MAX_TIME_SLICES*4];

/* Actual time intervals in output file.			*/
static int	currentTimeSegment;
static int	BtimeInt[MAX_TIME_SLICES*4][3],
		EtimeInt[MAX_TIME_SLICES*4][3];

void QueueMissingData(int h, int m, int s, int nRecords);


/* -------------------------------------------------------------------- */
void GetUserTimeIntervals() /* From TimeSliceWindow	*/
{
  int	i;
  int	hour, minute, second;
  char	*bp, *ep;

  nTimeIntervals = 0;
  currentTimeSegment = (-1);

  for (i = 0; i < MAX_TIME_SLICES; ++i)
    {
    bp = XmTextFieldGetString(ts_text[i]);
    ep = XmTextFieldGetString(ts_text[i+MAX_TIME_SLICES]);

    if (strlen(bp) == 0 && strlen(ep) == 0)
      continue;

    sscanf(bp, "%02d:%02d:%02d", &hour, &minute, &second);
    UserBtim[nTimeIntervals] = (hour * 3600) + (minute * 60) + second;

    sscanf(ep, "%02d:%02d:%02d", &hour, &minute, &second);
    UserEtim[nTimeIntervals] = (hour * 3600) + (minute * 60) + second;

    ++nTimeIntervals;
    }

  if (nTimeIntervals == 0)
    {
    UserBtim[0] = BEG_OF_TAPE;
    UserEtim[0] = END_OF_TAPE;

    ++nTimeIntervals;
    }

  /* And while we're here let's get the indices for the time variables,
   * HOUR, MINUTE, SECOND, used by UpdateTime().
   */
  timeIndex[0] = raw[SearchTable(raw, "HOUR")]->SRstart;
  timeIndex[1] = raw[SearchTable(raw, "MINUTE")]->SRstart;
  timeIndex[2] = raw[SearchTable(raw, "SECOND")]->SRstart;

}	/* END GETUSERTIMEINTERVALS */

/* -------------------------------------------------------------------- */
int NextTimeInterval(long *start, long *end)
{
  if (++currentTimeSegment >= nTimeIntervals)
    return(false);

  prevTime = NEW_SEG;

  *start	= UserBtim[currentTimeSegment];
  *end		= UserEtim[currentTimeSegment];

  BtimeInt[currentTimeSegment][0] = NEW_SEG;

  return(true);

}	/* END NEXTTIMEINTERVAL */

/* -------------------------------------------------------------------- */
void ResetTimeGapper()
{
  prevTime = NEW_SEG;
}

/* -------------------------------------------------------------------- */
long UTCseconds(const void *record, int *h, int *m, int *s)
{
  if (cfg.isADS2())
  {
    const Hdr_blk *r = (Hdr_blk *)record;
    *h = (int)ntohs(r->hour);
    *m = (int)ntohs(r->minute);
    *s = (int)ntohs(r->second);
  }
  else
  {
    const NR_TYPE *r = (NR_TYPE *)record;
    *h = (int)r[timeIndex[0]];
    *m = (int)r[timeIndex[1]];
    *s = (int)r[timeIndex[2]];
  }

  return *h * 3600 + *m * 60 + *s;
}

/* -------------------------------------------------------------------- */
int CheckForTimeGap(void *ADShdr, int initMode)
{
  long	i, j;
  long	newTime;
  int	hour, minute, second;

  newTime = UTCseconds(ADShdr, &hour, &minute, &second);

  /* If everthing is peachy, then bail out.
   */
  if (prevTime+1 == newTime || prevTime == NEW_SEG ||
      (newTime == 0 && prevTime == 86399L))	/* Midnight crossover */
    {
    prevTime = newTime;
    return(false);
    }


  if (initMode)
    {
    LogMessage("Non-sequential time, moving up start time.\n");
    prevTime = NEW_SEG;
    return(GAP_FOUND);
    }


  if (prevTime >= newTime)
    {
    extern char     *ADSrecord;
    int		h, m, s;

    h = prevTime / 3600;
    m = (prevTime - (h * 3600)) / 60;
    s = (prevTime - (h * 3600 + m * 60));

    while (prevTime >= newTime)
      {
      if (prevTime == newTime)
        LogMessage("Advancing past duplicate time stamp:\n");
      else
        LogMessage("Advancing past odd-ball time stamp:\n");

      sprintf(buffer,
        "  previous time = %02d:%02d:%02d\n       odd time = %02d:%02d:%02d\n",
      	h, m, s, hour, minute, second);
      LogMessage(buffer);

      if ((*FindNextLogicalRecord)(ADSrecord, END_OF_TAPE) <= 0)
        return(GAP_FOUND);

      newTime = UTCseconds(ADShdr, &hour, &minute, &second);
      }

    sprintf(buffer, "    advanced to = %02d:%02d:%02d\n", hour, minute, second);
    LogMessage(buffer);

    return(CheckForTimeGap(ADSrecord, initMode));
    }


  /* Time gaps less than 1 hour, fill with records with MISS_VAL
   */
  if (newTime - prevTime < 3600)	/* 3600 seconds.	*/
    {
    int		h, m, s;

    h = prevTime / 3600;
    m = (prevTime - (h * 3600)) / 60;
    s = (prevTime - (h * 3600 + m * 60));


    sprintf(buffer, "Time break of %ld seconds ending @ %02d:%02d:%02d, filling in with MISSING_VALUE.\n",
	newTime - prevTime - 1, hour, minute, second);
    LogMessage(buffer);

    QueueMissingData(h, m, s, newTime - prevTime - 1);

    prevTime = newTime;
    return(false);
    }



  /* It's a long time gap, introduce a new time segment.
   */
 
  /* Locate current time segment
   */
  for (i = 0; i < nTimeIntervals; ++i)
    {
    if (prevTime > UserBtim[i] && prevTime <= UserEtim[i])
      break;

    if (UserEtim[i] == END_OF_TAPE)
      break;
    }

  if (i == nTimeIntervals)
    {
    LogMessage("Impossible, no time segment currently active.\n");
    LogMessage("  This usually results from a tape with no EOF marker.\n");
    return(GAP_FOUND);
    }


  /* Insert new time segment.
   */
  if (newTime <= UserEtim[i] || UserEtim[i] == END_OF_TAPE)
    {
    sprintf(buffer, "Break in time sequence of %ld seconds, ", newTime-prevTime);
    LogMessage(buffer);

    LogMessage("adding new time interval.\n");

    for (j = nTimeIntervals; j > i; --j)
      {
      UserEtim[j] = UserEtim[j-1];
      UserBtim[j] = UserBtim[j-1];
      }

    UserEtim[i] = prevTime;
    UserBtim[i+1] = newTime;


    if (cfg.ProcessingRate() == Config::HighRate)
      UserBtim[i+1] += 14;
    else
      UserBtim[i+1] += 2;

    ++nTimeIntervals;

    return(GAP_FOUND);
    }


  LogMessage("CheckForTimeGap: you shouldn't receive this message.\n");
  prevTime = newTime;
  return(GAP_FOUND);

}	/* END CHECKFORTIMEGAP */

/* -------------------------------------------------------------------- */
void UpdateTime(NR_TYPE *record)
{
  int	hour = (int)record[timeIndex[0]],
	minute = (int)record[timeIndex[1]],
	second = (int)record[timeIndex[2]];

  if (BtimeInt[currentTimeSegment][0] == NEW_SEG)
    {
    BtimeInt[currentTimeSegment][0] = hour;
    BtimeInt[currentTimeSegment][1] = minute;
    BtimeInt[currentTimeSegment][2] = second;
    }

  EtimeInt[currentTimeSegment][0] = hour;
  EtimeInt[currentTimeSegment][1] = minute;
  EtimeInt[currentTimeSegment][2] = second;

  sprintf(buffer, "%02d:%02d:%02d", hour, minute, second);

  if (cfg.Interactive() && second == 0)
    {
    XmTextFieldSetString(timeDisplayText, buffer);
    FlushXEvents();

    if (minute == 0)
      SyncNetCDF();
    }

}	/* END UPDATETIME */

/* -------------------------------------------------------------------- */
void FormatTimeSegmentsForOutputFile(char *buff)
{
  int		i;
  char	temp[32];

  buff[0] = '\0';

  for (i = 0; i < nTimeIntervals; ++i)
    {
    if (i > 0)
      strcat(buff, ", ");

    if (BtimeInt[i][0] == NEW_SEG)
      BtimeInt[i][0] = 0;

    sprintf(temp, "%02d:%02d:%02d-%02d:%02d:%02d",
		BtimeInt[i][0], BtimeInt[i][1], BtimeInt[i][2],
		EtimeInt[i][0], EtimeInt[i][1], EtimeInt[i][2]);

    strcat(buff, temp);
    }

}	/* END FORMATTIMESEGMENTSFOROUTPUTFILE */

/* -------------------------------------------------------------------- */
int GetPreviousTime()
{
  return(prevTime);

}	/* END GETPREVIOUSTIME */

/* END TIMESEG.C */
