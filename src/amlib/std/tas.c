/*   TAS = TRUE AIRSPEED CALCULATION FUNCTION (m/s)                     TAS */

#include "nimbus.h"
#include "amlib.h"


static const double Yd = Cpd / Cvd;	// d for dry constants.

static int rt_sensor = -1;	// Default to unheated.

double mach(double p, double q);
double unheatedRecoveryFactor(double mach);
double heatedRecoveryFactor(double mach, int sensor);
double WaterVaporPressure(double Tk);

void setEOP(double);

static NR_TYPE dry_tas;

/* -------------------------------------------------------------------- */
void tasInit(var_base *varp)
{
  // Add flag variable metadata
  varp->addToMetadata("ancillary_variables", "TASFLG");
}

/* -------------------------------------------------------------------- */
void tasFlgInit(var_base *varp)
{
  size_t i;
  DERTBL *dp = (DERTBL *)varp;
  DERTBL *rt = 0;
  std::vector<int> values;

  // Locate Recovry Temperature in depends list.
  for (i = 0; i < dp->depends.size(); ++i)
    if (strncmp(dp->depends[i]->name, "RT", 2) == 0)
      rt = (DERTBL *)dp->depends[i];

  if (rt == 0)
  {
    fprintf(stderr, "tas.c::tasFlgInit(), no RT dependency, fatal.\n");
    exit(1);
  }


  // Now determine if heated or unheated.
  if (strncmp(rt->depend[0], "RTH", 3) == 0)	// Unheated will have an 'F' as 3rd character.
    rt_sensor = rt->depend[0][3] - '1';		// Get sensor number for (see recovery factor for why).

  fprintf(stderr, "tas.c::tasFlgInit() found RT = [%s]\n", rt->depend[0]);

  // flag attributes.
  values.push_back(0);
  values.push_back(1);

  varp->addToMetadata("flag_values", values);
  varp->addToMetadata("flag_meanings", "humidity_corrected dry");
}

/* -------------------------------------------------------------------- */
NR_TYPE compute_tas(NR_TYPE t, NR_TYPE m)
{
  double Y = Cp() / Cv();
  double term = m * sqrt(Y * R() * (t + Kelvin));	// Using ATX

  if (term < 0.0)
    term = -term;

  return term;
}

/* -------------------------------------------------------------------- */
void stas_flag(DERTBL *varp)
{
  NR_TYPE q = GetSample(varp, 0);
  NR_TYPE p = GetSample(varp, 1);
  NR_TYPE rt = GetSample(varp, 2); // If this changes from index 2, then modify tasFlgInit() above.
  NR_TYPE ewx = GetSample(varp, 3);

  setEOP(0.0);	// Set for dry constants.

  NR_TYPE dry_mach, recovery, dry_at, ews, ewt, retval = 0.0;
  dry_mach = mach(p, q);

  if (rt_sensor < 0)
    recovery = unheatedRecoveryFactor(dry_mach);
  else
    recovery = heatedRecoveryFactor(dry_mach, rt_sensor);

  dry_at = AMBIENT(rt, recovery, dry_mach*dry_mach);
  dry_tas = compute_tas(dry_at, dry_mach);	// stash for later retrieval in stas_dry()

  ews = WaterVaporPressure(dry_at+Kelvin);
//printf("at=%f : mach=%f : ewx=%f : ews = %f\n", dry_at, dry_mach, ewx, ews);
  if (!std::isnan(ewx) && ewx < 1.05 * ews)
  {
    ewt = ewx;
    retval = 0.0;
  }
  else
  {
    ewt = ews * 0.5;
    retval = 1.0;
  }

  if (!std::isnan(p) && p > 0.0)
    setEOP(ewt/p);

  PutSample(varp, retval);
}

/* -------------------------------------------------------------------- */
void stas_dry(DERTBL *varp)
{
  PutSample(varp, dry_tas);
}

/* -------------------------------------------------------------------- */
void stas(DERTBL *varp)
{
  NR_TYPE mach	= GetSample(varp, 0);
  NR_TYPE at	= GetSample(varp, 1);

  NR_TYPE tas	= compute_tas(at, mach);

  PutSample(varp, tas);
}
