/*
-------------------------------------------------------------------------
OBJECT NAME:	asc2cdf.c

FULL NAME:	ASCII to Nimbus-netCDF Low Rate

ENTRY POINTS:	main()

STATIC FNS:	ProcessArgv()
		WriteMissingData()

DESCRIPTION:	Translate ASCII file to Nimbus Low Rate netCDF file

COPYRIGHT:	University Corporation for Atmospheric Research, 1996-2007
-------------------------------------------------------------------------
*/

#include "define.h"

#include <sys/types.h>
#include <sys/stat.h>

char	buffer[BUFFSIZE];

int	ncid;
int	baseTimeID, timeOffsetID, timeVarID, varid[MAX_VARS], nVariables;
time_t	BaseTime = 0;
float	scale[MAX_VARS], offset[MAX_VARS], missingVals[MAX_VARS];

char FlightDate[50];

static FILE	*inFP;
static size_t	nRecords;
static char	*globalAttrFile = 0;

/* Command line option flags.
 */
InputFileType	fileType = PLAIN_FILE;
bool	secondsSinceMidnight = false, Colonless = false;
bool    verbose = false, histogram = false;
int	SkipNlines = 1;

int		BaseDataRate = 1, dataRate = 1;
const int	rateOne = 1;

const char	*noTitle = "No Title";
const char	*noUnits = "Unk";

void addGlobalAttrs(const char *p), WriteBaseTime();

static int ProcessArgv(int argc, char **argv);
static void WriteMissingData(int, int);


/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  int	i, hz, hour, minute, second, firstSecond, prevSecond = -1;
  float currSecond;
  int   subsec;
  int	startHour, startMinute, startSecond;
  char	*p;
  float	dataValue;
  size_t index[3];

  putenv((char *)"TZ=UTC");	// All time calcs in UTC.
  FlightDate[0] = 0;

  i = ProcessArgv(argc, argv);

  if ((argc - i) < 2)
    {
    fprintf(stderr, "Usage: asc2cdf [-b time_t] [-a] [-l] [-m] [-r n] [-s n] ascii_file output.nc\n");
    exit(1);
    }

  if ((inFP = fopen(argv[i], "r")) == NULL)
    {
    fprintf(stderr, "Can't open input file %s.\n", argv[i]);
    exit(1);
    }


  if (nc_create(argv[i+1], NC_CLOBBER, &ncid) != NC_NOERR)
    {
    fprintf(stderr, "Can't destroy/create output file %s.\n", argv[i+1]);
    exit(1);
    }


  switch (fileType)
    {
    case PLAIN_FILE:
      CreatePlainNetCDF(inFP);
      SetPlainBaseTime();
      break;

    case NASA_AMES:
      CreateNASAamesNetCDF(inFP);
      break;

    case NASA_LANGLEY:
      CreateNASAlangNetCDF(inFP);
      break;
    }

  addGlobalAttrs(globalAttrFile);
  nc_enddef(ncid);

  WriteBaseTime();

  printf("Averaging Period = %d, Data Rate = %dHz\n", BaseDataRate, dataRate);


  /* Start uploading data.
   */
  rewind(inFP);

  while (SkipNlines--)
    fgets(buffer, BUFFSIZE, inFP);


  for (nRecords = 0; fgets(buffer, BUFFSIZE, inFP); )
    {
    if (strlen(buffer) < 5)
      continue;

    p = strtok(buffer, ", \t");

    if (fileType == NASA_LANGLEY)	/* Skip Julian day	*/
      p = strtok(NULL, ", \t");


    if (secondsSinceMidnight)
      {
      currSecond = atof(p);

      hour = int(currSecond) / 3600; currSecond -= hour * 3600;
      minute = int(currSecond) / 60; currSecond -= minute * 60;
      second = int(currSecond);
      if (dataRate > 1) {
        currSecond -= second; subsec = int(currSecond*dataRate+.5);
      } else {
        subsec = 0;
      }

      if (nRecords == 0 && fileType != PLAIN_FILE)
        SetNASABaseTime(hour, minute, second);
      }
    else
    if (Colonless)
      {
      second = atoi(&p[4]); p[4] = '\0';
      minute = atoi(&p[2]); p[2] = '\0';
      hour = atoi(p);
      }
    else
      sscanf(p, "%d:%d:%d", &hour, &minute, &second);

    if (hour > 23)
      hour -= 24;

    currSecond = hour * 3600 + minute * 60 + second;

    // Watch for midnight wrap around.
    if (currSecond < firstSecond)
      currSecond += 86400;

    if (prevSecond == -1) // 1st time through loop.
    {
      firstSecond = int(currSecond);
//      if (strlen(FlightDate) > 0)
      if (fileType != NASA_AMES)
        BaseTime += int(currSecond);
    }

    if (nRecords == 0)
      {
      startHour = hour;
      startMinute = minute;
      startSecond = second;
      }
    else
    if (currSecond == prevSecond)
      {
      printf("Duplicate time stamp, ignoring.\n");
      prevSecond = int(currSecond);
      continue;
      }
    else
    if (currSecond > prevSecond + BaseDataRate)
      {
      if (currSecond - prevSecond > 2)
        printf("last time = %d, new time = %d\n", prevSecond, int(currSecond));
//      WriteMissingData(currSecond, prevSecond);
      }

    prevSecond = int(currSecond);

    dataValue = currSecond - firstSecond;
    size_t rec = int(dataValue);
    nc_put_var1_float(ncid, timeVarID, &rec, &dataValue);
    nc_put_var1_float(ncid, timeOffsetID, &rec, &dataValue);
//    dataValue = (float)(nRecords * BaseDataRate);
//    nc_put_var1_float(ncid, timeVarID, &nRecords, &dataValue);
//    nc_put_var1_float(ncid, timeOffsetID, &nRecords, &dataValue);

    //If subseconds are given in the time column, offset here.-JAG
    for (hz = subsec; hz < dataRate; ++hz)
      {
      // If histogram data, zeroth data bin must be zero for legacy
      // reasons.
      if (histogram) {
	dataValue = 0;
        index[0] = rec; index[1] = hz; index[2] = 0;
        nc_put_var1_float(ncid, varid[1], index, &dataValue);
      }
      for (i = 0; i < nVariables; ++i)
        {
        if ((p = strtok(NULL, ", \t\n\r")) == NULL)
          break;

        dataValue = atof(p);

        if (fileType != PLAIN_FILE)
          {
          if (dataValue == missingVals[i])
            dataValue = MISSING_VALUE;
          else
            dataValue = dataValue * scale[i] + offset[i];
          }

        if (histogram) {
          index[0] = rec; index[1] = hz; index[2] = i+1;
          nc_put_var1_float(ncid, varid[1], index, &dataValue);
        }
        else
        {
          index[0] = rec; index[1] = hz;
          nc_put_var1_float(ncid, varid[i], index, &dataValue);
        }
      }

      if (hz != dataRate-1)
        if (fgets(buffer, BUFFSIZE, inFP) == NULL)
          break;

      p = strtok(buffer, ", \t");

      if (fileType == NASA_LANGLEY)     /* Skip Julian day      */
        p = strtok(NULL, ", \t");
      }

    ++nRecords;
    }

  fclose(inFP);

  // If FlightDate was specified, we need to re-write the base_time & Time::units
  // since we didn't know the start time of the first data record, just the date
  // of the flight.
  WriteBaseTime();

  nc_redef(ncid);
  SetPlainBaseTime();

  sprintf(buffer, "%02d:%02d:%02d-%02d:%02d:%02d",
          startHour, startMinute, startSecond, hour, minute, second);

  nc_put_att_text(ncid, NC_GLOBAL, "TimeInterval", strlen(buffer)+1, buffer);
  printf("Time interval completed = %s\n", buffer);
  nc_enddef(ncid);
  nc_close(ncid);

  return(0);

}	/* END MAIN */

