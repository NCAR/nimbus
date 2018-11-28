/**************************** QCTFC *******************************  QCTFC
 ** Corrected dynamic pressure (bird strike) (mb)
 	Input:
 		qctf - raw dynamic pressure, bird strike zone mounted
 	Output:
 		qctfc - corrected dynamic pressure, bird strike zone
 	Include:
 		pcortf - dynamic pressure correction, bird strike zone
*/

#include "nimbus.h"
#include "amlib.h"

extern NR_TYPE (*pcorQCTF)(NR_TYPE, NR_TYPE, NR_TYPE, NR_TYPE, NR_TYPE);

/* -------------------------------------------------------------------- */
void sqctfc(DERTBL *varp)
{
  NR_TYPE qctfc;

  NR_TYPE qctf = GetSample(varp, 0);
  NR_TYPE pstf = GetSample(varp, 1);

  if (qctf < 0.01)
    qctf = 0.01;

  if (varp->nDependencies > 3)	// Lenschow method
  {
    NR_TYPE attack = GetSample(varp, 2);
    NR_TYPE sslip = GetSample(varp, 3);
    NR_TYPE mach = GetSample(varp, 4);
    qctfc = qctf + (*pcorQCTF)(qctf, pstf, attack, sslip, mach);
  }
  else	// Cooper method
  {
    NR_TYPE psfc = GetSample(varp, 2);
    qctfc = qctf + pstf - psfc;	// LRT only.  Will need to befilteres/smoothed for HRT.
  }

  if (qctfc < 10.0)
    qctfc = qctf;

  if (qctfc < 0.001)
    qctfc =  0.001;

  PutSample(varp, qctfc);
}
