/*
-------------------------------------------------------------------------
OBJECT NAME:	lrloop.c

FULL NAME:	Main loop for low rate processing

ENTRY POINTS:	LowRateLoop()
		FindFirstLogicalRecord()
		FindNextLogicalRecord()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		long beginning and ending times

OUTPUT:		

REFERENCES:	average.c, compute.c, netcdf.c, timeseg.c

REFERENCED BY:	StartProcessing()

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include "define.h"

static long	timeVarID[3];
static int	currentTime[3];

extern XtAppContext context;


/* -------------------------------------------------------------------- */
LowRateLoop(starttime, endtime)
long	starttime;
long	endtime;
{
	int		rc;


	if ((rc = FindFirstRecordNumber(starttime)) == ERR)
		goto exit;

	PassThroughData(endtime - starttime);
	SetBaseTime();		/* See netcdf.c	*/


	/* This is the main control loop.
	 */
	do
		{
		if (CheckForTimeGap(currentTime) == TRUE)
			break;

		ComputeDerived();
		UpdateTime(currentTime);

		while (PauseFlag == TRUE)
			XtAppProcessEvent(context, XtIMAll);

		if (PauseWhatToDo == P_QUIT)
			{
			rc = ERR;
			break;
			}
		}
	while ((rc = FindNextRecordNumber(endtime)) == OK);


exit:
	return(rc);

}	/* END LOWRATELOOP */

/* -------------------------------------------------------------------- */
FindFirstRecordNumber(starttime)
long	starttime;
{
	timeVarID[0] =
		Variable[SearchTable(Variable, nVariables, "HOUR")]->inVarID;
	timeVarID[1] =
		Variable[SearchTable(Variable, nVariables, "MINUTE")]->inVarID;
	timeVarID[2] =
		Variable[SearchTable(Variable, nVariables, "SECOND")]->inVarID;

	CurrentInputRecordNumber = -1;
	CurrentOutputRecordNumber = 0;

	if (starttime != BEG_OF_TAPE)
		while (FindNextRecordNumber(starttime) == OK)
			;
	else
		FindNextRecordNumber(starttime);

	return(OK);

}	/* END FINDFIRSTRECORDNUMBER */

/* -------------------------------------------------------------------- */
FindNextRecordNumber(endtime)
long	endtime;
{
	int		current_time;
	long	mindex[1];
	NR_TYPE	f;

	static bool	rollOver = FALSE;

	if (++CurrentInputRecordNumber >= nRecords)	/* End of tape	*/
		return(ERR);

	mindex[0] = CurrentInputRecordNumber;

	ncvarget1(InputFile, timeVarID[0], mindex, (void *)&f);
	current_time = (int)f * 3600;
	currentTime[0] = (int)f;

	ncvarget1(InputFile, timeVarID[1], mindex, (void *)&f);
	current_time += (int)f * 60;
	currentTime[1] = (int)f;

	ncvarget1(InputFile, timeVarID[2], mindex, (void *)&f);
	current_time += (int)f;
	currentTime[2] = (int)f;

	if (endtime == END_OF_TAPE)
		return(OK);


	if (current_time == 0 && CurrentInputRecordNumber > 0)
		rollOver = TRUE;

	if (rollOver)
		current_time += 86400;

	if (current_time < endtime)
		return(OK);
	else
		return(ERR);

}	/* END FINDNEXTRECORDNUMBER */

/* END LRLOOP.C */
