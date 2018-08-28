/*
-------------------------------------------------------------------------
OBJECT NAME:	turbulence.c

FULL NAME:	

ENTRY POINTS:	sfunc()

STATIC FNS:	

DESCRIPTION:	Turbulence fn developed for TREX.

COPYRIGHT:	University Corporation for Atmospheric Research, 2006
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static const int N = 30;

/* -------------------------------------------------------------------- */
void stke(DERTBL *varp)
{
  NR_TYPE ui, vi, ws, wi, TKE, VRMS;
  static double v2bar = 0.0, ubar = 0.0, vbar = 0.0, wbar = 0.0;

  ui = GetSample(varp, 0);
  vi = GetSample(varp, 1);
  wi = GetSample(varp, 2);
  ws = GetSample(varp, 3);

  if (isnan(ui) || isnan(vi) || isnan(wi))
    TKE = MISSING_VALUE;
  else
  {
    v2bar = (v2bar * (N-1) + ws*ws + wi*wi) / N;
    ubar = (ubar * (N-1) + ui) / N;
    vbar = (vbar * (N-1) + vi) / N;
    wbar = (wbar * (N-1) + wi) / N;
    TKE = 0.5 * (v2bar - ubar*ubar - vbar*vbar - wbar*wbar);

    VRMS = sqrt(2.0 * TKE);
  }

  PutSample(varp, TKE);

}	/* END STKE */

/* -------------------------------------------------------------------- */
void sacvar(DERTBL *varp)
{
  NR_TYPE acins, ACVAR;
  static double g2bar = 0.0, gbar = 0.0;

  acins = GetSample(varp, 0);

  if (isnan(acins))
    ACVAR = floatNAN;
  else
  {
    g2bar = (g2bar * (N-1) + acins*acins) / N;
    gbar = (gbar * (N-1) + acins) / N;
    ACVAR = g2bar - gbar*gbar;
  }

  PutSample(varp, ACVAR);

}	/* END SACVAR */

/* END TURBULENCE.C */
