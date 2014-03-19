#ifndef _ncTrack_hh_
#define _ncTrack_hh_

#include "AircraftTrack.hh"
#include "Config.hh"

class NcFile;
class NcVar;

/**
 * ncTrack provides an interface for loading an AircraftTrack from a netcdf
 * file.
 **/
class ncTrack
{
public:
  ncTrack();

  ~ncTrack();

  void
  setConfig(const Config& config)
  {
    cfg = config;
  }

  bool
  open(const std::string& filepath);

  void
  close();

  void
  fillAircraftTrack(AircraftTrack& track);

private:

  NcVar *
  getNetcdfVariable(const std::string& var_name);

  NcFile* nc;

  Config cfg;
};



#endif // _ncTrack_hh_
