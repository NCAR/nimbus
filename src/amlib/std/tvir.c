/*******       VIRTUAL TEMPERATURE (C)                                  TVIR
                  REQUIRES --- ATX, MR
 	Input:
 		atx - derived ambient temperature
 		mr - mixing ratio
 	Output:
 		tvir - derived virtual temperature - DegC
*/
 
#include "nimbus.h"
#include "amlib.h"

/* -------------------------------------------------------------------- */
void stvir(DERTBL *varp)
{
  NR_TYPE	tvir, atx, mr;

  atx	= GetSample(varp, 0);
  mr	= GetSample(varp, 1);

  // factors of 0.001 are conversion factors to obtain dimensionless mixing ratios [kg/kg]
  tvir = (atx + Kelvin)
	* (1.0 + 0.001 * mr * MolecularWeightDryAir / MolecularWeightWater)
	/ (1.0 + 0.001 * mr) - Kelvin;

  PutSample(varp, tvir);
}
