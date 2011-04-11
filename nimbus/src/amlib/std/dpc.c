/*
-------------------------------------------------------------------------
OBJECT NAME:	dpc.c

FULL NAME:	Dew point

ENTRY POINTS:	dpInit()
		sdpc_GG()
		sdpc_MK()

STATIC FNS:	none

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
#include <map>

#include <gsl/gsl_spline.h>

double fw(double, double);	// See esubt.c

std::map<std::string, bool> dp_method;

static const size_t TableSize = 300;
static double ttable[TableSize];
static double etable[TableSize];

static gsl_interp_accel *acc;
static gsl_interp *linear = 0;

double WaterVaporPressure(double Tk);
double WaterVaporPressureOfWater(double Tk);


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
  double dp = GetSample(varp, 0);
  double psxc = GetSample(varp, 1);
  double Tk = dp + Kelvin;
  double e;

  // fw() From Murphy and Koop, 2005.  Enhancement factor.  See esubt.c
  e = WaterVaporPressure(Tk) * fw(Tk, psxc);
  dp = gsl_interp_eval(linear, etable, ttable, e, acc);

  PutSample(varp, (NR_TYPE)dp);
}

/* -------------------------------------------------------------------- */
void dpInit(var_base *varp)
{
  if (((DERTBL *)varp)->ndep == 1)
  {
    dp_method[varp->name] = false;
    ((DERTBL *)varp)->compute = (void (*)(void*))sdpc_GG;
    AddToAttributes(varp->name, "Method", "Goff-Gratch");
  }
  else
  {
    dp_method[varp->name] = true;
    AddToAttributes(varp->name, "Method", "Murphy-Koop");
  }

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
