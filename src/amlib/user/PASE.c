/*
-------------------------------------------------------------------------
OBJECT NAME:	PASE.c

FULL NAME:

ENTRY POINTS:	sfunc()

STATIC FNS:  

DESCRIPTION:	Calcs for PASE, Steve Howell

COPYRIGHT:	University Corporation for Atmospheric Research, 2007
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static NR_TYPE d[3] = { 6.35, 6.17, 5.16 };

/* -------------------------------------------------------------------- */
void spflow(DERTBL *varp)
{
  NR_TYPE psfd = GetSample(varp, 0);
  PutSample(varp, -0.44 + 0.0013 * psfd);
}

/* -------------------------------------------------------------------- */
void sScatter(DERTBL *varp)
{
  NR_TYPE  Vdc = GetSample(varp, 0);
  PutSample(varp, pow(10.0, Vdc / 2.0));
}

/* -------------------------------------------------------------------- */
void ssteve(DERTBL *varp)
{
  NR_TYPE slpm, atx, psx, tasx, out;

  slpm = GetSample(varp, 0);
  psx = GetSample(varp, 1);
  atx = GetSample(varp, 2);
  tasx = GetSample(varp, 3);

  out = slpm * (1013.25 / psx) * ((atx+Kelvin) / 298) * 66.6666 / (M_PI * d[0] * d[0] * tasx);

  PutSample(varp, out);
}

/* END PASE.C */
