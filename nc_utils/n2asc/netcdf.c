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

INPUT:			

OUTPUT:		none

REFERENCES:	none

REFERENCED BY:	LowRateLoop()

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include <errno.h>
#include <time.h>

#include "define.h"
#include "netcdf.h"

void FormatTimeSegmentsForOutputFile(char buff[]);
void SortTable(char **table, int beg, int end);
void getNCattr(int ncid, char attr[], char **dest);


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
int ReadInputFile(char fileName[])
{
  VARTBL	*vp;
  int		i, nVars, nDims, dimIDs[3], recDim;
  size_t		tmp;
  char	name[NAMELEN];

  nVariables = 0;


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
      continue;

    vp = Variable[nVariables++] = (VARTBL *)GetMemory(sizeof(VARTBL));

    strcpy(vp->name, name);
    vp->SampleRate = 0;
    nc_get_att_int(InputFile, i, "SampledRate", (void *)&vp->SampleRate);

    if (nDims >= 2)
      {
      nc_inq_dimlen(InputFile, dimIDs[1], &tmp);
      vp->OutputRate = tmp;
      }
    else
      vp->OutputRate = 1;

    vp->Output	= FALSE;
    vp->inVarID	= i;
    if (strcmp(vp->name, "Time") == 0)
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

  SortTable(Variable, 0, nVariables - 1);

  return(OK);

}	/* END READINPUTFILE */

/* -------------------------------------------------------------------- */
void getNCattr(int ncid, char attr[], char **dest)
{
  size_t len;

  if (nc_inq_attlen(ncid, NC_GLOBAL, attr, &len) == NC_NOERR)
    {
    *dest = (char *)GetMemory(len+1);
    nc_get_att_text(ncid, NC_GLOBAL, attr, *dest);
    (*dest)[len] = '\0';

    while ((*dest)[--len] < 0x20)       /* Remove extraneous CR/LF, etc */
      (*dest)[len] = '\0';
    }
  else
    {
    *dest = (char *)GetMemory(1);
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

/* END NETCDF.C */
