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
  NR_TYPE	psfd, aq_ratio, psfdc;

  psfd		= GetSample(varp, 0);
  aq_ratio	= GetSample(varp, 1);

  psfdc = psfd + (*pcorPSFD)(aq_ratio, 1.0);

  PutSample(varp, psfdc);

}	/* END SPSFDC */

/* END PSFDC.C */
