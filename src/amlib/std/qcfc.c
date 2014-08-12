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
  NR_TYPE qcfc, mach;

  NR_TYPE psf = GetSample(varp, 0);
  NR_TYPE qcf = GetSample(varp, 1);

  if (cfg.Aircraft() == Config::HIAPER)
  {
    NR_TYPE attack = GetSample(varp, 2);
    NR_TYPE qcr = GetSample(varp, 3);
    NR_TYPE M = sqrt( (2.0 * Cv / Rd) * (pow((qcf+psf)/psf, Rd_DIV_Cpd) - 1.0) );
    qcfc = qcf - 
	(psf * (-0.01317 + 0.07256 * qcf / psf - 0.08611 * M*M*M + 0.002253 * attack/qcr));
  }
  else if (cfg.Aircraft() == Config::C130 && varp->ndep > 2)
  {
    NR_TYPE adifr = GetSample(varp, 2);
    NR_TYPE qcr = GetSample(varp, 3);
    NR_TYPE bdifr = GetSample(varp, 4);
    NR_TYPE R1 = (14.28 * (adifr / qcf) + 5.78 - 5.57) / 16.71;
    qcfc = qcf - psf * (0.00186 + 0.0202 * R1 + 0.0135 * mach);
/*
    qcfc = qcf -
	(psf * (-0.0416 + 0.01485 * (adifr/qcr) + 0.37296 * mach - 1.17 * mach*mach + 0.90159 * mach*mach*mach
	+ 0.23147 * qcf/psf + 0.00239 * bdifr/qcr));
*/
  }
  else if (cfg.Aircraft() == Config::C130 && varp->ndep > 1)	// Older C130 (ICE-T and older).
  {
    NR_TYPE akrd  = GetSample(varp, 1);
    qcfc = qcf - (*pcorQCF)(akrd, 1.0);
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
