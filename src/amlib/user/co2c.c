/*
-------------------------------------------------------------------------
OBJECT NAME:	co2c.c

FULL NAME:	CO2 concentrations

ENTRY POINTS:	sco2c()
                xco_al()

STATIC FNS:	none

DESCRIPTION:	As far as I know this is/was only used for project 280,
		Fires In Brazil.

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

/*
-------------------------------------------------------------------------
OBJECT NAME:	co_al.c

FULL NAME:	CO Mixing Ratio

ENTRY POINTS:	sco_al()

DESCRIPTION:	Calculate CO mixing ration of the AL instrument.  This
		just applies a calibration.  Seperate variable so that
		raw variable is available also.

COPYRIGHT:	University Corporation for Atmospheric Research, 2010.
-------------------------------------------------------------------------
*/

static NR_TYPE CO_CAL[3] = { 7000.0, 170.0, 0.0 };

void initCOAL(var_base *varp)
{
  float *tmp;

  if ( (tmp = GetDefaultsValue("CO_CAL", varp->name)) )
  {
    for (int i = 0; i < 2; ++i)
      CO_CAL[i] = tmp[i];
  }
}

/* -------------------------------------------------------------------- */
void sco_al(DERTBL *varp)
{
  NR_TYPE	xcoraw_al, xco_al;

  xcoraw_al = GetSample(varp, 0);

   /* CO mixing ratio according to Teresa Campos */
    xco_al = (xcoraw_al - CO_CAL[0]) / CO_CAL[1];


  /* co2 concentration calculation - 308.11 is licor std. cal. temp. */
  PutSample(varp, xco_al);

}	/* END SCO_AL */

/* END CO2C.C */
