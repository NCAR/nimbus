/************************* PSBC **********************************  PSBC
 ** Corrected static pressure (boom) (mb)
 	Input:
 		psb - raw static pressure, boom mounted
 		qcx - raw default dynamic pressure
 	Output:
 		psbc - corrected static pressure
 	Include:
 		pcorb - boom dynamic pressure correction
*/

#include "nimbus.h"
#include "amlib.h"

extern NR_TYPE	(*pcorPSB)(NR_TYPE);

/* -------------------------------------------------------------------- */
void spsbc(DERTBL *varp)
{
  NR_TYPE	psb, qcx, psbc;

  psb = GetSample(varp, 0);
  qcx = GetSample(varp, 1);

  psbc = psb + (*pcorPSB)(qcx);

  if (qcx < 10.0)
    psbc = psb;

  if (psbc <50.0)
    psbc = 50.0;

  PutSample(varp, psbc);

}	/* END SPSBC */

/* END PSBC.C */
