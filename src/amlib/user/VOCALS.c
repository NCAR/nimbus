/**
 * Primary altitude with a secondary backup for switch over.
 * If the difference between radar altitude and gps altitude is greater than
 * 100m then use the GPS, radar alt is probably bad.  Designed for VOCALS
 * where radar alt was not good during/near take-off and landing.
 */

#include "nimbus.h"
#include "amlib.h"

/* -------------------------------------------------------------------- */
void svocalt(DERTBL *varp)
{
  NR_TYPE       ralt, ggalt, diff;

  ralt = GetSample(varp, 0);
  ggalt = GetSample(varp, 1);

  diff = ralt - ggalt;

  if ( isnan(ralt) || (fabs(diff) > 100.))
    ralt = ggalt;

  PutSample(varp, ralt);

}       /* END SPSBC */

/* END PSBC.C */

