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

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-06
-------------------------------------------------------------------------
*/

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <netcdf.h>

#define NAMELEN		16
#define MAX_IN_VARS	800
#define MAX_OUT_VARS	1000

char	buffer[2048];
char	VarList[MAX_IN_VARS][NAMELEN];
int	inVarID[MAX_IN_VARS], outVarID[MAX_OUT_VARS];
void	*inPtrs[MAX_IN_VARS], *outPtrs[MAX_OUT_VARS];
int	infd1, infd2, VarCnt = 0;
long	bt1, bt2, et1, et2;
float	DataRecord[MAX_IN_VARS*25];
bool	ExactTimeSegments = true;

void	CopyVariablesDefinitions(), MoveData();

extern "C" char	*strupr(char s[]);


/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  int	rc = 0,
	argp = 1;

  if (argc < 3)
    {
    fprintf(stderr, "Usage: ncmerge [-v var0,var1,..,varn] primary_file secondary_file\n");
    exit(1);
    }


  /* Get variable list if it exists.
   */
  if (strcmp(argv[argp], "-v") == 0)
    {
    char	*p;

    ++argp;
    p = strtok(argv[argp++], ",");

    do
      {
      strcpy(VarList[VarCnt++], strupr(p));
      }
    while ((p = strtok(NULL, ",")));
    }
  else
    printf("All variables being merged.\n");


  ncopts = 0;


  /* Open files.
   */
  if ((infd1 = ncopen(argv[argp++], NC_WRITE)) == (-1))
    {
    fprintf(stderr, "Can't open primary file %s\n", argv[argp-1]);
    rc = 1;
    goto exit;
    }

  if ((infd2 = ncopen(argv[argp++], NC_NOWRITE)) == (-1))
    {
    fprintf(stderr, "Can't open secondary file %s\n", argv[argp-1]);
    rc = 1;
    goto exit;
    }


  /* Make sure files are similar.
   */
  ncattget(infd1, NC_GLOBAL, "Conventions", (void *)buffer);
  ncattget(infd2, NC_GLOBAL, "Conventions", (void *)&buffer[500]);

  if (strcmp(buffer, &buffer[500]) != 0)
    {
    fprintf(stderr, "Conventions don't match, invalid merge.\n");
    rc = 1;
    goto exit;
    }


  /* Make sure files have overlapping time intervals.
   */
  {
  long	mindex[2];

  mindex[0] = mindex[1] = 0;

  ncvarget1(infd1, ncvarid(infd1, "base_time"), mindex, &bt1);
  ncvarget1(infd2, ncvarid(infd2, "base_time"), mindex, &bt2);

  ncdiminq(infd1, ncdimid(infd1, "Time"), NULL, &et1);
  ncdiminq(infd2, ncdimid(infd2, "Time"), NULL, &et2);
  }

  if (bt1 == 0 || bt2 == 0)
    {
    fprintf(stderr, "At least one file has a base_time of 0, this implies asc2cdf was used\nwithout '-b' option.  Merged data may be shifted in time.\n");

    if (bt1 == 0 && bt2 == 0)
      fprintf(stderr, "Both base_time's are 0, files must have identical start times\nor else merged data will be shifted in time.\n");
    }

  et1 += bt1;
  et2 += bt2;

  if (et2 < bt1 || bt2 > et1)
    {
    fprintf(stderr, "No overlapping time segments, nothing to merge.\n");
    rc = 0;
    goto exit;
    }

  if (bt1 != bt2 || et1 != et2)
    {
    printf("Time segments do not match exactly, this has potential to produce inconsistent results.\n");
    ExactTimeSegments = false;
    }


  CopyVariablesDefinitions();
  MoveData();


exit:
  ncclose(infd1);
  ncclose(infd2);

  return(rc);

}	/* END MAIN */

/* -------------------------------------------------------------------- */
void CopyVariablesDefinitions()
{
  int	i, j, rc, inCnt = 0;
  char	name[32];
  nc_type	dataType;
  int	nVars1, nVars2, varID[MAX_IN_VARS], nDims, nAtts, dimIDs[4];
  float	missing_value = -32767.0;


  ncredef(infd1);

  ncrecinq(infd1, &nVars1, 0, (long *)0);
  ncrecinq(infd2, &nVars2, varID, (long *)0);


  for (i = 0; i < nVars1; ++i)
    outPtrs[i] = NULL;


  for (i = 0; i < nVars2; ++i)
    {
    ncvarinq(infd2, varID[i], name, &dataType, &nDims, dimIDs, &nAtts); 
    inVarID[inCnt] = varID[i];
    inPtrs[i] = NULL;


    /* Non-valid variables.
    */
    if (strcmp(name, "HOUR") == 0 || strcmp(name, "MINUTE") == 0 ||
        strcmp(name, "SECOND") == 0 || strcmp(name, "time_offset") == 0 ||
        strcmp(name, "Time") == 0)
      continue;


    /* If user has specified a var list, then only add those that qualify.
    */
    if (VarCnt > 0)
      {
      int		j;

      for (j = 0; j < VarCnt; ++j)
        if (strcmp(VarList[j], name) == 0)
          {
          VarList[j][0] = ' ';
          break;
          }

      if (j == VarCnt)
        continue;
      }


    /* See if variable already exists in output file, if not then create it.
    */
    if ((rc = ncvarid(infd1, name)) == (-1))
      {
      if ((rc = ncvardef(infd1, name, dataType, nDims, dimIDs)) == (-1))
        {
        fprintf(stderr, "Error in creating variable %s, %s will not be in merged dataset.\n", name, name);
        continue;
        }
      }
    else
      {
      printf("Variable %s exists in primary file, it will be overwritten.\n", name);
      if (!ExactTimeSegments)
        printf("  Warning: beware of non-matching time segment.\n");
      }


    for (j = 0; j < nAtts; ++j)
      {
      ncattname(infd2, inVarID[inCnt], j, buffer);
      ncattcopy(infd2, inVarID[inCnt], buffer, infd1, rc);
      }

    ncattput(infd1, rc, "_FillValue", NC_FLOAT, 1, &missing_value);
    ncattput(infd1, rc, "missing_value", NC_FLOAT, 1, &missing_value);

    inPtrs[i] = &DataRecord[inCnt * 25];
    outPtrs[rc-1] = &DataRecord[inCnt * 25];
    ++inCnt;
    }

  ncendef(infd1);

  if (VarCnt > 0)
    for (i = 0; i < VarCnt; ++i)
      if (VarList[i][0] != ' ')
        fprintf(stderr, "%s does not exist in secondary file.\n", VarList[i]);

}	/* END COPYVARIABLES */

/* -------------------------------------------------------------------- */
void MoveData()
{
  int	inRec, outRec, nRecords;

  inRec = outRec = 0;
  nRecords = std::min(et1-bt1, et2-bt2);

  if (bt1 < bt2)
    outRec = bt2 - bt1;

  if (bt1 > bt2)
    inRec = bt1 - bt2;

  printf("Starting to move data...."); fflush(stdout);

  for (; inRec < nRecords; ++inRec, ++outRec)
    {
    if (ncrecget(infd2, inRec, inPtrs) == (-1))
      fprintf(stderr, "ncrecget error.\n");

    if (ncrecput(infd1, outRec, outPtrs) == (-1))
      fprintf(stderr, "ncrecget error.\n");
    }

  printf("\n");

}	/* END MOVEDATA */

/* -------------------------------------------------------------------- */
char *strupr(char s[])
{
  char  *p;

  for (p = s; *p; ++p)
    *p = toupper(*p);

  return(s);

}       /* END STRUPR */

/* END NCMERGE.CC */
