

#include "AircraftTrack.hh"

#include <cmath>
#include <sstream>

using std::stringstream;

bool
AircraftTrack::
isMissingValue(float target)
{
  if (isnan(target) || target == missing_value)
    return true;
  
  return false;
}


void
AircraftTrack::
clear()
{
  date.clear();
  lat.clear();
  lon.clear();
  alt.clear();
  at.clear();
  dp.clear();
  tas.clear();
  ws.clear();
  wd.clear();
  wi.clear();
}

void
AircraftTrack::
reserve(size_t n)
{
  date.reserve(n);
  lat.reserve(n);
  lon.reserve(n);
  alt.reserve(n);
  at.reserve(n);
  dp.reserve(n);
  tas.reserve(n);
  ws.reserve(n);
  wd.reserve(n);
  wi.reserve(n);
}


/**
 * Format a time into the conventional time string.
 **/
std::string
AircraftTrack::
formatTimestamp(ptime pt, const std::string& format)
{
  char timestring[80];
  struct tm tm = to_tm(pt);
  strftime(timestring, sizeof(timestring), format.c_str(), &tm);
  return timestring;
}


void
AircraftTrack::
normalizeLongitude(float lon0)
{
  normalizeAngles(lon.begin(), lon.end(), lon0);
}

