/*
-------------------------------------------------------------------------
OBJECT NAME:	sail.c

FULL NAME:	Misc Sailplane items.

ENTRY POINTS:	sspias()
		sspalt()

STATIC FNS:	none

DESCRIPTION:	Sailplane stuff, AirSpeed, Alt.

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	compute.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1995
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void sspias(DERTBL *varp)
{
	NR_TYPE	qcx, ias;

	qcx = GetSample(varp, 0);

	ias = 2.858236 * sqrt((double)qcx * 100.0);

	PutSample(varp, ias);

}	/* END SSPIAS */

/* -------------------------------------------------------------------- */
void sspalt(DERTBL *varp)
{
	NR_TYPE	psx, palt, p;

	psx = GetSample(varp, 0);

	p = 0.1903 * log(psx / 1013.25);
	palt = 44.287 * (1.0 - exp(p));

	PutSample(varp, palt);

}	/* END SSPALT */

/* END SAIL.C */
