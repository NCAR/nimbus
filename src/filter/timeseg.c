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
#include "NetCDF.h"
#include <raf/header.h>

#include "timeseg.h"

#include <sstream>
using std::ostringstream;

#define NEW_SEG		(-1)

static int	nTimeIntervals;

/* User specified time intervals, stored as int's	*/
static time_t	prevTime,
		UserBtim[MAX_TIME_SLICES*4],
		UserEtim[MAX_TIME_SLICES*4];

/* Actual time intervals in output file.			*/
static int	currentTimeSegment;
static int	BtimeInt[MAX_TIME_SLICES*4][3],
		EtimeInt[MAX_TIME_SLICES*4][3];

extern NetCDF *ncFile;


/* -------------------------------------------------------------------- */
void GetUserTimeIntervals() /* From TimeSliceWindow	*/
{
  int offset;
  char	*bp, *ep;
  struct tm ft;

  memset(&ft, 0, sizeof(struct tm));
  nTimeIntervals = 0;
  currentTimeSegment = (-1);

  sscanf(	cfg.FlightDate().c_str(), "%d/%d/%d",
		&ft.tm_mon, &ft.tm_mday, &ft.tm_year);

  ft.tm_mon -= 1;
  ft.tm_year -= 1900;

  for (size_t i = 0; i < MAX_TIME_SLICES; ++i)
  {
    bp = XmTextFieldGetString(ts_text[i]);
    ep = XmTextFieldGetString(ts_text[i+MAX_TIME_SLICES]);

    if (strlen(bp) == 0 && strlen(ep) == 0)
      continue;

    sscanf(bp, "%02d:%02d:%02d", &ft.tm_hour, &ft.tm_min, &ft.tm_sec);
    if (ft.tm_hour > 23)
    {
      ft.tm_hour -= 24;
      offset = 86400;
    }
    else
      offset = 0;
    UserBtim[nTimeIntervals] = mktime(&ft) + offset;
    struct tm *start = gmtime(&UserBtim[nTimeIntervals]);

    // Reset the FlightDate in case we moved the start time past midnight.
    sprintf(buffer, "%02d/%02d/%d", start->tm_mon+1, start->tm_mday, start->tm_year+1900);
    cfg.SetFlightDate(buffer);

    sscanf(ep, "%02d:%02d:%02d", &ft.tm_hour, &ft.tm_min, &ft.tm_sec);
    if (ft.tm_hour > 23)
    {
      ft.tm_hour -= 24;
      offset = 86400;
    }
    else
      offset = 0;
    UserEtim[nTimeIntervals] = mktime(&ft) + offset;

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
  ostringstream msg;
  const char* fields[] = { "HOUR", "MINUTE", "SECOND", "YEAR", "MONTH", "DAY" };
  for (int i = 0; i < 6; ++i)
  {
    RAWTBL* vp = raw[SearchTable(raw, fields[i])];
    timeIndex[i] = vp->SRstart;
    msg << "timeIndex[" << i << "] (" << vp->name << ") = " << timeIndex[i] << ", ";
  }
#ifdef DEBUG
  std::cerr << msg.str() << "\n";
#endif

}	/* END GETUSERTIMEINTERVALS */

/* -------------------------------------------------------------------- */
int NextTimeInterval(time_t *start, time_t *end)
{
  if (++currentTimeSegment >= nTimeIntervals)
    return false;

  prevTime = NEW_SEG;

  *start	= UserBtim[currentTimeSegment];
  *end		= UserEtim[currentTimeSegment];

  BtimeInt[currentTimeSegment][0] = NEW_SEG;

  return true;

}	/* END NEXTTIMEINTERVAL */


// Get the largest time window within which data will be processed.
// Everything outside this window will be ignored.
void
GetTimeWindow(time_t* start, time_t* end)
{
  *start = BEG_OF_TAPE;
  *end = END_OF_TAPE;
  if (nTimeIntervals > 0)
  {
    *start = UserBtim[0];
    *end = UserEtim[nTimeIntervals-1];
  }
}


/* -------------------------------------------------------------------- */
void ResetTimeGapper()
{
  prevTime = NEW_SEG;
}

/* -------------------------------------------------------------------- */
time_t HdrBlkTimeToSeconds(Hdr_blk * hdr)
{
  struct tm ft;

  ft.tm_hour = ntohs(hdr->hour);
  ft.tm_min = ntohs(hdr->minute);
  ft.tm_sec = ntohs(hdr->second);
  ft.tm_year = ntohs(hdr->year);
  ft.tm_mon = ntohs(hdr->month)-1;
  ft.tm_mday = ntohs(hdr->day);

  if (ft.tm_year < 1000)   // We now have year 2070 problem.
  {
    if (ft.tm_year < 70)
      ft.tm_year += 100;
  }
  else
    ft.tm_year -= 1900;

  return mktime(&ft);
}

/* -------------------------------------------------------------------- */
time_t SampledDataTimeToSeconds(const NR_TYPE * record)
{
  struct tm ft;

  // Quell valgrind concerns
  memset(&ft, 0, sizeof(ft));

  ft.tm_hour = (int)record[timeIndex[0]];
  ft.tm_min = (int)record[timeIndex[1]];
  ft.tm_sec = (int)record[timeIndex[2]];
  ft.tm_year = (int)record[timeIndex[3]]-1900;
  ft.tm_mon = (int)record[timeIndex[4]]-1;
  ft.tm_mday = (int)record[timeIndex[5]];

  return mktime(&ft);
}

/* -------------------------------------------------------------------- */
time_t UTCseconds(const void *record)
{
  time_t t;

  if (cfg.isADS2())
    t = HdrBlkTimeToSeconds((Hdr_blk *)record);
  else
    t = SampledDataTimeToSeconds((const NR_TYPE *)record);

  return t;
}

/* -------------------------------------------------------------------- */
int CheckForTimeGap(const void *ADShdr, int initMode)
{
  int i, j;

  time_t newTime = UTCseconds(ADShdr);

  /* If everthing is peachy, then bail out.
   */
  if (prevTime+1 == newTime || prevTime == NEW_SEG ||
        (cfg.isADS2() && newTime == 0 && prevTime == 86399L))
  {
    prevTime = newTime;
    return false;
  }


  if (initMode)
  {
    LogMessage("Non-sequential time, moving up start time.\n");
    prevTime = NEW_SEG;
    return GAP_FOUND;
  }


  if (prevTime >= newTime)
  {
    extern char     *ADSrecord;
    struct tm *pft, *nft;

    pft = gmtime(&prevTime);
    nft = gmtime(&newTime);

    while (prevTime >= newTime)
    {
      if (prevTime == newTime)
        LogMessage("Advancing past duplicate time stamp:\n");
      else
        LogMessage("Advancing past odd-ball time stamp:\n");

      sprintf(buffer,
        "  previous time = %02d:%02d:%02d\n       odd time = %02d:%02d:%02d\n",
	pft->tm_hour, pft->tm_min, pft->tm_sec,
	nft->tm_hour, nft->tm_min, nft->tm_sec);
      LogMessage(buffer);

      if ((*FindNextLogicalRecord)(ADSrecord, END_OF_TAPE) <= 0)
        return GAP_FOUND;

      newTime = UTCseconds(ADShdr);
      nft = gmtime(&newTime);
    }

    sprintf(	buffer, "    advanced to = %02d:%02d:%02d\n",
		nft->tm_hour, nft->tm_min, nft->tm_sec);
    LogMessage(buffer);

    return CheckForTimeGap(ADSrecord, initMode);
  }


  /* Time gaps less than 2 hour, fill with records with MISS_VAL
   */
  if (newTime - prevTime < 7200)	/* 7200 seconds.	*/
  {
    struct tm *pft, *nft;

    pft = gmtime(&prevTime);
    nft = gmtime(&newTime);

    sprintf(buffer, "Time break of %ld seconds ending @ %02d:%02d:%02d, filling in with MISSING_VALUE.\n",
	newTime - prevTime - 1, nft->tm_hour, nft->tm_min, nft->tm_sec);
    LogMessage(buffer);

    ncFile->QueueMissingData(pft->tm_hour, pft->tm_min, pft->tm_sec, newTime - prevTime - 1);

    prevTime = newTime;
    return false;
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
    return GAP_FOUND;
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

    ++nTimeIntervals;

    return GAP_FOUND;
  }


  LogMessage("CheckForTimeGap: you shouldn't receive this message.\n");
  prevTime = newTime;
  return GAP_FOUND;

}	/* END CHECKFORTIMEGAP */

/* -------------------------------------------------------------------- */
void UpdateTime(const NR_TYPE *record)
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
      ncFile->Sync();
  }
}	/* END UPDATETIME */

/* -------------------------------------------------------------------- */
void FormatTimeSegmentsForOutputFile(char *buff)
{
  int	i;
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
void PrintHHMMSS(const NR_TYPE *recp)
{
  // Function to print record time; for logging and debugging.
  printf("%02d:%02d:%02d", (int)recp[timeIndex[0]], (int)recp[timeIndex[1]], (int)recp[timeIndex[2]]);
}

/* -------------------------------------------------------------------- */
time_t GetPreviousTime()
{
  return prevTime;
}

/* END TIMESEG.C */
