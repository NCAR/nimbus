/*   TAS = TRUE AIRSPEED CALCULATION FUNCTION (m/s)                     TAS */

#include "nimbus.h"
#include "amlib.h"


extern NR_TYPE compute_tas(NR_TYPE, NR_TYPE);

/* -------------------------------------------------------------------- */
void stas(DERTBL *varp)
{
  NR_TYPE mach	= GetSample(varp, 0);
  NR_TYPE at	= GetSample(varp, 1);

  NR_TYPE tas	= compute_tas(at, mach);

  PutSample(varp, tas);
}

/* -------------------------------------------------------------------- */
NR_TYPE compute_tas(NR_TYPE t, NR_TYPE m)
{
  double term = m * sqrt((double)7.0 / 5.0 * Rd * (t + Kelvin));	// Using ATX

  if (term < 0.0)
    term = -term;

  return term;
}
