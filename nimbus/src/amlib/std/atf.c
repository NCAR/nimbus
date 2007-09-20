/*******       AMBIENT TEMPERATURE (FUSELAGE)       (C)                 ATF
                  REQUIRES --- TTF, RECFF, XMACH2, AMBT FCN.
 	Input:
 		ttf - raw total temperature
 		xmach2 - derived mach number
 	Output:
 		atf - derived ambient temperature (C)
 	Include:
 		recff - recovery factor 
 		ambtf - ambient temperature function
*/

#include "nimbus.h"
#include "amlib.h"

extern NR_TYPE	recff;

/* -------------------------------------------------------------------- */
void satf(DERTBL *varp)
{
	NR_TYPE	ttf, xmach2;

	ttf	= GetSample(varp, 0);
	xmach2	= GetSample(varp, 1);

	if (ttf < -Kelvin)
		ttf  = -Kelvin;

	PutSample(varp, AMBIENT(ttf,recff,xmach2));

}	/* END SATF */

/* END ATF.C */
