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

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2007
-------------------------------------------------------------------------
*/

#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <Xm/TextF.h>

#include "define.h"
#include <netcdf.h>

#define DEFAULT_TI_LENGTH	(19 * MAX_TIME_SLICES)
#define MISSING_DATA		(-32767.0)

static int	baseTimeID, nOutRecords;
char		*passThrough[] = { NULL };

static void AverageInt(int * data, size_t count[]);
static void AverageReal(NR_TYPE * data, size_t count[]);

/* -------------------------------------------------------------------- */
int ReadInputFile(char fileName[])
{
  VARTBL	*vp;
  int		i, indx, nVars, nDims, dimIDs[3];
  size_t	len;
  int		startSortIndx = 0;
  char		name[NAMELEN];

  ncopts = 0;
  nVariables = 0;


  /* Open Input File
   */
  if (nc_open(fileName, NC_NOWRITE, &InputFile) != NC_NOERR)
  {
    sprintf(buffer, "Can't open %s.\n", fileName);
    HandleError(buffer);
    return(ERR);
  }

  nc_inq(InputFile, NULL, &nVars, NULL, NULL);

  nc_inq_att(InputFile, NC_GLOBAL, "ProjectName", (nc_type *)0, &len);
  ProjectName = GetMemory(len+1);
  nc_get_att_text(InputFile, NC_GLOBAL, "ProjectName", ProjectName);
  ProjectName[len] = '\0';

  nc_inq_att(InputFile, NC_GLOBAL, "FlightNumber", (nc_type *)0, &len);
  FlightNumber = GetMemory(len+1);
  nc_get_att_text(InputFile, NC_GLOBAL, "FlightNumber", FlightNumber);
  FlightNumber[len] = '\0';

  nc_inq_att(InputFile, NC_GLOBAL, "TimeInterval", (nc_type *)0, &len);
  TimeInterval = GetMemory(len+1);
  nc_get_att_text(InputFile, NC_GLOBAL, "TimeInterval", TimeInterval);
  FlightNumber[len] = '\0';


  for (i = 0; i < nVars; ++i)
  {
    nc_inq_var(InputFile, i, name, NULL, &nDims, dimIDs, NULL);

    if (strcmp(name, "base_time") == 0 ||
	strcmp(name, "time_offset") == 0 ||
	strcmp(name, "Time") == 0)
      continue;

    vp = Variable[nVariables++] = (VARTBL *)GetMemory(sizeof(VARTBL));

    strcpy(vp->name, name);
    nc_get_att_text(InputFile, i, "SampledRate", (void *)&vp->SampleRate);
    nc_get_att_text(InputFile, i, "OutputRate", (void *)&vp->OutputRate);

    vp->inVarID	= i;
    vp->Output	= FALSE;
    vp->VectorLength = 1;

    if (nDims == 3)
      nc_inq_dimlen(InputFile, i, &vp->VectorLength);
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
    int		id;

    first_time = FALSE;

    if (nc_inq_varid(InputFile, "base_time", &baseTimeID) == NC_NOERR)
    {
      nc_get_var1_int(InputFile, baseTimeID, NULL, (void *)&BaseTime);
      if (nc_inq_varid(OutputFile, "base_time", &baseTimeID) == NC_NOERR) {
        nc_put_var1_int(OutputFile, baseTimeID, NULL, (void *)&BaseTime);
      }
    }
  }
}	/* END SETBASETIME */

