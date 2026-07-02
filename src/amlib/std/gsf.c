/*
-------------------------------------------------------------------------
OBJECT NAME:	gsf.c

FULL NAME:	Compute ground speed for the Litton 51.

ENTRY POINTS:	sgsf()

STATIC FNS:	none

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void sgsf(DERTBL *varp)
{
  NR_TYPE	xvi, yvi;

  xvi = GetSample(varp, 0);
  yvi = GetSample(varp, 1);

  PutSample(varp, (NR_TYPE)sqrtf(xvi*xvi + yvi*yvi));

}	/* END SGSF */

/* END GSF.C */
