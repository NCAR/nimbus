/**************************** PLWCC *******************************  PLWCC
 ** Corrected PMS liquid water content (g/m3)
 	Input:
 		plwc - raw liquid water content
 		tasx - derived true air speed
 		atx - derived ambient temperature
 		psxc - derived static pressure
		twire - wire temperature(degC) for this probe
 	Output:
 		plwcc - corrected PMS liquid water content
*/

#include "nimbus.h"
#include "amlib.h"
 
/*  From Darrel Baumgardner  OCT. 1990   -cc
			King Probe Liquid Water Content Code
*/

static const NR_TYPE L1 = 2.1;	/* length of long King probe (cm) */

/* -------------------------------------------------------------------- */
NR_TYPE kinglwcc(NR_TYPE plwc, NR_TYPE tasx, NR_TYPE atx, NR_TYPE psxc, NR_TYPE twire, NR_TYPE diam)
{
	NR_TYPE	tk, twk1, tflm1, cnd1, cndw1, visc1, vscw1, dens1, fct1,
		re1, prf1, prw1, dryp1, fact;


	tk = atx+Kelvin; 
	twk1 = twire+Kelvin;
	tflm1 = (twk1+tk)/2.;
	if(tflm1 == 0.0) tflm1 = 0.001;

	/* Calculate the thermal conductivity  */
	cnd1 = 5.8e-5*(398./(125.+tflm1))*pow(tflm1/Kelvin, 1.5);
	cndw1 = 5.8e-5*(398./(125.+twk1))*pow(twk1/Kelvin, 1.5); 

	/* Calculate the viscosity  */
	visc1 = 1.718e-4*(393./(120.+tflm1))*pow(tflm1/Kelvin, 1.5);
	vscw1 = 1.718e-4*(393./(120.+twk1))*pow(twk1/Kelvin, 1.5);

	/* Calculate the density  */
	dens1 = psxc/(2870.5*tflm1);
	fct1 = 3.14159*L1*cnd1*(twk1-tk);
        if(visc1 == 0.0 ) visc1 = 0.001;
        if(cnd1 == 0.0 ) cnd1 = 0.001;
        if(cndw1 == 0.0 ) cndw1 = 0.001;
	re1 = 100.0 * dens1 * tasx * diam / visc1; 
	prf1 = 0.24*visc1/cnd1;
	prw1 = 0.24*vscw1/cndw1;
        if(prw1 == 0.0 ) prw1 = 0.001;
        if(tasx == 0.0 ) tasx = 1.0;
	dryp1 = 0.26*pow(re1,0.6)*pow(prf1,0.37)*pow(prf1/prw1,0.25)*fct1/0.239;

	fact = 1.0e6*0.239/(L1*diam*tasx*100.*(597.3+373.16-tk));
/*	fact = 1.238e6*0.239/(tasx*100.*(597.3+373.16-tk)); */

	return((plwc - dryp1) * fact);
}
