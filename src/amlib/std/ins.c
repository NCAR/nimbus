/*
-------------------------------------------------------------------------
OBJECT NAME:	ins.c

FULL NAME:	Verticle Velocity for Litton 51.

ENTRY POINTS:	swp3L51(), shi3L51(), svewL51(), svnsL51(), sacinsL51()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		

OUTPUT:		WP3, HI3, VEW, VNS, ACINS

REFERENCES:	none

REFERENCED BY:	compute.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"
#include "gust.h"

#define WH_GRAVITY	9.7959	/* Gravity in Woodland Hills	*/

/* 60 Second time constants.
 */
#define C1	0.31415927
#define C2	0.03289866
#define C3	0.00114838

static NR_TYPE	vew, vns, acins, wp3[nFeedBackTypes], hi3[nFeedBackTypes];

/* -------------------------------------------------------------------- */
void swp3L51(DERTBL *varp)
{
	NR_TYPE	palt, xvi, yvi, gfact, lat, alpha, vcorac, acz, vzi;

	static bool	firstTime[nFeedBackTypes] = { TRUE, TRUE };
	static NR_TYPE	hx[nFeedBackTypes], hxx[nFeedBackTypes],
			vzi0[nFeedBackTypes],
			DELT[nFeedBackTypes];

	lat	= GetSample(varp, 0) * DEG_RAD;
	xvi	= GetSample(varp, 1);
	yvi	= GetSample(varp, 2);
	palt	= GetSample(varp, 3);
	alpha	= GetSample(varp, 4);
	vzi	= GetSample(varp, 5);

	if (firstTime[FeedBack])
		{
		if (FeedBack == LOW_RATE_FEEDBACK)
			DELT[FeedBack]	= 1.0;
		else
			DELT[FeedBack]	= 1.0 / (float)cfg.ProcessingRate();

		hx[FeedBack]	= hxx[FeedBack] = wp3[FeedBack] = 0.0;
		hi3[FeedBack]	= palt;
		vzi0[FeedBack]	= vzi;

		firstTime[FeedBack] = FALSE;
		}


	/* Wander angle rotation of INS platform coordinates
	 */
	{
	double	wai, cw, sw;

	wai	= -alpha * DEG_RAD;
	cw	= cos(wai);
	sw	= sin(wai);

	vns	=  xvi * cw - yvi * sw;
	vew	= -xvi * sw - yvi * cw;
	}


	/* Computation of vertical component of coriolis acceleration
	 */
	vcorac = 2.0 * OMEGAE * vew * (NR_TYPE)cos((double)lat) +
			(vew * vew + vns * vns) / EARTH_RADIUS;


	/* Computation of local gravity estimate, ACZ
	 */
	gfact = 9.780356 * (1.0 - 0.31391116e-6 * palt) *
		(1.0 + 0.0052885 * (NR_TYPE)pow(sin((double)lat), 2.0));

	acins	= (vzi - vzi0[FeedBack]) / DELT[FeedBack];
	acz	= acins + (WH_GRAVITY - gfact) + vcorac;
	vzi0[FeedBack]	= vzi;


	/* 3rd order vertical velocity damping using PALT as reference
	 */
	wp3[FeedBack]	= wp3[FeedBack] +
			(acz - C2 * hx[FeedBack] - C3 * hxx[FeedBack]) *
			DELT[FeedBack];

	hi3[FeedBack]	= hi3[FeedBack] + (wp3[FeedBack] - C1 * hx[FeedBack]) *
				DELT[FeedBack];
	hx[FeedBack]	= hi3[FeedBack] - palt;
	hxx[FeedBack]	= hxx[FeedBack] + hx[FeedBack] * DELT[FeedBack];


	PutSample(varp, wp3[FeedBack]);

}	/* END SWP3L51 */

/* -------------------------------------------------------------------- */
void shi3L51(DERTBL *varp)
{
	PutSample(varp, hi3[FeedBack]);
}

/* -------------------------------------------------------------------- */
void svewL51(DERTBL *varp)
{
	PutSample(varp, vew);
}

/* -------------------------------------------------------------------- */
void svnsL51(DERTBL *varp)
{
	PutSample(varp, vns);
}

/* -------------------------------------------------------------------- */
void sacinsL51(DERTBL *varp)
{
	PutSample(varp, acins);
}

/* END INS.C */
