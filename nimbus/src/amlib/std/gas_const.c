/*
-------------------------------------------------------------------------
OBJECT NAME:	gas_const.c

FULL NAME:	Humidity corrected gas constants.

ENTRY POINTS:	Cp(), Rd(), Cv()

STATIC FNS:	Fr()

DESCRIPTION:	Calculations for gas constants.  Takes a water vapor
		to compensate for dry vs. moist air.
		Setting eop (EWX over PSXC) to 0.0 will give dry gas constants,
		Otherwise humidity corrected.

COPYRIGHT:	University Corporation for Atmospheric Research, 2015
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

// Water vapor pressure over static pressure (Ew / P)
static double eop = 0.0;

/* -------------------------------------------------------------------- */
void setEOP(double newEOP)
{
  eop = newEOP;
}

/* -------------------------------------------------------------------- */
static double Fr()
{
  return 1.0 / (1.0 + ((MolecularWeightWater / MolecularWeightDryAir) - 1.0) * eop);
}

// Gas constant for humidity corrected air.
double R()
{
  return Rd * Fr();
}

// Specific heat of humidity corrected air at constant volume.
double Cv()
{
  return Cvd * Fr() * (1.0 + (eop / 5.0));
}

// Specific heat of humidity corrected air at constant pressure.
double Cp()
{
  return Cpd * Fr() * (1.0 + (eop / 7.0));
}

/* END GAS_CONST.C */
