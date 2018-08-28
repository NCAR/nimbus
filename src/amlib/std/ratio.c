/*
-------------------------------------------------------------------------
OBJECT NAME:	ratio.c

FULL NAME:	Ratios

ENTRY POINTS:	sratio()
		saqratio()

STATIC FNS:	none

DESCRIPTION:	This function produces ratios.  If DependTable entry
		looks like 'RATIO  HGME PALT', that equates to
		RATIO = HGME / PALT.

COPYRIGHT:	University Corporation for Atmospheric Research, 1996-2005
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void sratio(DERTBL *varp)
{
  NR_TYPE	x1, x2;

  /* These 'GetSample()'s must match the same order as the variables
   * are listed in the DependTable.
   */
  x1 = GetSample(varp, 0);
  x2 = GetSample(varp, 1);

  PutSample(varp, x1 / x2);

}	/* END SRATIO */

/* -------------------------------------------------------------------- */
void saqratio(DERTBL *varp)
{
  NR_TYPE	aqratio, adifr, qcr;

  adifr = GetSample(varp, 0);
  qcr = GetSample(varp, 1);

  if (qcr < 5.0)
    aqratio = -0.1;
  else
    aqratio = GetSample(varp, 0) / GetSample(varp, 1);

  PutSample(varp, aqratio);

}	/* END SAQRATIO */

/* END RATIO.C */
