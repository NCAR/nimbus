/*
-------------------------------------------------------------------------
OBJECT NAME:	func.c

FULL NAME:	

ENTRY POINTS:	sfunc()

STATIC FNS:	

DESCRIPTION:	Template for AMLIB std & user directories.

COPYRIGHT:	University Corporation for Atmospheric Research, 2013
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void sfunc(DERTBL *varp)
{
  NR_TYPE	x1, x2, x3...xn, y;

  /* These 'GetSample()'s must match the same order as the variables
   * are listed in the DependTable.
   */
  x1 = GetSample(varp, 0);
  x2 = GetSample(varp, 1);
  ....
  xn = GetSample(varp, n-1);


  y = ..............;


  PutSample(varp, y);

}	/* END SFUNC */

/* END FUNC.C */
