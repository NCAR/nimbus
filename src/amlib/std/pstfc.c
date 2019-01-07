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

extern NR_TYPE (*pcorPSTF)(NR_TYPE, NR_TYPE, NR_TYPE, NR_TYPE, NR_TYPE);

/* -------------------------------------------------------------------- */
void spstfc(DERTBL *varp)
{
  NR_TYPE pstfc;

  NR_TYPE pstf = GetSample(varp, 0);
  NR_TYPE qctf = GetSample(varp, 1);
  NR_TYPE attack = GetSample(varp, 2);
  NR_TYPE sslip = GetSample(varp, 3);
  NR_TYPE mach = GetSample(varp, 4);

  pstfc = pstf - (*pcorPSTF)(qctf, pstf, attack, sslip, mach);

  if (pstfc < 50.0)
    pstfc = 50.0;

  PutSample(varp, pstfc);

}	/* END SPSTFC */

/* END PSTFC.C */
