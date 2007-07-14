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

COPYRIGHT:	University Corporation for Atmospheric Research, 1996-7
-------------------------------------------------------------------------
*/

#include "define.h"
#include <raf/constants.h>
#include <raf/vardb.h>

extern int dataRate;

/* -------------------------------------------------------------------- */
void SetPlainBaseTime()
{
  nc_put_var_long(ncid, baseTimeID, &BaseTime);

}	/* END SETBASETIME */

/* -------------------------------------------------------------------- */
void CreatePlainNetCDF(FILE *fp)
{
  int	i, ndims, dims[3], TimeDim, RateDim;
  char	*p, *p1;
  float	missing_val = MISSING_VALUE;
  bool	varDB = TRUE;

  if (InitializeVarDB("VarDB") == ERR)
    if (InitializeVarDB("/home/local/proj/defaults/VarDB") == ERR)
      {
      fprintf(stderr,"Can't open ./VarDB or /home/local/proj/defaults/VarDB\n");
      varDB = FALSE;
      }


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
  strftime(buffer, 128, "%F %T %z", &tm);
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

  AddTimeVariables(dims);

  /* Create Time variables.
   */
  for (i = 0; time_vars[i]; ++i)
    {
    nc_def_var(ncid, time_vars[i], NC_FLOAT, 1, dims, &varid[i]);
    nc_put_att_float(ncid, varid[i], "_FillValue", NC_FLOAT, 1, &missing_val);

    const char * s1 =
#ifdef VARDB
        varDB ? VarDB_GetUnits(time_vars[i]) :
#endif
        noUnits;
    nc_put_att_text(ncid, varid[i], "units", strlen(s1)+1, s1);

    s1 =
#ifdef VARDB
        varDB ? VarDB_GetTitle(time_vars[i]) :
#endif
        noTitle;
    nc_put_att_text(ncid, varid[i], "long_name", strlen(s1)+1, s1);
    }



  /* For each variable:
   *	- Set dimensions
   *	- define variable
   *	- Set attributes
   *	- Set data_pointer
   */
  while (strlen(fgets(buffer, BUFFSIZE, fp)) < 2)
    ;

  if (isdigit(buffer[0]))
    {
    fprintf(stderr, "plain.c: no variable names, fatal.\n");
    exit(1);
    }

  /* Attempt to skip title for 'time' column.
   */
  if ((p = strtok(buffer, " \t\n")) == buffer)
    p = strtok(NULL, " \t\n");

  nVariables = 3;
  ndims = 2;
  dims[1] = RateDim;

  do
    {
    printf("Creating variable [%s]\n", p);
    printf(" nc_def_var = %d\n",
	nc_def_var(ncid, p, NC_FLOAT, ndims, dims, &varid[nVariables]));

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
  while ((p = strtok(NULL, " \t\r\n")) );

  nVariables -= 3;

  nc_enddef(ncid);

  if (varDB)
    ReleaseVarDB();

}	/* END CREATEPLAINNETCDF */

/* END PLAIN.C */
