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
  ProbeInfo(std::string new_id, int ndiodes, int numbins, int binoffset)
  : id(new_id), nDiodes(ndiodes), numBins(numbins), firstBin(binoffset),
    lastBin(numbins), resolution(25), armWidth(6.1)
  {
    if (id[0] == 'P') {
      resolution = 200;
      armWidth = 26.1;
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
  int nDiodes;          // 32 or 64 at this time.
  int numBins;
  int firstBin, lastBin;
  float resolution;     // micrometers
  float armWidth;       // cm

  std::vector<float> bin_endpoints;
  std::vector<float> bin_midpoints;
  std::vector<float> dof;    // Depth Of Field
  std::vector<float> eaw;    // Effective Area Width
  std::vector<float> samplearea;
};

#endif
