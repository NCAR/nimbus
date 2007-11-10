/*
-------------------------------------------------------------------------
OBJECT NAME:	tasx.c

FULL NAME:	True Airspeed

ENTRY POINTS:	sfunc()

STATIC FNS:	none

DESCRIPTION:	TASX is used by other parameters to compute their values.
		TASX takes on one of the sampled TAS variables.  This
		method allows users to set TASX to what ever is working
		or most accurate.

INPUT:		True Airspeed to use

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	compute.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void stasx(DERTBL *varp)
{
  NR_TYPE	tas = GetSample(varp, 0);

  if (tas <= 0.0)
    tas = 0.001;

  PutSample(varp, tas);

}	/* END STASX */

/* END TASX.C */
