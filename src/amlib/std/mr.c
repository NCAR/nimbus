/*******       MIXING RATIO (G/KG)                                      MR
                  REQUIRES --- PSXC, EDPC(DPXC)
 	Input:
 		psxc - derived static pressure
 		dpxc - derived dew point
 	Output:
 		mr - mixing ratio
*/
 
#include "nimbus.h"
#include "amlib.h"

/* -------------------------------------------------------------------- */
void smr(DERTBL *varp)
{
  NR_TYPE	mr, psxc, edpc;

  psxc = GetSample(varp, 0);
  edpc = GetSample(varp, 1);

  if (psxc != edpc)
    mr = 622.0 * edpc / (psxc - edpc);
  else
    mr = 0.0;

  PutSample(varp, mr);

}	/* END SMR */

/* END MR.C */
