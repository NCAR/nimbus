/**************************** QCBC *******************************  QCBC
 ** Corrected dynamic pressure (boom) (mb)
 	Input:
 		qcb - raw dynamic pressure, boom mounted
 	Output:
 		qcbc - corrected dynamic pressure, boom
 	Include:
 		pcorb - dynamic pressure correction, boom
*/

#include "nimbus.h"
#include "amlib.h"

extern NR_TYPE (*pcorQCB)(NR_TYPE, NR_TYPE);

/* -------------------------------------------------------------------- */
void sqcbc(DERTBL *varp)
{
  NR_TYPE	qcb,qcbc;

  qcb	= GetSample(varp, 0);
  qcbc	= qcb - (*pcorQCB)(qcb,1.0);

  if (qcbc < 10.0)
    qcbc = qcb;

  if (qcbc < 0.001)
    qcbc =  0.001;

  PutSample(varp, qcbc);
}
