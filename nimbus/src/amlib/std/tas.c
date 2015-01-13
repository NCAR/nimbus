/*   TAS = TRUE AIRSPEED CALCULATION FUNCTION (m/s)                     TAS */

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
NR_TYPE compute_tas(NR_TYPE t, NR_TYPE r, NR_TYPE m)
{
//  double term = (t + Kelvin) * (401.86 / (1.0 / xm2 + 0.2 * r));	// Using RTX
  double term = m * sqrt((double)7.0 / 5.0 * Rd * (t + Kelvin));	// Using ATX

  if (term < 0.0)
    term = -term;

//  return(sqrt(term));
  return term;
}