/* -------------------------------------------------------------------- */
void CreateNetCDF(const char file_name[])
{
  int		i, indx = 0, rc, in_id, out_id;
  long		One = 1;
  VARTBL	*vp;
  char		name[100];

  int		nDims, dims[3], TimeDim, LowRateDim;
  size_t	size;

  if (nc_create(file_name, NC_CLOBBER, &OutputFile) != NC_NOERR)
  {
    sprintf(buffer, "Can't create output file %s.\n", file_name);
    HandleError(buffer);
    return;
  };

  chmod(file_name, 0666);


  /* Dimensions.
  */
  nc_def_dim(OutputFile, "Time", NC_UNLIMITED, &TimeDim);

  for (i = 1; nc_inq_dim(InputFile, i, name, &size) == NC_NOERR; ++i)
    nc_def_dim(OutputFile, name, size, &dims[0]);

  LowRateDim = 1;


  /* Global Attributes.
   */
  nc_copy_att(InputFile, NC_GLOBAL, "Source", OutputFile, NC_GLOBAL);
  nc_copy_att(InputFile, NC_GLOBAL, "Address", OutputFile, NC_GLOBAL);
  nc_copy_att(InputFile, NC_GLOBAL, "Phone", OutputFile, NC_GLOBAL);
  nc_copy_att(InputFile, NC_GLOBAL, "Conventions", OutputFile, NC_GLOBAL);
  nc_copy_att(InputFile, NC_GLOBAL, "WARNING", OutputFile, NC_GLOBAL);

  {
  time_t	t;
  struct tm	*tm;

  t = time(0);
  tm = gmtime(&t);
  strftime(buffer, 128, "%h %d %R GMT %Y", tm);
  nc_put_att_text(OutputFile, NC_GLOBAL, "DateProcessed", 
				strlen(buffer)+1, buffer);
  }


  nc_copy_att(InputFile, NC_GLOBAL, "ProjectName", OutputFile, NC_GLOBAL);
  nc_copy_att(InputFile, NC_GLOBAL, "Aircraft", OutputFile, NC_GLOBAL);
  nc_copy_att(InputFile, NC_GLOBAL, "FlightNumber", OutputFile, NC_GLOBAL);
  nc_copy_att(InputFile, NC_GLOBAL, "FlightDate", OutputFile, NC_GLOBAL);
  nc_copy_att(InputFile, NC_GLOBAL, "Defaults", OutputFile, NC_GLOBAL);


  /* Will be updated later.
   */
  memset(buffer, ' ', DEFAULT_TI_LENGTH+1);
  nc_put_att_text(OutputFile, NC_GLOBAL, "TimeInterval", DEFAULT_TI_LENGTH, buffer);


  /* First dimension is time dimension.
   * Second is Rate in Hz.
   * Third is Vector Length.
   */
  dims[0] = TimeDim;


  /* "Unix Time" Variables.
   */
  if (nc_inq_varid(InputFile, "base_time", &baseTimeID) == NC_NOERR)
  {
    int newBTID;
    nc_def_var(OutputFile, "base_time", NC_LONG, 0, 0, &newBTID);
    nc_copy_att(InputFile, baseTimeID, "units", OutputFile, newBTID);
    nc_copy_att(InputFile, baseTimeID, "long_name", OutputFile, newBTID);
    indx = 0;	/* Index for data_p	*/
  }
  else
    baseTimeID = -1;

  if (nc_inq_varid(InputFile, "Time", &in_id) == NC_NOERR)
  {
    nc_inq_var(InputFile, in_id, NULL, NULL, &nDims, dims, NULL);
    nc_def_var(OutputFile, "Time", NC_INT, nDims, dims, &out_id);
    nc_copy_att(InputFile, in_id, "long_name", OutputFile, out_id);
    nc_copy_att(InputFile, in_id, "units", OutputFile, out_id);
    nc_copy_att(InputFile, in_id, "standard_name", OutputFile, out_id);
    nc_copy_att(InputFile, in_id, "strptime_format", OutputFile, out_id);
  }

  if (nc_inq_varid(InputFile, "time_offset", &in_id) == NC_NOERR)
  {
    nc_inq_var(InputFile, in_id, NULL, NULL, &nDims, dims, NULL);
    nc_def_var(OutputFile, "time_offset", NC_INT, nDims, dims, &out_id);
    nc_copy_att(InputFile, in_id, "long_name", OutputFile, out_id);
    nc_copy_att(InputFile, in_id, "units", OutputFile, out_id);
    nc_copy_att(InputFile, in_id, "standard_name", OutputFile, out_id);
  }


  /* For each variable:
   *	- Set dimensions
   *	- define variable
   *	- Set attributes
   *	- Set data_pointer
   */
  for (i = indx; i < nVariables; ++i)
  {
    if ((vp = Variable[i])->Output == FALSE || vp->VectorLength > 1)
      continue;

    dims[1] = LowRateDim;
    nDims = 1;

    nc_def_var(OutputFile, vp->name, NC_FLOAT, nDims, dims, &vp->outVarID);

    nc_copy_att(InputFile,vp->inVarID,"_FillValue", OutputFile, vp->outVarID);
    nc_copy_att(InputFile,vp->inVarID,"units", OutputFile, vp->outVarID);
    nc_copy_att(InputFile,vp->inVarID,"long_name", OutputFile, vp->outVarID);
    nc_copy_att(InputFile,vp->inVarID,"Category", OutputFile, vp->outVarID);
    nc_copy_att(InputFile,vp->inVarID,"standard_name", OutputFile, vp->outVarID);
    nc_copy_att(InputFile,vp->inVarID,"missing_value", OutputFile, vp->outVarID);
    nc_copy_att(InputFile,vp->inVarID,"SampledRate", OutputFile, vp->outVarID);
    nc_copy_att(InputFile,vp->inVarID,"DataQuality", OutputFile, vp->outVarID);
    nc_copy_att(InputFile,vp->inVarID,"Dependencies",OutputFile,vp->outVarID);
    nc_copy_att(InputFile,vp->inVarID,"CalibrationCoefficients",OutputFile,vp->outVarID);
  }

  nc_enddef(OutputFile);

}	/* END CREATENETCDF */

