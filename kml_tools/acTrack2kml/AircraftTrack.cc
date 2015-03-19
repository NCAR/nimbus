

#include "AircraftTrack.hh"
#include "TrackPath.hh"

#include <cmath>
#include <sstream>

using std::vector;
using std::stringstream;

const float AircraftTrack::missing_value;

bool
AircraftTrack::
isMissingValue(double target)
{
  if (isnan(target) || target == missing_value)
    return true;
  
  return false;
}


AircraftTrack::
AircraftTrack() :
  offset_longitude(false)
{
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


bool
AircraftTrack::
getHeading(int i, double& true_heading)
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
  vector<longitude_float> lat2(&(this->lat[i-1]), &(this->lat[i-1]) + 2);
  vector<longitude_float> lon2(&(this->lon[i-1]), &(this->lon[i-1]) + 2);

  //longitude_degrees<float>::normalize(lon2.begin(), lon2.end());

  double rlon[2], rlat[2];
  rlat[0] = lat2[0] * M_PI / 180.0;
  rlon[0] = lon2[0] * M_PI / 180.0;
  rlat[1] = lat2[1] * M_PI / 180.0;
  rlon[1] = lon2[1] * M_PI / 180.0;
  double y = sin(rlon[1] - rlon[0]) * cos(rlat[1]);
  double x = cos(rlat[0]) * sin(rlat[1]) - 
    sin(rlat[0]) * cos(rlat[1]) * cos(rlon[1]-rlon[0]);
  heading_double thdg(atan2(y, x) * 180.0 / M_PI);
  true_heading = thdg;
  return true;
}


void
AircraftTrack::
setStatus(StatusFlag flag, const std::string& msg)
{
  status = flag;
  statusMessage = msg;
}


void
AircraftTrack::
clearStatus()
{
  status = NOSTATUS;
  statusMessage = "";
}


int
AircraftTrack::
findTime(ptime when, bool ending)
{
  int where = -1;
  for (size_t i = 0; (i < npoints()) && (date[i] <= when); ++i)
  {
    where = i + ending;
  }
  return where;
}


void
AircraftTrack::
computeLongitudeOffset()
{
  // Use a path to get the bounding box on the whole track.
  if (npoints())
  {
    TrackPath path(this);
    path.generate();
    PathStats stats = path.getStats();
    PathRegion& bbox = stats.bounding_box;
    if (bbox.sw.lon.value() > bbox.ne.lon.value())
    {
      offset_longitude = true;
    }
  }
}

