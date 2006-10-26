/*
-------------------------------------------------------------------------
OBJECT NAME:	plain.c

FULL NAME:	Plain ASCII files.

ENTRY POINTS:	SetPlainBaseTime()
		CreatePlainNetCDF()

STATIC FNS:	none

DESCRIPTION:	

REFERENCES:	none

REFERENCED BY:	main()

COPYRIGHT:	University Corporation for Atmospheric Research, 1996-05
-------------------------------------------------------------------------
*/

#include "define.h"

#ifdef VARDB
#include "vardb.h"
#endif

const char *timeUnitsFormat = "seconds since %F %T %z";
const char *baseTimeUnits = "seconds since 1970-01-01 00:00:00 +0000";


/* -------------------------------------------------------------------- */
void SetPlainBaseTime()
{
  struct tm     *t;

  t = gmtime(&BaseTime);
  strftime(buffer, 128, timeUnitsFormat, t);

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
  int	i, ndims, dims[3], TimeDim, RateDim;
  char	*p, *p1;
  float	missing_val = MISSING_VALUE;
#ifdef VARDB
  bool	varDB = TRUE;
#endif

#ifdef VARDB
  if (InitializeVarDB("VarDB") == ERR)
    {
    fprintf(stderr, "No VarDB in local directory, trying $PROJ_DIR/proj/defaults/VarDB.\n");

    if (getenv("PROJ_DIR") > 0)
      strcpy(buffer, getenv("PROJ_DIR"));
    else
      {
      strcpy(buffer, "/usr/local/proj");
      fprintf(stderr, "env variable PROJ_DIR undefined, trying /usr/local/proj.\n");
      }

    strcat(buffer, "/defaults/VarDB");

    if (InitializeVarDB(buffer) == ERR)
      {
      fprintf(stderr,"Can't open %s, continuing with no VarDB in use.\n", buffer);
      varDB = FALSE;
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


  /* Time segments.  Will be updated later.
   */
  nc_put_att_text(ncid, NC_GLOBAL, "TimeInterval", DEFAULT_TI_LENGTH, buffer);


  /* First dimension is time dimension.
   * Second is Rate in Hz.
   * Third is Vector Length.
   */
  ndims = 1;
  dims[0] = TimeDim;


  /* Time Variables, here to keep Gary/WINDS happy.
   */
  createTime(dims);


  /* Create Time variables.
   */
  for (i = 0; i < 3; ++i)
    {
    nc_def_var(ncid, time_vars[i], NC_FLOAT, 1, dims, &varid[i]);
    nc_put_att_float(ncid, varid[i], "_FillValue", NC_FLOAT, 1, &missing_val);

    p =
#ifdef VARDB
	varDB ? VarDB_GetUnits(time_vars[i]) :
#endif
	(char *)noUnits;
    nc_put_att_text(ncid, varid[i], "units", strlen(p)+1, p);

    p =
#ifdef VARDB
	varDB ? VarDB_GetTitle(time_vars[i]) :
#endif
	(char *)noTitle;
    nc_put_att_text(ncid, varid[i], "long_name", strlen(p)+1, p);
    nc_put_att_float(ncid, varid[i], "missing_value", NC_FLOAT, 1, &missing_val);
    }



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
  if ((p = strtok(buffer, " \t\n\r")) == buffer)
    p = strtok(NULL, " \t\n\r");

  nVariables = 3;

  do
    {
    nc_def_var(ncid, p, NC_FLOAT, ndims, dims, &varid[nVariables]);
    nc_put_att_float(ncid,varid[nVariables],"_FillValue",NC_FLOAT,1,&missing_val);

    p1 =
#ifdef VARDB
	varDB ? VarDB_GetUnits(p) :
#endif
	(char *)noUnits;
    nc_put_att_text(ncid, varid[nVariables], "units", strlen(p1)+1, p1);

    p1 =
#ifdef VARDB
	varDB ? VarDB_GetTitle(p) :
#endif
	(char *)noTitle;
    nc_put_att_text(ncid, varid[nVariables], "long_name", strlen(p1)+1, p1);
    nc_put_att_float(ncid,varid[nVariables],"missing_value",NC_FLOAT,1,&missing_val);

    ++nVariables;
    }
  while ((p = strtok(NULL, " \t\n\r")) );

  nVariables -= 3;

#ifdef VARDB
  if (varDB)
    ReleaseVarDB();
#endif

}	/* END CREATEPLAINNETCDF */

/* -------------------------------------------------------------------- */
void createTime(int dims[])
{
  /* Time Variables, base_time/time_offset being deprecated.  Feb05
   */
  nc_def_var(ncid, "base_time", NC_INT, 0, 0, &baseTimeID);
  nc_put_att_text(ncid, baseTimeID, "units",
                strlen(baseTimeUnits)+1, baseTimeUnits);
  strcpy(buffer, "Start time of data recording.");
  nc_put_att_text(ncid, baseTimeID, "long_name", strlen(buffer)+1, buffer);

  nc_def_var(ncid, "Time", NC_INT, 1, dims, &timeVarID);
  nc_put_att_text(ncid, timeVarID, "units",
                strlen(baseTimeUnits)+1, baseTimeUnits);
  strcpy(buffer, "time");
  nc_put_att_text(ncid, timeVarID, "standard_name", strlen(buffer)+1, buffer);
  strcpy(buffer, "time of measurement");
  nc_put_att_text(ncid, timeVarID, "long_name", strlen(buffer)+1, buffer);
  nc_put_att_text(ncid, timeVarID, "strptime_format",
		strlen(timeUnitsFormat)+1, timeUnitsFormat);

  nc_def_var(ncid, "time_offset", NC_FLOAT, 1, dims, &timeOffsetID);
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