/* -------------------------------------------------------------------- */
void CloseNetCDF()
{
  int	len;

  FormatTimeSegmentsForOutputFile(buffer);

  if ((len = strlen(buffer) + 1) > DEFAULT_TI_LENGTH)
    nc_redef(OutputFile);

  nc_put_att_text(OutputFile, NC_GLOBAL, "TimeInterval", len, buffer);
  nc_close(InputFile);
  nc_close(OutputFile);

  LogMessage("Time interval(s) completed : ");
  LogMessage(buffer);
  LogMessage("\n");

}	/* END CLOSENETCDF */

/* -------------------------------------------------------------------- */
void PassThroughData(long * start, long * end)
{
  int		i, j, StartInputRecordNumber, EndInputRecordNumber, nInputRecords;
  int		nDims, dimID[3], bytesPerSec, id;
  size_t	inStart[3], outStart[3], count[3];
  VARTBL	*vp;

  int		* iData;
  NR_TYPE	* data;

  sprintf(buffer, "%02d:%02d:%02d", start[0], start[1], start[2]);
  StartInputRecordNumber = GetFlightRecordNumber(InputFile, buffer);

  if (end[0] < start[0])
    end[0] += 24;

  sprintf(buffer, "%02d:%02d:%02d", end[0], end[1], end[2]);
  EndInputRecordNumber = GetFlightRecordNumber(InputFile, buffer);

  nInputRecords = EndInputRecordNumber - StartInputRecordNumber+1;
  nOutRecords = nInputRecords / AverageRate;

  inStart[0] = StartInputRecordNumber;
  inStart[1] = inStart[2] = 0;
  outStart[0] = outStart[1] = outStart[2] = 0;
  count[1] = count[2] = 0;

  /* Pass through any "time" variables.
   */
  if (nc_inq_varid(InputFile, "Time", &id) == NC_NOERR)
  {
    iData = (int *)GetMemory(sizeof(int) * nInputRecords);

    count[0] = nInputRecords;
    nc_get_vara_int(InputFile, id, inStart, count, iData);
    AverageInt(iData, count);

    nc_inq_varid(OutputFile, "Time", &id);
    count[0] = nOutRecords;
    nc_put_vara_int(OutputFile, id, outStart, count, iData);
    free((char *)iData);
  }

  if (nc_inq_varid(InputFile, "time_offset", &id) == NC_NOERR)
  {
    iData = (int *)GetMemory(sizeof(int) * nInputRecords);

    count[0] = nInputRecords;
    nc_get_vara_int(InputFile, id, inStart, count, iData);
    AverageInt(iData, count);

    nc_inq_varid(OutputFile, "time_offset", &id);
    count[0] = nOutRecords;
    nc_put_vara_int(OutputFile, id, outStart, count, iData);
    free((char *)iData);
  }

  /* Pass through User requested variables.
   */
  for (i = 0; i < nVariables; ++i)
  {
    if ((vp = Variable[i])->Output == FALSE)
      continue;

    XmTextFieldSetString(timeDisplayText, vp->name);
    FlushXEvents();

    count[1] = count[2] = 0;

    nc_inq_var(InputFile, vp->inVarID, NULL, NULL, &nDims, dimID, NULL);

    bytesPerSec = NR_SIZE;

    switch (nDims)
    {
      case 3:
        nc_inq_dim(InputFile, dimID[2], NULL, &count[2]);
        bytesPerSec *= count[2];

      case 2:
        nc_inq_dim(InputFile, dimID[1], NULL, &count[1]);
        bytesPerSec *= count[1];

      case 1:
        count[0] = nInputRecords;
    }

    data = (NR_TYPE *)GetMemory(bytesPerSec * nInputRecords);

    nc_get_vara_float(InputFile, vp->inVarID, inStart, count, data);
    AverageReal(data, count);

    count[0] = nOutRecords;
    nc_put_vara_float(OutputFile, vp->outVarID, outStart, count, data);
    free((char *)data);
  }
}	/* END PASSTHROUGHDATA */

/* -------------------------------------------------------------------- */
static void AverageInt(int * data, size_t count[])
{
  int	i, j, missDataCnt, dataCnt;
  long	sum;

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
}	/* END AVERAGEINT */

/* -------------------------------------------------------------------- */
static void AverageReal(NR_TYPE * data, size_t count[])
{
  int		i, j, missDataCnt, dataCnt;
  double	sum;

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
}	/* END AVERAGEREAL */

/* END NETCDF.C */
