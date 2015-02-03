

#include "AircraftTrack.hh"

#include <cmath>
#include <sstream>

using std::stringstream;

const float AircraftTrack::missing_value;

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
  thdg.clear();
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
  thdg.reserve(n);
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


bool
AircraftTrack::
getHeading(int i, float& true_heading)
{
  // Either return the true heading variable, or compute a true heading
  // with the preceding point.
  if (!this->thdg.empty() && this->thdg[i] != missing_value)
  {
    true_heading = this->thdg[i];
    return true;
  }

  // We're stuck with computing the heading, which only works with two
  // points.  For the very first point, use the succeeding point instead.
  if (this->npoints() < 2)
  {
    return false;
  }
  if (i == 0)
  {
    ++i;
  }

  // Copy the last two points so we can shift them to the range we want for the
  // position file.
  std::vector<float> lat2(&(this->lat[i-1]), &(this->lat[i-1]) + 2);
  std::vector<float> lon2(&(this->lon[i-1]), &(this->lon[i-1]) + 2);

  normalizeAngles(lon2.begin(), lon2.end(), -180);
  normalizeAngles(lat2.begin(), lat2.end(), -90);

  double rlon[2], rlat[2];
  rlat[0] = lat2[0] * M_PI / 180.0;
  rlon[0] = lon2[0] * M_PI / 180.0;
  rlat[1] = lat2[1] * M_PI / 180.0;
  rlon[1] = lon2[1] * M_PI / 180.0;
  double y = sin(rlon[1] - rlon[0]) * cos(rlat[1]);
  double x = cos(rlat[0]) * sin(rlat[1]) - 
    sin(rlat[0]) * cos(rlat[1]) * cos(rlon[1]-rlon[0]);
  double thdg = int( (atan2(y, x) * 180.0 / M_PI + 360.0) + 0.5);

  normalizeAngles(&thdg, &thdg+1, 0.0);
  true_heading = thdg;
  return true;
}
