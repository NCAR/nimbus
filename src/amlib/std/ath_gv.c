/*
-------------------------------------------------------------------------
OBJECT NAME:    ath_gv.c

FULL NAME:      Corrected Total Temperature and Ambient Temperature

ENTRY POINTS:   satfhGV()
		stthcGV()

STATIC FNS:     none

DESCRIPTION:    Ambient Temperature for anti-iced sensors:
			GV	Harco anti-iced sensor

NOTES:		TTHR1 and TTHL1 have different zee computation and recovery
		factors than TTHR2 and TTHL2. Warning: Code hack using strchr()
		to determine which probe.

COPYRIGHT:      University Corporation for Atmospheric Research, 1992-2008
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

#include <gsl/gsl_poly.h>

extern NR_TYPE	recfrhGV[];

static const int MAX_TT = 8;
static int ProbeCount = 0;

static const double tt1_cals[] = { 9.64259925E-1, 3.71190364E-4, -1.29108970E-6,
		2.03150864E-9, -1.49998879E-12, 4.23086720E-16 };

static NR_TYPE atfh[nFeedBackTypes][MAX_TT];

/* -------------------------------------------------------------------- */
void atfhGV_Init(var_base *varp)
{
  int	indx;

  if (ProbeCount > MAX_TT)
  {
    HandleFatalError("\natfh.c: atfhInit: MAX_TT exceeded, get a programmer to fix.  Fatal.");
  }


  if (strncmp(varp->name, "ATH", 3) == 0)	// HARCO only.
  {
    std::vector<float> values;
    if (strchr(varp->name, '1'))
      values.push_back((float)recfrhGV[0]);	// TTH?1
    else
      values.push_back((float)recfrhGV[1]);	// TTH?2

    AddToDefaults(varp->name, "RecoveryFactor", values);
  }

  /* Frequently ProbeCount gets set in hdr_decode.c, but we are doing it here for
   * this instrument.
   */
  if ((indx = SearchTable(raw, ((DERTBL *)varp)->depend[0])) != -1)
  {
    raw[indx]->ProbeCount = ProbeCount;
  }
  else
  if ((indx = SearchTable(derived, ((DERTBL *)varp)->depend[0])) != -1)
  {
    derived[indx]->ProbeCount = ProbeCount;
  }
  else
  {
    sprintf(buffer, "\natfh.c: atfhInit: Can't find %s, shouldn't happen.  Fatal.",
		((DERTBL *)varp)->depend[0]);
    HandleFatalError(buffer);
  }

  varp->ProbeCount = ProbeCount;
  ++ProbeCount;

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void stthcGV(DERTBL *varp)	// HARCO
{
  NR_TYPE zee, tth, xmach2, psxc;
  static bool firstTime[nFeedBackTypes] = { true, true };

  tth = GetSample(varp, 0);
  xmach2 = GetSample(varp, 1);
  psxc = GetSample(varp, 2);

  if (isnan(tth) || isnan(xmach2))
  {
    PutSample(varp, floatNAN);
    return;
  }

  if (firstTime[FeedBack])
  {
    firstTime[FeedBack] = false;
    atfh[FeedBack][varp->ProbeCount] = tth;
  }

  if (tth < -Kelvin)
    tth = -Kelvin;

  if (xmach2 <= 0.0 || isnan(xmach2))
    xmach2 = 0.0001;

  if (strchr(varp->name, '1'))
//    zee = (1.007093  + psxc * (-0.00001809 + 0.00000001 * psxc));	// TTH?1
    zee = gsl_poly_eval(tt1_cals, 6, psxc);
  else
    zee = (1.0071235 + psxc * (-0.00001574 + 0.00000001 * psxc));	// TTH?2

  tth = ((tth + Kelvin) * zee) - Kelvin; 

  if (tth < -Kelvin)
    tth = -Kelvin;

  PutSample(varp, tth);
}

/* -------------------------------------------------------------------- */
void satfhGVharco(DERTBL *varp)
{
  NR_TYPE ttfh, xmach2, recovery, atfh;

  ttfh = GetSample(varp, 0);
  xmach2 = GetSample(varp, 1);

  if (strchr(varp->name, '1'))
    recovery = recfrhGV[0];	// TTH?1
  else
    recovery = recfrhGV[1];	// TTH?2

  atfh = AMBIENT(ttfh, recovery, xmach2);

  PutSample(varp, atfh);

}

/* -------------------------------------------------------------------- */
void satfhGVrose(DERTBL *varp)
{
  NR_TYPE ttfh, xmach2, recovery, atfh, logMach = 0.0;

  ttfh = GetSample(varp, 0);
  xmach2 = GetSample(varp, 1);

  if (xmach2 > 0.0)
    logMach = log10(xmach2);

//  recovery = 0.988 + 0.036 * (log10(xmach2) / 2);
  recovery = 0.988 + 0.053 * logMach + 0.090 * logMach * logMach + 0.091 * logMach * logMach * logMach;

  atfh = AMBIENT(ttfh, recovery, xmach2);

  PutSample(varp, atfh);

}

/* END ATFH.C */
