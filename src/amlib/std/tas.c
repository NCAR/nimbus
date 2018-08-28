/*   TAS = TRUE AIRSPEED CALCULATION FUNCTION (m/s)                     TAS */

#include "nimbus.h"
#include "amlib.h"


static const double Yd = Cpd / Cvd;	// d for dry constants.

double mach(double p, double q);
double unheatedRecoveryFactor(double mach);
double heatedRecoveryFactor(double mach);
double WaterVaporPressure(double Tk);

void setEOP(double);

static NR_TYPE dry_tas;

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
  NR_TYPE rt = GetSample(varp, 2);
  NR_TYPE ewx = GetSample(varp, 3);

  setEOP(0.0);	// Set for dry constants.

  NR_TYPE dry_mach, recovery, dry_at, ews, ewt, retval = 0.0;
  dry_mach = mach(p, q);

  if (strncmp(varp->depend[2], "RTH", 3) == 0)	// Unheated will have an 'F' as 3rd character.
    recovery = heatedRecoveryFactor(dry_mach);
  else
    recovery = unheatedRecoveryFactor(dry_mach);

  dry_at = AMBIENT(rt, recovery, dry_mach*dry_mach);
  dry_tas = compute_tas(dry_at, dry_mach);	// stash for later retrieval in stas_dry()

  ews = WaterVaporPressure(dry_at+Kelvin);
//printf("at=%f : mach=%f : ewx=%f : ews = %f\n", dry_at, dry_mach, ewx, ews);
  if (!isnan(ewx) && ewx < 1.05 * ews)
  {
    ewt = ewx;
    retval = 0.0;
  }
  else
  {
    ewt = ews * 0.5;
    retval = 1.0;
  }

  if (!isnan(p) && p > 0.0)
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
