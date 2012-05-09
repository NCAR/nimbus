/*
-------------------------------------------------------------------------
OBJECT NAME:	gustpod.c

FULL NAME:	Gust Pod calculations

DESCRIPTION:	Derived GustPod calcs.

COPYRIGHT:	University Corporation for Atmospheric Research, 2010-2012
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static size_t timeCount = 0;
static NR_TYPE PSTcor, PSQcor;

static NR_TYPE ak_coeff[] = { -0.0478, 0.1299 };
static NR_TYPE ss_coeff[] = { -0.1564, 0.09218 };	// Left Wing
//static NR_TYPE ss_coeff[] = { 0.1662, 0.09218 };	// Right Wing

static NR_TYPE recfx = 0.985;

NR_TYPE tas(NR_TYPE, NR_TYPE, NR_TYPE);


/* -------------------------------------------------------------------- */
void GPinit(var_base *varp)
{
  float *tmp;

  if ((tmp = GetDefaultsValue("GP_SS_COEFF", varp->name)) == NULL)
  {
    sprintf(buffer,"SS cals set to %f,%f in AMLIB function initGust.\n", ss_coeff[0], ss_coeff[1]);
    LogMessage(buffer);
  }
  else
    for (int i = 0; i < 2; ++i)
      ss_coeff[i] = tmp[i];

  if ((tmp = GetDefaultsValue("GP_RECOVERY", varp->name)) == NULL)
  {
    sprintf(buffer,"Recovery value set to %f in AMLIB function initGust.\n", recfx);
    LogMessage(buffer);
  }
  else
    recfx = tmp[0];

  if ((tmp = GetDefaultsValue("GP_AK_COEFF", varp->name)) == NULL)
  {
    sprintf(buffer,"AK cals set to %f,%f in AMLIB function initGust.\n", ak_coeff[0], ak_coeff[1]);
    LogMessage(buffer);
  }
  else
    for (int i = 0; i < 2; ++i)
      ak_coeff[i] = tmp[i];
}

/* -------------------------------------------------------------------- */
void spsc_gp(DERTBL *varp)	// Static Pressure
{
  NR_TYPE ps = GetSample(varp, 0);
  NR_TYPE qc = GetSample(varp, 1);
  NR_TYPE tas = GetSample(varp, 2);

  if (FeedBack == LOW_RATE_FEEDBACK && !isnan(tas))
    if (tas < 50.0)	// 50 is random, just looking for takeoff.
      timeCount = 0;

  // This first correction is for fuel-burn.  Aircraft angle changes with time.
  PSTcor = 0.000074 * timeCount++;
  PSQcor = -8.8 - 0.1416 * qc;

  PutSample(varp, ps + PSTcor + PSQcor);
}

/* -------------------------------------------------------------------- */
void sqcc_gp(DERTBL *varp)	// Dynamic pressure
{
  NR_TYPE qc = GetSample(varp, 0);

  PSQcor = -9.0 - 0.1764 * qc;

  PutSample(varp, qc - PSTcor - PSQcor);
}

/* -------------------------------------------------------------------- */
void sak_gp(DERTBL *varp)	// Attack
{
  NR_TYPE ratio;
  NR_TYPE adifr = GetSample(varp, 0);
  NR_TYPE qc = GetSample(varp, 1);

  ratio = adifr / qc;
  PutSample(varp, (ratio + ak_coeff[0]) / ak_coeff[1]);
}

/* -------------------------------------------------------------------- */
void sss_gp(DERTBL *varp)	// Sideslip
{
  NR_TYPE ratio;
  NR_TYPE bdifr = GetSample(varp, 0);
  NR_TYPE qc = GetSample(varp, 1);

  ratio = bdifr / qc;
  PutSample(varp, (ratio + ss_coeff[0]) / ss_coeff[1]);
}

/* -------------------------------------------------------------------- */
void stas_gp(DERTBL *varp)	// True airspeed
{
  NR_TYPE	fmach2;
  NR_TYPE	tt, qc, ps, tasg;

  qc	= GetSample(varp, 0);
  ps	= GetSample(varp, 1);
  tt	= GetSample(varp, 2);

  fmach2 = XMAC2(qc / ps);
  tasg	= tas(tt, recfx, fmach2);

  PutSample(varp, tasg);
}

/* END GUSTPOD.C */
