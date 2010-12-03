/*
-------------------------------------------------------------------------
OBJECT NAME:	netcdf.c

FULL NAME:	NetCDF IO

ENTRY POINTS:	ReadInputFile()
		CreateOutputFile()
		CloseNetCDF()

STATIC FNS:	none

DESCRIPTION:	This file has the routines necassary to Create and write
		data for distribution of NCAR/RAF aircraft data in netCDF
		format.

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2005
-------------------------------------------------------------------------
*/

#include <algorithm>
#include <cerrno>

#include "define.h"
#include <netcdf.h>

void FormatTimeSegmentsForOutputFile(char buff[]);
void SortTable(std::vector<VARTBL*>, int beg, int end);
void getNCattr(int ncid, const char attr[], char **dest);
static bool VarCompareLT(const VARTBL *x, const VARTBL *y);


/* -------------------------------------------------------------------- */
int CreateOutputFile(char fileName[])
{
  if ((OutputFile = fopen(fileName, "w+")) == NULL)
    {
    sprintf(buffer, "Can't open %s.\n", fileName);
    HandleError(buffer);
    return(ERR);
    }

  return(OK);

}	/* END CREATEOUTPUTFILE */

/* -------------------------------------------------------------------- */
void processTime(int bt, int ncid, int varid)
{
  char units[128];

  // base_time/time_offset pair.
  if (bt != 0)
  {
    FileStartTime = bt;
  }
  else
  // New 'Time' variable.
  {
    if (nc_get_att_text(ncid, varid, "units", units) != NC_NOERR)
    {
      fprintf(stderr, "No units for 'Time' variable!!! Fatal.\n");
      exit(1);
    }

    struct tm tm;
    strptime(units, "seconds since %F %T %z", &tm);
    FileStartTime = mktime(&tm);
  }

  time_data = (int *)malloc(nRecords * sizeof(int));
  nc_get_var_int(ncid, varid, time_data);
  FileEndTime = FileStartTime + time_data[nRecords-1];

  UserStartTime = FileStartTime;
  UserEndTime = FileEndTime;
}

/* -------------------------------------------------------------------- */
int ReadInputFile(char fileName[])
{
  VARTBL	*vp;
  int		i, nVars, nDims, dimIDs[3], recDim;
  size_t	tmp;
  char		name[128], units[128];


  /* Open Input File
  */
  if (nc_open(fileName, NC_NOWRITE, &InputFile) != NC_NOERR)
  {
    sprintf(buffer, "Can't open %s.\n", fileName);
    HandleError(buffer);
    return(ERR);
  }

  nc_inq_nvars(InputFile, &nVars);

  if (nc_inq_dimid(InputFile, "Time", &recDim) != NC_NOERR)
    if (nc_inq_dimid(InputFile, "time", &recDim) != NC_NOERR)
    {
      fprintf(stderr, "Can't locate 'record' dimension, fatal.\n");
      exit(1);
    }

  nc_inq_dimlen(InputFile, recDim, &nRecords);


  getNCattr(InputFile, "ProjectName", &ProjectName);
  getNCattr(InputFile, "ProjectNumber", &ProjectNumber);
  getNCattr(InputFile, "FlightNumber", &FlightNumber);

  for (i = 0; i < nVars; ++i)
  {
    nc_inq_var(InputFile, i, name, 0, &nDims, dimIDs, 0);

    if (strcmp(name, "base_time") == 0)
    {
      tmp = 0;
      int bt;
      nc_get_var1_int(InputFile, i, &tmp, &bt);
      processTime(bt, InputFile, i+1);
      continue;	// because this isn't a record variable, we can't output it.
    }
    if (strcmp(name, "Time") == 0)
    {
      processTime(0, InputFile, i);
    }

    vp = new VARTBL;
    Variable.push_back(vp);

    vp->name = name;
    vp->SampleRate = 0;
    nc_get_att_int(InputFile, i, "SampledRate", &vp->SampleRate);
    nc_get_att_text(InputFile, i, "units", units);
    vp->units = units;

    if (nDims >= 2)
    {
      nc_inq_dimlen(InputFile, dimIDs[1], &tmp);
      vp->OutputRate = tmp;
    }
    else
      vp->OutputRate = 1;

    vp->Output	= false;
    vp->inVarID	= i;
    if (vp->name == "Time")
      strcpy(vp->Format, " %d");
    else
      strcpy(vp->Format, DefaultFormat);

    if (nDims >= 3)
    {
      nc_inq_dimlen(InputFile, dimIDs[2], &tmp);
      vp->VectorLength = tmp;
    }
    else
       vp->VectorLength = 1;
  }

  std::sort(Variable.begin(), Variable.end(), VarCompareLT);

  return(OK);

}	/* END READINPUTFILE */

/* -------------------------------------------------------------------- */
void getNCattr(int ncid, const char attr[], char **dest)
{
  size_t len;

  if (nc_inq_attlen(ncid, NC_GLOBAL, attr, &len) == NC_NOERR)
    {
    *dest = new char[len+1];
    nc_get_att_text(ncid, NC_GLOBAL, attr, *dest);
    (*dest)[len] = '\0';

    while ((*dest)[--len] < 0x20)       /* Remove extraneous CR/LF, etc */
      (*dest)[len] = '\0';
    }
  else
    {
    *dest = new char[1];
    *dest[0] = '\0';
    }

}       /* END GETNCATTR */

/* -------------------------------------------------------------------- */
void CloseNetCDF()
{
  nc_close(InputFile);
  fclose(OutputFile);

  FormatTimeSegmentsForOutputFile(buffer);

  LogMessage("Time interval(s) completed : ");
  LogMessage(buffer);
  LogMessage("\n");

}	/* END CLOSENETCDF */

/* -------------------------------------------------------------------- */
static bool VarCompareLT(const VARTBL *x, const VARTBL *y)
{
    return(x->name < y->name);
}

/* END NETCDF.C */
