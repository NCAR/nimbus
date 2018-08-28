/*********************** RHODT *****************************         RHODT
 ** Absolute humidity (thermoelec) (g/m3)
    All moisture variables are computed with respect to a plane water
    surface.
 
 	Input:
 		atx - derived ambient temperature
 		dpxc - derived dew point temperature
 	Output:
 		rhodt - derived absolute humidity
*/

#include "nimbus.h"
#include "amlib.h"

/* -------------------------------------------------------------------- */
void srhodt(DERTBL *varp)
{
  NR_TYPE	atx, edpc;

  atx	= GetSample(varp, 0);
  edpc	= GetSample(varp, 1);

  PutSample(varp, 216.68 * edpc / (atx + Kelvin));

}
