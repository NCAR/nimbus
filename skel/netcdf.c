/*
-------------------------------------------------------------------------
OBJECT NAME:	netcdf.c

FULL NAME:	NetCDF IO

ENTRY POINTS:	ReadInputFile()
		CreateNetCDF()
		CloseNetCDF()
		SetBaseTime()
		PassThroughData()

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

#define DEFAULT_TI_LENGTH	(19 * MAX_TIME_SLICES)

static int	baseTimeID;

extern char	*passThrough[];
extern DERVAR	derivftns[];

/* -------------------------------------------------------------------- */
ReadInputFile(fileName)
char	fileName[];
{
	VARTBL	*vp;
	int	i, indx, len, nVars, nDims, dimIDs[3], recDim;
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


	for (i = 2; i < nVars; ++i)
		{
		ncvarinq(InputFile, i, name, NULL, &nDims, dimIDs, NULL);

		vp = Variable[nVariables++] = (VARTBL *)GetMemory(sizeof(VARTBL));

		strcpy(vp->name, name);
		ncattget(InputFile, i, "SampledRate", (void *)&vp->SampleRate);
		ncattget(InputFile, i, "OutputRate", (void *)&vp->OutputRate);

		vp->Output	= FALSE;
		vp->inVarID	= i;
		vp->Average	= NULL;
		vp->Compute	= NULL;

		if (ncattget(InputFile, i, "VectorLength", (void *)&vp->VectorLength) == ERR)
			vp->VectorLength = 1;
		}

	SortTable(Variable, 0, nVariables - 1);

	/* Turn on variables for Pass through.
	 */
	for (i = 0; passThrough[i]; ++i)
		if ((indx = SearchTable(Variable, nVariables, passThrough[i])) != ERR)
			Variable[indx]->Output = TRUE;


	/* Add new derived variables.
	 */
	nVars = nVariables;

	for (i = 0; derivftns[i].Compute; ++i)
		if ((indx = SearchTable(Variable,nVariables,derivftns[i].Name)) != ERR)
			{
			Variable[indx]->Output = TRUE;
			Variable[indx]->Compute = derivftns[i].Compute;
			}
		else
			{
			vp = Variable[nVars++] = (VARTBL *)GetMemory(sizeof(VARTBL));

			strcpy(vp->name, derivftns[i].Name);

			vp->SampleRate	= 0;
			vp->OutputRate	= derivftns[i].Rate;
			vp->VectorLength= derivftns[i].VectorLength;
			vp->Output		= TRUE;
			vp->inVarID		= ERR;
			vp->Average		= NULL;
			vp->Compute		= derivftns[i].Compute;
			vp->units		= derivftns[i].Units;
			vp->title		= derivftns[i].Title;
			}

	nVariables = nVars;
	SortTable(Variable, 0, nVariables - 1);

	return(OK);

}	/* END READINPUTFILE */

/* -------------------------------------------------------------------- */
void SetBaseTime()
{
	static bool	first_time = TRUE;
	long		BaseTime;

	if (first_time)
		{
		time_t	BaseTime;

		first_time = FALSE;

		ncvarget1(InputFile, baseTimeID, NULL, (void *)&BaseTime);
		ncvarput1(OutputFile, baseTimeID, NULL, (void *)&BaseTime);
		}

}	/* END SETBASETIME */

