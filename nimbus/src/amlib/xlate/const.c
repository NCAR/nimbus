/*
-------------------------------------------------------------------------
OBJECT NAME:	const.c

FULL NAME:	Constants

ENTRY POINTS:	FirstPoly()
		SecondPoly()
		smissval()
		szero()
		sone()

STATIC FNS:	none

DESCRIPTION:	Globals file containing misc constants used by multiple
		routines.

COPYRIGHT:	University Corporation for Atmospheric Research, 1996-2011
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

const double Kelvin = 273.15;

// Reference pressure for US standard atmosphere (hPa)
const double StdPress = 1013.25;

// The specific gas constant for dry air (J/(kg*K))
const double Rd = 287.05832;

// Specific heat capacity of dry air (J/(kg*K))
const double Cpd = 1003.5;

// Gamma for an ideal gas (do not change this to actually divide Rd/Cpd).
const double Rd_DIV_Cpd = 2.0 / 7.0;

const double WH_GRAVITY = 9.7959;	// Gravity in Woodland Hills
const double GRAVITY = 9.80621770635;	// Gravity in Michigan

const double StdSpeedOfSound = 340.3 * 2.23693629;

const double MolecularWeightDryAir = 28.9644;
const double MolecularWeightWater = 18.01526;

const double PI = M_PI;			// defined in math.h 

const double EARTH_RADIUS = 6.371229e06;	// Mean Earth radius (m)
const double OMEGAE = 7.292116e-5;	// Angular vel of the Earth (rad/sec)

const double FeetToMeters = 0.3048;
const double MPS2   = 9.7959;
const double KTS2MS = 0.5144444444;
const double FTMIN  = 0.00508;

const double RAD_DEG = 180.0 / M_PI;
const double DEG_RAD = M_PI / 180.0;

const double RESOLV14BIT = 180.0 / 8192.0;
const double RESOLV16BIT = 360.0 / 65536.0;

/* -------------------------------------------------------------------- */
NR_TYPE FirstPoly(NR_TYPE x, std::vector<float>& c)
{
  if (x == MISSING_VALUE)
    return(floatNAN);
  else
    return(c[0] + (x * c[1]));
}

/* -------------------------------------------------------------------- */
NR_TYPE SecondPoly(NR_TYPE x, std::vector<float>& c)
{
  if (x == MISSING_VALUE)
    return(floatNAN);
  else
    return(c[0] + x * (c[1] + x * c[2]));
}

/* -------------------------------------------------------------------- */
void smissval(DERTBL *varp)
{
  PutSample(varp, floatNAN);

}	/* END SMISSVAL */

/* -------------------------------------------------------------------- */
void sone(DERTBL *varp)
{
  PutSample(varp, 1.0);

}	/* END SONE */

/* -------------------------------------------------------------------- */
void szero(DERTBL *varp)
{
  PutSample(varp, 0.0);

}	/* END SZERO */

/* END CONST.C */
