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
  NR_TYPE	psfd, akrd, psfdc;

  psfd	= GetSample(varp, 0);
  akrd	= GetSample(varp, 1);

  psfdc = psfd + (*pcorPSFD)(akrd,1.0);

  if (qcx < 10.0)
    psfdc = psfd;

  PutSample(varp, psfdc);

}	/* END SPSFDC */

/* END PSFDC.C */
