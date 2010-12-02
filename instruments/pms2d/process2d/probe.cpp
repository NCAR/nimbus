#include "probe.h"

/**
 * Compute sample area of a probe for given diameter.
 * The sample area unit should be meter^2 for all probes.
 * @param probe
 * @param diam diameter to generate sample volume.
 * @param reconstruct Are doing particle reconstruction, or all-in.
 * @returns sample area
 */
void ProbeInfo::ComputeSamplearea(bool reconstruct)
{
  for (int i = 0; i < numBins+1; ++i)
    bin_endpoints.push_back((i+0.5) * resolution);

  for (int i = 0; i < numBins; ++i)
  {
    float sa, prht, DoF, eff_wid, diam;

    diam = (bin_endpoints[i] + bin_endpoints[i+1]) / 2.0;

    prht = armWidth * 1.0e4;  //convert cm to microns
    DoF = std::min((2.37f * diam*diam), prht);  // in microns, limit on dof is physical distance between arms
    if (reconstruct)
      eff_wid = resolution*nDiodes+0.72*diam;  //from eq 17 in Heymsfield & Parrish 1978
    else
      eff_wid = resolution * (nDiodes-1)-diam;   //from eq 6 in HP78

    sa = DoF * eff_wid * 1e-12;  //compute sa and convert to m^2 

    bin_midpoints.push_back(diam);
    dof.push_back(DoF);
    eaw.push_back(eff_wid);
    samplearea.push_back(sa);
  }
}

