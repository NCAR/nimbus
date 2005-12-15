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
  NR_TYPE qcf, qcfc;

  qcf = GetSample(varp, 0);

  switch (Aircraft)
  {
    case HIAPER:
      NR_TYPE aqratio = GetSample(varp, 1);
      NR_TYPE psf = GetSample(varp, 2);
      qcfc = qcf - (*pcorQCF)(psf, aqratio);
      break;

    case C130:
      NR_TYPE akrd  = GetSample(varp, 1);
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
