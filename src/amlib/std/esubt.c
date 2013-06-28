/*
-------------------------------------------------------------------------
OBJECT NAME:	esubt.c

FULL NAME:	Water Vapor Pressure

ENTRY POINTS:	esubt()
		ew()
		sedpc()

DESCRIPTION:	ESUBT function prior to 2011was based on the Goff-Gratch formula
		("Trans. Am.  Soc. Heat. Vent. Eng.," Vol. 52, pp. 95-121, 1946)
		for computing vapor pressure over a plane water surface.  After
		2011 Murphy and Koop below is used.
		
		Prior to 2011, the code included the enhancement factor of Buck
		("J. Appl. Meteorol.," Vol. 20, pp. 1527-1532, 1981).

		After Feb 18, the code includes the enhancement factor of Murphy
		and Koop ("Q. J. Roy. Met. Soc.", Vol. 131, pp. 1539-1565, 2005).

NOTE:		esubt() produces a water vapor pressure from a corrected
		dew/frost point (DPC or such).
		ew() produces water vapor pressure from a mirror temperature
		or raw DP value.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2012
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"
#include <map>

static const double	Ts = 373.16;

/* -------------------------------------------------------------------- */
double WaterVaporPressureOfIce(double Tk)
{
//  static const double b0 = 9.550426;
  static const double b1 = -5723.265;
  static const double b2 = 3.53068;
  static const double b3 = -0.00728332;	// K-1
  static const double b0prime = 6.111536;	// hPa

  return b0prime *
        exp(b1 * (Kelvin - Tk) / (Kelvin * Tk) + b2 * log(Tk / Kelvin) + b3 * (Tk - Kelvin));
}

double WaterVaporPressureOfWater(double Tk)
{
  static const double e0 = 6.091888;	// hPa
//  static const double e1 = 6.564725;
  static const double e2 = -6763.22;	// K
  static const double e3 = -4.210;
  static const double e4 = 0.000367;	// K-1
  static const double e5 = 0.0415;	// K-1
  static const double e6 = -0.1525967;
  static const double e7 = -1331.22;	// K
  static const double e8 = -9.44523;
  static const double e9 = 0.014025;	// K-1
  static const double Tx = 218.8;	// K

  double alpha = tanh(e5 * (Tk - Tx));

  double d2 = e2 + alpha * e7;
  double d3 = e3 + alpha * e8;
  double d4 = e4 + alpha * e9;

  return e0 *
        exp((alpha - 1.0) * e6 + d2 * (Kelvin - Tk) / (Tk*Kelvin) + d3 * log(Tk/Kelvin) + d4 * (Tk - Kelvin));
}

/* -------------------------------------------------------------------- */
double WaterVaporPressure(double Tk)
{
  // Murphy-Koop.
  if (Tk < Kelvin)
    return WaterVaporPressureOfIce(Tk);
  else
    return WaterVaporPressureOfWater(Tk);
}

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

  // Arden Buck's pressure enhancement factor.
  double fw = 1.0007 + (3.46e-6 * pressure);

  return ew * fw;
}

/* -------------------------------------------------------------------- */
void ewInit(var_base *varp)
{
  AddToAttributes(varp->name, "Method", "Murphy-Koop");
}

/* -------------------------------------------------------------------- */
void edpcInit(var_base *varp)
{
  AddToAttributes(varp->name, "Method", "Goff-Gratch");
}

/* -------------------------------------------------------------------- */
void sew(DERTBL *varp)
{
  double e = floatNAN;

  if (varp->depends[0]->Units.compare("#/cm3") == 0)
  {
    // E from Density.

    NR_TYPE h2o_conc_vxl, atx;

    h2o_conc_vxl = GetSample(varp, 0);
    atx = GetSample(varp, 1);

    if (h2o_conc_vxl > 1.0)
      e = Boltzmann * (atx+Kelvin) * (h2o_conc_vxl * 1.0e6) * 1.0e-2;
  }
  else
  {
    // E from mirror temperature.

    double Tk = GetSample(varp, 0) + Kelvin;
    double psxc = GetSample(varp, 1);
 
    // fw() From Murphy and Koop, 2005.  Enhancement factor.  See esubt.c
    e = WaterVaporPressure(Tk) * fw(Tk, psxc);
    if (varp->ndep > 2)
    {
      NR_TYPE psdp = GetSample(varp, 2);	// DP Cavity Pressure.
      NR_TYPE Rx = psxc / psdp;
      e *= Rx;
    }
  }

  PutSample(varp, e);
}

/* -------------------------------------------------------------------- */
void sedpc(DERTBL *varp)
{
  NR_TYPE dpxc = GetSample(varp, 0);
  NR_TYPE psxc = GetSample(varp, 1);
 
  PutSample(varp, (NR_TYPE)esubt(dpxc, psxc));
}
