/*
-------------------------------------------------------------------------
OBJECT NAME:	netcdf.c

FULL NAME:	NetCDF IO

ENTRY POINTS:	ReadInputFile()
		CreateNetCDF()
		CloseNetCDF()
		SetBaseTime()
		PassThroughData()

STATIC FNS:	Average()

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
#include <sys/types.h>
#include <sys/stat.h>

#include <Xm/TextF.h>

#include "define.h"
#include "netcdf.h"

#define DEFAULT_TI_LENGTH	(19 * MAX_TIME_SLICES)
#define MISSING_DATA		(-32767.0)

static int	baseTimeID, nOutRecords;
char		*passThrough[] = { "HOUR", "MINUTE", "SECOND", NULL };

static void Average(NR_TYPE *data, long count[]);

/* -------------------------------------------------------------------- */
int ReadInputFile(char fileName[])
{
  VARTBL	*vp;
  int		i, indx, len, nVars, nDims, dimIDs[3], recDim;
  char		name[NAMELEN];

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

  ncattinq(InputFile, NC_GLOBAL, "TimeInterval", (nc_type *)0, &len);
  TimeInterval = GetMemory(len);
  ncattget(InputFile, NC_GLOBAL, "TimeInterval", TimeInterval);


  for (i = 2; i < nVars; ++i)
    {
    ncvarinq(InputFile, i, name, NULL, &nDims, dimIDs, NULL);

    vp = Variable[nVariables++] = (VARTBL *)GetMemory(sizeof(VARTBL));

    strcpy(vp->name, name);
    ncattget(InputFile, i, "SampledRate", (void *)&vp->SampleRate);
    ncattget(InputFile, i, "OutputRate", (void *)&vp->OutputRate);

    vp->Output	= FALSE;
    vp->inVarID	= i;

    if (ncattget(InputFile, i, "VectorLength",(void *)&vp->VectorLength) == ERR)
    vp->VectorLength = 1;
    }

  SortTable((char **)Variable, 0, nVariables - 1);

  /* Turn on variables for Pass through.
   */
  for (i = 0; passThrough[i]; ++i)
    if ((indx = SearchTable((char **)Variable, nVariables, passThrough[i])) != ERR)
      Variable[indx]->Output = TRUE;


  return(OK);

}	/* END READINPUTFILE */

/* -------------------------------------------------------------------- */
void SetBaseTime()
{
  static bool	first_time = TRUE;

  if (first_time)
    {
    time_t	BaseTime;

    first_time = FALSE;

    ncvarget1(InputFile, baseTimeID, NULL, (void *)&BaseTime);
    ncvarput1(OutputFile, baseTimeID, NULL, (void *)&BaseTime);
    }

}	/* END SETBASETIME */

