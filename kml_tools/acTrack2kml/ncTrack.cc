
#include "ncTrack.hh"
#include <netcdf.hh>
#include <stdlib.h> // putenv()

#include <iostream>

using std::cerr;
using std::endl;

using namespace boost::posix_time;

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

  NcAtt* attr;
  attr = file.get_att("FlightNumber");
  if (attr)
    projInfo.flightNumber = attr->as_string(0);

  attr = file.get_att("ProjectName");
  if (attr)
    projInfo.projectName = attr->as_string(0);

  attr = file.get_att("Platform");
  if (attr)
    projInfo.platform = attr->as_string(0);

  attr = file.get_att("landmarks");
  if (attr)
    projInfo.landmarks = attr->as_string(0);

  char *lon = (char *)"LONC", *lat = (char *)"LATC", *tim = (char *)"Time";

  attr = file.get_att("longitude_coordinate");
  if (attr)
    lon = attr->as_string(0);

  attr = file.get_att("latitude_coordinate");
  if (attr)
    lat = attr->as_string(0);

  attr = file.get_att("time_coordinate");
  if (attr)
    tim = attr->as_string(0);

  NcVar *tim_v = getNetcdfVariable(tim);
  NcVar *tas_v = getNetcdfVariable("TASX");
  NcVar *lat_v = getNetcdfVariable(lat);
  NcVar *lon_v = getNetcdfVariable(lon);
  NcVar *atx_v = getNetcdfVariable("ATX");
  NcVar *dp_v = getNetcdfVariable("DPXC");
  NcVar *ws_v = getNetcdfVariable("WSC");
  NcVar *wi_v = getNetcdfVariable("WIC");
  NcVar *wd_v = getNetcdfVariable("WDC");

  // Pressure altitude is preferred.
  NcVar *alt_v;
  const char *alt = "PALTF";
  if ((alt_v = file.get_var(alt)) == 0)
  {
    cfg.convertToFeet = 3.2808;
    alt = "PALT";
    if ((alt_v = file.get_var(alt)) == 0)
    {
      attr = file.get_att("zaxis_coordinate");
      alt = attr->as_string(0);

      alt_v = getNetcdfVariable(alt);
    }
  }

  NcValues *tim_vals = tim_v->values();
  NcValues *tas_vals = tas_v->values();
  NcValues *lat_vals = lat_v->values();
  NcValues *lon_vals = lon_v->values();
  NcValues *alt_vals = alt_v->values();
  NcValues *atx_vals = atx_v->values();
  NcValues *dp_vals = dp_v->values();
  NcValues *ws_vals = ws_v->values();
  NcValues *wi_vals = wi_v->values();
  NcValues *wd_vals = wd_v->values();

  attr = alt_v->get_att("units");
  if (strcmp(attr->as_string(0), "m") == 0)
    cfg.convertToFeet = 3.2808; // feet per meter

  cerr << "alt: " << alt << " units: " << attr->as_string(0) << endl;
  cerr << "cfg.convertToFeet: " << cfg.convertToFeet << endl;

  attr = tim_v->get_att("units");
  struct tm tm;
  strptime(attr->as_string(0), "seconds since %F %T +0000", &tm);
  char tz[] = "TZ=UTC";
  putenv(tz);
  ptime basetime = from_time_t(mktime(&tm));

  size_t n = tim_vals->num();
  track.clear();
  track.reserve(n);
  for (size_t i = 0; i < n; ++i)
  {
    for (; i < n && tas_vals->as_float(i) < cfg.TAS_CutOff; ++i)
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
    track.at.push_back( atx_vals->as_float(i) );
    track.dp.push_back( dp_vals->as_float(i) );
    value = ws_vals->as_float(i);
    if (value != AircraftTrack::missing_value)
      value *= 1.9438;		// conver to knots.
    track.ws.push_back( value );
    track.wi.push_back( wi_vals->as_float(i) );
    track.wd.push_back( wd_vals->as_float(i) );
  }
}

