/*******       AIRCRAFT TRUE AIRSPEED FUSELAGE  (M/S)                 * TASF
                REQUIRES --- TTF,RECFF ,FMACH2, TAS FCN.
 	Input:
 		qcfc - derived dynamic pressure
 		psfc - derived static pressure
 		ttf - raw temperature
 	Output:
 		tasf - derived true airspeed
 	Include:
 		mac2 - mach number function
 		tas - true airspeed function
 		recff - recovery factor
*/

#include "nimbus.h"
#include "amlib.h"

extern NR_TYPE compute_tas(NR_TYPE, NR_TYPE, NR_TYPE);
extern NR_TYPE recff;

/* -------------------------------------------------------------------- */
void stasf(DERTBL *varp)
{
  NR_TYPE q	= GetSample(varp, 0);
  NR_TYPE p	= GetSample(varp, 1);
  NR_TYPE tt	= GetSample(varp, 2);

  NR_TYPE mach2	= XMAC2(q / p);
  NR_TYPE tas	= compute_tas(tt, recff, mach2);

  PutSample(varp, tas);
}
