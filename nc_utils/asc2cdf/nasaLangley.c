/*
-------------------------------------------------------------------------
OBJECT NAME:	nasaLangley.c

FULL NAME:	NASA ASCII to Nimbus-netCDF Low Rate

ENTRY POINTS:	SetNASABaseTime()
		CreateNASAlangNetCDF()

STATIC FNS:	none

DESCRIPTION:	Translate NASA ASCII file to Nimbus Low Rate netCDF file

REFERENCES:	none

REFERENCED BY:	main()

COPYRIGHT:	University Corporation for Atmospheric Research, 1996-7
-------------------------------------------------------------------------
*/

#include "define.h"

static int		baseTimeID;
static struct tm	StartFlight;

/* -------------------------------------------------------------------- */
void CreateNASAlangNetCDF(FILE *fp)
{
  int	i, j, cnt;
  int	year, month, day;
  int	ndims, dims[3], TimeDim, RateDim;
  char	*p, name[16], units[16], tmp[32];
  float	missingVal = MISSING_VALUE;


  /* Global Attributes.
   */
  strcpy(buffer, "NCAR-RAF/nimbus");
  nc_put_att_text(ncid, NC_GLOBAL, "Conventions", strlen(buffer)+1, buffer);

  {
  time_t	t;
  struct tm	tm;

  t = time(0);
  tm = *gmtime(&t);
  strftime(buffer, 128, "%h %d %R GMT %Y", &tm);
  nc_put_att_text(ncid, NC_GLOBAL, "DateConvertedFromASCII", 
                  strlen(buffer)+1, buffer);
  }


  fgets(buffer, BUFFSIZE, fp);
  SkipNlines = atoi(buffer);

  fgets(buffer, BUFFSIZE, fp);	/* Skip file name	*/

  fgets(buffer, BUFFSIZE, fp);
  buffer[strlen(buffer)-1] = '\0';
  nc_put_att_text(ncid, NC_GLOBAL, "PI", strlen(buffer)+1, buffer);

  fgets(buffer, BUFFSIZE, fp);
  buffer[strlen(buffer)-1] = '\0';
  nc_put_att_text(ncid, NC_GLOBAL, "Species", strlen(buffer)+1, buffer);

  fgets(buffer, BUFFSIZE, fp);
  buffer[strlen(buffer)-1] = '\0';
  nc_put_att_text(ncid, NC_GLOBAL, "ProjectName", strlen(buffer)+1, buffer);

  fgets(buffer, BUFFSIZE, fp);
  if (strchr(buffer, ','))
    sscanf(buffer, "%d , %d , %d", &year, &month, &day);
  else
    sscanf(buffer, "%d %d %d", &year, &month, &day);

  if (year > 1900) year -= 1900;
  StartFlight.tm_year = year;
  StartFlight.tm_mon = month - 1;
  StartFlight.tm_mday = day;

  sprintf(tmp, "%02d/%02d/%d", month, day, 1900 + year);
  nc_put_att_text(ncid, NC_GLOBAL, "FlightDate", strlen(tmp)+1, tmp);

  if (strchr(buffer, ','))
    sscanf(buffer, "%*d , %*d , %*d , %d , %d , %d", &year, &month, &day);
  else
    sscanf(buffer, "%*d %*d %*d %d %d %d", &year, &month, &day);

  if (year > 1900) year -= 1900;
  sprintf(buffer, "%02d/%02d/%d", month, day, 1900 + year);
  nc_put_att_text(ncid, NC_GLOBAL, "DateProcessed", strlen(buffer)+1, buffer);


  fgets(buffer, BUFFSIZE, fp);
  sprintf(tmp, "%d", atoi(buffer));
  nc_put_att_text(ncid, NC_GLOBAL, "FlightNumber", strlen(tmp)+1, tmp);

  /* Time segments.  Will be updated later.
   */
  nc_put_att_text(ncid, NC_GLOBAL, "TimeInterval", DEFAULT_TI_LENGTH, buffer);


  fgets(buffer, BUFFSIZE, fp);
  nVariables = atoi(buffer);

  fgets(buffer, BUFFSIZE, fp);	/* Skip nComment lines.	*/

  /* File type, we support type = 1 */
  fgets(buffer, BUFFSIZE, fp);
  if (atoi(buffer) != 1)
    {
    fprintf(stderr, "Only support data type = 1 (line 10).\n");
    exit(1);
    }


  /* First dimension is time dimension.
   * Second is Rate in Hz.
   * Third is Vector Length.
   */
  nc_def_dim(ncid, "Time", NC_UNLIMITED, &TimeDim);

  fgets(buffer, BUFFSIZE, fp);	/* Line 11, Averaging Rate	*/
  BaseDataRate = atoi(buffer);

  fgets(buffer, BUFFSIZE, fp);	/* Line 12, Data Rate (Hz)	*/
  dataRate = atoi(buffer);
  sprintf(buffer, "sps%d", dataRate);
  nc_def_dim(ncid, buffer, dataRate, &RateDim);


  if (dataRate > 1)
    {
    ndims = 2;
    dims[0] = TimeDim;
    dims[1] = RateDim;
    }
  else
    {
    ndims = 1;
    dims[0] = TimeDim;
    }


  /* Time Variables.
   */
  nc_def_var(ncid, "base_time", NC_INT, 0, 0, &baseTimeID);
  strcpy(buffer, "Seconds since Jan 1, 1970.");
  nc_put_att_text(ncid, baseTimeID, "long_name", strlen(buffer)+1, buffer);

  nc_def_var(ncid, "time_offset", NC_FLOAT, 1, dims, &timeOffsetID);
  strcpy(buffer, "Seconds since base_time.");
  nc_put_att_text(ncid, timeOffsetID, "long_name", strlen(buffer)+1, buffer);

  for (i = 0; i < 3; ++i)
    {
    p = time_vars[i];
    nc_def_var(ncid, time_vars[i], NC_FLOAT, 1, dims, &varid[i]);

    nc_put_att_text(ncid, varid[i], "units", strlen(p)+1, p);
    nc_put_att_text(ncid, varid[i], "long_name", strlen(p)+1, p);
    nc_put_att_int(ncid, varid[i], "OutputRate", NC_INT, 1, &rateOne);
    nc_put_att_float(ncid, varid[i], "missing_value", NC_FLOAT, 1, &missingVal);
    }


  /* For each variable:
   *	- Set dimensions
   *	- define variable
   *	- Set attributes
   */
  fgets(buffer, BUFFSIZE, fp);
  fgets(buffer, BUFFSIZE, fp);	/* Skip Julian day & seconds.	*/
  nVariables -= 2;

  for (i = 0; i < nVariables; ++i)
    {
    fgets(buffer, BUFFSIZE, fp);

    p = strtok(buffer, "\t ,");
    for (cnt = j = 0; j < strlen(p) && j < 16; ++j)
      if (isalnum(p[j]))
        name[cnt++] = p[j];

    name[cnt] = '\0';

    p = strtok(NULL, "\t ,");
    for (cnt = j = 0; j < strlen(p) && j < 16; ++j)
      if (isalnum(p[j]) || p[j] == '/')
        units[cnt++] = p[j];

    units[cnt] = '\0';


    p = strtok(NULL, "\t ,");
    scale[i] = atof(p);

    p = strtok(NULL, "\t ,");
    offset[i] = atof(p);

    p = strtok(NULL, "\t ,");
    p = strtok(NULL, "\t ,");
    p = strtok(NULL, "\t ,");
    missingVals[i] = atof(p);


printf("Adding variable %s with units of %s\n", name, units);

    nc_def_var(ncid, name, NC_FLOAT, ndims, dims, &varid[i+3]);
    nc_put_att_text(ncid, varid[i+3], "units", strlen(units)+1, units);
    nc_put_att_text(ncid, varid[i+3], "long_name", strlen(noTitle)+1, noTitle);
    nc_put_att_int(ncid, varid[i+3], "OutputRate", NC_INT, 1, &dataRate);
    nc_put_att_float(ncid,varid[i+3], "missing_value",NC_FLOAT, 1, &missingVal);
    }

  nc_enddef(ncid);

}	/* END CREATENASALANGNETCDF */

/* END NASALANGLEY.C */
