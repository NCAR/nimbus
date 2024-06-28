#include "nimbus.h"
#include "amlib.h"

static NR_TYPE TSURF_CAL = 0.001933;

/* -------------------------------------------------------------------- */
void initTsurf(var_base *varp)
{
  float  *tmp;
  if ((tmp = GetDefaultsValue("TSURF_CAL", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function initTsurf.\n", TSURF_CAL);
    LogMessage(buffer);
  }
  else
    TSURF_CAL = tmp[0];

  std::vector<float> val;
  val.push_back(TSURF_CAL);
  varp->addToMetadata("AltitudeCal", val);
}

/* -------------------------------------------------------------------- */
void stsurf(DERTBL *varp)
{
  NR_TYPE  rstb, galt;

  rstb  = GetSample(varp, 0);
  galt  = GetSample(varp, 1);

  PutSample(varp, rstb + (TSURF_CAL * galt));

}
