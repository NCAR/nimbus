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
  NR_TYPE	mr, psxc, ew;

  psxc = GetSample(varp, 0);
  ew = GetSample(varp, 1);

  if (psxc != ew)
    mr = 622.0 * ew / (psxc - ew);
  else
    mr = 0.0;

  PutSample(varp, mr);

}	/* END SMR */


/* -------------------------------------------------------------------- */
// Convert a mixing ratio to water vapor pressure (MR -> EW).
// Originally for HIPPO-1 RF08, where we used UCATS MR to compute an EW.
void smr2ew(DERTBL *varp)
{
  NR_TYPE h2oMR = GetSample(varp, 0);	// ppmv
  NR_TYPE psxc = GetSample(varp, 1);	// hPa

  NR_TYPE Rv = h2oMR * 1.e-6;
  NR_TYPE ew = Rv * psxc / (1.0 + Rv);

  PutSample(varp, ew);
}

/* END MR.C */
