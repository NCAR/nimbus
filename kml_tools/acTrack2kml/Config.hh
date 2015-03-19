
#ifndef _Config_hh_
#define _Config_hh_

#include <string>
#include <iosfwd>
#include <vector>

class Config
{
public:
  typedef std::string string;

  Config();

  bool
  verifyDatabaseHost();

  std::string
  dump();

  /**
   * The path to the output directory which contains the GE subdirectory.
   **/
  string flightDataDir;

  /**
   * URL to the flight_data directory from which the static wind barbs and
   * icons can be retrieved.  This can be a full URL with a web host like
   * http://www.eol.ucar.edu/flight_data, or it can be an absolute
   * reference on the server host like /flight_data, or it could be a
   * relative path for either server hosting or local filesystem browsing.
   **/
  string flightDataURL;

  /**
   * All datapoints are read into the track, but only use one point every
   * 'TimeStep' seconds.  e.g. 15 would mean use 1 data point out of every
   * 15 seconds.
   **/
  int TimeStep;

  /**
   * The degrees by which heading must change to add a point in the
   * "heading" track simplification algorithm.  The default is 0.25 degrees.
   **/
  float HeadingStep;

  /**
   * For renderings like KML which select a subset of points to include in
   * the track, this can be used to select the algorithm for simplifying
   * the track.  The "timestep" method, the default, picks points every
   * TimeStep seconds.  The "headingstep" method picks points when the
   * heading changes by HeadingStep degrees.
   **/
  string path_method;

  /// True Airspeed cut-off (take-off and landing speed).
  float TAS_CutOff;

  /// Frequency of Time Stamps (in minutes).
  int ts_Freq;

  /// Frequency of Wind Barbs (in minutes).
  int barb_Freq;

  /// Should we use KML clampToGround or ABSOLUTE.
  string altMode;

  string netCDFinputFile;
  string outputKML;
  string outputAnimatedKML;
  string outputPositionKML;
  string outputPositionJSON;
  string outputPrefix;
  string database_host;
  string platform;
  string dbname;

  /// Compress KML output files.
  bool compressKML;

  /// If alt is in meters this gets changed to 3.28.
  float convertToFeet;

  bool onboard;
  int verbose;
  bool showstats;

  int update_interval_secs;
  int position_interval_secs;

  string latVariable;
  string lonVariable;
  string altVariable;

  bool run_once;
  bool check;

  /// Override the red-yellow track colors with this hex color string for
  /// KML track styles.  The order of expression is aabbggrr, where
  /// aa=alpha (00 to ff); bb=blue (00 to ff); gg=green (00 to ff); rr=red
  /// (00 to ff).
  string color;

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

  bool
  setPlatform(const std::string& platname);

  void
  fillDefaults();

  bool
  realtimeMode();

  static
  std::vector<std::string>
  getPlatformNames();

  static string
  defaultGroundFlightDataURL();

  static string
  defaultOnboardFlightDataURL();

};


#endif // _Config_hh_