/* -------------------------------------------------------------------- */
static void WriteMissingData(int currSecond, int lastSecond)
{
  float	dataValue;

  for (int i = lastSecond; i < currSecond; i += BaseDataRate, ++nRecords)
  {
    dataValue = (float)(nRecords * BaseDataRate);
    nc_put_var1_float(ncid, timeOffsetID, &nRecords, &dataValue);

    dataValue = MISSING_VALUE;

    for (int j = 0; j < nVariables; ++j)
      nc_put_var1_float(ncid, varid[j], &nRecords, &dataValue);
  }
}	/* END WRITEMISSINGDATA */

/* -------------------------------------------------------------------- */
static int ProcessArgv(int argc, char **argv)
{
  int	i;

  for (i = 1; i < argc; ++i)
  {
    if (argv[i][0] != '-')
      break;

    switch (argv[i][1])
    {
      case 'b':
        if (strlen(FlightDate) > 0)
          fprintf(stderr, "-d option trumps -b option, BaseTime ignored.\n");
        else
          BaseTime = atoi(argv[++i]);
        break;

      case 'd':
        {
        struct tm tm;
        strptime(argv[++i], "%F", &tm);
        tm.tm_hour = tm.tm_min = tm.tm_sec = 0;
        BaseTime = mktime(&tm);
        sprintf(FlightDate, "%02d/%02d/%4d", tm.tm_mon+1, tm.tm_mday, tm.tm_year+1900);
        }
        break;

      case 'm':
        secondsSinceMidnight = true;
        break;

      case 'a':
        fileType = NASA_AMES;
        secondsSinceMidnight = true;
        break;

      case 'h':
	histogram = true;
	break;

      case 'l':
        fileType = NASA_LANGLEY;
        secondsSinceMidnight = true;
        break;

      case 'g':
        globalAttrFile = argv[++i];
        break;

      case 'r':
        {
        float rate = atof(argv[++i]);
        if (rate < 1.0)
          BaseDataRate = (int)(1.0 / rate + 0.5);
        if (rate > 1.0)
          dataRate = (int)rate;
        }
        break;

      case 's':
        SkipNlines = atoi(argv[++i]);
        break;

      case 'v':
        verbose = true;
        break;

      case ':':
        Colonless = true;
        break;

      default:
        fprintf(stderr, "Invalid option %s, ignoring.\n", argv[i]);
    }
  }

  return(i);

}	/* END PROCESSARGV */

/* END ASC2CDF.C */
