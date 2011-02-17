/*
-------------------------------------------------------------------------
OBJECT NAME:	uvhyg.c

FULL NAME:	

ENTRY POINTS:	suvmr()

STATIC FNS:	

DESCRIPTION:	Stuarts UV Hygrometer

COPYRIGHT:	University Corporation for Atmospheric Research, 2007
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void suvmr(DERTBL * varp)
{
  NR_TYPE	xuvi, psx, atx;

  xuvi = GetSample(varp, 0);
  psx = GetSample(varp, 1);
  atx = GetSample(varp, 2);

  if (cfg.isADS2())	// Old UV Hygrometer
    PutSample(varp, (log((xuvi + 0.125) / 1.33) + 0.35 * psx / atx) / -0.1);
  else	// VOCALS and later.
    PutSample(varp, -37.95 * ((atx + Kelvin) / psx) * (log((xuvi - 0.076) / 12.25) + 0.00045 * psx));
}
