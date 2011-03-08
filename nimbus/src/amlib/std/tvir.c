/*
-------------------------------------------------------------------------
OBJECT NAME:	tvir.c

FULL NAME:	Virtual Temperature

DESCRIPTION:	

INPUT:		ATX, MR

OUTPUT:		TVIR deg_C

REFERENCES:	Introduction to Theoretical Meteorology, Seymour Hess, 1959, p 60.

COPYRIGHT:	University Corporation for Atmospheric Research, 2011
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

/* -------------------------------------------------------------------- */
void stvir(DERTBL *varp)
{
  double atx	= GetSample(varp, 0);
  double mr	= GetSample(varp, 1);

  // factors of 0.001 are conversion factors to obtain dimensionless mixing ratios [kg/kg]
  double tvir = (atx + Kelvin)
	* (1.0 + 0.001 * mr * MolecularWeightDryAir / MolecularWeightWater)
	/ (1.0 + 0.001 * mr) - Kelvin;

  PutSample(varp, (NR_TYPE)tvir);
}
