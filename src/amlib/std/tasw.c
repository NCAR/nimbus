/*******       AIRCRAFT TRUE AIRSPEED WING  (M/S)                 * TASW
                REQUIRES --- TTW,RECFW ,WMACH2, TAS FCN.
 	Input:
 		qcwc - derived dynamic pressure
 		pswc - derived static pressure
 		ttw - raw temperature
 	Output:
 		tasw - derived true airspeed
 	Include:
 		mac2 - mach number function
 		tas - true airspeed function
 		recfw - recovery factor
*/

#include "nimbus.h"
#include "amlib.h"

extern NR_TYPE compute_tas(NR_TYPE, NR_TYPE, NR_TYPE);
extern NR_TYPE recfw;

/* -------------------------------------------------------------------- */
void stasw(DERTBL *varp)
{
  NR_TYPE q	= GetSample(varp, 0);
  NR_TYPE p	= GetSample(varp, 1);
  NR_TYPE tt	= GetSample(varp, 2);

  NR_TYPE mach2	= XMAC2(q / p);
  NR_TYPE tas	= compute_tas(tt, recfw, mach2);

  PutSample(varp, tas);
}