/* -------------------------------------------------------------------- */
void CreateNetCDF(file_name)
char	file_name[];
{
	register int	i;
	register VARTBL	*vp;

	int	timeOffsetID;
	int	ndims, dims[3],
		TimeDim,
		LowRateDim, HighRateDim, Dim5Hz, Dim10Hz, Dim50Hz, Dim250Hz,
		Vector16Dim, Vector32Dim, Vector64Dim, Dim1000Hz, Dim2Hz,
		AsyncDim;
	int	j, indx;
	char	*p;


	OutputFile = nccreate(file_name, NC_CLOBBER);
	chmod(file_name, 0666);


	/* Dimensions.
	 */
	TimeDim		= ncdimdef(OutputFile, "Time", NC_UNLIMITED);

	LowRateDim	= ncdimdef(OutputFile, "sps1", 1);
	Dim2Hz		= ncdimdef(OutputFile, "sps2", 2);
	Dim5Hz		= ncdimdef(OutputFile, "sps5", 5);
	Dim10Hz		= ncdimdef(OutputFile, "sps10", 10);
	HighRateDim	= ncdimdef(OutputFile, "sps25", 25);
	Dim50Hz		= ncdimdef(OutputFile, "sps50", 50);
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
	ncattput(OutputFile, NC_GLOBAL, "DateProcessed", NC_CHAR, strlen(buffer)+1, (void *)buffer);
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
		if ((vp = Variable[i])->Output == FALSE)
			continue;

		ndims = 2;

		switch (vp->OutputRate)
			{
			case 1:
				dims[1] = LowRateDim;
				ndims = 1;
				break;

			case 2:
				dims[1] = Dim2Hz;
				break;

			case 5:
				dims[1] = Dim5Hz;
				break;

			case 10:
				dims[1] = Dim10Hz;
				break;

			case 25:
				dims[1] = HighRateDim;
				break;

			case 50:
				dims[1] = Dim50Hz;
				break;

			case 250:
				dims[1] = Dim250Hz;
				break;

			case 1000:
				dims[1] = Dim1000Hz;
				break;

			default:
				sprintf(buffer, "Variable %s has unsupported output rate of %d, setting to 1.\n", vp->name, vp->OutputRate);
				LogMessage(buffer);
				vp->OutputRate = 1;
				ndims = 1;
			}


		if (vp->VectorLength > 1)
			{
			ndims = 3;

			if (vp->VectorLength <= 16)
				dims[2] = Vector16Dim;
			else
			if (vp->VectorLength <= 32)
				dims[2] = Vector32Dim;
			else
				dims[2] = Vector64Dim;
			}


		vp->outVarID = ncvardef(OutputFile, vp->name, NC_FLOAT, ndims, dims);


		if (ncvarid(InputFile, vp->name) != ERR)
			{
			ncattcopy(InputFile, vp->inVarID, "units", OutputFile, vp->outVarID);
			ncattcopy(InputFile, vp->inVarID, "long_name", OutputFile, vp->outVarID);
			ncattcopy(InputFile, vp->inVarID, "SampledRate", OutputFile, vp->outVarID);
			ncattcopy(InputFile, vp->inVarID, "OutputRate", OutputFile, vp->outVarID);
			ncattcopy(InputFile, vp->inVarID, "MissingValue", OutputFile, vp->outVarID);
			ncattcopy(InputFile, vp->inVarID, "VectorLength", OutputFile, vp->outVarID);
			ncattcopy(InputFile, vp->inVarID, "Dependencies", OutputFile, vp->outVarID);
			ncattcopy(InputFile, vp->inVarID, "CalibrationCoefficients", OutputFile, vp->outVarID);
			}
		else
			{
			ncattput(OutputFile, vp->outVarID, "units", NC_CHAR, strlen(vp->units)+1, vp->units);
			ncattput(OutputFile, vp->outVarID, "long_name", NC_CHAR, strlen(vp->title)+1, vp->title);
			ncattput(OutputFile, vp->outVarID, "OutputRate", NC_LONG, 1, &vp->OutputRate);
			ncattput(OutputFile, vp->outVarID, "VectorLength", NC_LONG, 1, &vp->VectorLength);
			}

		}


	ncendef(OutputFile);

}	/* END CREATENETCDF */

/* -------------------------------------------------------------------- */
void CloseNetCDF()
{
	int		len;

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
#define nBYTES	500000

void PassThroughData(nRecs)
long	nRecs;
{
	int		i, j;
	int		nDims, dimID[3], bytesPerSec, nSec, nPasses, itoid, otoid;
	long	inStart[3], outStart[3], count[3];
	VARTBL	*vp;
	NR_TYPE	*data;

	if (nRecs == 0)
		nRecs = nRecords;

	data = (float *)GetMemory(nBYTES);


	/* Pass through time_offset
	 */
	inStart[0] = CurrentInputRecordNumber;
	inStart[1] = inStart[2] = 0;
	outStart[0] = outStart[1] = outStart[2] = 0;
	count[1] = count[2] = 0;

	count[0] = nRecs;
	bytesPerSec = NR_SIZE;
	nSec = nBYTES / bytesPerSec;
	nPasses = count[0] / nSec;

	if (count[0] % nSec != 0)
		++nPasses;

	itoid = ncvarid(InputFile, "time_offset");
	otoid = ncvarid(OutputFile, "time_offset");

	FlushXEvents();

	if (nPasses <= 1)
		{
		ncvarget(InputFile, itoid, inStart, count, (void *)data);
		ncvarput(OutputFile, otoid, outStart, count, (void *)data);
		}
	else
		{
		count[0] = nSec;

		for (j = 0; j < nPasses; )
			{
			if (nSec * (j+1) > nRecs)
				count[0] = nRecs % nSec;

			ncvarget(InputFile, itoid, inStart, count, (void *)data);
			ncvarput(OutputFile, otoid, outStart, count, (void *)data);

			inStart[0] += nSec;
			outStart[0] += nSec;
			}
		}



	/* Pass through User requested variables
	 */
	for (i = 0; i < nVariables; ++i)
		{
		if ((vp = Variable[i])->Output == FALSE || vp->Compute)
			continue;

		XmTextFieldSetString(timeDisplayText, vp->name);

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
		nPasses = count[0] / nSec;

		if (count[0] % nSec != 0)
			++nPasses;

		FlushXEvents();

		if (nPasses <= 1)
			{
			ncvarget(InputFile, vp->inVarID, inStart, count, (void *)data);
			ncvarput(OutputFile, vp->outVarID, outStart, count, (void *)data);
			}
		else
			{
			count[0] = nSec;

			for (j = 0; j < nPasses; ++j)
				{
				if (nSec * (j+1) > nRecs)
					count[0] = nRecs % nSec;

				ncvarget(InputFile, vp->inVarID, inStart, count, (void *)data);
				ncvarput(OutputFile, vp->outVarID,outStart,count,(void *)data);

				inStart[0] += nSec;
				outStart[0] += nSec;
				}
			}
		}


	free((char *)data);

}	/* END PASSTHROUGHDATA */

/* END NETCDF.C */
