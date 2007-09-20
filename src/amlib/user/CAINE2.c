/*
-------------------------------------------------------------------------
OBJECT NAME:	func.c

FULL NAME:	

ENTRY POINTS:	sfunc()

STATIC FNS:	

DESCRIPTION:	Template for AMLIB std & user directories.

INPUT:		

OUTPUT:		

REFERENCES:	

REFERENCED BY:	compute.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1995
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static const NR_TYPE
	m[2] = { 0.001387, 0.00128 },
	b[2] = { 0.00002152, 0.0000137 },
	p0 = 101325.0,
	t0 = 293.0,
	d[2] = { 0.00635, 0.00635 };

/* -------------------------------------------------------------------- */
void sxisore(DERTBL *varp)
{
	NR_TYPE	isor, F, atx, psx, tasx;

	F = GetSample(varp, 0);
	atx = GetSample(varp, 1);
	psx = GetSample(varp, 2);
	tasx = GetSample(varp, 3);

	isor =	(((F * m[0] + b[0]) * ( atx + Kelvin )) / ( 100. *psx * tasx)) *
		((4.0 * p0) / (M_PI * t0 * d[0] * d[0]));

	PutSample(varp, isor);

}	/* END SFUNC */

/* -------------------------------------------------------------------- */
void sxisort(DERTBL *varp)
{
	NR_TYPE	isor, F, atx, psx, tasx;

	F = GetSample(varp, 0);
	atx = GetSample(varp, 1);
	psx = GetSample(varp, 2);
	tasx = GetSample(varp, 3);

	isor =	(((F * m[1] + b[1]) * ( atx + Kelvin )) / ( 100. *psx * tasx)) *
		((4.0 * p0) / (M_PI * t0 * d[1] * d[1]));

	PutSample(varp, isor);

}

/* END CAINE2.C */
