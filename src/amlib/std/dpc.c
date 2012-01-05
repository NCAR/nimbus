/*
-------------------------------------------------------------------------
OBJECT NAME:	dpc.c

FULL NAME:	Dew point

ENTRY POINTS:	dp??Init()
		sdpc_GG()
		sdpc_MK()

DESCRIPTION:	Computes the difference between dew point and frost point
		derived from Murphy-Koop vapor pressure equations for plane
		water and ice surfaces. Final corrected dew point (DPC) is
		with respect to a plane water surface.  The accuracy of
		conversion (one sigma) is 0.02C over range of 0C to -80 C.

INPUT:		GG <= Raw DP
		MK <= EW

OUTPUT:		Dew/Frost Point in deg_C

REFERENCES:	Suggested Changes to the Algorithms for the Calculation of
		Vapor Pressure, Al Cooper, February 21, 2011

NOTE:		The processing chain has been reversed between Goff-Gratch
		and Murphy-Koop.  GG took a mirror temperature (raw DP) and
		produced a dew/frost point.  This was used to produce water
		vapor pressure.  In Murphy Koop we compute the water vapor
		pressure first with a pressure correction, then we produce
		a dew/frost point.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2012
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"
#include <map>

#include <gsl/gsl_spline.h>

static const size_t TableSize = 300;
static double ttable[TableSize];
static double etable[TableSize];

static gsl_interp_accel *acc;
static gsl_interp *linear = 0;


/* -------------------------------------------------------------------- */
void sdpc_GG(DERTBL *varp)	// Goff-Gratch.
{
  double dp = GetSample(varp, 0);

  if (dp < 0.0)
    dp = 0.009109 + dp * (1.134055 + dp * 0.001038);

  PutSample(varp, dp);
}

/* -------------------------------------------------------------------- */
void sdpc_MK(DERTBL *varp)	// Murphy-Koop.
{
  double e = GetSample(varp, 0);

  NR_TYPE dp = gsl_interp_eval(linear, etable, ttable, e, acc);

  PutSample(varp, (NR_TYPE)dp);
}

/* -------------------------------------------------------------------- */
void dpggInit(var_base *varp)
{
  AddToAttributes(varp->name, "Method", "Goff-Gratch");
}

/* -------------------------------------------------------------------- */
void dpmkInit(var_base *varp)
{
  double WaterVaporPressure(double Tk);
  double WaterVaporPressureOfWater(double Tk);

  AddToAttributes(varp->name, "Method", "Murphy-Koop");

  // Bail out if we've already initialized the interp stuff.  Only needed once.
  if (linear)
    return;

  // Setup temperature / water vapor table in half degC increments for Murphy-Koop.
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
