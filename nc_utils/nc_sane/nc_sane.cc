#include <cstdio>
#include "netcdf.h"

#define SecondsSinceMidnite(t) (t[0] * 3600 + t[1] * 60 + t[2])

/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  char buffer[512];

  if (argc < 2)
  {
    fprintf(stderr, "Perform sanity check on a netcdf file.\n  Usage: nc_sane file_list\n");
    return 1;
  }


  for (size_t i = 1; i < argc; ++i)
  {
    int InputFile;
    size_t FileStartTime[4], FileEndTime[4];

    /* Open Input File
     */
    if (nc_open(argv[i], NC_NOWRITE, &InputFile) != NC_NOERR)
    {
      fprintf(stderr, "Can't open %s.", argv[i]);
      continue;
    }

    nc_get_att_text(InputFile, NC_GLOBAL, "TimeInterval", buffer);
    sscanf(buffer, "%02d:%02d:%02d-%02d:%02d:%02d",
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

    // Perform a sanity check.
    int id;
    if (nc_inq_dimid(InputFile, "Time", &id) == NC_NOERR)
    {
      size_t length;
      size_t deltaT = (size_t)(FileEndTime[3] - FileStartTime[3]) + 1;
      nc_inq_dimlen(InputFile, id, &length);
      if (length != deltaT)
      {
        fprintf(stderr, "Sanity check failed for file %s.", argv[i]);
        fprintf(stderr, " %d records vs. %d computed.\n", length, deltaT);
      }
    }

    nc_close(InputFile);
  }
  return 0;
}
