/*
-------------------------------------------------------------------------
OBJECT NAME:	timeseg.c

FULL NAME:	Time Segment Routines

ENTRY POINTS:	GetUserTimeIntervals()		Used by StartProcesing()
		NextTimeInterval()		Used by StartProcesing()
		UpdateTime()			oppo.c & mrf.c
		FormatTimeSegmentsForOutputFile() netcdf.c

STATIC FNS:	none

DESCRIPTION:	Functions should be used in the order shown above.
		User specified time intervals may not correspond to
		what is actually output (e.g. User requests 00:00:00 -
		15:00:00, and ADS actually started recording at 13:00:00).

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include <Xm/TextF.h>

#include "define.h"

#define NEW_SEG		(-1)

static int	nTimeIntervals;

/* User specified time intervals, stored as int's	*/
static long	prev_time,
		UserBtim[MAX_TIME_SLICES*4],
		UserEtim[MAX_TIME_SLICES*4];

/* Actual time intervals in output file.		*/
static int	currentTimeSegment;
static int	BtimeInt[MAX_TIME_SLICES*4][3],
		EtimeInt[MAX_TIME_SLICES*4][3];


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

}	/* END GETUSERTIMEINTERVALS */

/* -------------------------------------------------------------------- */
bool NextTimeInterval(long *start, long *end)
{
  if (++currentTimeSegment >= nTimeIntervals)
    return(false);

  prev_time = NEW_SEG;

  *start= UserBtim[currentTimeSegment];
  *end	= UserEtim[currentTimeSegment];
  long delta = *end - *start;
  if (delta < 0)
  {
    delta += 86400;
    fprintf(stderr, "timeseg.c: NextTimeInterval: delta < 0, adding 86400.\n");
  }

  BtimeInt[currentTimeSegment][0] = NEW_SEG;

  if (*start != BEG_OF_TAPE)
  {
    struct tm *tm = gmtime(&FileStartTime);
    int btim = (tm->tm_hour * 3600) + (tm->tm_min * 60) + tm->tm_sec;
    tm->tm_sec += (*start - btim);
    *start = mktime(tm);
  }

  if (*end != END_OF_TAPE)
  {
    *end = *start + delta;
  }

  return(true);

}	/* END NEXTTIMEINTERVAL */

/* -------------------------------------------------------------------- */
void UpdateTime(struct tm * currentTime)
{
  if (BtimeInt[currentTimeSegment][0] == NEW_SEG)
    {
    BtimeInt[currentTimeSegment][0] = currentTime->tm_hour;
    BtimeInt[currentTimeSegment][1] = currentTime->tm_min;
    BtimeInt[currentTimeSegment][2] = currentTime->tm_sec;
    }

  EtimeInt[currentTimeSegment][0] = currentTime->tm_hour;
  EtimeInt[currentTimeSegment][1] = currentTime->tm_min;
  EtimeInt[currentTimeSegment][2] = currentTime->tm_sec;

  if (Interactive && currentTime->tm_sec == 0)
    {
    sprintf(buffer, "%02d:%02d:00", currentTime->tm_hour, currentTime->tm_min);
    XmTextFieldSetString(timeDisplayText, buffer);

    FlushXEvents();
    }

}	/* END UPDATETIME */

/* -------------------------------------------------------------------- */
void FormatTimeSegmentsForOutputFile(char buff[])
{
  int	i;
  char	temp[32];

  buff[0] = '\0';

  for (i = 0; i < nTimeIntervals; ++i)
    {
    if (i > 0)
      strcat(buff, ", ");

    sprintf(temp, "%02d:%02d:%02d-%02d:%02d:%02d",
    BtimeInt[i][0], BtimeInt[i][1], BtimeInt[i][2],
    EtimeInt[i][0], EtimeInt[i][1], EtimeInt[i][2]);

    strcat(buff, temp);
    }
 
}	/* END FORMATTIMESEGMENTSFOROUTPUTFILE */
 
/* END TIMESEG.C */
