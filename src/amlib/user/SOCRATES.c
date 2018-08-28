#include "nimbus.h"
#include "decode.h"
#include "amlib.h"


static NR_TYPE area = 0.0018;

/* -------------------------------------------------------------------- */
void sphips(DERTBL *varp)
{
  NR_TYPE pps, tas, out;

  pps = GetSample(varp, 0);
  tas = GetSample(varp, 1);

  out = (pps-1.43) * 1000.0 / (tas * 100.0) / area;

  PutSample(varp, out);

}

