/*
-------------------------------------------------------------------------
OBJECT NAME:	geopt.c

FULL NAME:	Geopotential Altitude

DESCRIPTION:	Convert Mean Sea Level (MSL) Altitude to Geopotential
		Altitude.

COPYRIGHT:	University Corporation for Atmospheric Research, 1996-2018
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static double g0 = 9.80665;
static double g1 = 0.001931851;
static double g2 = 0.006694380;
static double k1 = 3.1570428706e-07;
static double k2 = 2.1026896504e-09;
static double k3 = 7.3745167729e-14;


/* -------------------------------------------------------------------- */
void sGeopot2(DERTBL *varp)	// Al Cooper equation, 2018.
{
  NR_TYPE       geop_alt;

  double msl_alt = GetSample(varp, 0);		// ggalt
  double lat = GetSample(varp, 1) * DEG_RAD;	// any latititude
  double geoidht = GetSample(varp, 2);		// ellipsoid - geoid (m)

  double sin_lat_2 = sin(lat) * sin(lat);
  geop_alt =    GRAVITY * ((1.0 + g1 * sin_lat_2) / pow(1.0 - g2 * sin_lat_2, 0.5) ) *
                ( msl_alt - ((pow(msl_alt + geoidht, 2.0) - pow(geoidht, 2.0)) / 2.0) *
                (k1 - k2 * sin_lat_2) + ((pow(msl_alt + geoidht, 3.0) - pow(geoidht, 3.0)) / 3.0 * k3) );

  PutSample(varp, geop_alt / g0);
}


/* -------------------------------------------------------------------- */
void sGeopot(DERTBL *varp)	// This was implemented for NOAA/AOC in 1996.
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
