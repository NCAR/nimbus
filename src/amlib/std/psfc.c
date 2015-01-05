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

extern NR_TYPE (*pcorPSF)(NR_TYPE);
extern NR_TYPE (*pcorPSFv2)(NR_TYPE, NR_TYPE, NR_TYPE);

/* -------------------------------------------------------------------- */
void spsfc(DERTBL *varp)
{
  NR_TYPE psf, qcf, psfc;

  psf = GetSample(varp, 0);
  qcf = GetSample(varp, 1);

  if (cfg.Aircraft() == Config::HIAPER || cfg.Aircraft() == Config::C130)
  {
    NR_TYPE attack = GetSample(varp, 2);

    psfc = psf + (*pcorPSFv2)(qcf, psf, attack);
  }
  else
  {
    psfc = psf + (*pcorPSF)(qcf);
  }

  if (psfc < 50.0)
    psfc = 50.0;

  PutSample(varp, psfc);

}	/* END SPSFC */

/* END PSFC.C */
