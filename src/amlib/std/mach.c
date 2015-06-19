/*
-------------------------------------------------------------------------
OBJECT NAME:	mach.c

FULL NAME:	Aircraft Mach Number

ENTRY POINTS:	smach()

DESCRIPTION:	Calculate mach number from pitot tube pressure using
		Bernoulli's equation for M < 1.

		In fluid mechanics, Mach number (M or Ma) is a dimensionless
		quantity representing the ratio of speed of an object moving
		through a fluid and the local speed of sound.

COPYRIGHT:	University Corporation for Atmospheric Research, 2013
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"
      
/* -------------------------------------------------------------------- */
double mach(double p, double q)
{
  return sqrt((2.0 * Cv() / R()) * (pow((p + q) / p, R() / Cp()) - 1.0));
}

/* -------------------------------------------------------------------- */
double mach_dry(double p, double q)
{
  return sqrt(5.0 * (pow((q/p)+1.0, Rd_DIV_Cpd) - 1.0));
}

/* -------------------------------------------------------------------- */
void smach(DERTBL *varp)
{
  NR_TYPE q = GetSample(varp, 0);
  NR_TYPE p = GetSample(varp, 1);

  PutSample(varp, mach(p, q));
}
