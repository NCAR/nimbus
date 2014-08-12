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
  NR_TYPE psf, qcf, psfc;

  psf = GetSample(varp, 0);
  qcf = GetSample(varp, 1);

  if (cfg.Aircraft() == Config::HIAPER)
  {
    NR_TYPE attack = GetSample(varp, 2);
    NR_TYPE qcr = GetSample(varp, 3);
    NR_TYPE M = sqrt( (2.0 * Cv / Rd) * (pow((qcf+psf)/psf, Rd_DIV_Cpd) - 1.0) );

    psfc = psf +
	(psf * (-0.01317 + 0.07256 * qcf / psf - 0.08611 * M*M*M + 0.002253 * attack/qcr));
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
