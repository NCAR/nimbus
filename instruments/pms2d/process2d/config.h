#ifndef _config_h_
#define _config_h_

#include <string>
#include <ctime>

/**
 * Command line options / program configuration
 */
class Config
{
public:
  Config() : nOutputBins(128), nInterarrivalBins(40), shattercorrect(true), recon(true), smethod('c'), verbose(false), debug(false) {}

  std::string inputFile;
  std::string outputFile;

  std::string platform;
  std::string project;
  std::string flightDate;
  std::string flightNumber;

  time_t starttime;
  time_t stoptime;

  int nOutputBins;
  int nInterarrivalBins;

  bool shattercorrect;
  bool recon;           // Particle reconstruction
  char smethod;
  bool verbose;
  bool debug;
};

#endif
