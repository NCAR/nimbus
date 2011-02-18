/*
-------------------------------------------------------------------------
OBJECT NAME:	esubt.c

FULL NAME:	Water Vapor Pressure

ENTRY POINTS:	esubt()
		sedpc()

DESCRIPTION:	ESUBT function implements the Goff-Gratch formula ("Trans. Am.
		Soc. Heat. Vent. Eng.," Vol. 52, pp. 95-121, 1946) for
		computing vapor pressure over a plane wather surface.
		
		Prior to 2011, the code included the enhancement factor of Buck
		("J. Appl. Meteorol.," Vol. 20, pp. 1527-1532, 1981).
		After Feb 18, the code includes the enhancement factor of
		Murphy and Koop ("Q. J. Roy. Met. Soc.", Vol. 131, pp. 1539-1565, 2005).

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2011
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static const double	Ts = 373.16;

/* -------------------------------------------------------------------- */
double fw(double Tk, double pressure)
{
  // From Murphy and Koop, 2005.  Enhancement factor.
  return 1.0 + 1.0e-5 * pressure * (4.923 - 0.0325 * Tk + 5.84e-5 * Tk * Tk);
}

/* -------------------------------------------------------------------- */
double esubt(double temperature, double pressure)
{
  double Tk = temperature + Kelvin;
  double ew;

  if (Tk < 1.0)
    Tk = 1.0;

  ew = pow(10.0, (
	-7.90298 * ((Ts / Tk) - 1.0)
	+5.02808 * log10(Ts / Tk)
	-1.3816e-7 * (pow(10.0, 11.344 * (1.0 - Tk / Ts)) - 1.0)
	+8.1328e-3 * (pow(10.0, -3.49149 * (Ts / Tk - 1.0)) - 1.0))
	+log10(StdPress) );

  /* Arden Buck's pressure enhancement factor.  Used prior to 2011.
   * double fw = 1.0007 + (3.46e-6 * pressure);
   */


  return ew * fw(Tk, pressure);
}

/* -------------------------------------------------------------------- */
void sedpc(DERTBL *varp)
{
  NR_TYPE	dpxc, psxc;

  dpxc = GetSample(varp, 0);
  psxc = GetSample(varp, 1);
 
  PutSample(varp, (NR_TYPE)esubt(dpxc, psxc));
}
