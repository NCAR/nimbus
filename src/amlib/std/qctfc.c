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

extern NR_TYPE (*pcorQCTF)(NR_TYPE, NR_TYPE, NR_TYPE, NR_TYPE, NR_TYPE);

NR_TYPE defaultATTACK(), defaultSSLIP();

/* -------------------------------------------------------------------- */
void sqctfc(DERTBL *varp)
{
  NR_TYPE qctfc;

  NR_TYPE qctf = GetSample(varp, 0);
  NR_TYPE pstf = GetSample(varp, 1);

  if (qctf < 0.1)
    qctf = 0.1;

  if (varp->nDependencies > 3)	// Lenschow method
  {
    NR_TYPE attack = GetSample(varp, 2);
    NR_TYPE sslip = GetSample(varp, 3);
    NR_TYPE mach = GetSample(varp, 4);

    if (std::isnan(attack))
      attack = defaultATTACK();
    if (std::isnan(sslip))
      attack = defaultSSLIP();

    qctfc = qctf + (*pcorQCTF)(qctf, pstf, attack, sslip, mach);
  }
  else	// Cooper method
  {
    NR_TYPE psffc = GetSample(varp, 2);
    qctfc = qctf + pstf - psffc;
  }

  if (qctfc < 10.0)
    qctfc = qctf;

  if (qctfc < 0.1)
    qctfc =  0.1;

  PutSample(varp, qctfc);
}
