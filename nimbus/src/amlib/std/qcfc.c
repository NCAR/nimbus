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


/* -------------------------------------------------------------------- */
void sqcfc(DERTBL *varp)
{
  NR_TYPE qcf, qcfc, aqratio, mach2, mach, psf;

  qcf = GetSample(varp, 0);

  if (cfg.Aircraft() == Config::HIAPER)
  {
    aqratio = GetSample(varp, 1);	// aqratio = qcf / psf
    psf = GetSample(varp, 2);
    mach = GetSample(varp, 3);
    qcfc = qcf - psf * (*pcorQCF)(aqratio, mach);
  }
  else if (cfg.Aircraft() == Config::C130 && varp->ndep > 1)
  {
    aqratio  = GetSample(varp, 1);	// aqratio = adifr / qcf
    psf = GetSample(varp, 2);
    mach2 = GetSample(varp, 3);
    qcfc = qcf - psf * (*pcorQCF)(aqratio, mach2);
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
