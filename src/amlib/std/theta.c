/*******      Potential Temperature (K)                                 theta
                 REQUIRES --- ATX, PSXC
 	Input:
 		atx - derived ambient temperature
 		psxc - derived static pressure
 	Output:
 		theta - derived potential temperature
*/
 
#include "nimbus.h"
#include "amlib.h"

/* -------------------------------------------------------------------- */
void stheta(DERTBL *varp)
{
  NR_TYPE atx, psxc;

  atx	= GetSample(varp, 0);
  psxc	= GetSample(varp, 1);

  if (psxc != 0.0)
    PutSample(varp, (atx + 273.16) * pow(1000.0 / psxc, 0.28571));
  else
    PutSample(varp, 0.0);
}
