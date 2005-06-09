/*
-------------------------------------------------------------------------
OBJECT NAME:	nasa.c

FULL NAME:	NASA ASCII to Nimbus-netCDF Low Rate

ENTRY POINTS:	SetNASABaseTime()
		CreateNASANetCDF()

STATIC FNS:	none

DESCRIPTION:	Translate NASA ASCII file to Nimbus Low Rate netCDF file

REFERENCES:	none

REFERENCED BY:	main()

COPYRIGHT:	University Corporation for Atmospheric Research, 1996-7
-------------------------------------------------------------------------
*/

#include "define.h"
#include "constants.h"
#include "vardb.h"

char *GetMemory();

/* -------------------------------------------------------------------- */
void SetNASABaseTime()
{
  struct tm	*gt;

  StartFlight.tm_hour	= 0;
  StartFlight.tm_min	= 0;
  StartFlight.tm_sec	= 0;
  StartFlight.tm_isdst	= -1;

  BaseTime = mktime(&StartFlight);
  gt = gmtime(&BaseTime);
  StartFlight.tm_hour += StartFlight.tm_hour - gt->tm_hour;

  BaseTime = mktime(&StartFlight);
  nc_put_var_long(ncid, baseTimeID, &BaseTime);

}	/* END SETNASABASETIME */

/* -------------------------------------------------------------------- */
void CreateNASANetCDF(FILE *fp)
{
  int	i, start, orate = 1;
  int	FFI, year, month, day;
  int	ndims, dims[3], TimeDim, LowRateDim, HighRateDim, Dim5Hz, Dim10Hz,
        Dim50Hz, Dim250Hz, Dim1000Hz, Vector16Dim, Vector32Dim, Vector64Dim;
  char	*p, *p1, *titles[MAX_VARS], *units[MAX_VARS];
  float	missing_val = MISSING_VALUE;

  if (InitializeVarDB("/home/local/proj/defaults/VarDB") == ERR)
    {
    fprintf(stderr, "Can't open /home/local/proj/defaults/VarDB\n");
    exit(1);
    }


  /* Dimensions.
   */
  nc_def_dim(ncid, "Time", NC_UNLIMITED, &TimeDim);

  nc_def_dim(ncid, "sps1", 1, &LowRateDim);
  nc_def_dim(ncid, "sps18", 18, &HighRateDim);

  /* Global Attributes.
   */
  strcpy(buffer, "NCAR-RAF/nimbus");
  nc_put_att_text(ncid, NC_GLOBAL, "Conventions", strlen(buffer)+1, buffer);

  {
  time_t	t;
  struct tm	tm;

  t = time(0);
  tm = *gmtime(&t);
  strftime(buffer, 128, "%F %T %z", &tm);
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
  sscanf(buffer, "%d %d %d", &year, &month, &day);
  StartFlight.tm_mon = year;
  StartFlight.tm_mday = month;
  StartFlight.tm_year = day;

  sprintf(buffer, "%02d/%02d/%d", month, day, year);
  nc_put_att_text(ncid, NC_GLOBAL, "FlightDate", strlen(buffer)+1, buffer);

  sscanf(buffer, "%*d %*d %*d %d %d %d", &year, &month, &day);
  sprintf(buffer, "%02d/%02d/%d", month, day, year);
  nc_put_att_text(ncid, NC_GLOBAL, "DateProcessed", strlen(buffer)+1, buffer);


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

  AddTimeVariables(dims);

  /* Create Time variables.
   */
  for (i = 0; i < 3; ++i)
    {
    nc_def_var(ncid, time_vars[i], NC_FLOAT, ndims, dims, &varid[i]);

    p = VarDB_GetUnits(buffer);
    nc_put_att_text(ncid, varid[i], "units", strlen(p)+1, p);
    p = VarDB_GetTitle(buffer);
    nc_put_att_text(ncid, varid[i], "long_name", strlen(p)+1, p);
    nc_put_att_int(ncid, varid[i], "OutputRate", NC_INT, 1, &orate);
    nc_put_att_float(ncid, varid[i], "MissingValue", NC_FLOAT, 1, &missing_val);
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
  p = strtok(buffer, " \t\n");
  for (i = 0; i < nVariables; ++i)
    {
    scaleFactors[i] = atof(p);
    p = strtok(NULL, " \t\n");
    }

  /* Get missing values, these will be translated to nimbus MISSING_VALUES.
   */
  fgets(buffer, BUFFSIZE, fp);
  p = strtok(buffer, " \t\n");
  for (i = 0; i < nVariables; ++i)
    {
    missingVals[i] = atof(p);
    p = strtok(NULL, " \t\n");
    }

  /* Get Titles. */
  for (i = 0; i < nVariables; ++i)
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


  /* Scan in Auxilary variables.
   */
  if (FFI == 1010)
    {
    fgets(buffer, BUFFSIZE, fp);
    start = nVariables;
    nVariables += atoi(buffer);

    /* Get Scale Factors. */
    fgets(buffer, BUFFSIZE, fp);
    p = strtok(buffer, " \t\n");
    for (i = start; i < nVariables; ++i)
      {
      scaleFactors[i] = atof(p);
      p = strtok(NULL, " \t\n");
      }

    /* Get missing values, these will be xlated to nimbus MISSING_VALUES.
     */
    fgets(buffer, BUFFSIZE, fp);
    p = strtok(buffer, " \t\n");
    for (i = start; i < nVariables; ++i)
      {
      missingVals[i] = atof(p);
      p = strtok(NULL, " \t\n");
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
  for (i = 0; i < SkipNlines-1; ++i)
    fgets(buffer, BUFFSIZE, fp);

  ndims = 2;
  dims[1] = HighRateDim;
  orate = 18;

  p = strtok(buffer, " \t");

  for (i = 0; i < nVariables; ++i)
    {
    p = strtok(NULL, " \t\n");
    printf("Adding var %s = %d\n", p, nc_def_var(ncid, p, NC_FLOAT, ndims, dims, &varid[i+3]));

    p = units[i];
    nc_put_att_text(ncid, varid[i+3], "units", strlen(p)+1, p);
    p = titles[i];
    nc_put_att_text(ncid, varid[i+3], "long_name", strlen(p)+1, p);
    nc_put_att_int(ncid, varid[i+3], "OutputRate", NC_INT, 1, &orate);
    nc_put_att_float(ncid,varid[i+3], "MissingValue",NC_FLOAT, 1, &missing_val);

    free(units[i]);
    free(titles[i]);
    }

  nc_enddef(ncid);

  ReleaseVarDB();

}	/* END CREATENASANETCDF */

/* END NASA.C */
