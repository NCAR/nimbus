/*******       MIXING RATIO from Lyman Alpha (G/KG)                      MRLA

	Input:
		rhola - derived absolute humidity from lyman alpha
		psxc - derived static pressure
		atx - derived dew point
	Output:
		mrla - mixing ratio from lyman alpha
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void smrla(DERTBL *varp)
{
  NR_TYPE	edla, rhola, atx, psxc;
  NR_TYPE	mrla;

  rhola	= GetSample(varp, 0);
  psxc	= GetSample(varp, 1);
  atx	= GetSample(varp, 2);

  edla	= rhola * (atx + Kelvin) / 216.68;

  if (psxc != edla)
    mrla = 622.0 * edla / (psxc-edla);

  PutSample(varp, mrla);

}	/* END SMRLA */

/* MRLA.C */
