/*
-------------------------------------------------------------------------
OBJECT NAME:	gust.c

FULL NAME:	Gust calculations

ENTRY POINTS:	swi(), sui(), svi(), sux(), svy()

STATIC FNS:	

DESCRIPTION:	Template for AMLIB std & user directories.

INPUT:		

OUTPUT:		WI, UI, VI, UX, VY

REFERENCES:	

REFERENCED BY:	compute.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"
#include "gust.h"

static const int MAX_PROBES = 4;

static const NR_TYPE	THDG_TEST	= 180.0 * M_PI / 180.0;
static const NR_TYPE	PITCH_TEST	= 22.5 * M_PI / 180.0;

static NR_TYPE	ui[MAX_PROBES], vi[MAX_PROBES], ux[MAX_PROBES], vy[MAX_PROBES];

NR_TYPE GetBoomLength();


/* -------------------------------------------------------------------- */
void swi(DERTBL *varp)
{
  int		probeCnt = varp->ProbeCount;
  NR_TYPE	wi, attack, sslip, tas, thdg, vew, vns, pitch, roll, wp3;
  NR_TYPE	tas_dab, delph, thedot, delth,
		e, f, h, ab, p, r, s, t, psidot, bvns, bvew,
		cs, ss, ch, sh, cr, sr, ta, tb;

  static bool		firstTime[nFeedBackTypes] = { TRUE, TRUE };
  static float		boomln;
  static NR_TYPE	pitch0[MAX_PROBES][nFeedBackTypes],
			thdg0[MAX_PROBES][nFeedBackTypes],
			DELT[nFeedBackTypes];

  tas	= GetSample(varp, 0);
  vew	= GetSample(varp, 1);
  vns	= GetSample(varp, 2);
  pitch	= GetSample(varp, 3) * DEG_RAD;
  roll	= GetSample(varp, 4) * DEG_RAD;
  thdg	= GetSample(varp, 5) * DEG_RAD;
  attack= GetSample(varp, 6) * DEG_RAD;
  sslip	= GetSample(varp, 7) * DEG_RAD;
  wp3	= GetSample(varp, 8);

  if (firstTime[FeedBack])
    {
    DELT[FeedBack] = 1.0;

    if (FeedBack != LOW_RATE_FEEDBACK)
      DELT[FeedBack] /= ProcessingRate;

    pitch0[probeCnt][FeedBack]	= pitch;
    thdg0[probeCnt][FeedBack]	= thdg;

    boomln = GetBoomLength();

    firstTime[FeedBack] = FALSE;
    }

  /* Coordinate transformation
   */
  cs	= (NR_TYPE)cos((double)thdg);
  ss	= (NR_TYPE)sin((double)thdg);
  ch	= (NR_TYPE)cos((double)pitch);
  sh	= (NR_TYPE)sin((double)pitch);
  cr	= (NR_TYPE)cos((double)roll);
  sr	= (NR_TYPE)sin((double)roll);
  ta	= (NR_TYPE)tan((double)attack);
  tb	= (NR_TYPE)tan((double)sslip);

  tas_dab	= -tas * 1.0 / (NR_TYPE)sqrt((double)1.0 + ta*ta + tb*tb);
  e	= -tb * (ss * cr - cs * sh * sr);
  f	= cs * sh * cr + ss * sr;
  h	= tb * (cs * cr + ss * sh * sr);
  p	= ss * sh * cr - cs * sr;
  ab	= sh - tb * ch * sr - ta * ch * cr;


  /* Nose boom angular velocity correction terms
   */
  delph	= pitch - pitch0[probeCnt][FeedBack];

  if ((NR_TYPE)fabs((double)delph) >= PITCH_TEST)
    {
    if (delph > 0)
      pitch0[probeCnt][FeedBack] += PITCH_TEST * 2.0;

    if (delph < 0)
      pitch0[probeCnt][FeedBack] -= PITCH_TEST * 2.0;
    }

  thedot = (pitch - pitch0[probeCnt][FeedBack]) / DELT[FeedBack];


  delth	= thdg - thdg0[probeCnt][FeedBack];

  if ((NR_TYPE)fabs((double)delth) >= THDG_TEST)
    {
    if (delth > 0)
      thdg0[probeCnt][FeedBack] += THDG_TEST * 2.0;

    if (delth < 0)
      thdg0[probeCnt][FeedBack] -= THDG_TEST * 2.0;
    }

  psidot = (thdg - thdg0[probeCnt][FeedBack]) / DELT[FeedBack];


  bvns	= boomln * (psidot * ss * ch + thedot * cs * sh);
  bvew	= boomln * (thedot * sh * ss - psidot * cs * ch);

  pitch0[probeCnt][FeedBack]	= pitch;
  thdg0[probeCnt][FeedBack]	= thdg;


  /* Compute wind vector components WRT geographic coordinates
   */
  r  = ss * ch + h + ta * p;
  s  = cs * ch + e + ta * f;
  t  = wp3 + boomln * thedot * ch;

  ui[probeCnt] = tas_dab * r + (vew - bvew);
  vi[probeCnt] = tas_dab * s + (vns - bvns);
  ux[probeCnt] =  ui[probeCnt] * ss + vi[probeCnt] * cs;
  vy[probeCnt] = -ui[probeCnt] * cs + vi[probeCnt] * ss;
  wi = tas_dab * ab + t;

  /* Blow-up protection:  output no winds while on-ground (TAS < 30.0 m/s)
       installed by Ron Ruth  18 October 2001 */
  if (tas < 30.0)
    {
    ui[probeCnt] = 0.0;
    vi[probeCnt] = 0.0;
    ux[probeCnt] = 0.0;
    vy[probeCnt] = 0.0;
    wi = 0.0;
    }

//printf("wi=%g, tas_dab=%g, ab=%g, t=%g, thedot=%g\n", wi, tas_dab, ab, t, thedot);
  PutSample(varp, wi);

}	/* END SWI */

/* -------------------------------------------------------------------- */
void sui(DERTBL *varp)
{
  PutSample(varp, ui[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void svi(DERTBL *varp)
{
  PutSample(varp, vi[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void sux(DERTBL *varp)
{
  PutSample(varp, ux[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void svy(DERTBL *varp)
{
  PutSample(varp, vy[varp->ProbeCount]);
}

/* END GUST.C */
