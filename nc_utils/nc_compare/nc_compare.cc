/*
-------------------------------------------------------------------------
OBJECT NAME:	nc_compare.cc

FULL NAME:	netCDF compare program

DESCRIPTION:	Compare two netCDF files and output the differnces.

NOT IMPLEMENTED:
		Does NOT compare histogram variables.

		Expects time segments to match exactly.  Not smart enough
		at this time to only compare overlapping time segments.

COPYRIGHT:	University Corporation for Atmospheric Research, 2011
-------------------------------------------------------------------------
*/

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>
#include <vector>
#include <netcdf.h>

#include <gsl/gsl_statistics_float.h>

#define NAMELEN		32
#define MAX_VARS	1000

class Var
{
public:
  char	name[NAMELEN];
  int	varID1;
  int	varID2;

  float	missing_value1;
  float	missing_value2;

  double	mean1;
  double	mean2;
  double	diff;
};

std::vector<Var> file_vars;

char	buffer[2048];
int	infd1, infd2;
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
    printf("Time dimensions are of differeing length, %u vs. %u\n", et1, et2);
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

  if (bt1 != bt2) {
    printf("Start time does not match exactly, this has potential to produce inconsistent results.\n");
    printf("  %d vs. %d\n", bt1, bt2);
  }
  if (et1 != et2) {
    printf("End time does not match exactly, this has potential to produce inconsistent results.\n");
    printf("  %d vs. %d\n", et1, et2);
  }
}

/* -------------------------------------------------------------------- */
void CompareVariables()
{
  char		name[32];
  Var		var;
  nc_type	dataType;
  int		nVars1, nVars2, nDims, dimIDs[8];
  size_t	nPts1, nPts2;

  nc_inq_nvars(infd1, &nVars1);
  nc_inq_nvars(infd2, &nVars2);

  nc_inq_dimlen(infd1, timeDimID1, &nPts1);
  nc_inq_dimlen(infd2, timeDimID2, &nPts2);

  for (int i = 0; i < nVars1; ++i)
  {
    // Get next variable in first file.
    nc_inq_var(infd1, i, name, &dataType, &nDims, dimIDs, 0); 
    nc_inq_varid(infd1, name, &var.varID1);

    // Ignore histogram data, we can add this at some later date.
    if (nDims > 1)
      continue;

    if (nc_inq_varid(infd2, name, &var.varID2) != NC_NOERR) {
      printf("Variable %s in first file does not exist in second file.\n", name);
      continue;
    }

    // OK, variable exists in both files, read data.
    strcpy(var.name, name);
    float data1[nPts1], data2[nPts2];
    nc_get_var_float(infd1, var.varID1, data1);
    nc_get_var_float(infd2, var.varID2, data2);

    // Remove Missing values.
    std::vector<float> cleaned_data1;
    std::vector<float> cleaned_data2;
    nc_get_att_float(infd1, var.varID1, "_FillValue", &var.missing_value1);
    nc_get_att_float(infd2, var.varID2, "_FillValue", &var.missing_value2);

    for (size_t i = 0; i < nPts1; ++i) {
      if (data1[i] != var.missing_value1)
        cleaned_data1.push_back(data1[i]);
    }

    for (size_t i = 0; i < nPts2; ++i) {
      if (data2[i] != var.missing_value2)
        cleaned_data2.push_back(data2[i]);
    }

    // Compute means.
    var.mean1 = gsl_stats_float_mean(&cleaned_data1[0], 1, cleaned_data1.size());
    var.mean2 = gsl_stats_float_mean(&cleaned_data2[0], 1, cleaned_data2.size());
    var.diff = fabs(var.mean2 - var.mean1);

    file_vars.push_back(var);
  }
}

/* -------------------------------------------------------------------- */
void Output()
{
  printf("\nGross differences; >= 1.0\n");
  printf("                                Mean\n");
  printf("Variable                File1          File2      Diff\n");
  for (size_t i = 0; i < file_vars.size(); ++i)
  {
    if (file_vars[i].diff >= 1.0)
      printf("%-16s %14.6f %14.6f %10.6f\n",
	file_vars[i].name, file_vars[i].mean1, file_vars[i].mean2, file_vars[i].diff);
  }

  printf("\nMedium differences; >= 0.1 && < 1.0\n");
  for (size_t i = 0; i < file_vars.size(); ++i)
  {
    if (file_vars[i].diff >= 0.1 && file_vars[i].diff < 1.0)
      printf("%-16s %14.6f %14.6f %10.6f\n",
	file_vars[i].name, file_vars[i].mean1, file_vars[i].mean2, file_vars[i].diff);
  }

  printf("\nFine differences; > 0.00001 && < 0.1\n");
  for (size_t i = 0; i < file_vars.size(); ++i)
  {
    if (file_vars[i].diff > 0.00001 && file_vars[i].diff < 0.1)
      printf("%-16s %14.6f %14.6f %10.6f\n",
	file_vars[i].name, file_vars[i].mean1, file_vars[i].mean2, file_vars[i].diff);
  }
}

/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  int	rc = 0, argp = 1;

  if (argc < 3)
  {
    fprintf(stderr, "Usage: nc_compare primary_file secondary_file\n");
    exit(1);
  }

  ncopts = 0;
  putenv((char *)"TZ=UTC");	// All time calcs in UTC.

  openFiles(argc, argv, argp);
  checkForOverlappingTimeSegments();

  CompareVariables();
  Output();

  Exit(0);
  return rc;
}
