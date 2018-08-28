/*
-------------------------------------------------------------------------
OBJECT NAME:	roc.c

FULL NAME:	Rate of Climb calculation

ENTRY POINTS:	sroc()

DESCRIPTION:	Rate of climb calculations

INPUT:		psxc, palt, vspd, atx;

OUTPUT:		roc

COPYRIGHT:	University Corporation for Atmospheric Research, 2015
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void sroc(DERTBL *varp)
{
  NR_TYPE	TStdA, roc;
  NR_TYPE	psxc, palt, vspd, atx;

  /* These 'GetSample()'s must match the same order as the variables
   * are listed in the DependTable.
   */
  psxc = GetSample(varp, 0);
  palt = GetSample(varp, 1);
  vspd = GetSample(varp, 2);
  atx  = GetSample(varp, 3);

  /* calculate the temperature in a standard atmosphere, TstdA (kelvin) */
  if (psxc < 226.3206)
      TStdA = 216.65;
  else
      TStdA = 288.15 - (0.0065 * palt);

  /* calculate ROC (rate of climb, m/s) */
  roc = vspd * (atx+273.15) / TStdA;

  PutSample(varp, roc);

}	/* END SROC */

/* END ROC.C */
