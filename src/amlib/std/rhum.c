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

double esubt(double, double);

/* -------------------------------------------------------------------- */
void srhum(DERTBL *varp)
{
  NR_TYPE	atx, psxc, edpc, rhum;

  atx	= GetSample(varp, 0);
  psxc	= GetSample(varp, 1);
  edpc	= GetSample(varp, 2);
  rhum	= (NR_TYPE)esubt(atx, psxc);

  if (rhum != 0.0)
    rhum = 100.0 * edpc / rhum;

  PutSample(varp, rhum);

}	/* END SRHUM */

/* END RHUM.C */
