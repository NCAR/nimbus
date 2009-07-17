/*
-------------------------------------------------------------------------
OBJECT NAME:	lams.c

FULL NAME:	

COPYRIGHT:	University Corporation for Atmospheric Research, 2009
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void slamsws(DERTBL *varp)
{
  NR_TYPE	*max, *avg, tas, lamsws;

  /* These 'GetSample()'s must match the same order as the variables
   * are listed in the DependTable.
   */
  avg = GetVector(varp, 0);
  max = GetVector(varp, 1);
  tas = GetSample(varp, 2);




  PutSample(varp, lamsws);

}	/* END SFUNC */

/* END FUNC.C */
