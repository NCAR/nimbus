/*
-------------------------------------------------------------------------
OBJECT NAME:    atfh.c

FULL NAME:      Ambient Temperature Wrapper

ENTRY POINTS:   satfh()

STATIC FNS:     none

DESCRIPTION:    Ambient Temperature for anti-iced sensors:
			C130	Rosemount 102 heated

COPYRIGHT:      University Corporation for Atmospheric Research, 1992-2008
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

extern NR_TYPE	recfrh;
extern NR_TYPE	tfher1,tfher2;	/* set in initAC.c */

static NR_TYPE atfh[nFeedBackTypes] = { 0.0, 0.0 };

/* -------------------------------------------------------------------- */
void atfhInit(var_base *varp)
{
  std::vector<float> values;
  values.push_back((float)recfrh);
  AddToDefaults(varp->name, "RecoveryFactor", values);

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void tthcInit(var_base *varp)
{
  memset(atfh, 0, sizeof(atfh));
  std::vector<float> values;
  values.push_back((float)tfher2);
  values.push_back((float)tfher1);
  AddToDefaults(varp->name, "Calibrations", values);

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void sttwhc(DERTBL *varp)
{
  NR_TYPE zee, tth, mach, psxc;
  static bool firstTime[nFeedBackTypes] = { true, true };

  tth	= GetSample(varp, 0);
  mach	= GetSample(varp, 1);
  psxc	= GetSample(varp, 2);

  if (isnan(tth) || isnan(mach))
  {
    PutSample(varp, floatNAN);
    return;
  }

  if (firstTime[FeedBack])
  {
    firstTime[FeedBack] = false;
    atfh[FeedBack] = tth;
  }

  if (tth < -Kelvin)
    tth = -Kelvin;

  if (mach <= 0.0 || isnan(mach))
    mach = 0.0001;

  zee = 0.269589 * psxc * mach / (atfh[FeedBack] + Kelvin);

  if (zee < 0.18 || isnan(zee))
    zee = 0.18;

  tth -= (NR_TYPE)pow((double)10.0,
		(double)tfher1 * log10((double)zee) + tfher2);

  if (tth < -Kelvin)
    tth = -Kelvin;

  PutSample(varp, tth);
}

/* -------------------------------------------------------------------- */
void satfh(DERTBL *varp)
{
  NR_TYPE ttfh, mach;

  ttfh = GetSample(varp, 0);
  mach = GetSample(varp, 1);

  atfh[FeedBack] = AMBIENT(ttfh, (NR_TYPE)recfrh, mach*mach);

  PutSample(varp, atfh[FeedBack]);

}	/* END SATFH */

/* END ATFH.C */
