/*
-------------------------------------------------------------------------
OBJECT NAME:	timeseg.c

FULL NAME:	Time Segment Routines

ENTRY POINTS:	GetUserTimeIntervals()		Used by StartProcesing()
		NextTimeInterval()			Used by StartProcesing()
		CheckForTimeGap()			oppo.c & mrf.c
		UpdateTime()				oppo.c & mrf.c
		FormatTimeSegmentsForOutputFile()	netcdf.c

STATIC FNS:	none

DESCRIPTION:	Functions should be used in the order shown above.
		User specified time intervals may not correspond to
		what is actually output (e.g. User requests 00:00:00 -
		15:00:00, and ADS actually started recording at 13:00:00).

INPUT:		struct Hdr_blk

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	cb_main.c, oppo.c, mrf.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include <Xm/TextF.h>

#include "define.h"

#define NEW_SEG		(-1)

static int	nTimeIntervals;

/* User specified time intervals, stored as int's	*/
static long	prev_time,
		UserBtim[MAX_TIME_SLICES*4],
		UserEtim[MAX_TIME_SLICES*4];

/* Actual time intervals in output file.		*/
static int	currentTimeSegment;
static NR_TYPE	BtimeInt[MAX_TIME_SLICES*4][3],
		EtimeInt[MAX_TIME_SLICES*4][3];


/* -------------------------------------------------------------------- */
void GetUserTimeIntervals() /* From TimeSliceWindow	*/
{
	int	i;
	int	hour, minute, second;
	char	*bp, *ep;

	nTimeIntervals = 0;
	currentTimeSegment = (-1);

	for (i = 0; i < MAX_TIME_SLICES; ++i)
		{
		bp = XmTextFieldGetString(ts_text[i]);
		ep = XmTextFieldGetString(ts_text[i+MAX_TIME_SLICES]);

		if (strlen(bp) == 0 && strlen(ep) == 0)
			continue;

		sscanf(bp, "%02d:%02d:%02d", &hour, &minute, &second);
		UserBtim[nTimeIntervals] = (hour * 3600) + (minute * 60) + second;

		sscanf(ep, "%02d:%02d:%02d", &hour, &minute, &second);
		UserEtim[nTimeIntervals] = (hour * 3600) + (minute * 60) + second;

		++nTimeIntervals;
		}

	if (nTimeIntervals == 0)
		{
		UserBtim[0] = BEG_OF_TAPE;
		UserEtim[0] = END_OF_TAPE;

		++nTimeIntervals;
		}

}	/* END GETUSERTIMEINTERVALS */

/* -------------------------------------------------------------------- */
bool NextTimeInterval(long *start, long *end)
{
	if (++currentTimeSegment >= nTimeIntervals)
		return(FALSE);

	prev_time = NEW_SEG;

	*start	= UserBtim[currentTimeSegment];
	*end	= UserEtim[currentTimeSegment];

	BtimeInt[currentTimeSegment][0] = NEW_SEG;

	return(TRUE);

}	/* END NEXTTIMEINTERVAL */

/* -------------------------------------------------------------------- */
bool CheckForTimeGap(int currentTime[])
{
	long	new_time;
	long	i, j;

	new_time = currentTime[0] * 3600 + currentTime[1] * 60 + currentTime[2];


	/* If everthing is peachy, then bail out
	 */
	if (prev_time+1 == new_time || prev_time == NEW_SEG ||
		(new_time == 0 && prev_time == 86399L))		/* Midnight crossover */
		{
		prev_time = new_time;
		return(FALSE);
		}


	/* Locate current time segment
	 */
	for (i = 0; i < nTimeIntervals; ++i)
		if (prev_time > UserBtim[i] && prev_time <= UserEtim[i])
			break;

	if (i == nTimeIntervals)
		{
		LogMessage("Impossible, no time segment currently active.\n");
		return(TRUE);
		}


	if (new_time <= UserEtim[i])
		{
		LogMessage("Break in time sequence, adding new time interval.\n");

		for (j = nTimeIntervals; j >= i; --j)
			{
			UserEtim[j] = UserEtim[j-1];
			UserBtim[j] = UserBtim[j-1];
			}

		UserEtim[i] = prev_time;
		UserBtim[i+1] = new_time;

		++nTimeIntervals;

		NextTimeInterval(&i, &j);	/* Use i & j as dummies	*/

		return(FALSE);
		}


	LogMessage("CheckForTimeGap: you shouldn't receive this message.\n");
	return(TRUE);

}	/* END CHECKFORTIMEGAP */

/* -------------------------------------------------------------------- */
void UpdateTime(int currentTime[])
{
	++CurrentOutputRecordNumber;

	if (BtimeInt[currentTimeSegment][0] == NEW_SEG)
		{
		BtimeInt[currentTimeSegment][0] = currentTime[0];
		BtimeInt[currentTimeSegment][1] = currentTime[1];
		BtimeInt[currentTimeSegment][2] = currentTime[2];
		}

	EtimeInt[currentTimeSegment][0] = currentTime[0];
	EtimeInt[currentTimeSegment][1] = currentTime[1];
	EtimeInt[currentTimeSegment][2] = currentTime[2];

	if (Interactive && currentTime[2])
		{
		sprintf(buffer, "%02d:%02d:00", currentTime[0], currentTime[1]);
		XmTextFieldSetString(timeDisplayText, buffer);

		FlushXEvents();
		}

}	/* END UPDATETIME */

/* -------------------------------------------------------------------- */
void FormatTimeSegmentsForOutputFile(char buff[])
{
	int	i;
	char	temp[32];

	buff[0] = '\0';

	for (i = 0; i < nTimeIntervals; ++i)
		{
		if (i > 0)
			strcat(buff, ", ");

		sprintf(temp, "%02d:%02d:%02d-%02d:%02d:%02d",
			(int)BtimeInt[i][0],
			(int)BtimeInt[i][1],
			(int)BtimeInt[i][2],
			(int)EtimeInt[i][0],
			(int)EtimeInt[i][1],
			(int)EtimeInt[i][2]);

		strcat(buff, temp);
		}

}	/* END FORMATTIMESEGMENTSFOROUTPUTFILE */

/* END TIMESEG.C */
