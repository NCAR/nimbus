/*
-------------------------------------------------------------------------
OBJECT NAME:	plain.c

FULL NAME:	Plain ASCII files.

ENTRY POINTS:	SetPlainBaseTime()
		CreatePlainNetCDF()
		WriteBaseTime()
		createTime()
		addGlobalAttrs()

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1996-07
-------------------------------------------------------------------------
*/

#include "define.h"

#ifdef VARDB
#include <raf/vardb.h>
#endif

const char *timeUnitsFormat = "seconds since %F %T %z";
const char *baseTimeUnits = "seconds since 1970-01-01 00:00:00 +0000";


/* -------------------------------------------------------------------- */
void SetPlainBaseTime()
{
  struct tm *t;

  t = gmtime(&BaseTime);
  strftime(buffer, 128, timeUnitsFormat, t);

  printf("\nFlight start time = [%s]\nBaseTime = %lu\n", buffer, BaseTime);

  nc_put_att_text(ncid, timeVarID, "units", strlen(buffer)+1, buffer);
  nc_put_att_text(ncid, timeOffsetID, "units", strlen(buffer)+1, buffer);

}	/* END SETBASETIME */

/* -------------------------------------------------------------------- */
void WriteBaseTime()
{
  nc_put_var_long(ncid, baseTimeID, &BaseTime);
}

/* -------------------------------------------------------------------- */
void CreatePlainNetCDF(FILE *fp)
{
  int	ndims, dims[3], TimeDim, RateDim;
  char	*p;
  float	missing_val = MISSING_VALUE;
#ifdef VARDB
  bool	varDB = true;
#endif

#ifdef VARDB
  if (InitializeVarDB("VarDB") == ERR)
    {
    fprintf(stderr, "No VarDB in local directory, trying $PROJ_DIR/Configuration/raf/VarDB.\n");

    if (getenv("PROJ_DIR") > 0)
      strcpy(buffer, getenv("PROJ_DIR"));
    else
      {
      strcpy(buffer, "/usr/local/proj");
      fprintf(stderr, "env variable PROJ_DIR undefined, trying /usr/local/proj.\n");
      }

    strcat(buffer, "/Configuration/raf/VarDB");

    if (InitializeVarDB(buffer) == ERR)
      {
      fprintf(stderr,"Can't open %s, continuing with no VarDB in use.\n", buffer);
      varDB = false;
      }
    }
#endif

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


  extern char FlightDate[];
  if (strlen(FlightDate) > 0)
    nc_put_att_text(ncid, NC_GLOBAL, "FlightDate", strlen(FlightDate)+1, FlightDate);

  /* Time segments.  Will be updated later.
   */
  nc_put_att_text(ncid, NC_GLOBAL, "TimeInterval", DEFAULT_TI_LENGTH, buffer);


  /* First dimension is time dimension.
   * Second is Rate in Hz.
   * Third is Vector Length.
   */
  ndims = 1;
  dims[0] = TimeDim;
  dims[1] = RateDim;


  /* Time Variables, here to keep Gary/WINDS happy.
   */
  createTime(dims);

  /* For each variable:
   *	- Set dimensions
   *	- define variable
   *	- Set attributes
   *	- Set data_pointer
   */
  while (strlen(fgets(buffer, BUFFSIZE, fp)) < 2)
    ;

  if (isdigit((int)buffer[0]))
  {
    fprintf(stderr, "plain.c: no variable names, fatal.\n");
    exit(1);
  }

  /* Attempt to skip title for 'time' column.
   */
  for (p = buffer; isspace(*p); ++p)
    ;
  p = strtok(p, " \t\n\r");

  nVariables = 0;

  if (dataRate > 1)
    ndims = 2;

  while ( (p = strtok(NULL, " \t\n\r")) )
  {
    if (verbose)
      printf("Creating variable [%s]\n", p);

    nc_def_var(ncid, p, NC_FLOAT, ndims, dims, &varid[nVariables]);
    nc_put_att_float(ncid,varid[nVariables],"_FillValue",NC_FLOAT,1,&missing_val);

    const char * s1 =
#ifdef VARDB
	varDB ? VarDB_GetUnits(p) :
#endif
	noUnits;
    nc_put_att_text(ncid, varid[nVariables], "units", strlen(s1)+1, s1);

    s1 =
#ifdef VARDB
	varDB ? VarDB_GetTitle(p) :
#endif
	noTitle;
    nc_put_att_text(ncid, varid[nVariables], "long_name", strlen(s1)+1, s1);

    ++nVariables;
  }

#ifdef VARDB
  if (varDB)
    ReleaseVarDB();
#endif
}	/* END CREATEPLAINNETCDF */

