/*
-------------------------------------------------------------------------
OBJECT NAME:	teo3c.c

FULL NAME:	Ozone Calculations

ENTRY POINTS:	steo3c()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	compute.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void steo3c(DERTBL *varp)
{
  NR_TYPE	teo3c, tep, tet;

  teo3c	= GetSample(varp, 0);
  tep	= GetSample(varp, 1);
  tet	= GetSample(varp, 2);

  teo3c *= (1013.16 / tep) * ((tet + 273.15) / 273.15);

  if (teo3c <= 1.0)
    teo3c = 0.0;

  PutSample(varp, teo3c);

}	/* END STEO3C */

/* END TEO3C.C */
