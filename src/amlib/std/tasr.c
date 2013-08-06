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

extern NR_TYPE compute_tas(NR_TYPE, NR_TYPE, NR_TYPE);
extern NR_TYPE recfra;

/* -------------------------------------------------------------------- */
void stasr(DERTBL *varp)
{
  NR_TYPE q	= GetSample(varp, 0);
  NR_TYPE p	= GetSample(varp, 1);
  NR_TYPE tt	= GetSample(varp, 2);

  NR_TYPE mach2	= XMAC2(q / p);
  NR_TYPE tas	= compute_tas(tt, recfra, mach2);

  PutSample(varp, tas);
}
