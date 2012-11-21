#include "ProbeData.h"

ProbeData::ProbeData(size_t size) : _size(size)
{
  tas.reserve(size);
  cpoisson1.reserve(size);
  cpoisson2.reserve(size);
  cpoisson3.reserve(size);
  pcutoff.reserve(size);
  corrfac.reserve(size);

  all.accepted.reserve(size);
  all.rejected.reserve(size);
  all.total_conc.reserve(size);
  all.total_conc100.reserve(size);
  all.total_conc150.reserve(size);
  all.dbz.reserve(size);
  all.dbar.reserve(size);
  all.disp.reserve(size);
  all.lwc.reserve(size);
  all.eff_rad.reserve(size);

  round.accepted.reserve(size);
  round.rejected.reserve(size);
  round.total_conc.reserve(size);
  round.total_conc100.reserve(size);
  round.total_conc150.reserve(size);
  round.dbz.reserve(size);
  round.dbar.reserve(size);
  round.disp.reserve(size);
  round.lwc.reserve(size);
  round.eff_rad.reserve(size);

  for (size_t i = 0; i < size; ++i)
  {
    tas[i] = 0.0;
    cpoisson1[i] = cpoisson2[i] = cpoisson3[i] = 0.0;
    pcutoff[i] = corrfac[i] = 0.0;

    all.accepted[i] = round.accepted[i] = 0.0;
    all.rejected[i] = round.rejected[i] = 0.0;
    all.total_conc[i] = round.total_conc[i] = 0.0;
    all.total_conc100[i] = round.total_conc100[i] = 0.0;
    all.total_conc150[i] = round.total_conc150[i] = 0.0;
    all.disp[i] = round.disp[i] = 0.0;
    all.dbz[i] = round.dbz[i] = -100.0;
    all.dbar[i] = round.dbar[i] = 0.0;
    all.lwc[i] = round.lwc[i] = 0.0;
    all.eff_rad[i] = round.eff_rad[i] = 0.0;
  }
}
