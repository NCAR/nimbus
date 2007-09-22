/*******       AIRCRAFT TRUE AIRSPEED FUSELAGE  (M/S)                 * TASF
                REQUIRES --- TTF,RECFF ,FMACH2, TAS FCN.
 	Input:
 		qcfc - derived dynamic pressure
 		psfc - derived static pressure
 		ttf - raw temperature
 	Output:
 		tasf - derived true airspeed
 	Include:
 		xmac2 - mach number function
 		tas - true airspeed function
 		recff - recovery factor
*/

#include "nimbus.h"
#include "amlib.h"

extern NR_TYPE tas(NR_TYPE, NR_TYPE, NR_TYPE);
extern NR_TYPE recff;

/* -------------------------------------------------------------------- */
void stasf(DERTBL *varp)
{
  NR_TYPE	fmach2;
  NR_TYPE	ttf,qcfc,psfc,tasf;

  qcfc	= GetSample(varp, 0);
  psfc	= GetSample(varp, 1);
  ttf	= GetSample(varp, 2);

  fmach2 = XMAC2(qcfc/psfc);
  tasf	= tas(ttf,recff,fmach2);

  PutSample(varp, tasf);
}
