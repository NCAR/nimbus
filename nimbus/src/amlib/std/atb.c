/*******       AMBIENT TEMPERATURE (BOOM ROSEMOUNT) (C)                 ATB 
                   REQUIRES --- TTB, RECFB, XMACH2, AMBT FCN.
  	Input:
  		ttb - raw total temperature
  		xmach2 - derived mach number
  	Output:
  		atb - derived ambient temperature (C)
  	Include:
  		recfb - recovery factor 
  		ambtf - ambient temperature function
*/

#include "nimbus.h"
#include "amlib.h"

extern NR_TYPE recfb;

/* -------------------------------------------------------------------- */
void satb(DERTBL *varp)
{
	NR_TYPE	ttb, xmach2;

	ttb	= GetSample(varp, 0);
	xmach2	= GetSample(varp, 1);
	
	if (ttb < -Kelvin)
		ttb  = -Kelvin;

	PutSample(varp, AMBIENT(ttb, recfb, xmach2));

}	/* END SATB */

/* END SATB.C */
