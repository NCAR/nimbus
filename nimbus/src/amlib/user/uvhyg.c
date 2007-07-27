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

  PutSample(varp, (log((xuvi + 0.125) / 1.33) + 0.35 * psx / atx) / -0.1);

}
