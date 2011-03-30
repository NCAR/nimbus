/*
-------------------------------------------------------------------------
OBJECT NAME:	nc_compare.cc

FULL NAME:	netCDF compare program

DESCRIPTION:	Compare two netCDF files and output the differnces.

COPYRIGHT:	University Corporation for Atmospheric Research, 2011
-------------------------------------------------------------------------
*/

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <netcdf.h>

#include <gsl/gsl_statistics_float.h>

#define NAMELEN		32
#define MAX_VARS	1000

char	buffer[2048];
char	VarList[MAX_VARS][NAMELEN];
int	inVarID1[MAX_VARS], inVarID2[MAX_VARS];
void	*inPtrs1[MAX_VARS], *inPtrs2[MAX_VARS];
int	infd1, infd2, VarCnt = 0, xFerCnt = 0;
int	timeDimID1, timeDimID2;

void	CompareVariables(), CompareData();


/* -------------------------------------------------------------------- */
void Exit(int rc)
{
  nc_close(infd1);
  nc_close(infd2);
  exit(rc);
}

/* -------------------------------------------------------------------- */
void openFiles(int argc, char *argv[], int argp)
{
  int nDims1, nDims2, nVars1, nVars2, ngAtts1, ngAtts2;

  if (nc_open(argv[argp], NC_NOWRITE, &infd1) != NC_NOERR)
  {
    fprintf(stderr, "Can't open %s.\n", argv[argp]);
    exit(1);
  }

  if (nc_open(argv[++argp], NC_NOWRITE, &infd2) != NC_NOERR)
  {
    fprintf(stderr, "Can't open %s.\n", argv[argp]);
    nc_close(infd1);
    exit(1);
  }

  nc_inq(infd1, &nDims1, &nVars1, &ngAtts1, 0);
  nc_inq(infd2, &nDims2, &nVars2, &ngAtts2, 0);

  if (nDims1 != nDims2)
  {
    printf("Number of dimensions do not match, %d vs. %d.\n", nDims1, nDims2);
  }

  if (ngAtts1 != ngAtts2)
  {
    printf("Number of global attributes do not match, %d vs. %d.\n", ngAtts1, ngAtts2);
  }

  if (nVars1 != nVars2)
  {
    printf("Number of variables do not match, %d vs. %d.\n", nVars1, nVars2);
  }
}

/* -------------------------------------------------------------------- */
void checkForOverlappingTimeSegments()
{
  int varID1, varID2;
  time_t bt1, bt2;
  size_t et1, et2;

  if (nc_inq_varid(infd1, "Time", &varID1) != NC_NOERR)
  {
    fprintf(stderr, "Master file does not contain the variable 'Time', fatal.\n");
    fprintf(stderr, "  You are probably trying to merge old style RAF netCDF files.\n");
    Exit(1);
  }

  if (nc_inq_varid(infd2, "Time", &varID2) != NC_NOERR)
  {
    fprintf(stderr, "Secondary file does not contain the variable 'Time', fatal.\n");
    fprintf(stderr, "  You are probably trying to merge old style RAF netCDF files.\n");
    Exit(1);
  }

  nc_inq_dimid(infd1, "Time", &timeDimID1);
  nc_inq_dimid(infd2, "Time", &timeDimID2);

  nc_inq_dimlen(infd1, timeDimID1, &et1);
  nc_inq_dimlen(infd2, timeDimID2, &et2);

  if (et1 != et2)
  {
    printf("Time dimensions are of differeing length, %d vs. %d\n", et1, et2);
  }

  char units1[60], units2[60];
  nc_get_att_text(infd1, varID1, "units", units1);
  nc_get_att_text(infd2, varID2, "units", units2);

  char unitsFormat1[60], unitsFormat2[60];
  const char * defaultFormat = "seconds since %F %T %z";
  if (nc_get_att_text(infd1, varID1, "strptime_format", unitsFormat1) != NC_NOERR)
    strcpy(unitsFormat1, defaultFormat);
  if (nc_get_att_text(infd2, varID2, "strptime_format", unitsFormat2) != NC_NOERR)
    strcpy(unitsFormat2, defaultFormat);

  struct tm tm1, tm2;
  strptime(units1, unitsFormat1, &tm1);
  strptime(units2, unitsFormat2, &tm2);

  bt1 = mktime(&tm1);
  bt2 = mktime(&tm2);

  if (bt1 == 0 || bt2 == 0)
  {
    fprintf(stderr, "At least one file has a base_time of 0, this implies asc2cdf was used\nwithout '-b' option.  Merged data may be shifted in time.\n");

    if (bt1 == 0 && bt2 == 0)
      fprintf(stderr, "Both base_time's are 0, files must have identical start times\nor else merged data will be shifted in time.\n");
  }

  et1 += bt1;
  et2 += bt2;

  if ((time_t)et2 < bt1 || bt2 > (time_t)et1)
  {
    fprintf(stderr, "No overlapping time segments, nothing to compare.\n");
    Exit(1);
  }

  if (bt1 != bt2 || et1 != et2)
  {
    printf("Time segments do not match exactly, this has potential to produce inconsistent results.\n");
  }
}

/* -------------------------------------------------------------------- */
void CompareVariables()
{
  int	rc, varID1, varID2;
  char	name[32];
  char  dimname1[32], dimname2[32];
  nc_type	dataType;
  int	nVars1, nVars2, nDims, nAtts, dimIDs[8];
  size_t nPts1, nPts2;

  nc_inq_nvars(infd1, &nVars1);
  nc_inq_nvars(infd2, &nVars2);

  nc_inq_dimlen(infd1, timeDimID1, &nPts1);
  nc_inq_dimlen(infd2, timeDimID2, &nPts2);

  for (int i = 0; i < nVars1; ++i)
  {
    nc_inq_var(infd1, i, name, &dataType, &nDims, dimIDs, &nAtts); 
    nc_inq_varid(infd1, name, &varID1);

    if (nc_inq_varid(infd2, name, &varID2) != NC_NOERR)
    {
      printf("Variable %s in first file does not exist in second file.\n", name);
      continue;
    }

    float data1[1000000], data2[1000000];
    nc_get_var_float(infd1, varID1, data1);
    nc_get_var_float(infd2, varID2, data2);

    // Do statistics.
    double mean1 = gsl_stats_float_mean(data1, 1, nPts1);
    double mean2 = gsl_stats_float_mean(data2, 1, nPts2);

    double var1 = gsl_stats_float_variance_with_fixed_mean(data1, 1, nPts1, mean1);
    double var2 = gsl_stats_float_variance_with_fixed_mean(data2, 1, nPts2, mean2);

    printf("%s; mean %f %f; var = %f %f\n", name, mean1, mean2, var1, var2);

  }
}

/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  int	rc = 0, argp = 1;

  if (argc < 3)
  {
    fprintf(stderr, "Usage: ncmerge [-v var0,var1,..,varn] [-a] primary_file secondary_file\n");
    exit(1);
  }

  ncopts = 0;

  openFiles(argc, argv, argp);
  checkForOverlappingTimeSegments();

  CompareVariables();

  Exit(0);
  return rc;
}
