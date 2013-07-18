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

extern NR_TYPE (*pcorPSFD)(NR_TYPE, NR_TYPE);

/* -------------------------------------------------------------------- */
void spsfdc(DERTBL *varp)
{
  NR_TYPE	psfd, aqratio, xmach2 = 1.0, psfdc;

  psfd		= GetSample(varp, 0);
  aqratio	= GetSample(varp, 1);

  if (varp->ndep > 2)
    xmach2	= GetSample(varp, 2);

  psfdc = psfd * (1.0  + (*pcorPSFD)(aqratio, xmach2));

  PutSample(varp, psfdc);

}	/* END SPSFDC */

/* END PSFDC.C */
