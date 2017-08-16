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

extern NR_TYPE (*pcorQCTF)(NR_TYPE, NR_TYPE, NR_TYPE, NR_TYPE);

/* -------------------------------------------------------------------- */
void sqctfc(DERTBL *varp)
{
  NR_TYPE qctfc, mach;

  NR_TYPE pstf = GetSample(varp, 0);
  NR_TYPE qctf = GetSample(varp, 1);

  if (qctf < 0.01)
    qctf = 0.01;

  if (cfg.Aircraft() == Config::HIAPER)
  {
    NR_TYPE attack = GetSample(varp, 2);
    NR_TYPE sslip = GetSample(varp, 3);

    qctfc = qctf - (*pcorQCTF)(qctf, pstf, attack, sslip);
  }

  if (qctfc < 10.0)
    qctfc = qctf;

  if (qctfc < 0.001)
    qctfc =  0.001;

  PutSample(varp, qctfc);
}
