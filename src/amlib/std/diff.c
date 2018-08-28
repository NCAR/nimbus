/*
-------------------------------------------------------------------------
OBJECT NAME:	diff.c

FULL NAME:	Differences

ENTRY POINTS:	sdiff()

STATIC FNS:	

DESCRIPTION:	This function produces differences.  If DependTable entry
		looks like 'DIFF1  HGME PALT', that equates to
		DIFF1 = HGME - PALT.

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
void sdiff(DERTBL *varp)
{
  NR_TYPE	x1, x2;

  /* These 'GetSample()'s must match the same order as the variables
   * are listed in the DependTable.
   */
  x1 = GetSample(varp, 0);
  x2 = GetSample(varp, 1);

  PutSample(varp, x1 - x2);

}	/* END SDIFF */

/* END DIFF.C */
