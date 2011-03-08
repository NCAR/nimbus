/*
-------------------------------------------------------------------------
OBJECT NAME:	thetav.c

FULL NAME:	Virtual Potential Temperature

DESCRIPTION:	The potential temperature of dry air having the same density
		of the moist-air parcel.  Omits the weight of condensed water.

INPUT:		TVIR, PSXC

OUTPUT:		THETAV deg_K

REFERENCES:	

COPYRIGHT:	University Corporation for Atmospheric Research, 2011
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

double theta(double Tk, double psxc);

/* -------------------------------------------------------------------- */
void sthetav(DERTBL *varp)
{
  double tvir	= GetSample(varp, 0);
  double psxc	= GetSample(varp, 1);

  PutSample(varp, (NR_TYPE)theta(tvir + Kelvin, psxc));
}
