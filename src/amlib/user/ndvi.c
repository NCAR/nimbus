/*  Veggie meter calculation for test flights  Sept,Oct 91 */

#include "nimbus.h"
#include "amlib.h"

/* -------------------------------------------------------------------- */
void sndvi(DERTBL *varp)
{
	NR_TYPE	fi862, fi650;

	fi862 = GetSample(varp, 0);
	fi650 = GetSample(varp, 1);

	PutSample(varp, (fi862 - fi650) / (fi862 + fi650));
}
