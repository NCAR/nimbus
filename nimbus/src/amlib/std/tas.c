/*   TAS = TRUE AIRSPEED CALCULATION FUNCTION (m/s)                     TAS */

#include "nimbus.h"
#include "amlib.h"

NR_TYPE tas(NR_TYPE t, NR_TYPE r, NR_TYPE xm2)
{
  double	term;

  term = (t + Kelvin) * (401.86 / (1.0 / xm2 + 0.2 * r));

  if (term < 0.0)
    term = -term;

  return(sqrt(term));
}
