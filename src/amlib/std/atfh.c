/*******       AMBIENT TEMPERATURE (FUSELAGE Rosemount) (C)              ATFH
                  REQUIRES --- TTFH, RECFRH, XMACH2, AMBT FCN.
 	Input:
 		ttfh - raw total temperature
 		xmach2 - derived mach number squared
		psxc - derived static pressure
 	Output:
 		atfh - derived ambient temperature (C)
 	Include:
 		recfrh - recovery factor 
 		ambtf - ambient temperature function
*/

#include "nimbus.h"
#include "amlib.h"

extern NR_TYPE	recfrh;
extern NR_TYPE	tfher1,tfher2;	/* set in initAC.c */

/* -------------------------------------------------------------------- */
void atfhInit(DERTBL *varp)
{

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void satfh(DERTBL *varp)
{
	NR_TYPE		ttfh, zee, xmach2, psxc;
	static NR_TYPE	atfh[nFeedBackTypes];
	static bool	firstTime[nFeedBackTypes] = { TRUE, TRUE };

	ttfh	= GetSample(varp, 0);
	xmach2	= GetSample(varp, 1);
	psxc	= GetSample(varp, 2);

	if (firstTime[FeedBack])
		{
		firstTime[FeedBack] = FALSE;
		atfh[FeedBack] = ttfh;
		}

	if (ttfh < -273.15)
		ttfh = -273.15;

	zee = 0.269589 * psxc * sqrt((double)xmach2) / (atfh[FeedBack] +273.16);

	if (zee < 0.1)
		zee = 0.99;

	ttfh -= (NR_TYPE)pow(	(double)10.0,
				(double)tfher1 * log10((double)zee) + tfher2);

	if (ttfh < -273.15)
		ttfh = -273.15;

	atfh[FeedBack] = AMBIENT(ttfh, (NR_TYPE)recfrh, xmach2);


	if (FeedBack == LOW_RATE_FEEDBACK)
		AveragedData[varp->depend_LRindex[0]] = ttfh;
	else
		HighRateData[varp->depend_HRindex[0]+SampleOffset] = ttfh;

	PutSample(varp, atfh[FeedBack]);

}	/* END SATFH */

/* END ATFH.C */
