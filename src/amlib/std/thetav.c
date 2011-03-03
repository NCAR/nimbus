/*
-------------------------------------------------------------------------
OBJECT NAME:	thetav.c

FULL NAME:	Virtual Potential Temperature

DESCRIPTION:	

INPUT:		TVIR, PSXC

OUTPUT:		THETAV deg_K

REFERENCES:	

COPYRIGHT:	University Corporation for Atmospheric Research, 2011
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

/* -------------------------------------------------------------------- */
void sthetav(DERTBL *varp)
{
  NR_TYPE	thetav = 0.0, tvir, psxc;

  tvir	= GetSample(varp, 0);
  psxc	= GetSample(varp, 1);

  if (psxc != 0.0)
    thetav = (tvir + Kelvin) * pow((double)(1000.0 / psxc), Rd_DIV_Cpd);

  PutSample(varp, thetav);
}
