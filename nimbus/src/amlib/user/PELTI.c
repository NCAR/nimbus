#include "nimbus.h"
#include "decode.h"
#include "amlib.h"


static NR_TYPE d[3] = {29.2, 6.17, 5.16};

/* -------------------------------------------------------------------- */
void ssteve(DERTBL *varp)
{
  NR_TYPE slpm, atx, psx, tasx, out;

  slpm = GetSample(varp, 0);
  psx = GetSample(varp, 1);
  atx = GetSample(varp, 2);
  tasx = GetSample(varp, 3);

  out = slpm * (1013.25 / psx) * ((atx+273.15) / 298) * 66.6666 / (M_PI * d[varp->ProbeCount] * d[varp->ProbeCount] * tasx);

  PutSample(varp, out);

}

/* -------------------------------------------------------------------- */
void snasaFlow(DERTBL *varp)
{
  NR_TYPE	flow, lfsspf, ps, tt;

  lfsspf = GetSample(varp, 0);
  ps = GetSample(varp, 1);
  tt = GetSample(varp, 2);

  flow = lfsspf * 1013.25 / ps * ((273.15 + tt) / 298) * 66.66666 / (M_PI * 5.5555*5.5555);

  PutSample(varp, flow);

}
