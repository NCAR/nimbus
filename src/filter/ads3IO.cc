/*
-------------------------------------------------------------------------
OBJECT NAME:	ads3IO.cc

FULL NAME:	ADS3 Record IO routines

ENTRY POINTS:	FindFirstLogicalADS3(char *record, long starttime)
		FindNextLogicalADS3(char *record, long endtime)
		GetADSFileList(char *adsFileName)

DESCRIPTION:	

INPUT:		Pointer to where to place the record
		Time of first record desired / Time last record desired

REFERENCED BY:	lrloop.c, hrloop.c

COPYRIGHT:	University Corporation for Atmospheric Research, 2005-06
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"

#include <nidas/dynld/raf/SyncRecordReader.h>
#include <nidas/util/EOFException.h>

#include <iomanip>
#include <sys/param.h>

extern nidas::dynld::raf::SyncRecordReader* syncRecReader;

/* -------------------------------------------------------------------- */
time_t xlateToSecondsSinceMidnight(time_t ut) // Unused now.
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


#ifdef DEBUG
#include <sstream>
#include <nidas/dynld/raf/SyncRecordVariable.h>
#include <nidas/util/UTime.h>
using nidas::util::UTime;
using namespace nidas::dynld::raf;

static void
sync_read_debug(SyncRecordReader* srr, nidas::core::dsm_time_t tt, 
		char record[], int rc)
{
  static std::string tfmt("At %Y/%m/%d;%H:%M:%S.%3f: ");
  NR_TYPE* dp = (NR_TYPE*)record;
  const char* vnames[] = { "CAVP_DPR" };
  std::vector<std::string> vdebug(vnames, vnames + sizeof(vnames)/sizeof(vnames[0]));
  std::ostringstream msg;
  msg << UTime(tt).format(tfmt);
  for (std::vector<std::string>::iterator vp = vdebug.begin(); 
       vp != vdebug.end(); ++vp)
  {
    const SyncRecordVariable* var = srr->getVariable(*vp);
    if (var)
    {
      msg << var->getName() << " = ";
      NR_TYPE* first = dp + var->getSyncRecOffset();
      for (NR_TYPE* fp = first ; fp < first+25; ++fp)
      {
	msg << *fp << ", ";
      }
    }
  }
  msg << "\n";
  for (int i = 0; i < 6; ++i)
  {
    msg << "record@timeIndex[" << i << "] = " << dp[timeIndex[i]] << ", ";
  }
  msg << "\n"
      << "read() returned " << rc << " values, or " << rc*sizeof(NR_TYPE) << " bytes.";

  std::cerr << msg.str() << std::endl;
}
#endif


/* -------------------------------------------------------------------- */
int32_t FindFirstLogicalADS3(
	char	record[],	// First Data Record, for start time
	time_t	startTime)	// User specified start time
{
  nidas::core::dsm_time_t tt;
  time_t recTime = 0;
  int rc;

  do
  {
    try
    {
      rc = syncRecReader->read(&tt, (NR_TYPE *)record, syncRecReader->getNumValues());
    }
    catch (const nidas::util::IOException& e)
    {
      return 0;
    }

    recTime = tt / USECS_PER_SEC;
  }
  while (!(startTime == BEG_OF_TAPE || recTime >= startTime));

  processTimeADS3((NR_TYPE *)record, recTime);
#ifdef DEBUG
  sync_read_debug(syncRecReader, tt, record, rc);
#endif
  return rc * sizeof(NR_TYPE);

}	// END FINDFIRSTLOGICALADS3

/* -------------------------------------------------------------------- */
int32_t FindNextLogicalADS3(char record[], time_t endTime)
{
  nidas::core::dsm_time_t tt;
  int rc;

  try
  {
    rc = syncRecReader->read(&tt, (NR_TYPE *)record, syncRecReader->getNumValues());
  }
  catch (const nidas::util::IOException& e)
  {
    return 0;
  }
  
  time_t recTime = tt / USECS_PER_SEC;

  if (endTime != END_OF_TAPE)
  {
    if (recTime > endTime)
      return 0;        /* End Of Time Segment  */
  }

  processTimeADS3((NR_TYPE *)record, recTime);
#ifdef DEBUG
  sync_read_debug(syncRecReader, tt, record, rc);
#endif
  return rc * sizeof(NR_TYPE);

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
  char tmp_dir[MAXPATHLEN];

  strcpy(tmp_dir, adsFileName);
  char *directory = dirname(tmp_dir);

  if ((dir = opendir(directory)) == 0)
  {
    sprintf(buffer, "Failed to open directory %s\n", directory);
    perror(buffer);
    return fileList;
  }

  struct dirent *entry;

  // Read directory entries & get all files with matching flight number.
  while ( (entry = readdir(dir)) )
    if ( strstr(entry->d_name, cfg.FlightNumber().c_str()) &&
         strstr(entry->d_name, cfg.ADSfileExtension().c_str()))
    {
      std::string s(directory);
      s += "/";
      s += entry->d_name;
      fileList.insert(s);
    }

  std::set<std::string>::iterator it;
  for (it = fileList.begin(); it != fileList.end(); ++it)
    if ((*it).compare(adsFileName) < 0)
      fileList.erase(*it);

  for (it = fileList.begin(); it != fileList.end(); ++it)
    printf("%s\n", (*it).c_str());

  return fileList;
}

// END ADS3IO.CC