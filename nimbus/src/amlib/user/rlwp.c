/*
-------------------------------------------------------------------------
OBJECT NAME:	rlwp.c

FULL NAME:	Russian Liquid Water Probe

ENTRY POINTS:	srlwc()

STATIC FNS:	none

DESCRIPTION:	LWC = Koeff * V^2 / (R * S * U * (L + c * (Tprobe - Tair)))

		V = output signal ranged from 0 to +10 volts
		R = is resistance of sensor
		S = sample area of sensor
		U = True airspeed
		L = latent heat of water evaporation
		c = heat capacity of water

INPUT:		XLWC, TASX

REFERENCES:	none

REFERENCED BY:	compute.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

#define Koeff		1.0		/* Varied between 0.7 - 1.0	*/
#define RESISTANCE	0.0001
#define SAMPLE_AREA	1.0
#define LATENT_HEAT	2500		/* J/g		*/
#define HEAT_CAPACITY	4.2		/* J/g/degree K	*/
#define Tprobe		90.0		/* C		*/

/* -------------------------------------------------------------------- */
void srlwc(DERTBL *varp)
{
	NR_TYPE	rlwc, lwc, tas, at;

	lwc	= GetSample(varp, 0);
	tas	= GetSample(varp, 1);
	at	= GetSample(varp, 2);

	rlwc =	Koeff * (lwc * lwc);
	rlwc /= (RESISTANCE * SAMPLE_AREA * tas * (LATENT_HEAT + HEAT_CAPACITY
		* (Tprobe - at)));

	PutSample(varp, rlwc);

}	/* END SRLWC */

/* END RLWP.C */
