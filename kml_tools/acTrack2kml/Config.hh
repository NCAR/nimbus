
#ifndef _Config_hh_
#define _Config_hh_

#include <string>
#include <iosfwd>

class Config
{
public:
  typedef std::string string;

  Config();

  bool
  verifyDatabaseHost();

  void
  dump(std::ostream& out);

  string flightDataDir;
  string webHost;

  // All datapoints are read from file, but only use one point every 'TimeStep' seconds.
  // e.g. 15 would mean use 1 data point for every 15 seconds of data.
  int TimeStep;

  // True Airspeed cut-off (take-off and landing speed).
  float TAS_CutOff;

  // Frequency of Time Stamps (in minutes).
  int ts_Freq;

  // Frequency of Wind Barbs (in minutes).
  int barb_Freq;

  // Should we use KML clampToGround or ABSOLUTE.
  string altMode;

  string netCDFinputFile;
  string outputKML;
  string database_host;
  string platform;
  string dbname;

  // If alt is in meters this gets changed to 3.28.
  float convertToFeet;

  bool showit;
  bool onboard;
  int verbose;

  int update_interval_secs;
  int position_interval_secs;
};


#endif // _Config_hh_
