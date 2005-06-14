/*
-------------------------------------------------------------------------
OBJECT NAME:	asc2cdf.c

FULL NAME:	ASCII to Nimbus-netCDF Low Rate

ENTRY POINTS:	main()

STATIC FNS:		

DESCRIPTION:	Translate ASCII file to Nimbus Low Rate netCDF file

INPUT:		ASCII file.

OUTPUT:		Nimbus netCDF file.

REFERENCES:		

REFERENCED BY:	User

COPYRIGHT:	University Corporation for Atmospheric Research, 1996-7
-------------------------------------------------------------------------
*/

#define _XOPEN_SOURCE
#include "define.h"
#include "constants.h"

#include <sys/types.h>
#include <sys/stat.h>

char	buffer[BUFFSIZE];

int	ncid;
FILE	*inFP;
int	baseTimeID, timeOffsetID, timeVarID, varid[MAX_VARS],
	nVariables, nRecords;
time_t	BaseTime = 0;
float	scale[MAX_VARS], offset[MAX_VARS], missingVals[MAX_VARS];
char	*time_vars[] = {"HOUR", "MINUTE", "SECOND"};
struct tm StartFlight;


/* Command line option flags.
 */
bool	fileType = PLAIN_FILE, sexSinceMidnight = FALSE;
int	SkipNlines = 1;

int		BaseDataRate = 1, dataRate = 10;
const int	rateOne = 1;

const char	*noTitle = "No Title";
const char	*noUnits = "Unk";


static int ProcessArgv(int argc, char **argv);
static void WriteMissingData(int, int);


/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  int	i, hz, hour, minute, second, currSecond;
  int	startHour, startMinute, startSecond;
  char	*p;
  float	dataValue;
  size_t	index[2];

  i = ProcessArgv(argc, argv);

  if ((argc - i) < 2)
    {
    fprintf(stderr, "Usage: asc2cdf [-a] [-l] [-m] [-r n] [-s n] ascii_file output.nc\n");
    exit(1);
    }

  if ((inFP = fopen(argv[i], "r")) == NULL)
    {
    fprintf(stderr, "Can't open %s.\n", argv[i]);
    exit(1);
    }


  if (nc_create(argv[i+1], NC_CLOBBER, &ncid) != NC_NOERR)
    {
    fprintf(stderr, "Can't destroy %s.\n", argv[i+1]);
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
      SetNASABaseTime();
      break;

    case NASA_LANGLEY:
      CreateNASAlangNetCDF(inFP);
      SetNASABaseTime();
      break;
    }


  if (BaseDataRate < 1)
    BaseDataRate = 1;

  printf("Averaging Period = %d, Data Rate = %dHz\n", BaseDataRate, dataRate);


  /* Start uploading data.
   */
  rewind(inFP);

  while (SkipNlines--)
    fgets(buffer, BUFFSIZE, inFP);


  for (nRecords = 0; fgets(buffer, BUFFSIZE, inFP); )
    {
    if (strlen(buffer) < 2)
      continue;

    p = strtok(buffer, ", \t");

    if (fileType == NASA_LANGLEY)	/* Skip Julian day	*/
      p = strtok(NULL, ", \t");


    if (sexSinceMidnight)
      {
//      currSecond = atof(p);

      if (nRecords == 0)	// Roll our own time, for non-uniform time.
        currSecond = atof(p);
      else
        ++currSecond;

//if (atoi(p) == 70200) printf("19:30 = %d\n", currSecond);
//if (atoi(p) == 73800) printf("20:30 = %d\n", currSecond);

      hour = currSecond / 3600; currSecond -= hour * 3600;
      minute = currSecond / 60; currSecond -= minute * 60;
      second = currSecond;
      }
    else
      {
      hour = atoi(p);
      minute = atoi(&p[3]);
      second = atoi(&p[6]);
      }

    if (hour > 23)
      hour -= 24;

    currSecond = hour * 3600 + minute * 60 + second;

    if (nRecords == 0)
      {
      startHour = hour;
      startMinute = minute;
      startSecond = second;
      }

    dataValue = (float)(nRecords * BaseDataRate);
    nc_put_var1_float(ncid, timeOffsetID, &nRecords, &dataValue);
    nc_put_var1_float(ncid, timeVarID, &nRecords, &dataValue);
    dataValue = (float)hour;
    nc_put_var1_float(ncid, varid[0], &nRecords, &dataValue);
    dataValue = (float)minute;
    nc_put_var1_float(ncid, varid[1], &nRecords, &dataValue);
    dataValue = (float)second;
    nc_put_var1_float(ncid, varid[2], &nRecords, &dataValue);

    for (hz = 0; hz < dataRate; ++hz)
      {
      for (i = 0; i < nVariables; ++i)
        {
        p = strtok(NULL, ", \t\r\n");
        dataValue = atof(p);

        if (fileType != PLAIN_FILE)
          {
          if (dataValue == missingVals[i])
            dataValue = MISSING_VALUE;
          else
            dataValue = dataValue * scale[i] + offset[i];
          }

        index[0] = nRecords; index[1] = hz;
        nc_put_var1_float(ncid, varid[i+3], index, &dataValue);
        }

      if (hz != dataRate-1)
        if (fgets(buffer, BUFFSIZE, inFP) == NULL)
          break;

      p = strtok(buffer, ", \t");

      if (fileType == NASA_LANGLEY)	/* Skip Julian day	*/
        p = strtok(NULL, ", \t");
      }

    ++nRecords;
    }


  sprintf(buffer, "%02d:%02d:%02d-%02d:%02d:%02d",
          startHour, startMinute, startSecond, hour, minute, second);
