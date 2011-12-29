/*
-------------------------------------------------------------------------
OBJECT NAME:	ncmerge.cc

FULL NAME:	Nimbus/netCDF merge program

DESCRIPTION:	A merge may proceed IF:
			- Conventions attributes are identical.
			- There exists overlapping time segment.

		Primary file will retain original time intervals.  All new
		vars that will get missing value if time intervals don't
		match 100%.

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-09
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
int	inVarID[MAX_IN_VARS], outVarID[MAX_OUT_VARS];
void	*inPtrs[MAX_IN_VARS], *outPtrs[MAX_OUT_VARS];
int	infd1, infd2, VarCnt = 0, xFerCnt = 0;
time_t	bt1, bt2;
size_t	et1, et2;

void	CopyVariablesDefinitions(int),MoveData();


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
  /* Open files.
   */
  if (nc_open(argv[argp], NC_WRITE, &infd1) != NC_NOERR)
  {
    fprintf(stderr, "Can't open primary file %s. Do you have write permission?\n", argv[argp]);
    Exit(1);
  }

  if (nc_open(argv[++argp], NC_NOWRITE, &infd2) != NC_NOERR)
  {
    fprintf(stderr, "Can't open secondary file %s\n", argv[argp]);
    Exit(1);
  }

  /* Make sure files are similar.
   */
  nc_get_att_text(infd1, NC_GLOBAL, "Conventions", buffer);
  nc_get_att_text(infd2, NC_GLOBAL, "Conventions", &buffer[500]);

  if (strcmp(buffer, &buffer[500]) != 0)
  {
    fprintf(stderr, "Conventions don't match, invalid merge.\n");
    Exit(1);
  }
}

/* -------------------------------------------------------------------- */
void checkForOverlappingTimeSegments()
{
  int varID1, varID2;

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

  int timeDimID1, timeDimID2;
  nc_inq_dimid(infd1, "Time", &timeDimID1);
  nc_inq_dimid(infd2, "Time", &timeDimID2);

  nc_inq_dimlen(infd1, timeDimID1, &et1);
  nc_inq_dimlen(infd2, timeDimID2, &et2);

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
    fprintf(stderr, "No overlapping time segments, nothing to merge.\n");
    Exit(1);
  }

  if (bt1 != bt2 || et1 != et2)
  {
    printf("Time segments do not match exactly, this has potential to produce inconsistent results.\n");
  }
}

/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  int	rc = 0, argp = 1;
  int	merge_all = 0;

  if (argc < 3)
  {
    fprintf(stderr, "Usage: ncmerge [-v var0,var1,..,varn] [-a] primary_file secondary_file\n");
    exit(1);
  }

  putenv((char *)"TZ=UTC");

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
    printf("All variables being merged.\n");

  if (strcmp(argv[argp], "-a") == 0)
  {
    argp++;
    merge_all = 1;
  }

  ncopts = 0;

  openFiles(argc, argv, argp);
  checkForOverlappingTimeSegments();

  CopyVariablesDefinitions(merge_all);
  MoveData();

  Exit(0);
  return rc;

}	/* END MAIN */

/* -------------------------------------------------------------------- */
void CopyVariablesDefinitions(int merge_all)
{
  int	rc;
  char	name[32];
  char  dimname1[32], dimname2[32];
  nc_type	dataType;
  int	nVars1, nVars2, nDims, nAtts, dimIDs[8];
  float	missing_value = -32767.0;
  int	missing_value_int = -32767;

  nc_redef(infd1);

  nc_inq_nvars(infd1, &nVars1);
  nc_inq_nvars(infd2, &nVars2);

  for (int i = 0; i < nVars1; ++i)
    outPtrs[i] = NULL;

  for (int i = 0; i < nVars2; ++i)
  {
    nc_inq_var(infd2, i, name, &dataType, &nDims, dimIDs, &nAtts); 

    inVarID[xFerCnt] = i;
    inPtrs[i] = NULL;

    if (!merge_all) 
    {
    // Non-valid variables.
    if (strcmp(name, "HOUR") == 0 || strcmp(name, "MINUTE") == 0 ||
        strcmp(name, "SECOND") == 0 || strcmp(name, "time_offset") == 0 ||
        strcmp(name, "Time") == 0 || strcmp(name, "base_time") == 0)
      continue;
    }


    // If user has specified a var list, then only add those that qualify.
    if (VarCnt > 0)
    {
      int j;

      for (j = 0; j < VarCnt; ++j)
        if (strcmp(VarList[j], name) == 0)
        {
          VarList[j][0] = ' ';
          break;
        }

      if (j == VarCnt)
        continue;
    }


    // See if variable already exists in output file, if not then create it.
    if (nc_inq_varid(infd1, name, &rc) != NC_NOERR)
    {

      //printf("Creating variable %s with %d dimensions\n",name,nDims);
      // Figure out the dimID from the primary file that matched the dim taken from 
      // the secondary file.
      int nDims1;
      nc_inq_ndims(infd1,&nDims1);
      for (int j = 0; j < nDims; ++j)
      {
        nc_inq_dimname(infd2, dimIDs[j], dimname2);
	//printf("Found dimension %d:%d:%s in secondary file\n",j,dimIDs[j],dimname2); fflush(stdout);
	for (int k = 0; k < nDims1; ++k)
	{
          nc_inq_dimname(infd1,k,dimname1);
	  //printf("Found dimension %d:%s in base file\n",k,dimname1); fflush(stdout);
          if (strcmp(dimname1,dimname2) == 0)
          {
	    //printf("Changing dimID %d from %d to %d\n",j,dimIDs[j],k);
	    dimIDs[j] = k;
	    break;
          }
        }
      }

      if (nc_def_var(infd1, name, dataType, nDims, dimIDs, &rc) != NC_NOERR)
      {
        fprintf(stderr, "Error in creating variable %s, %s will not be in merged dataset.\n", name, name);
        continue;
      }
    }
    else
    {
      printf("Variable %s exists in primary file, it will be overwritten.\n", name);
    }

    for (int j = 0; j < nAtts; ++j)
    {
      nc_inq_attname(infd2, inVarID[xFerCnt], j, buffer);
      nc_copy_att(infd2, inVarID[xFerCnt], buffer, infd1, rc);
    }

    if (dataType == NC_INT)
    {
      nc_put_att_int(infd1, rc, "_FillValue", NC_INT, 1, &missing_value_int);
    }
    else
    {
      nc_put_att_float(infd1, rc, "_FillValue", NC_FLOAT, 1, &missing_value);
    }

    outVarID[xFerCnt++] = rc;
  }

  nc_enddef(infd1);

  if (VarCnt > 0)
    for (int i = 0; i < VarCnt; ++i)
      if (VarList[i][0] != ' ')
        fprintf(stderr, "%s does not exist in secondary file.\n", VarList[i]);

}	/* END COPYVARIABLES */

