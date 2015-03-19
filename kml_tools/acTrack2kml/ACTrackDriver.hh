
#ifndef _ACTrackDriver_hh_
#define _ACTrackDriver_hh_

#include <string>

#include "Config.hh"
#include "AircraftTrack.hh"
#include "AircraftTrackKML.hh"
#include "acDatabase.hh"
#include "osm.hh"

class ACTrackDriver
{
public:

  ACTrackDriver();

  int
  StatusCheck(bool ok, const std::string& msg);

  std::string
  usage(const char *argv0);

  int
  main(int argc, char *argv[]);

  int
  parseRunstring(int argc, char** argv);

  int
  parseConfig(int argc, char *argv[]);

  void
  setConfig(Config& config);

  int
  mainLoop();

  void
  loadTrackNetCDF(const std::string& ncpath);

  void
  loadTrackDatabase();

  /**
   * Load or update the track from the currently configured source, either
   * a netcdf file or a database.
   **/
  void
  loadTrack();

  AircraftTrack&
  getTrack()
  {
    return track;
  }

  Config
  getConfig()
  {
    return cfg;
  }

  AircraftTrackKML&
  getKML()
  {
    return kml;
  }

private:

  void
  store_args(int argc, char* argv[]);

  Config cfg;
  AircraftTrack track;
  AircraftTrackKML kml;
  OSM osm;

  /**
   * Keep the database object around so it can maintain the global
   * attributes and variable information, none of which need to be
   * refreshed unless the track gets too old.
   **/
  acDatabase db;

};


#endif // _ACTrackDriver_hh_
