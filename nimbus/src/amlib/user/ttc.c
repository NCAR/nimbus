/*
-------------------------------------------------------------------------
OBJECT NAME:	ttc.c

FULL NAME:	Corrected Total Temperature

ENTRY POINTS:	satc()

DESCRIPTION:	Create a corrected set of Reference variables by combining
		the ADC TT and Rosemount TT data.

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2006
-------------------------------------------------------------------------
*/

#include <values.h>
#include "nimbus.h"
#include "amlib.h"

static NR_TYPE
	UPFCTR	= 0.999444,
	TAU	= 30.0;

static const int NCF = 3;

static NR_TYPE	vnsc[nFeedBackTypes];

static NR_TYPE	DELT[nFeedBackTypes],
		dvy[nFeedBackTypes], dvx[nFeedBackTypes],
		time_duration[nFeedBackTypes];

static double	h[NCF][NCF], zf[nFeedBackTypes][4][6];
static double	am[2][NCF], bm[2][NCF];

static NR_TYPE	filter(double, double *);

static bool returnMissingValue = false;

/* -------------------------------------------------------------------- */
void initTTC(var_base *varp)
{
  NR_TYPE  *tmp;

  if ((tmp = GetDefaultsValue("TT_TAU", varp->name)) == NULL)
  {
    sprintf(buffer, "TT_TAU set to %f in AMLIB function satc.\n", TAU);
    LogMessage(buffer);
  }
  else
    TAU = tmp[0];

  memset((char *)zf, 0, sizeof(zf));
  memset((char *)am, 0, sizeof(am));
  memset((char *)bm, 0, sizeof(bm));
  memset((char *)h, 0, sizeof(h));

  for (int i = 0; i < nFeedBackTypes; ++i)
    {
    if (i == LOW_RATE_FEEDBACK)
      DELT[i] = 1.0;
    else
      DELT[i] = 1.0 / (float)cfg.ProcessingRate();

    time_duration[i] = dvy[i] = dvx[i] = 0.0;
    }

}	/* END INITLATC */

/* -------------------------------------------------------------------- */
void sttc(DERTBL *varp)
{
  NR_TYPE	vns, gvns;
  NR_TYPE	omegat, sinwt, coswt, gvnsf, gvewf, vnsf, vewf;

  vns	= GetSample(varp, 0);	// IRS NS ground speed
  gvns	= GetSample(varp, 1);	// GPS NS ground speed

  if ( isnan(vns) || isnan(gvns) )
    {
    returnMissingValue = true;
    PutSample(varp, floatNAN);
    return;
    }
  else
    returnMissingValue = false;

  time_duration[FeedBack] += DELT[FeedBack];

    {
    /* Good GPS comes here.
     */
    gvnsf	= filter((double)gvns, zf[FeedBack][0]);
    vnsf	= filter((double)vns, zf[FeedBack][2]);

    dvy[FeedBack]	= gvnsf - vnsf;

    if (FeedBack == LOW_RATE_FEEDBACK) /* Only do this in the Low-rate pass */
      {
      omegat = 2.0 * M_PI * time_duration[FeedBack] / 5040.0;
      sinwt = sin(omegat);
      coswt = cos(omegat);

      am[0][0] = UPFCTR * am[0][0] + dvy[FeedBack];
      am[0][1] = UPFCTR * am[0][1] + dvy[FeedBack] * sinwt;
      am[0][2] = UPFCTR * am[0][2] + dvy[FeedBack] * coswt;
      am[1][0] = UPFCTR * am[1][0] + dvx[FeedBack];
      am[1][1] = UPFCTR * am[1][1] + dvx[FeedBack] * sinwt;
      am[1][2] = UPFCTR * am[1][2] + dvx[FeedBack] * coswt;

      /* Note: only one information matrix needed: independant of dx,dy
       */
      h[0][0] = UPFCTR * h[0][0] + 1.0;
      h[1][0] = UPFCTR * h[1][0] + sinwt;
      h[2][0] = UPFCTR * h[2][0] + coswt;
      h[1][1] = UPFCTR * h[1][1] + sinwt * sinwt;
      h[2][1] = UPFCTR * h[2][1] + sinwt * coswt;
      h[2][2] = UPFCTR * h[2][2] + coswt * coswt;
      }
    }

  vnsc[FeedBack] = vns + dvy[FeedBack];

  if (returnMissingValue)
    PutSample(varp, floatNAN);
  else
    PutSample(varp, vnsc[FeedBack]);
}

/* -------------------------------------------------------------------- */
/* Version 2 (A Cooper, 5/28/92, coded by CJW 7/30/93)
 * This function returns the low-pass-filtered value of the time series
 * 'x', if the function is called sequentially for each element in x.
 * The filter is a three-pole Butterworth lowpass filter with
 * cutoff frequency (i.e., attentuation by 0.707) at T/tau where T is
 * the sampling frequency and tau is the data constant below.  (For
 * a sequence sampled once per second and tau=600, the cutoff frequency
 * is at 1/600 = (10 min)**{-1}.)  The algorithm used is described
 * in Bosic, p. 49.
 * The array zf is used to save values for the next call, to make it
 * easy to filter more than one variable.
 */
static NR_TYPE filter(double x, double zf[])
{
  static double	a[nFeedBackTypes], a2[nFeedBackTypes],
		a3[nFeedBackTypes], a4[nFeedBackTypes];
  static bool	firstTime[nFeedBackTypes] = { true, true };

  if (firstTime[FeedBack])
  {
    double	b1, b2, c, c2, e, f;

    if (FeedBack == HIGH_RATE_FEEDBACK)
      TAU *= (float)cfg.ProcessingRate();

    a[FeedBack]	= 2.0 * M_PI / TAU;
    b1		= sqrt(3.0) / 2.0;
    b2		= sqrt(1.0 / 3.0);
    c		= exp(-0.5 * a[FeedBack]);
    c2		= c * 2.0;
    e		= a[FeedBack] * b1;
    f		= c * (cos(e) + b2 * sin(e));
    a2[FeedBack]= a[FeedBack] * f;
    a3[FeedBack]= 2.0 * exp(-a[FeedBack] / 2.0) * cos(e);
    a4[FeedBack]= exp(-a[FeedBack]);

    firstTime[FeedBack] = false;
  }

  zf[2] = -a[FeedBack] * x + a2[FeedBack] * zf[5] + a3[FeedBack] * zf[3] - a4[FeedBack] * zf[4];
  zf[1] = a[FeedBack] * x + a4[FeedBack] * zf[1];
  zf[4] = zf[3];
  zf[3] = zf[2];
  zf[5] = x;

  return(zf[1] + zf[2]);

}	/* END FILTER */