/* -------------------------------------------------------------------- */
void CreateNetCDF(char file_name[])
{
  int		i, indx;
  long		One = 1;
  VARTBL	*vp;

  int	timeOffsetID;
  int	ndims, dims[3], TimeDim,
	LowRateDim, HighRateDim, Dim5Hz, Dim10Hz, Dim50Hz, Dim250Hz,
	Dim2Hz, Dim1000Hz, Vector16Dim, Vector32Dim, Vector64Dim,
	AsyncDim, Dim20Hz;


  OutputFile = nccreate(file_name, NC_CLOBBER);
  chmod(file_name, 0666);


  /* Dimensions.
  */
  TimeDim	= ncdimdef(OutputFile, "Time", NC_UNLIMITED);

  LowRateDim	= ncdimdef(OutputFile, "sps1", 1);
  Dim2Hz	= ncdimdef(OutputFile, "sps2", 2);
  Dim5Hz	= ncdimdef(OutputFile, "sps5", 5);
  Dim10Hz	= ncdimdef(OutputFile, "sps10", 10);
  Dim20Hz	= ncdimdef(OutputFile, "sps20", 20);
  HighRateDim	= ncdimdef(OutputFile, "sps25", 25);
  Dim50Hz	= ncdimdef(OutputFile, "sps50", 50);
  Dim250Hz	= ncdimdef(OutputFile, "sps250", 250);
  Dim1000Hz	= ncdimdef(OutputFile, "sps1000", 1000);

  AsyncDim	= ncdimdef(OutputFile, "Async", 3);
  Vector16Dim	= ncdimdef(OutputFile, "Vector16", 16);
  Vector32Dim	= ncdimdef(OutputFile, "Vector32", 32);
  Vector64Dim	= ncdimdef(OutputFile, "Vector64", 64);


  /* Global Attributes.
   */
  ncattcopy(InputFile, NC_GLOBAL, "Source", OutputFile, NC_GLOBAL);
  ncattcopy(InputFile, NC_GLOBAL, "Address", OutputFile, NC_GLOBAL);
  ncattcopy(InputFile, NC_GLOBAL, "Phone", OutputFile, NC_GLOBAL);
  ncattcopy(InputFile, NC_GLOBAL, "Conventions", OutputFile, NC_GLOBAL);
  ncattcopy(InputFile, NC_GLOBAL, "WARNING", OutputFile, NC_GLOBAL);

  {
  time_t		t;
  struct tm	*tm;

  t = time(0);
  tm = gmtime(&t);
  strftime(buffer, 128, "%h %d %R GMT %Y", tm);
  ncattput(OutputFile, NC_GLOBAL, "DateProcessed", NC_CHAR,
				strlen(buffer)+1, (void *)buffer);
  }


  ncattcopy(InputFile, NC_GLOBAL, "ProjectName", OutputFile, NC_GLOBAL);
  ncattcopy(InputFile, NC_GLOBAL, "Aircraft", OutputFile, NC_GLOBAL);
  ncattcopy(InputFile, NC_GLOBAL, "ProjectNumber", OutputFile, NC_GLOBAL);
  ncattcopy(InputFile, NC_GLOBAL, "FlightNumber", OutputFile, NC_GLOBAL);
  ncattcopy(InputFile, NC_GLOBAL, "FlightDate", OutputFile, NC_GLOBAL);
  ncattcopy(InputFile, NC_GLOBAL, "Defaults", OutputFile, NC_GLOBAL);


  /* Will be updated later.
   */
  ncattput(OutputFile, NC_GLOBAL, "TimeInterval", NC_CHAR, DEFAULT_TI_LENGTH, (void *)"");


  /* First dimension is time dimension.
   * Second is Rate in Hz.
   * Third is Vector Length.
   */
  dims[0] = TimeDim;


  /* "Unix Time" Variables.
   */
  baseTimeID = ncvardef(OutputFile, "base_time", NC_LONG, 0, 0);
  strcpy(buffer, "Seconds since Jan 1, 1970.");
  ncattput(OutputFile, baseTimeID, "long_name", NC_CHAR, strlen(buffer)+1, buffer);

  timeOffsetID = ncvardef(OutputFile, "time_offset", NC_FLOAT, 1, dims);
  strcpy(buffer, "Seconds since base_time.");
  ncattput(OutputFile, timeOffsetID, "long_name", NC_CHAR, strlen(buffer)+1, buffer);


  /* ALL variables.
   */
  indx = 1;	/* Index for data_p	*/

  /* For each variable:
   *	- Set dimensions
   *	- define variable
   *	- Set attributes
   *	- Set data_pointer
   */
  for (i = 0; i < nVariables; ++i)
    {
    if ((vp = Variable[i])->Output == FALSE || vp->VectorLength > 1)
      continue;

    dims[1] = LowRateDim;
    ndims = 1;

    vp->outVarID = ncvardef(OutputFile, vp->name, NC_FLOAT, ndims, dims);

    ncattcopy(InputFile,vp->inVarID,"units", OutputFile, vp->outVarID);
    ncattcopy(InputFile,vp->inVarID,"long_name", OutputFile, vp->outVarID);
    ncattcopy(InputFile,vp->inVarID,"SampledRate", OutputFile, vp->outVarID);
    ncattput(OutputFile, vp->outVarID, "OutputRate", NC_LONG, 1, &One);
    ncattcopy(InputFile,vp->inVarID,"MissingValue",OutputFile,vp->outVarID);
    ncattcopy(InputFile,vp->inVarID,"VectorLength",OutputFile,vp->outVarID);
    ncattcopy(InputFile,vp->inVarID,"Dependencies",OutputFile,vp->outVarID);
    ncattcopy(InputFile,vp->inVarID,"CalibrationCoefficients",OutputFile,vp->outVarID);
    }


  ncendef(OutputFile);
  ncsync(OutputFile);

}	/* END CREATENETCDF */