printf("TimeInterval = %s\n", buffer);
  nc_put_att_text(ncid, NC_GLOBAL, "TimeInterval", strlen(buffer)+1, buffer);
  nc_close(ncid);
  chmod(argv[2], 0666);

  fclose(inFP);

  return(0);

}	/* END MAIN */

/* -------------------------------------------------------------------- */
void AddTimeVariables(int dims[])
{
  nc_def_var(ncid, "base_time", NC_INT, 0, 0, &baseTimeID);
  strcpy(buffer, "seconds since 1970-01-01 00:00:00 +0000");
  nc_put_att_text(ncid, baseTimeID, "units", strlen(buffer)+1, buffer);
  strcpy(buffer, "Start time of data recording.");
  nc_put_att_text(ncid, baseTimeID, "long_name", strlen(buffer)+1, buffer);

  nc_def_var(ncid, "Time", NC_INT, 1, dims, &timeVarID);
  strcpy(buffer, "time of measurement");
  nc_put_att_text(ncid, timeVarID, "long_name", strlen(buffer)+1, buffer);
  strcpy(buffer, "time");
  nc_put_att_text(ncid, timeVarID, "standard_name", strlen(buffer)+1, buffer);

  nc_def_var(ncid, "time_offset", NC_FLOAT, 1, dims, &timeOffsetID);
  strcpy(buffer, "Seconds since base_time.");
  nc_put_att_text(ncid, timeOffsetID, "long_name", strlen(buffer)+1, buffer);


  strftime(buffer, 256, "seconds since %F %T +0000", &StartFlight);
  nc_put_att_text(ncid, timeVarID, "units", strlen(buffer)+1, buffer);
  nc_put_att_text(ncid, timeOffsetID, "units", strlen(buffer)+1, buffer);

}

/* -------------------------------------------------------------------- */
static void WriteMissingData(int currSecond, int lastSecond)
{
  int	i, j, ts;
  int	hour, minute, second;
  float	dataValue;

  ts = (lastSecond += BaseDataRate);

  hour = ts / 3600; ts -= hour * 3600;
  minute = ts / 60; ts -= minute * 60;
  second = ts;

  for (i = lastSecond; i < currSecond; i += BaseDataRate, ++nRecords)
    {
    dataValue = (float)(nRecords * BaseDataRate);
    nc_put_var1_float(ncid, timeOffsetID, &nRecords, &dataValue);
    dataValue = (float)hour;
    nc_put_var1_float(ncid, varid[0], &nRecords, &dataValue);
    dataValue = (float)minute;
    nc_put_var1_float(ncid, varid[1], &nRecords, &dataValue);
    dataValue = (float)second;
    nc_put_var1_float(ncid, varid[2], &nRecords, &dataValue);

    dataValue = MISSING_VALUE;

    for (j = 0; j < nVariables; ++j)
      nc_put_var1_float(ncid, varid[j+3], &nRecords, &dataValue);

    if ((second += BaseDataRate) > 59)
      {
      second = 0;
      if (++minute > 59)
        {
        minute = 0;
        if (++hour > 23)
          hour = 0;
        }
      }
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
        strptime(argv[++i], "%F %T +0000", &StartFlight);
        BaseTime = timegm(&StartFlight);
//        BaseTime = mktime(&StartFlight) - timezone;
        break;

      case 'm':
        sexSinceMidnight = TRUE;
        break;

      case 'a':
        fileType = NASA_AMES;
        sexSinceMidnight = TRUE;
        break;

      case 'l':
        fileType = NASA_LANGLEY;
        sexSinceMidnight = TRUE;
        break;

      case 'r':
        BaseDataRate = atoi(argv[++i]);
        break;

      case 's':
        SkipNlines = atoi(argv[++i]);
        break;

      default:
        fprintf(stderr, "Invalid option %s, ignoring.\n", argv[i]);
      }

    }

  return(i);

}	/* END PROCESSARGV */

/* END ASC2CDF.C */
