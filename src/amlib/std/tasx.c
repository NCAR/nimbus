/*
-------------------------------------------------------------------------
OBJECT NAME:	tasx.c

FULL NAME:	Reference True Airspeed Assignment

DESCRIPTION:	TASX is used by other parameters to compute their values.
		TASX takes on one of the sampled TAS variables.  This
		method allows users to set TASX to what ever is working
		or most accurate.  It can also take a secondary tas to
		use should the primary go nan.

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2010
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void stasx(DERTBL *varp)
{
  NR_TYPE	tas = GetSample(varp, 0);

  // If we have a secondary tas, use it if primary is nan.
  if (isnan(tas) && varp->ndep > 1)
    tas = GetSample(varp, 1);

  if (tas <= 0.0)
    tas = 0.001;

  PutSample(varp, tas);

}	/* END STASX */

/* END TASX.C */
