/*
-------------------------------------------------------------------------
OBJECT NAME:	co2c.c

FULL NAME:	CO2 concentrations

ENTRY POINTS:	sco2c()

STATIC FNS:	none

DESCRIPTION:	As far as I know this is/was only used for project 280,
		Fires In Brazil.

REFERENCES:	none

REFERENCED BY:	compute.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void sco2c(DERTBL *varp)
{

	NR_TYPE	xco2s, xco2t, psfdc;
	NR_TYPE	pc,x,v,p,fx;

	xco2s = GetSample(varp, 0);
	xco2t = GetSample(varp, 1);
	psfdc = GetSample(varp, 2);

	v = xco2s * 1000.0;	/* convert co2 signal to mv */

	/* apply pressure correction to adjust ambient pressure to co2
	 * cell pressure
	 */
	if (psfdc > 735.0)
		pc = 0.906;
	else
		pc = ((psfdc / 10.0) * 0.0018) + 0.7617;

	p = (psfdc / 10.0) * pc;
	x = v * (101.3 / p);

	/* calibrate co2 signal */
	fx = x * (0.1362 + x * (2.008e-5 + 2.029e-9 * x));

	/* co2 concentration calculation - 308.11 is licor std. cal. temp. */
	PutSample(varp, fx * (xco2t + 273.0) / 308.11);

}	/* END SCO2C */

/* END CO2C.C */
