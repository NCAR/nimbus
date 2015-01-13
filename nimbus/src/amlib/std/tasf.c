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
  NR_TYPE mach	= GetSample(varp, 0);
  NR_TYPE at	= GetSample(varp, 1);

  NR_TYPE tas	= compute_tas(at, recff, mach);

  PutSample(varp, tas);
}