/* -------------------------------------------------------------------- */
void createTime(int dims[])
{
  int	missing_val_i = MISSING_VALUE;
  float	missing_val_f = MISSING_VALUE;

  /* Time Variables, base_time/time_offset being deprecated.  Feb05
   */
  nc_def_var(ncid, "base_time", NC_INT, 0, 0, &baseTimeID);
  nc_put_att_text(ncid, baseTimeID, "units",
                strlen(baseTimeUnits)+1, baseTimeUnits);
  strcpy(buffer, "Start time of data recording.");
  nc_put_att_text(ncid, baseTimeID, "long_name", strlen(buffer)+1, buffer);

  nc_def_var(ncid, "Time", NC_INT, 1, dims, &timeVarID);
  nc_put_att_int(ncid, timeVarID, "_FillValue", NC_INT, 1, &missing_val_i);
  nc_put_att_text(ncid, timeVarID, "units",
                strlen(baseTimeUnits)+1, baseTimeUnits);
  strcpy(buffer, "time");
  nc_put_att_text(ncid, timeVarID, "standard_name", strlen(buffer)+1, buffer);
  strcpy(buffer, "time of measurement");
  nc_put_att_text(ncid, timeVarID, "long_name", strlen(buffer)+1, buffer);
  nc_put_att_text(ncid, timeVarID, "strptime_format",
		strlen(timeUnitsFormat)+1, timeUnitsFormat);

  nc_def_var(ncid, "time_offset", NC_FLOAT, 1, dims, &timeOffsetID);
  nc_put_att_float(ncid, timeOffsetID, "_FillValue", NC_FLOAT, 1, &missing_val_f);
  nc_put_att_text(ncid, timeOffsetID, "units",
                strlen(baseTimeUnits)+1, baseTimeUnits);
  strcpy(buffer, "Seconds since base_time.");
  nc_put_att_text(ncid, timeOffsetID, "long_name", strlen(buffer)+1, buffer);
}

/* -------------------------------------------------------------------- */
void addGlobalAttrs(const char *fileName)
{
  FILE *fp;

  if (fileName == 0)
    return;

  if ((fp = fopen(fileName, "r")) == NULL)
  {
    fprintf(stderr, "Can't open global attribute file %s.\n", fileName);
    exit(1);
  }

  while (fgets(buffer, BUFFSIZE, fp) > 0)
  {
    char *p, *attr, *value;

    if (buffer[0] == '#' || strlen(buffer) < 4)
      continue;

    if ((p = strchr(buffer, '=')) == 0)
    {
      fprintf(stderr, "Invalid global attribute line, skipping.\n  [%s]\n", buffer);
      continue;
    }

    buffer[strlen(buffer)-1] = '\0';
    *p = '\0';

    // Skip any leading whitespace at beginning of line.
    for (attr = buffer; isspace(*attr); ++attr)
      ;

    // Skip any leading whitespace after equal sign.
    for (value = ++p; isspace(*value); ++value)
      ;

    nc_put_att_text(ncid, NC_GLOBAL, attr, strlen(value)+1, value);
  }

  fclose(fp);
}

/* END PLAIN.C */
