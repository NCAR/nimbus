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

static std::vector<float> ak_coeff, ss_coeff, tas_coeff;
static std::vector<float> akc_coeff;

NR_TYPE compute_tas(NR_TYPE, NR_TYPE);


/* -------------------------------------------------------------------- */
void GP_AKinit(var_base *varp)
{
  float *tmp;

  ak_coeff.clear();
  ak_coeff.push_back(-0.0478);
  ak_coeff.push_back(0.1299);
  ak_coeff.push_back(0.0);
  ak_coeff.push_back(0.0);

  // sakc_gp(); This is used to adjust AK_GP to AKRD.  Used when you want to replace
  // aircraft attack (AKRD) with this attack.  e.g. Radome freeze-up.
  akc_coeff.clear();
  akc_coeff.push_back(3.73);
  akc_coeff.push_back(1.1);

  // tas coefficients from the WindUncertainty document, Al Cooper.  Feb/2015
  tas_coeff.clear();
  tas_coeff.push_back(1.2251);
  tas_coeff.push_back(28.3047);
  tas_coeff.push_back(-30.2417);
  tas_coeff.push_back(14.1312);
  tas_coeff.push_back(-0.581);
  tas_coeff.push_back(-7.8344);
  tas_coeff.push_back(0.1492);
  tas_coeff.push_back(1.2385);

  if ((tmp = GetDefaultsValue("GP_AK_COEFF", varp->name)) != NULL)
  {
    ak_coeff.clear();
    ak_coeff.push_back(tmp[0]);
    ak_coeff.push_back(tmp[1]);
    ak_coeff.push_back(tmp[2]);
    ak_coeff.push_back(tmp[3]);
    sprintf(buffer,"AK cals set to %f, %f, %f,%f in AMLIB function initGust.\n", ak_coeff[0], ak_coeff[1], ak_coeff[2], ak_coeff[3]);
    LogMessage(buffer);
  }
  else
    AddToDefaults(varp->name, "CalibrationCoefficients", ak_coeff);
}

void GP_SSinit(var_base *varp)
{
  float *tmp;

  ss_coeff.clear();
  ss_coeff.push_back(-0.1564);
  ss_coeff.push_back(0.09218);
  ss_coeff.push_back(0.0);
  ss_coeff.push_back(0.0);

  if ((tmp = GetDefaultsValue("GP_SS_COEFF", varp->name)) != NULL)
  {
    ss_coeff.clear();
    ss_coeff.push_back(tmp[0]);
    ss_coeff.push_back(tmp[1]);
    sprintf(buffer,"SS cals set to %f,%f in AMLIB function initGust.\n", ss_coeff[0], ss_coeff[1]);
    LogMessage(buffer);
  }
  else
    AddToDefaults(varp->name, "CalibrationCoefficients", ss_coeff);
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

  if (qc < 0.01)
    qc = 0.01;

  ratio	= adif / qc;
  QR	= qc / ps;
  mach	= sqrt(XMAC2(QR));
  QoverP= tas_coeff[0] + tas_coeff[1]*QR + tas_coeff[2]*mach*QR + tas_coeff[3]*QR*QR
	+ tas_coeff[4]*ratio + tas_coeff[5]*mach
	+ tas_coeff[6]*ratio*ratio + tas_coeff[7]*ratio*mach;

  mach	= sqrt(XMAC2(QoverP));
  tas	= compute_tas(at, mach);

  PutSample(varp, tas);
}

/* -------------------------------------------------------------------- */
void sakc_gp(DERTBL *varp)	// Gustpod Attack Corrected to aircraft attack
{
  NR_TYPE ak_gp = GetSample(varp, 0);

  PutSample(varp, akc_coeff[0] + akc_coeff[1] * ak_gp);
}

/* END GUSTPOD.C */
