/*
-------------------------------------------------------------------------
OBJECT NAME:	ncfillvar.cc

FULL NAME:	Nimbus/netCDF program to add an array of missing data to a
		netCDF file

DESCRIPTION:	Variable creation may proceed IF:
			- Conventions attributes are NCAR-RAF/nimbus.

		Primary file will retain original time intervals.
		Created by modifying ncmerge

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-06
-------------------------------------------------------------------------
*/

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <netcdf.h>

#define NAMELEN		32
#define MAX_IN_VARS	800
#define MAX_OUT_VARS	1000

char	buffer[2048];
char	VarList[MAX_IN_VARS][NAMELEN];
char    RefVar[NAMELEN];
int	inVarID[MAX_IN_VARS], outVarID[MAX_OUT_VARS];
void	*inPtrs[MAX_IN_VARS], *outPtrs[MAX_OUT_VARS];
int	infd1, infd2, VarCnt = 0, xFerCnt = 0;
time_t	bt1;
size_t	et1;

void	CopyVariablesDefinitions();


/* -------------------------------------------------------------------- */
void Exit(int rc)
{
  ncclose(infd1);
  ncclose(infd2);
  exit(rc);
}

/* -------------------------------------------------------------------- */
void openFiles(int argc, char *argv[], int argp)
{
  /* Open files.
   */
  if (nc_open(argv[argp], NC_WRITE, &infd1) != NC_NOERR)
  {
    fprintf(stderr, "Can't open primary file %s\n", argv[argp]);
    Exit(1);
  }

  /* Make sure files are similar.
   */
  nc_get_att_text(infd1, NC_GLOBAL, "Conventions", buffer);

  if (strcmp(buffer,"NCAR-RAF/nimbus") != 0)
  {
    fprintf(stderr, "Conventions not NCAR-RAF/nimbus, invalid fill.\n");
    Exit(1);
  }
}

/* -------------------------------------------------------------------- */
void determineTimeSegment()
{
  int varID1;

  if (nc_inq_varid(infd1, "Time", &varID1) != NC_NOERR)
  {
    fprintf(stderr, "Master file does not contain the variable 'Time', fatal.\n");
    fprintf(stderr, "  You are probably trying to add to old style RAF netCDF files.\n");
    Exit(1);
  }

  int timeDimID1;
  nc_inq_dimid(infd1, "Time", &timeDimID1);

  nc_inq_dimlen(infd1, timeDimID1, &et1);

  char units1[60];
  nc_get_att_text(infd1, varID1, "units", units1);

  char unitsFormat1[60];
  const char * defaultFormat = "seconds since %F %T %z";
  if (nc_get_att_text(infd1, varID1, "strptime_format", unitsFormat1) != NC_NOERR)
    strcpy(unitsFormat1, defaultFormat);

  struct tm tm1;
  strptime(units1, unitsFormat1, &tm1);

  bt1 = mktime(&tm1);

  if (bt1 == 0)
  {
    fprintf(stderr, "File has a base_time of 0, this implies asc2cdf was used\nwithout '-b' option.  Merged data may be shifted in time.\n");

  }

  et1 += bt1;

}

/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  int	rc = 0, argp = 1;

  if (argc < 2)
  {
    fprintf(stderr, "Usage: ncfillvar [-v var0,var1,..,varn] [-c refvar] primary_file\n");
    exit(1);
  }

  putenv("TZ=UTC");

  /* Get variable list if it exists.
   */
  if (strcmp(argv[argp], "-v") == 0)
  {
    char *p;

    ++argp;
    p = strtok(argv[argp++], ",");

    do
    {
      strcpy(VarList[VarCnt++], p);
    }
    while ((p = strtok(NULL, ",")));
  }
  else
  {
    printf("Must specify variables to create.\n");
    exit(1);
  }

  /* Get variable to copy dimensions from. All new vars will have same
   * dimensions as this var.
   */
  if (strcmp(argv[argp], "-c") == 0)
  {

    ++argp;
    strcpy(RefVar,argv[argp++]);

  }
  else
  {
    printf("Must specify a reference variable using -c.\n");
    exit(1);
  }

  ncopts = 0;

  openFiles(argc, argv, argp);
  determineTimeSegment();

  CopyVariablesDefinitions();

  Exit(0);
  return rc;

}	/* END MAIN */

/* -------------------------------------------------------------------- */
void CopyVariablesDefinitions()
{
  int	rc;
  char name[32];
  nc_type	dataType;
  int	nDims, nAtts, dimIDs[4], nVars;
  float	missing_value = -32767.0;

  nc_redef(infd1);
  nc_inq_nvars(infd1, &nVars);

  int j;

  for (j = 0; j < nVars; ++j)
  {
    nc_inq_var(infd1, j, name, &dataType, &nDims, dimIDs, &nAtts);

    if (strcmp(RefVar,name) == 0) 
    {
	printf("Found reference var %s in file.\n",name);
	break;
    }
  }

  if (strcmp(RefVar,name) != 0)
  {
      printf("Reference Var %s not found in file.  Aborting\n",RefVar);
      exit(1);
  }

  for (j = 0; j < VarCnt; ++j)
  {

    // See if variable already exists in output file, if not then create it.
    if (nc_inq_varid(infd1, VarList[j], &rc) != NC_NOERR)
    {
      if (nc_def_var(infd1, VarList[j], dataType, nDims, dimIDs, &rc) != NC_NOERR)
      {
        fprintf(stderr, "Error in creating variable %s, %s will not be in merged dataset.\n", VarList[j], VarList[j]);
        continue;
      }
    }
    else
    {
      printf("Variable %s exists in primary file, it will be overwritten.\n", VarList[j]);
    }

    nc_put_att_float(infd1, rc, "_FillValue", NC_FLOAT, 1, &missing_value);
  }

  nc_enddef(infd1);

}	/* END COPYVARIABLES */

/* -------------------------------------------------------------------- */
