
#ifndef _osm_hh_
#define _osm_hh_

#include "AircraftTrack.hh"
#include "Config.hh"

/**
 * Generate OSM representations in JSON format from an AircraftTrack.
 **/
class OSM
{
public:
  OSM();

  void
  setConfig(const Config& config)
  {
    _config = config;
  }

  void
  writePositionJSON(AircraftTrack& track, const std::string& file);

private:

  Config _config;

};






#endif // _osm_hh_
