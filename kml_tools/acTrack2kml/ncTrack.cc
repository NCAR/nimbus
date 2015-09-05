
#include "ncTrack.hh"
#include <netcdf.hh>
#include <stdlib.h> // putenv()

#include <iostream>

#include <boost/scoped_ptr.hpp>
using boost::scoped_ptr;

using std::cerr;
using std::endl;
using std::string;

using namespace boost::posix_time;

std::string
array_to_string(char* array)
{
  std::string x = array;
  delete[] array;
  return x;
}

ncTrack::
ncTrack()
{
  nc = 0;
}

ncTrack::
~ncTrack()
{
  close();
}

void
ncTrack::
close()
{
  delete nc;
  nc = 0;
}


/* -------------------------------------------------------------------- */
NcVar *
ncTrack::
getNetcdfVariable(const std::string& var_name)
{
  NcFile& file = *nc;
  NcVar *var;

  if (var_name.length() == 0 || (var = file.get_var(var_name.c_str())) == 0)
  {
    cerr << "acTrack2kml: unable to find variable "
	 << var << " in netCDF file, fatal." << endl;;
    exit(1);
  }

  return var;
}

/* -------------------------------------------------------------------- */
bool
ncTrack::
open(const std::string& fileName)
{
  cerr << "ReadDataFromNetCDF: " << fileName << endl;
  nc = new NcFile(fileName.c_str());
  NcFile& file = *nc;

  if (file.is_valid() == false)
  {
    cerr << "Failed to open, or invalid netCDF file." << endl;
    close();
    return false;
  }
  return true;
}


void
ncTrack::
fillAircraftTrack(AircraftTrack& track)
{
  NcFile& file = *nc;
  ProjectInfo& projInfo = track.projInfo;

  NcError err(NcError::silent_nonfatal);

  scoped_ptr<NcAtt> attr(file.get_att("FlightNumber"));
  if (attr)
    projInfo.flightNumber = array_to_string(attr->as_string(0));

  attr.reset(file.get_att("ProjectName"));
  if (attr)
    projInfo.projectName = array_to_string(attr->as_string(0));

  attr.reset(file.get_att("Platform"));
  if (attr)
    projInfo.platform = array_to_string(attr->as_string(0));

  attr.reset(file.get_att("landmarks"));
  if (attr)
    projInfo.landmarks = array_to_string(attr->as_string(0));

  string lon = "LONC";
  string lat = "LATC";
  string tim = "Time";

  attr.reset(file.get_att("longitude_coordinate"));
  if (attr)
    lon = array_to_string(attr->as_string(0));
  lon = cfg.getLongitudeVariable(lon);

  attr.reset(file.get_att("latitude_coordinate"));
  if (attr)
    lat = array_to_string(attr->as_string(0));
  lat = cfg.getLatitudeVariable(lat);

  attr.reset(file.get_att("time_coordinate"));
  if (attr)
    tim = array_to_string(attr->as_string(0));

  NcVar *tim_v = getNetcdfVariable(tim);
  NcVar *tas_v = getNetcdfVariable("TASX");
  NcVar *lat_v = getNetcdfVariable(lat);
  NcVar *lon_v = getNetcdfVariable(lon);
  NcVar *atx_v = getNetcdfVariable("ATX");
  NcVar *dp_v = getNetcdfVariable("DPXC");
  NcVar *ws_v = getNetcdfVariable("WSC");
  NcVar *wi_v = getNetcdfVariable("WIC");
  NcVar *wd_v = getNetcdfVariable("WDC");
  NcVar *thdg_v = getNetcdfVariable("THDG");

  // Pressure altitude is preferred.
  NcVar *alt_v;
  string alt = "PALTF";
  if ((alt_v = file.get_var(alt.c_str())) == 0)
  {
    alt = "PALT";
    if ((alt_v = file.get_var(alt.c_str())) == 0)
    {
      attr.reset(file.get_att("zaxis_coordinate"));
      alt = array_to_string(attr->as_string(0));
    }
  }
  alt = cfg.getAltitudeVariable(alt);
  alt_v = getNetcdfVariable(alt);
  attr.reset(alt_v->get_att("units"));
  cfg.setAltitudeUnits(array_to_string(attr->as_string(0)));

  scoped_ptr<NcValues> tim_vals(tim_v->values());
  scoped_ptr<NcValues> tas_vals(tas_v->values());
  scoped_ptr<NcValues> lat_vals(lat_v->values());
  scoped_ptr<NcValues> lon_vals(lon_v->values());
  scoped_ptr<NcValues> alt_vals(alt_v->values());
  scoped_ptr<NcValues> atx_vals(atx_v->values());
  scoped_ptr<NcValues> dp_vals(dp_v->values());
  scoped_ptr<NcValues> ws_vals(ws_v->values());
  scoped_ptr<NcValues> wi_vals(wi_v->values());
  scoped_ptr<NcValues> wd_vals(wd_v->values());
  scoped_ptr<NcValues> thdg_vals(thdg_v->values());

  attr.reset(tim_v->get_att("units"));
  struct tm tm;
  strptime(array_to_string(attr->as_string(0)).c_str(),
	   "seconds since %F %T +0000", &tm);
  char tz[] = "TZ=UTC";
  putenv(tz);
  ptime basetime = from_time_t(mktime(&tm));

  size_t n = tim_vals->num();
  track.clear();
  track.reserve(n);
  for (size_t i = 0; i < n; ++i)
  {
    // TAS cutoff deprecated in favor of loading the whole track and
    // letting the renderer clip as necessary.
    for (; false && i < n && tas_vals->as_float(i) < cfg.TAS_CutOff; ++i)
      ;

    for (; i < n &&
	   (track.isMissingValue(lat_vals->as_float(i)) ||
	    track.isMissingValue(lon_vals->as_float(i)) ||
	    track.isMissingValue(alt_vals->as_float(i))); ++i)
      ;

    if (i >= n)
      break;

    track.date.push_back(basetime + seconds(tim_vals->as_int(i)));
    track.lon.push_back( lon_vals->as_float(i) );
    track.lat.push_back( lat_vals->as_float(i) );
    float value = alt_vals->as_float(i);
    if (value != AircraftTrack::missing_value)
      value *= cfg.convertToFeet;
    track.alt.push_back( value );
    track.tas.push_back ( tas_vals->as_float(i) );
    track.at.push_back( atx_vals->as_float(i) );
    track.dp.push_back( dp_vals->as_float(i) );
    value = ws_vals->as_float(i);
    if (value != AircraftTrack::missing_value)
      value *= 1.9438;		// conver to knots.
    track.ws.push_back( value );
    track.wi.push_back( wi_vals->as_float(i) );
    track.wd.push_back( wd_vals->as_float(i) );
    track.thdg.push_back( thdg_vals->as_float(i) );
  }
}


void
ncTrack::
updateTrack(AircraftTrack& track, const std::string& ncpath)
{
  std::ostringstream msg;
  if (cfg.verbose)
  {
    cerr << "updating track from netcdf: " << ncpath << endl;
  }
  if (open(ncpath))
  {
    fillAircraftTrack(track);
    msg << "Loaded track of " << track.npoints() << " points "
	<< " from " << ncpath;
    track.setStatus(AircraftTrack::UPDATED, msg.str());
  }
  else
  {
    msg << "Error opening " << ncpath;
    track.setStatus(AircraftTrack::ERROR, msg.str());
  }
}