/* -------------------------------------------------------------------- */
void MoveData()
{
  int inRec, outRec, nRecords;
  size_t rd_start[3], wr_start[3], count[3], size;

  inRec = outRec = 0;

  if (bt1 < bt2) outRec = bt2 - bt1;
  if (bt1 > bt2) inRec = bt1 - bt2;

  if ((nRecords = std::min(et1, et2) - std::max(bt1, bt2)) == 0)
  {
    fprintf(stderr, "Computed number of records to merge is zero.  Something is wrong.\n");
    fprintf(stderr, "  begTimeMaster = %ld, endTimeMaster = %d, nRecords=%lu\n", bt1, et1, et1-bt1);
    fprintf(stderr, "  begTimeSecond = %ld, endTimeSecond = %d, nRecords=%lu\n", bt2, et2, et2-bt2);
    Exit(1);
  }

  count[0] = nRecords;
  wr_start[0] = outRec; wr_start[1] = wr_start[2] = 0;
  rd_start[0] = inRec; rd_start[1] = rd_start[2] = 0;

  printf("Starting to move data...\n"); fflush(stdout);

  for (int i = 0; i < xFerCnt; ++i)
  {
    // Determine product of dimensions (i.e. total # floats to move).
    int nDims, dimids[3];
    nc_inq_varndims(infd2, inVarID[i], &nDims);
    nc_inq_vardimid(infd2, inVarID[i], dimids);
    nc_inq_dimlen(infd2, dimids[0], &size);

    for (int j = 1; j < nDims; ++j)
    {
      nc_inq_dimlen(infd2, dimids[j], &count[j]);
      size *= count[j];
    }

    char name[32];
    nc_type dataType;
    nc_inq_varname(infd2, inVarID[i], name);
    nc_inq_vartype(infd2, inVarID[i], &dataType);

    //printf("Moving data for variable %s of type %d with array of size %d\n",name,dataType,size); fflush(stdout);
    //printf("Type if int is %d\n",NC_INT);fflush(stdout);

    // If you get the error: "terminate called after throwing an instance of 'std::bad_alloc'
    // what():  St9bad_alloc", then there is not enough memory left in the heap to perform the alloc.

    if (dataType == NC_INT && strcmp(name,"Time")!=0)
    {
      size = 1;
      int *data_p_int;
      try
      {
        data_p_int = new int[size];
      }
      catch (const std::bad_alloc)
      {
        fprintf(stderr, "\n***\nNot enough memory to perform alloc of data_p_int: exiting\n***\n");
	fflush(stdout);
        exit(1);
      }

      // Read input file.
      if (nc_get_vara_int(infd2, inVarID[i], rd_start, count, data_p_int) != NC_NOERR)
      {
        nc_inq_varname(infd2, inVarID[i], name);
        fprintf(stderr, "MoveData: failed to read data for variable %s\n", name);
	fflush(stdout);
        continue;
      }

      if (nc_put_vara_int(infd1, outVarID[i], wr_start, count, data_p_int) != NC_NOERR)
      {
        nc_inq_varname(infd1, outVarID[i], name);
        fprintf(stderr, "MoveData: failed to write data for variable %s\n", name);
	fflush(stdout);
        continue;
      }
      delete [] data_p_int;
    } else {
      float *data_p;
      try
      {
        data_p = new float[size];
      }
      catch (const std::bad_alloc)
      {
        fprintf(stderr, "\n***\nNot enough memory to perform alloc of data_p: exiting\n***\n");
	fflush(stdout);
        exit(1);
      }

      // Read input file.
      if (nc_get_vara_float(infd2, inVarID[i], rd_start, count, data_p) != NC_NOERR)
      {
        nc_inq_varname(infd2, inVarID[i], name);
        fprintf(stderr, "MoveData: failed to read data for variable %s\n", name);
	fflush(stdout);
        continue;
      }

      if (nc_put_vara_float(infd1, outVarID[i], wr_start, count, data_p) != NC_NOERR)
      {
        nc_inq_varname(infd1, outVarID[i], name);
        fprintf(stderr, "MoveData: failed to write data for variable %s\n", name);
	fflush(stdout);
        continue;
      }
      delete [] data_p;
    }

    printf("."); fflush(stdout);
  }

  printf("\n");

}	/* END MOVEDATA */

/* END NCMERGE.CC */