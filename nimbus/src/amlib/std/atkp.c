/*******       AMBIENT TEMPERATURE (FAST RESPONSE)   (C)                ATKP
                  REQUIRES --- TTKP,RECFKP,XMACH2,AMBT FCN.
 	Input:
 		ttkp - raw total temperature
 		xmach2 - derived mach number
 	Output:
 		atkp - derived ambient temperature (C)
 	Include:
 		recfkp - recovery factor 
 		ambtf - ambient temperature function
*/

#include "nimbus.h"
#include "amlib.h"

extern NR_TYPE recfkp;

/* -------------------------------------------------------------------- */
void satkp(DERTBL *varp)
{
	NR_TYPE	ttkp, xmach2;

	ttkp	= GetSample(varp, 0);
	xmach2	= GetSample(varp, 1);
	
	if (ttkp < -273.15)
		ttkp  = -273.15;

	PutSample(varp, AMBIENT(ttkp, recfkp, xmach2));

}
