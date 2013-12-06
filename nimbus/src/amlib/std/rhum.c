/*******       RELATIVE HUMIDITY (PER CENT)                             RHUM
                  REQUIRES --- EDPC, ATX, ESUBT FUNCTION
 	Input:
 		atx - derived ambient temperature
 		edpc - derived vapor pressure
 	Output:
 		rhum - derived relative humidity
*/

#include "nimbus.h"
#include "amlib.h"

double WaterVaporPressureOfWater(double);
double WaterVaporPressureOfIce(double);

/* -------------------------------------------------------------------- */
void srhum(DERTBL *varp)
{
  NR_TYPE atx, e, esa, rhum = 0.0;

  atx = GetSample(varp, 0);
  if (varp->ndep == 3)	// Old method for computing RHUM.
  {
    double esubt(double, double);
    NR_TYPE psxc = GetSample(varp, 1);
    e	= GetSample(varp, 2);
    esa = (NR_TYPE)esubt(atx, psxc);
  }
  else	// New method.
  {
    e = GetSample(varp, 1);
    esa = (NR_TYPE)WaterVaporPressureOfWater(atx + Kelvin);
  }

  if (esa != 0.0)
    rhum = 100.0 * e / esa;

  PutSample(varp, rhum);
}

/* -------------------------------------------------------------------- */
void srhumi(DERTBL *varp)
{
  NR_TYPE rhum = 0.0;

  NR_TYPE atx = GetSample(varp, 0) + Kelvin;

  if (atx < Kelvin)
  {
    NR_TYPE e = GetSample(varp, 1);
    NR_TYPE esa = (NR_TYPE)WaterVaporPressureOfIce(atx);

    if (esa != 0.0)
      rhum = 100.0 * e / esa;
  }
  else
    rhum = floatNAN;

  PutSample(varp, rhum);

}	/* END SRHUM */

/* END RHUM.C */
