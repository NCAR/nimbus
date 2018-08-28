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

extern NR_TYPE (*pcorQCF)(NR_TYPE);
extern NR_TYPE (*pcorQCFv2)(NR_TYPE, NR_TYPE, NR_TYPE);

/* -------------------------------------------------------------------- */
void sqcfc(DERTBL *varp)
{
  NR_TYPE qcfc, mach;

  NR_TYPE psf = GetSample(varp, 0);
  NR_TYPE qcf = GetSample(varp, 1);

  if (qcf < 0.01)
    qcf = 0.01;

  if (cfg.Aircraft() == Config::HIAPER || cfg.Aircraft() == Config::C130)
  {
    NR_TYPE attack = GetSample(varp, 2);

    qcfc = qcf - (*pcorQCFv2)(qcf, psf, attack);
  }
  else
  {
    qcfc = qcf - (*pcorQCF)(qcf);
  }

  if (qcfc < 10.0)
    qcfc = qcf;

  if (qcfc < 0.001)
    qcfc =  0.001;

  PutSample(varp, qcfc);
}
