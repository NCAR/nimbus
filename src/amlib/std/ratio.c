/*
-------------------------------------------------------------------------
OBJECT NAME:	ratio.c

FULL NAME:	Ratios

ENTRY POINTS:	sratio()

STATIC FNS:	

DESCRIPTION:	This function produces ratios.  If DependTable entry
		looks like 'RATIO  HGME PALT', that equates to
		RATIO = HGME / PALT.

INPUT:		

OUTPUT:		

REFERENCES:	

REFERENCED BY:	compute.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1996
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

/* END RATIO.C */