/* -------------------------------------------------------------------- */
void CloseNetCDF()
{
  int	len;

  FormatTimeSegmentsForOutputFile(buffer);

  if ((len = strlen(buffer) + 1) > DEFAULT_TI_LENGTH)
    ncredef(OutputFile);

  ncattput(OutputFile, NC_GLOBAL, "TimeInterval", NC_CHAR, len, (void *)buffer);
  ncclose(InputFile);
  ncclose(OutputFile);

  LogMessage("Time interval(s) completed : ");
  LogMessage(buffer);
  LogMessage("\n");

}	/* END CLOSENETCDF */

/* -------------------------------------------------------------------- */
#define nBYTES	1000000

void PassThroughData(start, end)
long	*start;
long	*end;
{
  int		i, j, cnt, nRecs;
  int		nDims, dimID[3], bytesPerSec, nSec, nPasses, itoid, otoid;
  long		inStart[3], outStart[3], count[3];
  VARTBL	*vp;
  NR_TYPE	*data;
  bool		turnOver = FALSE;

  data = (NR_TYPE *)GetMemory(nBYTES);


  /* Locate start and stop records.
   */
  inStart[0] = 0;
  count[0] = nRecords;
  ncvarget(InputFile, ncvarid(InputFile, "HOUR"), inStart,count, (void *)data);

  if (data[0] > 12)
    turnOver = TRUE;

  cnt = 0;
  for (i = 0; i < nRecords; ++i, ++cnt)
    if (turnOver && data[i] > 12 && start[0] < 12)
      {
      if (data[i] >= start[0] + 24)
        break;
      }
    else
      {
      if (data[i] >= start[0])
        break;
      }

  inStart[0] = cnt;
  count[0] = MIN(3600, nRecords);
  ncvarget(InputFile, ncvarid(InputFile,"MINUTE"), inStart,count,(void *)data);

  for (i = 0; i < count[0]; ++i, ++cnt)
    if (data[i] >= start[1])
      break;

  inStart[0] = cnt;
  count[0] = MIN(60, nRecords);
  ncvarget(InputFile, ncvarid(InputFile,"SECOND"), inStart,count,(void *)data);

  for (i = 0; i < 60; ++i, ++cnt)
    if (data[i] >= start[2])
      break;

  CurrentInputRecordNumber = cnt;

  if (end[0] < start[0])
    end[0] += 24;

  itoid = ncvarid(InputFile, "time_offset");
  otoid = ncvarid(OutputFile, "time_offset");

  nRecs = ((end[0] * 3600) + (end[1] * 60) + end[2]) -
	  ((start[0] * 3600) + (start[1] * 60) + start[2]);

  inStart[0] = 0; count[0] = 2;
  ncvarget(InputFile, itoid, inStart, count, (void *)data);

  nRecs /= (data[1] - data[0]);

  /* Pass through time_offset
   */
  inStart[0] = CurrentInputRecordNumber;
  inStart[1] = inStart[2] = 0;
  outStart[0] = outStart[1] = outStart[2] = 0;
  count[1] = count[2] = 0;

  count[0] = nRecs;
  bytesPerSec = NR_SIZE;
  nSec = nBYTES / bytesPerSec;
  nPasses = nRecs / nSec;
  nOutRecords = nRecs / AverageRate;

  if (count[0] % nSec != 0)
    ++nPasses;

  FlushXEvents();

  if (nPasses <= 1)
    {
    ncvarget(InputFile, itoid, inStart, count, (void *)data);
    Average(data, count);

    count[0] = nOutRecords;
    ncvarput(OutputFile, otoid, outStart, count, (void *)data);
    }
  else
    {

    for (j = 0; j < nPasses; )
      {
      count[0] = nSec;

      if (nSec * (j+1) > nRecs)
        count[0] = nRecs % nSec;

      ncvarget(InputFile, itoid, inStart, count, (void *)data);
      Average(data, count);

      count[0] = nOutRecords;
      ncvarput(OutputFile, otoid, outStart, count, (void *)data);

      inStart[0] += nSec;
      outStart[0] += nSec / AverageRate;
      }
    }



  /* Pass through User requested variables
   */
  for (i = 0; i < nVariables; ++i)
    {
    if ((vp = Variable[i])->Output == FALSE)
      continue;

    XmTextFieldSetString(timeDisplayText, vp->name);
    FlushXEvents();

    inStart[0] = CurrentInputRecordNumber;
    inStart[1] = inStart[2] = 0;
    outStart[0] = outStart[1] = outStart[2] = 0;
    count[1] = count[2] = 0;


    ncvarinq(InputFile, vp->inVarID, NULL, NULL, &nDims, dimID, NULL);

    bytesPerSec = NR_SIZE;

    switch (nDims)
      {
      case 3:
        ncdiminq(InputFile, dimID[2], NULL, &count[2]);
        bytesPerSec *= count[2];

      case 2:
        ncdiminq(InputFile, dimID[1], NULL, &count[1]);
        bytesPerSec *= count[1];

      case 1:
        count[0] = nRecs;
      }

    nSec = nBYTES / bytesPerSec;
    nPasses = nRecs / nSec;

    if (count[0] % nSec != 0)
      ++nPasses;

    if (nPasses <= 1)
      {
      ncvarget(InputFile, vp->inVarID, inStart, count, (void *)data);
      Average(data, count);

      count[0] = nOutRecords;
      ncvarput(OutputFile, vp->outVarID, outStart, count, (void *)data);
      }
    else
      {
      for (j = 0; j < nPasses; ++j)
        {
        count[0] = nSec;

        if (nSec * (j+1) > nRecs)
          count[0] = nRecs % nSec;

        ncvarget(InputFile, vp->inVarID, inStart, count, (void *)data);
        Average(data, count);

        count[0] = nOutRecords;
        ncvarput(OutputFile, vp->outVarID,outStart,count,(void *)data);

        inStart[0] += nSec;
        outStart[0] += nOutRecords;
        }
      }
    }

  free((char *)data);

}	/* END PASSTHROUGHDATA */

/* -------------------------------------------------------------------- */
static void Average(NR_TYPE *data, long count[])
{
  int		i, j, missDataCnt, dataCnt;
  NR_TYPE	sum;

  if (count[1] > 0)	/* High rate data	*/
    dataCnt = AverageRate * count[1];
  else
    dataCnt = AverageRate;


  for (i = 0; i < nOutRecords; ++i)
    {
    sum = 0.0;
    missDataCnt = 0;

    for (j = 0; j < dataCnt; ++j)
      if (data[(i * dataCnt) + j] != MISSING_DATA)
        sum += data[(i * dataCnt) + j];
      else
        ++missDataCnt;

    if ((missDataCnt * 100 / AverageRate) < mvThreshold)
      data[i] = sum / (dataCnt - missDataCnt);
    else
      data[i] = MISSING_DATA;
    }

}	/* END AVERAGE */

/* END NETCDF.C */
