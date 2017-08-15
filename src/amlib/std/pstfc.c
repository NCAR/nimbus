/************************* PSTFC **********************************  PSTFC
 ** Corrected static pressure (top of fuselage) (mb)
 	Input:
 		pstf - raw static pressure, top of fuselage mounted
 		qctf - raw dynamic pressure, top of fuselage mounted
 	Output:
 		pstfc - corrected static pressure
 	Include:
 		pcorf - fuselage dynamic pressure correction
*/

#include "nimbus.h"
#include "amlib.h"

extern NR_TYPE (*pcorPSTF)(NR_TYPE, NR_TYPE, NR_TYPE, NR_TYPE);

/* -------------------------------------------------------------------- */
void spstfc(DERTBL *varp)
{
  NR_TYPE pstf, qctf, pstfc;

  pstf = GetSample(varp, 0);
  qctf = GetSample(varp, 1);

  if (cfg.Aircraft() == Config::HIAPER)
  {
    NR_TYPE attack = GetSample(varp, 2);
    NR_TYPE sslip = GetSample(varp, 3);

    pstfc = pstf + (*pcorPSTF)(qctf, pstf, attack, sslip);
  }

//  if (pstfc < 50.0)
//    pstfc = 50.0;

  PutSample(varp, pstfc);

}	/* END SPSTFC */

/* END PSTFC.C */
