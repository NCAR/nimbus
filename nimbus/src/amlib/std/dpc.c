/*
-------------------------------------------------------------------------
OBJECT NAME:	dpc.c

FULL NAME:	Dew point

ENTRY POINTS:	dpInit()
		sdpc()

STATIC FNS:	WaterVaporPressure*()
		WaterVaporPressure()

DESCRIPTION:	Computes the difference between dew point and frost point
		derived from Murphy-Koop vapor pressure equations for plane
		water and ice surfaces. Final corrected dew point (DPC) is
		with respect to a plane water surface.  The accuracy of
		conversion (one sigma) is 0.02C over range of 0C to -80 C.

INPUT:		DP measured, PSXC

OUTPUT:		DPC deg_C

REFERENCES:	Suggested Changes to the Algorithms for the Calculation of
		Vapor Pressure, Al Cooper, February 21, 2011

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2011
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

#include <gsl/gsl_spline.h>

double fw(double, double);	// See esubt.c

static const size_t TableSize = 300;
static double ttable[TableSize];
static double etable[TableSize];

static gsl_interp_accel *acc;
static gsl_interp *linear = 0;


/* -------------------------------------------------------------------- */
static double WaterVaporPressureOfIce(double Tk)
{
//  static const double b0 = 9.550426;
  static const double b1 = -5723.265;
  static const double b2 = 3.53068;
  static const double b3 = -0.00728332;	// K-1
  static const double b0prime = 6.111536;	// hPa

  return b0prime *
	exp(b1 * (Kelvin - Tk) / (Kelvin * Tk) + b2 * log(Tk / Kelvin) + b3 * (Tk - Kelvin));
}

static double WaterVaporPressureOfWater(double Tk)
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
static double WaterVaporPressure(double Tk)
{
  // Murphy Koop.
  if (Tk < Kelvin)
    return WaterVaporPressureOfIce(Tk);
  else
    return WaterVaporPressureOfWater(Tk);
}

/* -------------------------------------------------------------------- */
void dpInit(var_base *varp)
{
  // Bail out if we've already initialized the interp stuff.  Only needed once.
  if (linear)
    return;

  // Setup temperature / water vapor table in half degC increments.
  double t = -100.0;	// -100C
  for (size_t i = 0; i < TableSize; ++i)
  {
    ttable[i] = t;
    etable[i] = WaterVaporPressureOfWater(t + Kelvin);
    t += 0.5;
  }

  // Initialize gsl interpolation.
  acc = gsl_interp_accel_alloc();
  linear = gsl_interp_alloc(gsl_interp_linear, TableSize);
  gsl_interp_init(linear, etable, ttable, TableSize);
}

/* -------------------------------------------------------------------- */
void sdpc(DERTBL *varp)
{
  double dp = GetSample(varp, 0);

  /* Used prior to 2011.  We'll use the old algorithm if there is only one
   * dependency in the DependTable, that will keep old projects running.
   */
  if (varp->ndep == 1)
  {
    if (dp < 0.0)
      dp = 0.009109 + dp * (1.134055 + dp * 0.001038);

    PutSample(varp, dp);
    return;
  }


  double psxc = GetSample(varp, 1);
  double Tk = dp + Kelvin;
  double e;

  // fw() From Murphy and Koop, 2005.  Enhancement factor.  See esubt.c
  e = WaterVaporPressure(Tk) * fw(Tk, psxc);
  dp = gsl_interp_eval(linear, etable, ttable, e, acc);

  PutSample(varp, (NR_TYPE)dp);
}
