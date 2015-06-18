/*
-------------------------------------------------------------------------
OBJECT NAME:	const.c

FULL NAME:	Constants

ENTRY POINTS:	FirstPoly()
		SecondPoly()
		smissval()
		szero()
		sone()

DESCRIPTION:	Globals file containing misc scientific and engineering
		constants used by multiple routines.

COPYRIGHT:	University Corporation for Atmospheric Research, 1996-2012
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

const double Kelvin = 273.15;

// Reference pressure for US standard atmosphere (hPa)
const double StdPress = 1013.25;

// Reference temperature in a standard atmosphere at sea level..
const double Tr = 15.0 + Kelvin;

// Molecular weight dry air (kg/kmol-1)
const double Md = 28.9637;

// Molecular weight water (kg/kmol-1)
const double Mw = 18.0153;

// Universal gas constant
const double R0 = 8314.472;

// The specific gas constant for dry air (J/(kg*K))
const double Rd = R0 / Md;

/* Specific heat capacity of dry air (J/(kg*K))
 * Computed as: (7/2) * Rd
 */
const double Cpd = 7.0 / 2.0 * Rd;

const double Cvd = 5.0 / 2.0 * Rd;

/* Specific heat of water vapor (J/(kg*K))
 */
const double Cpv = 1875.0;

/* Specific heat of liquid water (J/(kg/K))
 */
const double Cw = 4218.0;

/* Latent heat of vaporizattion of water (J/(kg/K))
 */
const double Lv = -2370.0;

// Gamma for an ideal gas (do not change this to actually divide Rd/Cpd).
const double Rd_DIV_Cpd = 2.0 / 7.0;

// Boltzmann's constant
const double Boltzmann = 1.380658e-23;

// Planck's Constant
const double Planck = 6.62606957e-34;

const double GRAVITY = 9.780327;	// Gravity at surface and equator.

const double StdSpeedOfSound = sqrt(2.0 * Cp * Tr);

const double StdSpeedOfLight = 299792458.0;	// m/s

const double MolecularWeightDryAir = 28.9637;	// kg kmol-1
const double MolecularWeightWater = 18.01523;	// kg kmol-1

const double PI = M_PI;			// defined in math.h 

const double EARTH_RADIUS = 6.371229e06;	// Mean Earth radius (m)
const double OMEGAE = 7.292115e-5;	// Angular vel of the Earth (rad/sec)

const double FeetToMeters = 0.3048;
const double MPS2   = 9.7959;
const double KNOTS_TO_MPS = 1852.0 / 3600.0;
const double MPS_TO_KNOTS = 3600.0 / 1852.0;
const double FTMIN  = 0.00508;

const double RAD_DEG = 180.0 / M_PI;
const double DEG_RAD = M_PI / 180.0;

const double RESOLV14BIT = 180.0 / 8192.0;
const double RESOLV16BIT = 360.0 / 65536.0;

/* -------------------------------------------------------------------- */
NR_TYPE FirstPoly(NR_TYPE x, std::vector<NR_TYPE>& c)
{
  if (x == MISSING_VALUE)
    return(floatNAN);
  else
    return(c[0] + (x * c[1]));
}

/* -------------------------------------------------------------------- */
NR_TYPE SecondPoly(NR_TYPE x, std::vector<NR_TYPE>& c)
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
