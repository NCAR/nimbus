/*******       AIRCRAFT TRUE AIRSPEED WING  (M/S)                 * TASW
                REQUIRES --- TTW,RECFW ,WMACH2, TAS FCN.
 	Input:
 		qcwc - derived dynamic pressure
 		pswc - derived static pressure
 		ttw - raw temperature
 	Output:
 		tasw - derived true airspeed
 	Include:
 		xmac2 - mach number function
 		tas - true airspeed function
 		recfw - recovery factor
*/

#include "nimbus.h"
#include "amlib.h"

extern NR_TYPE tas(NR_TYPE, NR_TYPE, NR_TYPE);
extern NR_TYPE recfw;

/* -------------------------------------------------------------------- */
void stasw(DERTBL *varp)
{
  NR_TYPE	wmach2;
  NR_TYPE	ttw,qcwc,pswc,tasw;

  qcwc	= GetSample(varp, 0);
  pswc	= GetSample(varp, 1);
  ttw	= GetSample(varp, 2);

  wmach2 = XMAC2(qcwc / pswc);
  tasw	= tas(ttw, recfw, wmach2);

  PutSample(varp, tasw);
}
