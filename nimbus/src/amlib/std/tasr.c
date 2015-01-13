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
  NR_TYPE mach	= GetSample(varp, 0);
  NR_TYPE at	= GetSample(varp, 1);

  NR_TYPE tas	= compute_tas(at, recfra, mach);

  PutSample(varp, tas);
}
