/*
-------------------------------------------------------------------------
OBJECT NAME:	esubt.c

FULL NAME:	Water Vapor Pressure

ENTRY POINTS:	esubt()
		sedpc()

DESCRIPTION:	ESUBT function prior to 2011was based on the Goff-Gratch formula
		("Trans. Am.  Soc. Heat. Vent. Eng.," Vol. 52, pp. 95-121, 1946)
		for computing vapor pressure over a plane water surface.  After
		2011 Murphy and Koop below is used.
		
		Prior to 2011, the code included the enhancement factor of Buck
		("J. Appl. Meteorol.," Vol. 20, pp. 1527-1532, 1981).

		After Feb 18, the code includes the enhancement factor of Murphy
		and Koop ("Q. J. Roy. Met. Soc.", Vol. 131, pp. 1539-1565, 2005).

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2011
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

static bool MurphyKoop = true;

/* -------------------------------------------------------------------- */
double esubt(double temperature, double pressure)
{
  double Tk = temperature + Kelvin;
  double ew;

  if (Tk < 1.0)
    Tk = 1.0;

  if (MurphyKoop)
    ew = WaterVaporPressure(Tk);
  else
  {
    ew = pow(10.0, (
	-7.90298 * ((Ts / Tk) - 1.0)
	+5.02808 * log10(Ts / Tk)
	-1.3816e-7 * (pow(10.0, 11.344 * (1.0 - Tk / Ts)) - 1.0)
	+8.1328e-3 * (pow(10.0, -3.49149 * (Ts / Tk - 1.0)) - 1.0))
	+log10(StdPress) );

    // Arden Buck's pressure enhancement factor.
    double fw = 1.0007 + (3.46e-6 * pressure);
    ew *= fw;
  }

  return ew;
}

/* -------------------------------------------------------------------- */
void edpcInit(var_base *varp)
{
  extern std::map<std::string, bool> dp_method;
  std::string dp(((DERTBL *)((DERTBL *)varp)->depends[0])->depend[0]);

  if (dp_method.count(dp) > 0)
    MurphyKoop = dp_method[dp];

  if (MurphyKoop)
    AddToAttributes(varp->name, "Method", "MurphyKoop");
  else
    AddToAttributes(varp->name, "Method", "GoffGratch");
}

/* -------------------------------------------------------------------- */
void sedpc(DERTBL *varp)
{
  NR_TYPE	dpxc, psxc;

  dpxc = GetSample(varp, 0);
  psxc = GetSample(varp, 1);
 
  PutSample(varp, (NR_TYPE)esubt(dpxc, psxc));
}
