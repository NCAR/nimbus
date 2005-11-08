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

static NR_TYPE atfh[nFeedBackTypes] = { 0.0, 0.0 };

/* -------------------------------------------------------------------- */
void atfhInit(DERTBL *varp)
{
  std::vector<NR_TYPE> values;
  values.push_back(recfrh);
  AddToDefaults(varp->name, "RecoveryFactor", values);

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void tthcInit(DERTBL *varp)
{
  std::vector<NR_TYPE> values;
  values.push_back(tfher2);
  values.push_back(tfher1);
  AddToDefaults(varp->name, "RecoveryFactors", values);

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void sttwhc(DERTBL *varp)
{
  NR_TYPE zee, tth, xmach2, psxc;
  static bool firstTime[nFeedBackTypes] = { true, true };

  tth = GetSample(varp, 0);
  xmach2 = GetSample(varp, 1);
  psxc = GetSample(varp, 2);

  if (firstTime[FeedBack])
  {
    firstTime[FeedBack] = false;
    atfh[FeedBack] = tth;
  }

  if (tth < -273.15)
    tth = -273.15;

  if (xmach2 < 0.0)
  {
    LogStdMsg("atfh.c: xmach2 went negative!\n");
    xmach2 = 0.0;
  }

  zee = 0.269589 * psxc * sqrt((double)xmach2) / (atfh[FeedBack] + 273.16);

  if (zee < 0.1)
    zee = 0.99;

  tth -= (NR_TYPE)pow(	(double)10.0,
			(double)tfher1 * log10((double)zee) + tfher2);

  if (tth < -273.15)
    tth = -273.15;

  PutSample(varp, tth);
}

/* -------------------------------------------------------------------- */
void satfh(DERTBL *varp)
{
  NR_TYPE ttfh, xmach2;

  ttfh = GetSample(varp, 0);
  xmach2 = GetSample(varp, 1);

  atfh[FeedBack] = AMBIENT(ttfh, (NR_TYPE)recfrh, xmach2);

  PutSample(varp, atfh[FeedBack]);

}	/* END SATFH */

/* END ATFH.C */
