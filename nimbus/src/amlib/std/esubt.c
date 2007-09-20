/*
-------------------------------------------------------------------------
OBJECT NAME:	esubt.c

FULL NAME:	

ENTRY POINTS:	esubt()

STATIC FNS:	none

DESCRIPTION:	ESUBT function implements the Goff-Gratch formula ("Trans. Am.
		Soc. Heat. Vent. Eng.," Vol. 52, pp. 95-121, 1946) for
		computing vapor pressure over a plane wather surface and also
		includes the enhancement factor of Buck ("J. Appl. Meteorol.,"
		Vol. 20, pp. 1527-1532, 1981).

INPUT:		Temperature, Pressure

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	rhum.c cvi.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static const double	Ts = 373.16, To = Kelvin;

/* -------------------------------------------------------------------- */
double esubt(double temperature, double pressure)
{
  double Tk = temperature + To;
  double fw, ew;

  if (Tk < 1.0)
    Tk = 1.0;

  ew = pow(10.0, (
	-7.90298 * ((Ts / Tk) - 1.0)
	+5.02808 * log10(Ts / Tk)
	-1.3816e-7 * (pow(10.0, 11.334 * (1.0 - Tk / Ts)) - 1.0)
	+8.1328e-3 * (pow(10.0, -3.49149 * (Ts / Tk - 1.0)) - 1.0))
	+log10(1013.246) );

  /* Arden Buck's pressure enhancement factor.
  */
  fw = 1.0007 + (3.46e-6 * pressure);

  return(ew * fw);

}	/* END ESUBT */

/* -------------------------------------------------------------------- */
void sedpc(DERTBL *varp)
{
  NR_TYPE	dpxc, psxc;

  dpxc = GetSample(varp, 0);
  psxc = GetSample(varp, 1);
 
  PutSample(varp, (NR_TYPE)esubt(dpxc, psxc));
}

/* END ESUBT.C */
