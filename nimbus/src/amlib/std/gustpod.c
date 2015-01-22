/*
-------------------------------------------------------------------------
OBJECT NAME:	gustpod.c

FULL NAME:	Gust Pod calculations

DESCRIPTION:	Derived GustPod calcs.
		PSC & QCC, not used at this time.
		AOA, SSLIP, and TAS.

COPYRIGHT:	University Corporation for Atmospheric Research, 2010-2015
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static size_t timeCount = 0;
static NR_TYPE PSTcor, PSQcor;

static NR_TYPE ak_coeff[] = { -0.0478, 0.1299 };

// ADELE, PREDICT, & DEEPWAVE #'s - left wing.
static NR_TYPE ss_coeff[] = { -0.1564, 0.09218 };

// DC3 - right wing - Change in the project/Defaults file.
//static NR_TYPE ss_coeff[] = { 0.1662, 0.09218 };

static NR_TYPE tas_coeff[] = { -0.4732, -11.8787, 15.6659, -6.7811, 0.0542, 3.4354 };

NR_TYPE compute_tas(NR_TYPE, NR_TYPE);


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
  NR_TYPE adif = GetSample(varp, 0);
  NR_TYPE qc = GetSample(varp, 1);
  NR_TYPE ps = GetSample(varp, 2);
  
  NR_TYPE ratio = adif / qc;
  NR_TYPE QoverP = qc / ps;
  NR_TYPE mach = sqrt(XMAC2(QoverP));
  PutSample(varp, ak_coeff[0] + ratio * (ak_coeff[1]+ak_coeff[2]*mach) + ak_coeff[3]*QoverP);
}

/* -------------------------------------------------------------------- */
void sss_gp(DERTBL *varp)	// Sideslip
{
  NR_TYPE ratio;
  NR_TYPE bdif = GetSample(varp, 0);
  NR_TYPE qc = GetSample(varp, 1);

  ratio = bdif / qc;
  PutSample(varp, ss_coeff[0] + ss_coeff[1] * ratio);
}

/* -------------------------------------------------------------------- */
void stas_gp(DERTBL *varp)	// True airspeed
{
  NR_TYPE qc	= GetSample(varp, 0);
  NR_TYPE ps	= GetSample(varp, 1);
  NR_TYPE at	= GetSample(varp, 2);
  NR_TYPE adif	= GetSample(varp, 3);

  NR_TYPE ratio, QR, mach, QoverP, tas;

  ratio	= adif / qc;
  QR	= qc / ps;
  mach	= sqrt(XMAC2(QR));
  QoverP= tas_coeff[0] + tas_coeff[1]*QR + tas_coeff[2]*mach*QR + tas_coeff[3]*QR*QR
	+ tas_coeff[4]*ratio + tas_coeff[5]*mach;

  mach	= sqrt(XMAC2(QoverP));
  tas	= compute_tas(at, mach);

  PutSample(varp, tas);
}

/* END GUSTPOD.C */
