/************************* PSFC **********************************  PSFC
 ** Corrected static pressure (boom) (mb)
 	Input:
 		psf - raw static pressure, fuselage mounted
 		qcx - raw default dynamic pressure
 	Output:
 		psfc - corrected static pressure
 	Include:
 		pcorf - fuselage dynamic pressure correction
*/

#include "nimbus.h"
#include "amlib.h"

extern NR_TYPE (*pcorPSF)(NR_TYPE, NR_TYPE);

/* -------------------------------------------------------------------- */
void spsfc(DERTBL *varp)
{
  NR_TYPE psf, qcx, mach, psfc;

  psf = GetSample(varp, 0);
  qcx = GetSample(varp, 1);

  if (cfg.Aircraft() == Config::HIAPER)
  {
    mach = GetSample(varp, 2);
    psfc = psf * (1.0 + (*pcorPSF)(qcx, mach));
  }
  else
  {
    psfc = psf + (*pcorPSF)(qcx, psf);
  }

  if (psfc < 50.0)
    psfc = 50.0;

  PutSample(varp, psfc);

}	/* END SPSFC */

/* END PSFC.C */
