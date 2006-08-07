/*
-------------------------------------------------------------------------
OBJECT NAME:	geopt.c

FULL NAME:	Geopotential Altitude

ENTRY POINTS:	sGeopot()

STATIC FNS:	

DESCRIPTION:	Convert Mean Sea Level (MSL) Altitude to Geopotential
		Altitude.

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	compute.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1996
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void sGeopot(DERTBL *varp)
{
  NR_TYPE	msl_alt, geop_alt, lat;

  NR_TYPE	G,	/* MSL value of the accelleration of gravity @ given
			 * latitude, cm/s**2				*/
		R,	/* Effective earth radius @ given latitude,
			 * fictitious quantity that does not represent the
			 * radius vector of the earth.			*/
		DEM;	/* Partial derivative of gravity with respect to
			 * geometric height for a given latitude at height=0 */

  msl_alt = GetSample(varp, 0);
  lat = GetSample(varp, 1);

  G	= 9.80616 * (1.0 - cos((double)2.0 * lat) * 0.0026373);
  DEM	= 3.085462e-06 + 2.27e-09 * (cos((double)2.0 * lat)) - 2.0e-12 *
		  (cos((double)4.0 * lat));
  R	= (2.0 * G) / DEM;

  geop_alt = (G * R * msl_alt) / (9.8 * (R + msl_alt));

  PutSample(varp, geop_alt);

}	/* END SGEOPOT */

/* END GEOPOT.C */
