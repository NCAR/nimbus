
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

  // All datapoints are read into the track, but only use one point every
  // 'TimeStep' seconds.  e.g. 15 would mean use 1 data point out of every
  // 15 seconds.
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

  string latVariable;
  string lonVariable;
  string altVariable;

  bool run_once;
  bool check;

  /**
   * Select the latitude variable name from a default name derived from the
   * data source and the configured name, if any.  The configured name
   * always takes precedence, followed by the @p sourcename.
   **/
  string
  getLatitudeVariable(const string& sourcename);

  /**
   * Select the longitude variable name from a default name derived from
   * the data source and the configured name, if any.  The configured name
   * always takes precedence, followed by the @p sourcename.
   **/
  string
  getLongitudeVariable(const string& sourcename);

  /**
   * Select the altitude variable name from a default name derived from the
   * data source and the configured name, if any.  The configured name
   * always takes precedence, followed by the @p sourcename.
   **/
  string
  getAltitudeVariable(const string& sourcename);

  /**
   * Set the convertToFeet configuration parameter according to the given
   * @p units for the altitude variable.
   **/
  void
  setAltitudeUnits(const string& units);


};


#endif // _Config_hh_
