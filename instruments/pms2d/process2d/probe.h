#ifndef _probeinfo_h_
#define _probeinfo_h_

#include <iostream>
#include <string>
#include <vector>

/**
 * Probe information.
 */
class ProbeInfo
{
public:
  ProbeInfo(std::string new_id, std::string sn, int ndiodes, float res,
		std::string sfx, int binoffset, int numbins)

  : id(new_id), serialNumber(sn), suffix(sfx), nDiodes(ndiodes), resolution(res),
	numBins(numbins), firstBin(binoffset), lastBin(numbins), armWidth(6.1)
  {
    if (id[0] == 'P') {		// 2DP
      armWidth = 26.1;
    }
    if (id[0] == '3') {		// 3V-CPI
      armWidth = 5.08;
    }
  }

  /**
   * Compute sample area of a probe for given diameter.
   * The sample area unit should be meter^2 for all probes.
   * @param reconstruct Are we doing particle reconstruction, or all-in.
   */
  void ComputeSamplearea(bool reconstruction);

  std::string id;            // Two byte ID at the front of the data-record.
  std::string serialNumber;
  std::string suffix;
  int nDiodes;          // 32, 64, or 128 at this time.
  float resolution;     // micrometers
  int numBins;
  int firstBin, lastBin;
  float armWidth;       // cm

  std::vector<float> bin_endpoints;
  std::vector<float> bin_midpoints;
  std::vector<float> dof;    // Depth Of Field
  std::vector<float> eaw;    // Effective Area Width
  std::vector<float> samplearea;
};

#endif
