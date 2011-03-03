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
    PutSample(varp, (atx + Kelvin) * pow(1000.0 / psxc, Rd_DIV_Cpd));
  else
    PutSample(varp, 0.0);
}
