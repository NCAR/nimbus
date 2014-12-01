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

extern NR_TYPE (*pcorQCF)(NR_TYPE, NR_TYPE);
NR_TYPE pcorf5_3(NR_TYPE Qm, NR_TYPE Pm, NR_TYPE Attack);

/* -------------------------------------------------------------------- */
void sqcfc(DERTBL *varp)
{
  NR_TYPE qcfc, mach;

  NR_TYPE psf = GetSample(varp, 0);
  NR_TYPE qcf = GetSample(varp, 1);

  if (qcf < 0.01)
    qcf = 0.01;

  if (cfg.Aircraft() == Config::HIAPER)
  {
    NR_TYPE attack = GetSample(varp, 2);

    qcfc = qcf - pcorf5_3(qcf, psf, attack);
  }
  else if (cfg.Aircraft() == Config::C130 && varp->ndep > 2)
  {
    NR_TYPE adifr = GetSample(varp, 2);
    NR_TYPE R1 = (14.28 * (adifr / qcf) + 5.78 - 5.57) / 16.71;
    qcfc = qcf - psf * (0.00186 + 0.0202 * R1 + 0.0135 * mach);
  }
  else if (cfg.Aircraft() == Config::C130 && varp->ndep > 1)	// Older C130 (ICE-T and older).
  {
    NR_TYPE attack  = GetSample(varp, 1);
    qcfc = qcf - (*pcorQCF)(attack, 1.0);
  }
  else
  {
    qcfc = qcf - (*pcorQCF)(qcf, 1.0);
  }

  if (qcfc < 10.0)
    qcfc = qcf;

  if (qcfc < 0.001)
    qcfc =  0.001;

  PutSample(varp, qcfc);
}
