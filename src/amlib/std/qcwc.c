/**************************** QCWC *******************************  QCWC
 ** Corrected dynamic pressure (wing) (mb)
 	Input:
 		qcw - raw dynamic pressure, wing mounted
 	Output:
 		qcwc - corrected dynamic pressure, wing
 	Include:
 		pcorw - dynamic pressure correction, wing
*/

#include "nimbus.h"
#include "amlib.h"

extern NR_TYPE (*pcorQCW)(NR_TYPE);

/* -------------------------------------------------------------------- */
void sqcwc(DERTBL *varp)
{
  NR_TYPE	qcw,qcwc;

  qcw = GetSample(varp, 0);
  qcwc= qcw - (*pcorQCW)(qcw);

  if (qcwc < 10.0)
    qcwc = qcw;

  if (qcwc < 0.001)
    qcwc =  0.001;

  PutSample(varp, qcwc);
}
