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
NR_TYPE pcorf5_3(NR_TYPE Qm, NR_TYPE Pm, NR_TYPE Attack);

/* -------------------------------------------------------------------- */
void spsfc(DERTBL *varp)
{
  NR_TYPE psf, qcf, psfc;

  psf = GetSample(varp, 0);
  qcf = GetSample(varp, 1);

  if (cfg.Aircraft() == Config::HIAPER)
  {
    NR_TYPE attack = GetSample(varp, 2);

    psfc = psf + pcorf5_3(qcf, psf, attack);
  }
  else
  {
    psfc = psf + (*pcorPSF)(qcf, psf);
  }

  if (psfc < 50.0)
    psfc = 50.0;

  PutSample(varp, psfc);

}	/* END SPSFC */

/* END PSFC.C */
