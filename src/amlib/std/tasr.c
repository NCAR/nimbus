/********************* TASR ******************************************** TASR
 ***** CALC TRUE AIR SPEED USING QCRXC,PSRXC,TTRX                        TASR
 	Input:
 		qcxc - derived dynamic pressure
 		psxc - derived static pressure
 		ttrx - raw temperature
 	Output:
 		tasr - true airspeed from radome variables
 	Include:
 		tas - true airspeed function
*/

#include "nimbus.h"
#include "amlib.h"

extern NR_TYPE tas(NR_TYPE, NR_TYPE, NR_TYPE);
extern NR_TYPE recfra;

/* -------------------------------------------------------------------- */
void stasr(DERTBL *varp)
{
  NR_TYPE	rmach2;
  NR_TYPE	ttrx,qcxc,psxc,tasr;

  qcxc	= GetSample(varp, 0);
  psxc	= GetSample(varp, 1);
  ttrx	= GetSample(varp, 2);

  rmach2 = XMAC2(qcxc / psxc);
  tasr	= tas(ttrx, recfra, rmach2);

  PutSample(varp, tasr);
}
