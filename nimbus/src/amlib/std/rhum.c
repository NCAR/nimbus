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


/* -------------------------------------------------------------------- */
void srhum(DERTBL *varp)
{
  NR_TYPE atx, psxc, e, esa, rhum;

  atx = GetSample(varp, 0);
  if (varp->ndep == 3)	// Old method for computing RHUM.
  {
    double esubt(double, double);
    psxc = GetSample(varp, 1);
    e	= GetSample(varp, 2);
    esa = (NR_TYPE)esubt(atx, psxc);
  }
  else	// New method.
  {
    double WaterVaporPressureOfWater(double);
    e = GetSample(varp, 1);
    esa = (NR_TYPE)WaterVaporPressureOfWater(atx + Kelvin);
  }

  if (rhum != 0.0)
    rhum = 100.0 * e / esa;

  PutSample(varp, rhum);

}	/* END SRHUM */

/* END RHUM.C */
