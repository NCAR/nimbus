/*********************** PSFDC ************************************  PSFDC
 ** Corrected static pressure (fuselage digital) (mb)
 	Input:
 		psfd - raw digital fuselage static pressure
 		qcx  - default raw dynamic pressure
 	Output:
 		psfdc - corrected static pressure
 	Include:
 		pcord - dynamic correction for digital static pressure
*/

#include "nimbus.h"
#include "amlib.h"

extern NR_TYPE (*pcorPSFD)(NR_TYPE);
extern NR_TYPE (*pcorPSFDv2)(NR_TYPE, NR_TYPE, NR_TYPE);

/* -------------------------------------------------------------------- */
void spsfdc(DERTBL *varp)
{
  NR_TYPE psf, qcf, psfc;

  psf = GetSample(varp, 0);
  qcf = GetSample(varp, 1);

  // Doesn't currently exist on GV, but code for it anyways.
  if (cfg.Aircraft() == Config::C130 || cfg.Aircraft() == Config::HIAPER)
  {
    NR_TYPE attack = GetSample(varp, 2);

    psfc = psf + (*pcorPSFDv2)(qcf, psf, attack);
  }
  else
  {
    psfc = psf + (*pcorPSFD)(qcf);
  }

  if (psfc < 50.0)
    psfc = 50.0;

  PutSample(varp, psfc);

}	/* END SPSFDC */

/* END PSFDC.C */
