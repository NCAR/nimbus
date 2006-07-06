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

#include <nidas/dynld/raf/SyncRecordReader.h>
#include <nidas/util/EOFException.h>

#include <ctime>
#include <iomanip>

extern nidas::dynld::raf::SyncRecordReader* syncRecReader;

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
  nidas::core::dsm_time_t tt;
  time_t ut = 0;
  long recTime;
  int rc;

  do
  {
    try
    {
      rc = syncRecReader->read(&tt, (float *)record, nSRfloats);
    }
    catch (const nidas::util::IOException& e)
    {
      return 0;
    }

    ut = tt / USECS_PER_SEC;
    recTime = xlateToSecondsSinceMidnight(ut);

    /// @todo get rid of this one mid-night rollover bullshit.  i.e. use date/time
    //       12:00:00              23:59:59
    if (recTime < 43200L && startTime > 86399L)
      recTime += 86399L;
  }
  while (!(startTime == BEG_OF_TAPE || recTime >= startTime));

  processTimeADS3((float *)record, ut);
  return rc * sizeof(float);

}	// END FINDFIRSTLOGICALADS3

/* -------------------------------------------------------------------- */
long FindNextLogicalADS3(char record[], long endTime)
{
  nidas::core::dsm_time_t tt;
  int rc;

  try
  {
    rc = syncRecReader->read(&tt, (float *)record, nSRfloats);
  }
  catch (const nidas::util::IOException& e)
  {
    return 0;
  }
  
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

/* -------------------------------------------------------------------- */
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <set>

std::set<std::string> GetADSFileList(const char *adsFileName)
{
  std::set<std::string> fileList;
  DIR *dir;
  char tmp_dir[256], tmp_name[256];

  strcpy(tmp_dir, adsFileName);
  char *directory = dirname(tmp_dir);

  if ((dir = opendir(directory)) == 0)
  {
    fprintf(stderr, "Failed to open directory %s\n", tmp_dir);
    return fileList;
  }

  strcpy(tmp_name, adsFileName);
  char *file = basename(tmp_name);
  struct dirent *entry;

  while ( (entry = readdir(dir)) )
    if (strncmp(file, entry->d_name, 4) == 0 &&
        strcmp(file, entry->d_name) <= 0)
    {
      std::string s(directory);
      s += "/";
      s += entry->d_name;
      fileList.insert(s);
    }

//  std::set<std::string>::iterator it;
//  for (it = fileList.begin(); it != fileList.end(); ++it)
//    printf("%s\n", (*it).c_str());

  return fileList;
}

// END ADS3IO.CC
