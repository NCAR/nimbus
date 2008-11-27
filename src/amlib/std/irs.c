/*
-------------------------------------------------------------------------
OBJECT NAME:	irs.c

FULL NAME:	Compute derived winds variables for Honeywell Intertial

ENTRY POINTS:	swp3(), shi3()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		Variables from the decoded Honeywell IRS block.

OUTPUT:		WP3, HI3

REFERENCES:	none

REFERENCED BY:	compute.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

/* Honeywell Manual Time constants.  126 second time constants */
static NR_TYPE	C[3] = { 0.15, 0.0075, 0.000125 };

static NR_TYPE	hi3[nFeedBackTypes] = { 0.0, 0.0 };


/* -------------------------------------------------------------------- */
void initWP3(var_base *varp)
{
  NR_TYPE * tmp;

  if ((tmp = GetDefaultsValue("WP3_TIME_CONSTS", varp->name)) != NULL)
  {
    C[0] = tmp[0];
    C[1] = tmp[1];
    C[2] = tmp[2];
  }
}

/* -------------------------------------------------------------------- */
void swp3(DERTBL *varp)
{
  NR_TYPE	lat, vew, vns, palt, acins, WP3;
  NR_TYPE	gfact, vcorac, acz;

  static bool		firstTime[nFeedBackTypes] = { TRUE, TRUE };
  static NR_TYPE	hx[nFeedBackTypes], hxx[nFeedBackTypes],
			wp3[nFeedBackTypes] = { 0.0, 0.0 },
			deltaT[nFeedBackTypes];

  lat	= GetSample(varp, 0) * DEG_RAD;
  vew	= GetSample(varp, 1);
  vns	= GetSample(varp, 2);
  palt	= GetSample(varp, 3);
  acins	= GetSample(varp, 4);

  if (isnan(lat) || isnan(palt) || isnan(acins) || isnan(vew))
    {
    return;
    }

  if (firstTime[FeedBack])
    {
    if (FeedBack == LOW_RATE_FEEDBACK)
      deltaT[FeedBack] = 1.0;
    else
      deltaT[FeedBack] = 1.0 / (float)cfg.ProcessingRate();

    hx[FeedBack]	= hxx[FeedBack] = wp3[FeedBack] = 0.0;
    hi3[FeedBack]	= palt;

    firstTime[FeedBack] = FALSE;
    }


  WP3 = wp3[FeedBack];

  /* Computation of vertical component of coriolis acceleration
   * modified this line to test GENPRO / NIMBUS difference
   * see ajs 10/2/96
   */
  vcorac = 2.0 * OMEGAE * vew * (NR_TYPE)cos((double)lat) +
			(vew * vew + vns * vns) / EARTH_RADIUS; 


  /* Computation of local gravity estimate, ACZ
   */
  gfact = 9.780356 * (1.0 - 0.31391116e-6 * palt) *
		(1.0 + 0.0052885 * pow(sin((double)lat), (double)2.0));

  acz		= acins + (GRAVITY - gfact) + vcorac;
  wp3[FeedBack]	= wp3[FeedBack] +
		(acz - C[1] * hx[FeedBack] - C[2] * hxx[FeedBack]) *
		deltaT[FeedBack];

  WP3 += wp3[FeedBack];

  /* 3rd order vertical velocity damping using PALT as reference
   */
  hi3[FeedBack]	= hi3[FeedBack] + (wp3[FeedBack] - C[0] * hx[FeedBack])
			* deltaT[FeedBack];
  hx[FeedBack]	= hi3[FeedBack] - palt;
  hxx[FeedBack]	= hxx[FeedBack] + hx[FeedBack] * deltaT[FeedBack];

  /* We use an average of the previous computation and this one to
   * center the value in the time period.
   */
  PutSample(varp, WP3 / 2.0);	
  
}	/* END SWP3 */

/* -------------------------------------------------------------------- */
void shi3(DERTBL *varp)
{
  PutSample(varp, hi3[FeedBack]);
}

/* END IRS.C */
