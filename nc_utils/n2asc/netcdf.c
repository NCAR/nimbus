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
  int		i, len, nVars, nDims, dimIDs[3], recDim;
  long		tmp;
  char	name[NAMELEN];

  ncopts = 0;
  nVariables = 0;


  /* Open Input File
  */
  if ((InputFile = ncopen(fileName, NC_NOWRITE)) == ERR)
    {
    sprintf(buffer, "Can't open %s.\n", fileName);
    HandleError(buffer);
    return(ERR);
    }

  ncinquire(InputFile, NULL, &nVars, NULL, &recDim);

  /* University of Wyoming King Air files don't use record dimension LDO/UWyo */
  if ( recDim == -1 )
    recDim = ncdimid( InputFile, "time" );

  if ( recDim == -1 )
    recDim = ncdimid( InputFile, "Time" );

  if (recDim == -1)
    {
    fprintf(stderr, "Can't locate 'record' dimension, fatal.\n");
    exit(1);
    }

  ncdiminq(InputFile, recDim, (char *)NULL, &nRecords);


  ncattinq(InputFile, NC_GLOBAL, "ProjectName", (nc_type *)0, &len);
  ProjectName = GetMemory(len);
  ncattget(InputFile, NC_GLOBAL, "ProjectName", ProjectName);

  ncattinq(InputFile, NC_GLOBAL, "ProjectNumber", (nc_type *)0, &len);
  ProjectNumber = GetMemory(len);
  ncattget(InputFile, NC_GLOBAL, "ProjectNumber", ProjectNumber);

  ncattinq(InputFile, NC_GLOBAL, "FlightNumber", (nc_type *)0, &len);
  FlightNumber = GetMemory(len);
  ncattget(InputFile, NC_GLOBAL, "FlightNumber", FlightNumber);

  for (i = 0; i < nVars; ++i)
    {
    ncvarinq(InputFile, i, name, NULL, &nDims, dimIDs, NULL);

    if (strcmp(name, "base_time") == 0)
      continue;

    vp = Variable[nVariables++] = (VARTBL *)GetMemory(sizeof(VARTBL));

    strcpy(vp->name, name);
    vp->SampleRate = 0;
    ncattget(InputFile, i, "SampledRate", (void *)&vp->SampleRate);

    if (nDims >= 2)
      {
      ncdiminq(InputFile, dimIDs[1], NULL, (long *)&tmp);
      vp->OutputRate = tmp;
      }
    else
      vp->OutputRate = 1;

    vp->Output	= FALSE;
    vp->inVarID	= i;
    strcpy(vp->Format, DefaultFormat);

    if (nDims >= 3)
      {
      ncdiminq(InputFile, dimIDs[2], NULL, (long *)&tmp);
      vp->VectorLength = tmp;
      }
    else
       vp->VectorLength = 1;
    }

  SortTable(Variable, 0, nVariables - 1);

  return(OK);

}	/* END READINPUTFILE */

/* -------------------------------------------------------------------- */
void CloseNetCDF()
{
  ncclose(InputFile);
  fclose(OutputFile);

  FormatTimeSegmentsForOutputFile(buffer);

  LogMessage("Time interval(s) completed : ");
  LogMessage(buffer);
  LogMessage("\n");

}	/* END CLOSENETCDF */

/* END NETCDF.C */
