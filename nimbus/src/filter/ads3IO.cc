/*
-------------------------------------------------------------------------
OBJECT NAME:	ads3IO.cc

FULL NAME:	ADS3 Record IO routines

ENTRY POINTS:	ExtractHeaderIntoFile(char *ADSfileName)
		FindFirstLogicalRecord(char *record, long starttime)
		FindNextLogicalRecord(char *record, long endtime)
		FindNextDataRecord(char *record)

DESCRIPTION:	These routines locate data records that start with the
		ID = 0x8681.  (i.e. skips all PMS2D records).

INPUT:		Pointer to where to place the record
		Time of first record desired / Time last record desired

REFERENCED BY:	lrloop.c, hrloop.c

COPYRIGHT:	University Corporation for Atmospheric Research, 2005
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"

#include <SyncRecordReader.h>
#include <atdUtil/EOFException.h>

#include <ctime>
#include <iomanip>

extern dsm::SyncRecordReader* syncRecReader;

/* -------------------------------------------------------------------- */
long xlateToSecondsSinceMidnight(time_t ut)
{
  struct tm tm;
  gmtime_r(&ut, &tm);
//  int msec = (tt % USECS_PER_SEC) / USECS_PER_MSEC;

//  char timeStamp[64];
//  strftime(timeStamp, sizeof(timeStamp), "%Y-%m-%d %H:%M:%S", &tm);
//  std::cout << timeStamp << '.' << std::setw(3) << std::setfill('0') << msec
//        << ' ' << std::endl; 

  return tm.tm_hour * 3600 + tm.tm_min * 60 + tm.tm_sec;

}	// END XLATETOSECONDSSINCEMIDNIGHT

/* -------------------------------------------------------------------- */
long FindFirstLogicalADS3(
	char	record[],	// First Data Record, for start time
	long	startTime)	// User specified start time
{
  dsm::dsm_time_t tt;
  time_t ut = 0;
  int rc;

  while ((rc = syncRecReader->read(&tt, (float *)record, nSRfloats)) > 0)
  {
    ut = tt / USECS_PER_SEC;
    long recTime = xlateToSecondsSinceMidnight(ut);

    // @todo get rid of this one mid-night rollover bullshit.  i.e. use date/time
    //       12:00:00              23:59:59
    if (recTime < 43200L && startTime > 86399L)
      recTime += 86399L;

    if (startTime == BEG_OF_TAPE || recTime >= startTime)
      break;
  }

  processTimeADS3((float *)record, ut);
  return rc * sizeof(float);

}	// END FINDFIRSTLOGICALADS3

/* -------------------------------------------------------------------- */
long FindNextLogicalADS3(char record[], long endTime)
{
  dsm::dsm_time_t tt;
  int rc;

  if ((rc = syncRecReader->read(&tt, (float *)record, nSRfloats)) == 0)
    return 0;

  time_t ut = tt / USECS_PER_SEC;
  long recTime = xlateToSecondsSinceMidnight(ut);

  if (endTime != END_OF_TAPE)
  {
    /*       12:00:00            23:59:59       */
    if (recTime < 43200L && endTime > 86399L)
      recTime += 86399L;

    if (recTime > endTime)
      return 0;        /* End Of Time Segment  */
  }

  processTimeADS3((float *)record, ut);
  return rc * sizeof(float);

}	// END FINDNEXTLOGICALADS3

// END ADS3IO.CC
