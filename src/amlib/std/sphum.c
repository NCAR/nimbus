/*******       SPECIFIC HUMIDITY (THERMOELEC) (G/KG)                    SPHUM
                  REQUIRES --- PSXC, EDPC
 	Input:
 		psxc - derived static pressure
 		edpc - derived vapor pressure
 	Output:
 		sphum - derived specific humidity
*/

#include "nimbus.h"
#include "amlib.h"

/* -------------------------------------------------------------------- */
void ssphum(DERTBL *varp)
{
  NR_TYPE	psxc,edpc,sphum;

  psxc	= GetSample(varp, 0);
  edpc	= GetSample(varp, 1);

  sphum	= psxc - 0.378 * edpc;

  if (sphum != 0.0)
    sphum = 622.0 * edpc / sphum;

  PutSample(varp, sphum);

}	/* END SSPHUM */

/* END SPHUM.C */
