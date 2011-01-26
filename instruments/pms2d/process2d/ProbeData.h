#include <vector>

/**
 * Class to contain and manage data blocks for computed variables/data.
 */
class ProbeData
{
public:
  struct derived
  {
    float **counts, **conc;	// Histogram for counts and concentration.

    std::vector<float> accepted;	// Counts of accepted particles.
    std::vector<float> rejected;	// Counts of rejected particles.
    std::vector<float> total_conc, dbz, dbar, disp, lwc, eff_rad;
  };

  ProbeData(size_t size);


  std::vector<float> tas;
  std::vector<float> cpoisson1, cpoisson2, cpoisson3;
  std::vector<float> pcutoff, corrfac;

  struct derived all, round;
};
