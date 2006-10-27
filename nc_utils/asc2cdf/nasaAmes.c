/*
-------------------------------------------------------------------------
OBJECT NAME:	nasaAmes.c

FULL NAME:	NASA Ames ASCII to Nimbus-netCDF Low Rate

ENTRY POINTS:	SetNASABaseTime()
		CreateNASAamesNetCDF()

STATIC FNS:	none

DESCRIPTION:	Translate NASA ASCII file to Nimbus Low Rate netCDF file

REFERENCES:	none

REFERENCED BY:	main()

COPYRIGHT:	University Corporation for Atmospheric Research, 1996-7
-------------------------------------------------------------------------
*/

#include "define.h"

static struct tm	StartFlight;

char *GetMemory();

/* -------------------------------------------------------------------- */
void SetNASABaseTime(int hour, int min, int sec)
{
  struct tm	*gt;

  StartFlight.tm_hour	= hour;
  StartFlight.tm_min	= min;
  StartFlight.tm_sec	= sec;
  StartFlight.tm_isdst	= -1;

  BaseTime = mktime(&StartFlight);
  gt = gmtime(&BaseTime);
  StartFlight.tm_hour += StartFlight.tm_hour - gt->tm_hour;

  BaseTime = mktime(&StartFlight);

  strftime(buffer, 128, "seconds since %F %T %z", &StartFlight);

  nc_put_att_text(ncid, timeVarID, "units", strlen(buffer)+1, buffer);
  nc_put_att_text(ncid, timeOffsetID, "units", strlen(buffer)+1, buffer);

}	/* END SETNASABASETIME */

