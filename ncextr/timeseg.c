/*
-------------------------------------------------------------------------
OBJECT NAME:	timeseg.c

FULL NAME:		Time Segment Routines

ENTRY POINTS:	GetUserTimeIntervals()		Used by StartProcesing()
				NextTimeInterval()			Used by StartProcesing()
				FormatTimeSegmentsForOutputFile()	netcdf.c

STATIC FNS:		none

DESCRIPTION:	Functions should be used in the order shown above.
				User specified time intervals may not correspond to
				what is actually output (e.g. User requests 00:00:00 -
				15:00:00, and ADS actually started recording at 13:00:00).

INPUT:			struct Hdr_blk

OUTPUT:		

REFERENCES:		none

REFERENCED BY:	cb_main.c, oppo.c, mrf.c

COPYRIGHT:		University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include <Xm/TextF.h>

#include "define.h"

#define NEW_SEG		(-1)

static int	nTimeIntervals;

/* Actual time intervals in output file.			*/
static int	currentTimeSegment;
static long	BtimeInt[MAX_TIME_SLICES*4][3],
				EtimeInt[MAX_TIME_SLICES*4][3];


/* -------------------------------------------------------------------- */
void GetUserTimeIntervals() /* From TimeSliceWindow	*/
{
	int		i;
	char	*bp, *ep;

	nTimeIntervals = 0;
	currentTimeSegment = (-1);

	for (i = 0; i < MAX_TIME_SLICES; ++i)
		{
		bp = XmTextFieldGetString(ts_text[i]);
		ep = XmTextFieldGetString(ts_text[i+MAX_TIME_SLICES]);

		if (strlen(bp) == 0 && strlen(ep) == 0)
			continue;

		sscanf(bp, "%02d:%02d:%02d", &BtimeInt[i][0], &BtimeInt[i][1],
									 &BtimeInt[i][2]);

		sscanf(ep, "%02d:%02d:%02d", &EtimeInt[i][0], &EtimeInt[i][1],
									 &EtimeInt[i][2]);

		if (BtimeInt[i][0] > 23)
			BtimeInt[i][0] -= 24;

		if (EtimeInt[i][0] > 23)
			EtimeInt[i][0] -= 24;

		++nTimeIntervals;
		}

	if (nTimeIntervals == 0)
		{
        int     len = strlen(TimeInterval);

		nTimeIntervals = len / 17;

        for (i = 0; i < nTimeIntervals; ++i)
            {
			sscanf(&TimeInterval[i * 19], "%02d:%02d:%02d", &BtimeInt[i][0],
					&BtimeInt[i][1], &BtimeInt[i][2]);

			sscanf(&TimeInterval[i * 19 + 9], "%02d:%02d:%02d", &EtimeInt[i][0],
					&EtimeInt[i][1], &EtimeInt[i][2]);
            }
		}

}	/* END GETUSERTIMEINTERVALS */

/* -------------------------------------------------------------------- */
NextTimeInterval(start, end)
long	**start;
long	**end;
{
	if (++currentTimeSegment >= nTimeIntervals)
		return(FALSE);

	*start	= BtimeInt[currentTimeSegment];
	*end	= EtimeInt[currentTimeSegment];

	return(TRUE);

}	/* END NEXTTIMEINTERVAL */

/* -------------------------------------------------------------------- */
void FormatTimeSegmentsForOutputFile(buff)
char	*buff;
{
	int		i;
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
