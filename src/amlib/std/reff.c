/*
-------------------------------------------------------------------------
OBJECT NAME:	reff.c

FULL NAME:	Effective Radius

ENTRY POINTS:	srefff6()	Combined F100 & 260X
		srefff2()	Combined F100 & 1DC

STATIC FNS:	

DESCRIPTION:	Template for AMLIB std & user directories.

COPYRIGHT:	University Corporation for Atmospheric Research, 2003
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"
#include <raf/pms.h>

extern NR_TYPE	refff3[], refff2[],	/* FSSP */
		reffd3[], reffd2[],	/* CDP */
		reff63[], reff62[],	/* 260X */
		reff23[], reff22[];	/* 1DC	*/

/* -------------------------------------------------------------------- */
void srefff6(DERTBL *varp)
{
  NR_TYPE	reff_num, reff_denom, reff;

  reff_num = reff_denom = 0.0;

  reff_num += refff3[0];
  reff_denom += refff2[0];

  reff_num += reff63[0] / 1000.0;
  reff_denom += reff62[0] / 1000.0;

  if (reff_denom > 0.0)
    reff = 0.5 * (reff_num / reff_denom);
  else
    reff = 0.0;

  PutSample(varp, reff);
}

/* -------------------------------------------------------------------- */
void srefff2(DERTBL *varp)
{
  NR_TYPE	reff_num, reff_denom, reff;

  reff_num = reff_denom = 0.0;

  if (!isnan(refff3[0])) reff_num += refff3[0];
  if (!isnan(refff2[0])) reff_denom += refff2[0];

  if (!isnan(reff23[0])) reff_num += reff23[0] / 1000.0;
  if (!isnan(reff22[0])) reff_denom += reff22[0] / 1000.0;

  if (reff_denom > 0.0)
    reff = 0.5 * (reff_num / reff_denom);
  else
    reff = 0.0;

  PutSample(varp, reff);
}

/* -------------------------------------------------------------------- */
void sreffd2(DERTBL *varp)
{
  NR_TYPE	reff_num, reff_denom, reff;

  reff_num = reff_denom = 0.0;

  if (!isnan(reffd3[0])) reff_num += reffd3[0];
  if (!isnan(reffd2[0])) reff_denom += reffd2[0];

  if (!isnan(reff23[0])) reff_num += reff23[0] / 1000.0;
  if (!isnan(reff22[0])) reff_denom += reff22[0] / 1000.0;

  if (reff_denom > 0.0)
    reff = 0.5 * (reff_num / reff_denom);
  else
    reff = 0.0;

  PutSample(varp, reff);
}

/* END REFF.C */
