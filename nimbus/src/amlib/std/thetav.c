/*******       VIRTUAL POTENTIAL TEMPERATURE (K)                        THETAV
 	Input:
 		tvir - derived virtual temperature
 		psxc - derived static pressure
 	Output:
 		thetav - derived virtual potential temperature
*/
 
#include "nimbus.h"
#include "amlib.h"

/* -------------------------------------------------------------------- */
void sthetav(DERTBL *varp)
{
  NR_TYPE	thetav = 0.0, atx, tvir, psxc, edpc, sphum;

  tvir	= GetSample(varp, 0);
  psxc	= GetSample(varp, 1);

  if (psxc != 0.0)
    thetav = (tvir + Kelvin) * pow((double)(1000.0 / psxc), (double)0.28571);

  PutSample(varp, thetav);
}
