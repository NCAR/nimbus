/********************* ATRF *************************************  ATRF
 ** AMBIENT TEMPERATURE (REVERSE FLOW)(C)
 	Input:
 		ttrf = raw reverse flow temperature
 		xmach2 - derived mach number
 	Output:
 		atrf = corrected ambient reverse flow temperature
 	Include:
 		recfrn = recovery factor
*/

#include "nimbus.h"
#include "amlib.h"
 
extern NR_TYPE	recfrn;

/* -------------------------------------------------------------------- */
void satrf(DERTBL *varp)
{
	NR_TYPE	ttrf, xmach2;

	ttrf	= GetSample(varp, 0);
	xmach2	= GetSample(varp, 1);
	
	if (ttrf < -273.15)
		ttrf  = -273.15;

	PutSample(varp, AMBIENT(ttrf, recfrn, xmach2));

}
