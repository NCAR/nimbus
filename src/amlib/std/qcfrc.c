/**************************** QCFRC *******************************  QCFC
 ** Corrected dynamic pressure (fuselage) (mb)
 	Input:
 		qcf - raw dynamic pressure, fuselage mounted
 	Output:
 		qcfc - corrected dynamic pressure, fuselage
 	Include:
 		pcorf - dynamic pressure correction, fuselage
*/

#include "nimbus.h"
#include "amlib.h"

extern NR_TYPE (*pcorQCFRv2)(NR_TYPE, NR_TYPE, NR_TYPE);

/* -------------------------------------------------------------------- */
void sqcfrc(DERTBL *varp)
{
  NR_TYPE	qcfc;

  NR_TYPE qcf	= GetSample(varp, 0);
  NR_TYPE psf   = GetSample(varp, 1);
  NR_TYPE attack= GetSample(varp, 2);

  qcfc = qcf - (*pcorQCFRv2)(qcf, psf, attack);

  if (qcfc < 10.0)
    qcfc = qcf;

  if (qcfc < 0.001)
    qcfc =  0.001;

  PutSample(varp, qcfc);
}
