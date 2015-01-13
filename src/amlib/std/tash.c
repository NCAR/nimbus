/*
-------------------------------------------------------------------------
OBJECT NAME:	tash.c

FULL NAME:	True Airspeed, Humidity Corrected

ENTRY POINTS:	stash()

STATIC FNS:	none

DESCRIPTION:	TASX is used by other parameters to compute their values.
		TASX takes on one of the sampled TAS variables.  This
		method allows users to set TASX to what ever is working
		or most accurate.

COPYRIGHT:	University Corporation for Atmospheric Research, 1995-2013
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

/* -------------------------------------------------------------------- */
void stash(DERTBL *varp)
{
  NR_TYPE	tas, mr;

  tas	= GetSample(varp, 0);
  mr	= GetSample(varp, 1);

  if (isnan(mr))
    mr = 0.0;

  PutSample(varp, tas / (1.0 - 0.3039e-03 * mr));
}
