/*******       VIRTUAL TEMPERATURE (C)                                  TVIR
                  REQUIRES --- ATX, SPHUM
 	Input:
 		atx - derived ambient temperature
 		sphum - derived specific humidity
 	Output:
 		tvir - derived virtual temperature
*/
 
#include "nimbus.h"
#include "amlib.h"

/* -------------------------------------------------------------------- */
void stvir(DERTBL *varp)
{
  NR_TYPE	tvir = 0.0, sphum, atx, otvir;

  atx	= GetSample(varp, 0);
  sphum	= GetSample(varp, 1);

  if ((otvir = 1.0 - 0.6e-3 * sphum) != 0.0)
    otvir = (atx + 273.16) / tvir - 273.16;

  PutSample(varp, tvir);
}
