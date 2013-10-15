/*
-------------------------------------------------------------------------
OBJECT NAME:	itr.c

FULL NAME:	In-cloud Temperature Radiometer


DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 2013
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static const double lambda = 4.3e-6;
static const double Csquared = StdSpeedOfLight * StdSpeedOfLight;

// Move to Defaults file.
static const double a0 = -17004.4;
static const double a1 = 190.876;
static const double a2 = 0.9569;
static const double a3 = 0.05187;
static const double a4 = 0;
static const double a5 = 0;


/* -------------------------------------------------------------------- */
NR_TYPE ThermalRadiance(NR_TYPE raw_temp)	// in Kelvin
{
  static const double const1 =
	((2 * M_PI * Planck * Csquared) / pow(lambda, 5.0));

  return const1 / (exp(Planck * StdSpeedOfLight / (Boltzmann * raw_temp * lambda)) - 1);
}

/* -------------------------------------------------------------------- */
void sfunc(DERTBL *varp)
{
  NR_TYPE detector1 = GetSample(varp, 0);
  NR_TYPE detector2 = GetSample(varp, 1);
  NR_TYPE bentemp = GetSample(varp, 2);
  NR_TYPE strtemp = GetSample(varp, 3);
  NR_TYPE snktemp = GetSample(varp, 4);

  NR_TYPE M_BEN = ThermalRadiance(bentemp + Kelvin);
  NR_TYPE M_SNK = ThermalRadiance(snktemp + Kelvin);
  NR_TYPE M_STR = ThermalRadiance(strtemp + Kelvin);
	
  NR_TYPE M_air = a0 + a1*(detector1 - detector2) + a2*M_STR + a3*M_BEN + a4*M_SNK + a5*M_STR*M_STR;
	
  NR_TYPE at = (Planck * StdSpeedOfLight / (Kelvin * lambda))
	/ log(((2 * M_PI * Planck * Csquared) / (M_air * pow(lambda, 5.0))) + 1);

  PutSample(varp, at - Kelvin);

}	/* END SFUNC */

/* END FUNC.C */