/* -------------------------------------------------------------------- */
void CreateNASAamesNetCDF(FILE *fp)
{
  int	i, start;
  int	FFI, year, month, day;
  int	ndims, dims[3], TimeDim, RateDim;
  char	*p, *p1, *titles[MAX_VARS], *units[MAX_VARS], tmp[32];
  float	missing_val = MISSING_VALUE;


  /* Dimensions.
   */
  nc_def_dim(ncid, "Time", NC_UNLIMITED, &TimeDim);

  sprintf(buffer, "sps%d", dataRate);
  nc_def_dim(ncid, buffer, dataRate, &RateDim);


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
  sscanf(buffer, "%d %d", &SkipNlines, &FFI);

  if (FFI < 1000 || FFI > 1999)
    {
    fprintf(stderr, "Can't handle more than one independant variable.\n");
    exit(1);
    }

  fgets(buffer, BUFFSIZE, fp);
  buffer[strlen(buffer)-1] = '\0';
  nc_put_att_text(ncid, NC_GLOBAL, "PI", strlen(buffer)+1, buffer);

  fgets(buffer, BUFFSIZE, fp);
  buffer[strlen(buffer)-1] = '\0';
  nc_put_att_text(ncid, NC_GLOBAL, "Source", strlen(buffer)+1, buffer);

  fgets(buffer, BUFFSIZE, fp);
  buffer[strlen(buffer)-1] = '\0';
  nc_put_att_text(ncid, NC_GLOBAL, "SNAME", strlen(buffer)+1, buffer);

  fgets(buffer, BUFFSIZE, fp);
  buffer[strlen(buffer)-1] = '\0';
  nc_put_att_text(ncid, NC_GLOBAL, "ProjectName", strlen(buffer)+1, buffer);


  /* Skip IVOL NVOL */
  fgets(buffer, BUFFSIZE, fp);

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
  sprintf(tmp, "%02d/%02d/%d", month, day, 1900 + year);
  nc_put_att_text(ncid, NC_GLOBAL, "DateProcessed", strlen(tmp)+1, tmp);


  fgets(buffer, BUFFSIZE, fp);
  if (atof(buffer) != 1.0)
    fprintf(stderr, "Non-uniform time interval, results may be sketchy.\n");


  /* Time segments.  Will be updated later.
   */
  nc_put_att_text(ncid, NC_GLOBAL, "TimeInterval", DEFAULT_TI_LENGTH, buffer);


  /* First dimension is time dimension.
   * Second is Rate in Hz.
   * Third is Vector Length.
   */
  ndims = 1;
  dims[0] = TimeDim;


  /* Time Variables.
   */
  createTime(dims);


  /* Create Time variables.
   */
  for (i = 0; time_vars[i]; ++i)
    {
    p = (char *)time_vars[i];
    nc_def_var(ncid, time_vars[i], NC_FLOAT, 1, dims, &varid[i]);

    nc_put_att_float(ncid, varid[i], "_FillValue", NC_FLOAT, 1, &missing_val);
    nc_put_att_text(ncid, varid[i], "units", strlen(p)+1, p);
    nc_put_att_text(ncid, varid[i], "long_name", strlen(p)+1, p);
    }


  /* Skip XNAME */
  fgets(buffer, BUFFSIZE, fp);


  /* For each variable:
   *	- Set dimensions
   *	- define variable
   *	- Set attributes
   */
  /* Scan in Primary variables first.
   */
  fgets(buffer, BUFFSIZE, fp);
  nVariables = atoi(buffer);

  /* Get Scale Factors. */
  fgets(buffer, BUFFSIZE, fp);
  p = strtok(buffer, " \t\n\r");
  for (i = 0; i < nVariables; ++i)
    {
    scale[i] = atof(p);
    offset[i] = 0.0;
    p = strtok(NULL, " \t\n\r");
    }

  /* Get missing values, these will be translated to nimbus MISSING_VALUES.
   */
  fgets(buffer, BUFFSIZE, fp);
  p = strtok(buffer, " \t\n\r");
  for (i = 0; i < nVariables; ++i)
    {
    missingVals[i] = atof(p);
    p = strtok(NULL, " \t\n\r");
    }

  /* Get Titles. */
  for (i = 0; i < nVariables; ++i)
    {
    fgets(buffer, BUFFSIZE, fp);
    buffer[strlen(buffer)-1] = '\0';
    titles[i] = GetMemory(strlen(buffer)+1);
    strcpy(titles[i], buffer);
    p = strrchr(titles[i], '(');
    if ( (p = strrchr(titles[i], '(')) )
      *(p-1) = '\0';

    if ( (p = strrchr(buffer, '(')) && (p1 = strchr(p, ')')))
      {
      units[i] = GetMemory(p1 - p + 1);
      *p1 = '\0';
      strcpy(units[i], p+1);
      }
    else
      {
      units[i] = GetMemory(10);
      strcpy(units[i], "Unk");
      }
    }


  /* Scan in Auxilary variables.
   */
  if (FFI == 1010)
    {
    fgets(buffer, BUFFSIZE, fp);
    start = nVariables;
    nVariables += atoi(buffer);

    /* Get Scale Factors. */
    fgets(buffer, BUFFSIZE, fp);
    p = strtok(buffer, " \t\n\r");
    for (i = start; i < nVariables; ++i)
      {
      scale[i] = atof(p);
      offset[i] = 0.0;
      p = strtok(NULL, " \t\n\r");
      }

    /* Get missing values, these will be xlated to nimbus MISSING_VALUES.
     */
    fgets(buffer, BUFFSIZE, fp);
    p = strtok(buffer, " \t\n\r");
    for (i = start; i < nVariables; ++i)
      {
      missingVals[i] = atof(p);
      p = strtok(NULL, " \t\n\r");
      }

    /* Get Titles. */
    for (i = start; i < nVariables; ++i)
      {
      fgets(buffer, BUFFSIZE, fp);
      buffer[strlen(buffer)-1] = '\0';
      titles[i] = GetMemory(strlen(buffer)+1);
      strcpy(titles[i], buffer);

      if ( (p = strchr(buffer, '(')) && (p1 = strchr(buffer, ')')))
        {
        units[i] = GetMemory(p1 - p + 1);
        *p1 = '\0';
        strcpy(units[i], p+1);
        }
      else
        {
        units[i] = GetMemory(10);
        strcpy(units[i], "Unk");
        }
      }
    }


  rewind(fp);
  for (i = 0; i < SkipNlines; ++i)
    fgets(buffer, BUFFSIZE, fp);

  p = strtok(buffer, " \t");

  for (i = 0; i < nVariables; ++i)
    {
    p = strtok(NULL, " \t\n\r");
    nc_def_var(ncid, p, NC_FLOAT, ndims, dims, &varid[i+3]);
//*(strchr(titles[i], '(') -1) = '\0';
//    nc_def_var(ncid, titles[i], NC_FLOAT, ndims, dims, &varid[i+3]);

printf("%s\n", titles[i]);
    nc_put_att_float(ncid,varid[i+3], "_FillValue",NC_FLOAT, 1, &missing_val);
    p = units[i];
    nc_put_att_text(ncid, varid[i+3], "units", strlen(p)+1, p);
    p = titles[i];
    nc_put_att_text(ncid, varid[i+3], "long_name", strlen(p)+1, p);

    free(units[i]);
    free(titles[i]);
    }

}	/* END CREATENASAAMESNETCDF */

/* END NASAAMES.C */
