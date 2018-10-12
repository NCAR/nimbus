/*  University of Wyoming CCN	Wispit (3-278) Jeffco February, 1993	*/

#include "nimbus.h"
#include "amlib.h"
#include <cmath>

/* -------------------------------------------------------------------- */
void sconccn(DERTBL *varp)
{
	NR_TYPE	vdet;

	vdet = fabs((double)GetSample(varp, 0));

	PutSample(varp, 137.97 * pow((double)vdet, (double)0.54));
}
