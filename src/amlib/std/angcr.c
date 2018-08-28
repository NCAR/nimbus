/*
-------------------------------------------------------------------------
OBJECT NAME:	angcr.c

FULL NAME:	Angle Correction

ENTRY POINTS:	angcr()

STATIC FNS:	none

DESCRIPTION:	This subroutine links the syncro angle A2 to the previous
		value A1 by a tolerance test on 1/2 of the syncro range.
		Angle a2 is then put in the range of AMIN to AMIN+ACYCLE,
		where ACYCLE is generallY 360 degrees.  AMIN is generally
		-180 or 0 degrees.

INPUT:		

OUTPUT:		float corrected angle

REFERENCES:	none

REFERENCED BY:	hgme.c, thf.c, ins.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static const NR_TYPE aCycle = 360.0;

/* -------------------------------------------------------------------- */
NR_TYPE angcr(
	NR_TYPE	angle2,
	NR_TYPE	angle1,
	float	syncro_range,
	float	amin)
{
	NR_TYPE	amax	= amin + aCycle,
		tol	= syncro_range / 2.0,
		d;


	while (fabs((double)(d = angle2 - angle1)) > tol)
		{
		if (d < 0.0)
			angle2 += syncro_range;
		else
		if (d > 0.0)
			angle2 -= syncro_range;
		else
			LogStdMsg("angcr: Impossible.");
		}


	/* Bring angle into range
	 */
	while (angle2 < amin)
		angle2 += aCycle;

	while (angle2 > amax)
		angle2 -= aCycle;

	return(angle2);

}	/* END ANGCR */

/* END ANGCR.C */
