/*
-------------------------------------------------------------------------
OBJECT NAME:	nc_sane.cc

FULL NAME:	netCDF sanity checker.

DESCRIPTION:	Check netCDF file for various consistency issues,
		so far all related to time.

COPYRIGHT:	University Corporation for Atmospheric Research, 2006
-------------------------------------------------------------------------
*/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "netcdf.h"

#define SecondsSinceMidnite(t) (t[0] * 3600 + t[1] * 60 + t[2])

void checkNumberRecords(int fd, const char * fileName);
void checkStarts(int fd, const char * fileName);

char	tiAttr[64];	// TimeInterval attribute.
bool	repairFile = false;

/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  int	argvIdx = 1;

  if (argc < 2)
  {
    fprintf(stderr,
	"Perform sanity check on a netcdf file.\n  Usage: nc_sane [-r] file_list\n");
    return 1;
  }

  if (strcmp(argv[argvIdx], "-r") == 0)
  {
    repairFile = true;
    ++argvIdx;
  }

  putenv("TZ=UTC");     // Perform all time calculations at UTC.


  for (; argvIdx < argc; ++argvIdx)
  {
    int	rc, InputFile;

    /* Open Input File
     */
    if (repairFile)
      rc = nc_open(argv[argvIdx], NC_WRITE, &InputFile);
    else
      rc = nc_open(argv[argvIdx], NC_NOWRITE, &InputFile);

    if (rc != NC_NOERR)
    {
      fprintf(stderr, "Can't nc_open file %s.", argv[argvIdx]);
      continue;
    }


    // Check TimeInterval.
    nc_get_att_text(InputFile, NC_GLOBAL, "TimeInterval", tiAttr);
    if (strcmp(tiAttr, "00:00:00-00:00:00") == 0)
    {
      printf("TimeInterval of 00:00:00-00:00:00, incomplete file.\n");
      nc_close(InputFile);
      continue;
    }

    checkNumberRecords(InputFile, argv[argvIdx]);
    checkStarts(InputFile, argv[argvIdx]);

    nc_close(InputFile);
  }
}

/* -------------------------------------------------------------------- */
void checkNumberRecords(int InputFile, const char * fileName)
{
  size_t FileStartTime[4], FileEndTime[4];

  sscanf(tiAttr, "%02d:%02d:%02d-%02d:%02d:%02d",
	&FileStartTime[0], &FileStartTime[1], &FileStartTime[2],
	&FileEndTime[0], &FileEndTime[1], &FileEndTime[2]);

  if (FileStartTime[0] > FileEndTime[0])
    FileEndTime[0] += 24;

  /* Seconds since midnight.
   */
  FileStartTime[3] = SecondsSinceMidnite(FileStartTime);
  FileEndTime[3] = SecondsSinceMidnite(FileEndTime);
  if ( FileStartTime[3] > FileEndTime[3] )
  {
    FileEndTime[0] += 24;
    FileEndTime[3] += 86400;
  }

  // Perform sanity check.
  int id;
  if (nc_inq_dimid(InputFile, "Time", &id) == NC_NOERR)
  {
    size_t length;
    size_t deltaT = (size_t)(FileEndTime[3] - FileStartTime[3]) + 1;

    nc_inq_dimlen(InputFile, id, &length);
    if (length != deltaT)
    {
      printf("%s: record count mis-match:", fileName);
      printf(" %d records vs. %d computed.\n", length, deltaT);
    }
  }
}

/* -------------------------------------------------------------------- */
/* In general the global attributes FlightDate & TimeInterval are accurate.
 * check that base_time & Time:units agree, repair if requested.
 */
void checkStarts(int fd, const char * fileName)
{
  int           id;
  char		buffer[128];
  size_t        edge[3];
  time_t        oldBaseTime, BaseTime;
  struct tm     StartFlight;

  edge[0] = edge[1] = edge[2] = 0;

  nc_get_att_text(fd, NC_GLOBAL, "FlightDate", buffer);
  sscanf(buffer, "%d/%d/%d",    &StartFlight.tm_mon,
                                &StartFlight.tm_mday,
                                &StartFlight.tm_year);

  sscanf(tiAttr, "%02d:%02d:%02d",&StartFlight.tm_hour,
                                &StartFlight.tm_min,
                                &StartFlight.tm_sec);

  StartFlight.tm_mon--;
  StartFlight.tm_year -= 1900;
  BaseTime = timegm(&StartFlight);

  // Check if base_time is ok.
  if (nc_inq_varid(fd, "base_time", &id) == NC_NOERR &&
      nc_get_var1_int(fd, id, edge, (int*)&oldBaseTime) == NC_NOERR &&
      oldBaseTime != BaseTime)
  {
    printf("%s: has incorrect base_time\n", fileName);
    printf("  base_time is (%u) %s",
                (unsigned)oldBaseTime, asctime(gmtime(&oldBaseTime)));
    printf("  should be is (%u) %s",
                (unsigned)BaseTime, asctime(gmtime(&BaseTime)));

    if (repairFile)
    {
      nc_put_var1_int(fd, id, edge, (int*)&BaseTime);
    }
  }


  // Check that variable Time::units attribute is correct.
  if (nc_inq_varid(fd, "Time", &id) == NC_NOERR)
  {
    char att[128], oldAtt[128];

    nc_get_att_text(fd, id, "units", oldAtt);

    strftime(att, 128, "seconds since %F %T %z", &StartFlight);

    if (strcmp(att, oldAtt) != 0)
    {
      printf("%s: has incorrect Time:units\n", fileName);
      printf("         is [%s]\n", oldAtt);
      printf("  should be [%s]\n", att);

      if (repairFile)
      {
        nc_redef(fd);
        nc_put_att_text(fd, id, "units", strlen(att)+1, att);
        if (nc_inq_varid(fd, "time_offset", &id) == NC_NOERR)
          nc_put_att_text(fd, id, "units", strlen(att)+1, att);
        nc_enddef(fd);
      }
    }
  }
}

