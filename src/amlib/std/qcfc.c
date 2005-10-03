/**************************** QCFC *******************************  QCFC
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
#include "raf.h"

extern NR_TYPE (*pcorQCF)(NR_TYPE, NR_TYPE);

extern int	Aircraft;

/* -------------------------------------------------------------------- */
void sqcfc(DERTBL *varp)
{
  NR_TYPE	qcf, akrd, qcfc;

  qcf	= GetSample(varp, 0);
  akrd  = GetSample(varp, 1);

  switch (Aircraft)
  {
    case C130:
      qcfc = qcf - (*pcorQCF)(akrd, 1.0);
      break;

    default:
      qcfc = qcf - (*pcorQCF)(qcf, 1.0);
  }

  if (qcfc < 10.0)
    qcfc = qcf;

  if (qcfc < 0.001)
    qcfc =  0.001;

  PutSample(varp, qcfc);
}
