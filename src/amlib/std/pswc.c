/************************* PSWC **********************************  PSWC
 ** Corrected static pressure (wing) (mb)
 	Input:
 		psw - raw static pressure, wing mounted
 		qcx - raw default dynamic pressure
 	Output:
 		pswc - corrected static pressure
 	Include:
 		pcorw - wing dynamic pressure correction
*/

#include "nimbus.h"
#include "amlib.h"

extern NR_TYPE (*pcorPSW)(NR_TYPE, NR_TYPE);

/* -------------------------------------------------------------------- */
void spswc(DERTBL *varp)
{
  NR_TYPE	psw,qcx,pswc;

  psw = GetSample(varp, 0);
  qcx = GetSample(varp, 1);

  pswc= psw + (*pcorPSW)(qcx,1.0);

  if (qcx < 10.0)
    pswc = psw;

  if (pswc < 50.0)
    pswc = 50.0;

  PutSample(varp, pswc);

}	/* END SPSWC */

/* END PSWC.C */
