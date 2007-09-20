/*
-------------------------------------------------------------------------
OBJECT NAME:	dpcrc.c

FULL NAME:	Cryogenic Hygrometer Calculations

ENTRY POINTS:	sdpcrc()
		sfpcrc()
		scmrcr()
		srhocr()

STATIC FNS:	none

DESCRIPTION:	

DEPENDANCIES:	CMRCR   DPCRC
		DPCRC   ATX PSXC VCRH CRHP
		FPCRC   DPCRC
		RHOCR   DPCRC

INPUT:		
		CRHP    = Cavity pressure from cryo. hygro.
		VCRH    = Raw voltage from cryo. hygro. mirror
		FPCR    = Frost point (mirror) temp. from cryo. hygro.
		FPCRC   = Corrected frost point temp. from cryo. hygro.
		DPCRC   = Dew point from cryo. hygro.
		MRCR    = Mixing ratio from cryo. (g/kg)
		PMMCR   = Mass mixing ratio (grams/million of dry air)
		PMVCR   = Volumetric mixing ratio (ppm)
		RHOCR   = Absolute humidity from cryo. hygro.


OUTPUT:		

REFERENCES:	none

REFERENCED BY:	compute_derive.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static NR_TYPE	fpcrc, cmrcr, rhocr, es;

static NR_TYPE	dewpt(NR_TYPE), vapi(NR_TYPE), fpt(NR_TYPE), vapor(NR_TYPE),
		etodw(NR_TYPE), etofp(NR_TYPE);

/* -------------------------------------------------------------------- */
#define MAX_ITERATIONS  500

static NR_TYPE find_root_by_newton(NR_TYPE (*func)(NR_TYPE), NR_TYPE initial_guess, NR_TYPE tolerance)
{
  int   cnt = 0;
  NR_TYPE       a, b, c, fa, fb, delta, fprime;

  a = initial_guess;
  b = initial_guess * 1.01;

  delta = b - a;

  fa = (*func)(a);

  do
    {
    fb = (*func)(b);

    if (fa == fb)
      {
      b = initial_guess * 1.02;
      delta = b - a;
      fb = (*func)(b);
      }

    fprime      = (fb - fa) / delta;    /* Derivative   */
    c   = b - fb / fprime;
    delta       = c - b;
    b   = c;
    a   = b;
    fa  = fb;
    }
  while (fabs(delta) > tolerance * 0.1 && ++cnt < MAX_ITERATIONS);

  return(c);

}

/* -------------------------------------------------------------------- */
void sdpcrc(DERTBL *varp)
{
	NR_TYPE	atx, psxc, crhp, dpcrc, e, fi, ppmmcr, ppmvcr, fpcr;

	atx	= GetSample(varp, 0);
	psxc	= GetSample(varp, 1);
	fpcr	= GetSample(varp, 2);
	crhp	= GetSample(varp, 3);

	/* Test threshold to flag bad cryogenic data 
	 * 19Mar1990: Memo by Al Schanot    --CC
	if (fpcr > -15.0)
		{
		dpcrc = -15.0;
		cmrcr = 0.0;
		rhocr = 0.0;

		PutSample(varp, dpcrc);
		return;
		}
	 */

	/* Now get vapor pressure and dew point from frost point
	 */
	e = vapi(fpcr);

	/* Correct for pressure and temperature in cryo	*/
	if (crhp < 50.0)
		crhp = 50.0;

	e *= psxc / crhp;

	/* Enhancement factor wrt a plane ice surface	*/
	fi = 1.0003 + (4.18E-6 * psxc);
	e *= fi;

	/* Calculate mixing ratio(g/kg)	*/
	cmrcr = 622.0 * (e / (psxc - e));

	/* Calculate mass mixing ratio(g/million grams of dry air)	*/
	ppmmcr = cmrcr * 1000.0;

	/* Calculate volumetric mixing ratio (ppm)	*/
	ppmvcr = pow((double)10.0, (double)6.0) * (e / (psxc - e));
	fpcrc = fpt(e);
	dpcrc = dewpt(e);

	/* Absolute humidity	*/
	rhocr = e * 100.0 / (461.51 * (atx + Kelvin)) * 1.0E3;

	PutSample(varp, dpcrc);

}	/* END SDPCRC */

/* -------------------------------------------------------------------- */
void sfpcrc(DERTBL *varp)
{
  PutSample(varp, fpcrc);
}

/* -------------------------------------------------------------------- */
void scmrcr(DERTBL *varp)
{
  PutSample(varp, cmrcr);
}

/* -------------------------------------------------------------------- */
void srhocr(DERTBL *varp)
{
  PutSample(varp, rhocr);
}

/* -------------------------------------------------------------------- */
static NR_TYPE vapi(NR_TYPE tfp)
{
	double	vapi, e;

	/* Input is in degrees C, assumed to be frost point.
	 * routine codes Goff-Gratch formula.
	 */

	if (tfp < -200.0 || tfp > 200.0)
		{
		vapi = 1.0e-20;
		return(vapi);
		}

	tfp += Kelvin;

	/* This is ice saturation vapor pressure
	 */
	e =	-9.09718 * (Kelvin / tfp - 1.0) -
		3.56654 * log10(Kelvin / tfp) +
		0.876793 * (1.0 - tfp / Kelvin);

	vapi = 6.1071 * pow((double)10.0, e);

	return(vapi);

}

/* -------------------------------------------------------------------- */
static NR_TYPE vapor(NR_TYPE tfp)
{
  double	vapor, e;

  tfp += Kelvin;

  e = -7.90298 * (373.16 / tfp - 1.0) + 5.02808 * log10(373.16 / tfp) -
	1.3816E-7 * (pow((double)10.0, 11.344 * (1.0 - tfp / 373.16)) - 1.0) +
	8.1328E-3 * (pow((double)10.0, 3.49149 * (1.0 - 373.16 / tfp)) - 1.0);

  vapor = 1013.246 * pow((double)10.0, e);

  return(vapor);
}

/* -------------------------------------------------------------------- */
#define TOLERANCE	0.001

static NR_TYPE dewpt(NR_TYPE e)
{
  es = e;
  return(find_root_by_newton(etodw, -20.0, TOLERANCE));

}

/* -------------------------------------------------------------------- */
static NR_TYPE fpt(NR_TYPE e)
{
  es = e;
  return(find_root_by_newton(etofp, -20.0, TOLERANCE));

}

/* -------------------------------------------------------------------- */
static NR_TYPE etofp(NR_TYPE tdw)
{
  return(es - vapi(tdw));
}

/* -------------------------------------------------------------------- */
static NR_TYPE etodw(NR_TYPE tdw)
{
  return(es - vapor(tdw));
}

/* END DPCRC.C */
