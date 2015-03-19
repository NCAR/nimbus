

#include "AircraftTrack.hh"
#include "TrackPath.hh"

#include <algorithm> // lower_bound

using std::vector;
using std::string;
using std::lower_bound;
namespace bpt = boost::posix_time;

bpt::ptime
nextTimestamp(bpt::ptime current, int mod_seconds)
{
  if (mod_seconds <= 0)
    return current;
  bpt::ptime basetime = bpt::ptime(current.date());
  int seconds = current.time_of_day().total_seconds();
  seconds = (seconds / mod_seconds + 1) * mod_seconds;
  return basetime + bpt::seconds(seconds);
}


TrackPath::
TrackPath(AircraftTrack& track_, int begin_, int end_) :
  _timestep(0),
  _headingstep(0),
  _pointstep(0)
{
  resetTrack(&track_, begin_, end_);
}


TrackPath::
TrackPath(AircraftTrack* trackp, int begin_, int end_) :
  _timestep(0),
  _headingstep(0),
  _pointstep(0)
{
  resetTrack(trackp, begin_, end_);
}


void
TrackPath::
resetTrack(AircraftTrack* trackp, int begin_, int end_)
{
  _track = trackp;
  path.clear();
  setClipping(begin_, end_);
}


void
TrackPath::
clipTAS(float cutoff)
{
  setClipping();
  for (begin = 0; begin < _track->npoints(); ++begin)
  {
    if (!_track->isMissingValue(_track->tas[begin]) &&
	_track->tas[begin] >= cutoff)
    {
      break;
    }
  }
  for (end = _track->npoints(); end > begin; --end)
  {
    if (!_track->isMissingValue(_track->tas[end-1]) &&
	_track->tas[end-1] >= cutoff)
    {
      break;
    }
  }
  if (end <= begin)
  {
    // Empty clipping region.
    begin = 0;
    end = 0;
  }
}


void
TrackPath::
clipTimes(boost::posix_time::ptime first, boost::posix_time::ptime last)
{
  setClipping(_track->findTime(first), _track->findTime(last, true));
}


void
TrackPath::
setClipping(int begin_, int end_)
{
  if (begin_ == end_ || !_track)
  {
    // Catch cases where window is empty or both indexes are -1 or there is
    // no track yet.
    begin = 0;
    end = 0;
  }
  else
  {
    begin = begin_;
    if (end_ == -1 || end_ > int(_track->npoints()))
    {
      end = _track->npoints();
    }
    else
    {
      end = end_;
    }
  }
}


void
TrackPath::
setTimeStep(int timestep)
{
  _timestep = timestep;
}

void
TrackPath::
setHeadingStep(int headingstep)
{
  _headingstep = headingstep;
}

void
TrackPath::
setSpecifier(const std::string& spec)
{
  _specifier = spec;

  // Parse the specifier into <method>:<interval> and set the corresponding
  // interval.
  size_t pos = _specifier.find(':');
  string method(_specifier);
  string param;
  if (pos != string::npos)
  {
    method = _specifier.substr(0, pos);
    param = _specifier.substr(pos+1);
  }

  if (method == "timestep")
  {
    if (! param.empty())
      _timestep = atoi(param.c_str());
    _method = method;
  }
  else if (method == "headingstep")
  {
    if (! param.empty())
      _headingstep = atof(param.c_str());
    _method = method;
  }
  else if (method == "pointstep")
  {
    if (! param.empty())
      _pointstep = atoi(param.c_str());
    _method = method;
  }
}


template <typename T>
std::string
formatSpecifier(const std::string& method, const T& param)
{
  std::ostringstream buf;
  buf << method << ":" << param;
  return buf.str();
}


void
TrackPath::
generate(const std::string& specifier)
{
  if (specifier.length())
  {
    setSpecifier(specifier);
  }
  if (_method == "timestep")
  {
    generateByTimeStep(_timestep);
  }
  else if (_method == "headingstep")
  {
    generateByHeading(_headingstep);
  }
  else if (_method == "pointstep")
  {
    generateByPointStep(_track->npoints(), _pointstep);
  }
  else
  {
    generateByPointStep(_track->npoints(), 1);
  }
}


void
TrackPath::
generateByTimeStep(int seconds)
{
  setSpecifier(formatSpecifier("timestep", seconds));
  path.clear();
  bpt::ptime next_ts;

  for (size_t i = begin; i < end; ++i)
  {
    if (i == begin || _track->date[i] >= next_ts || i == end-1)
    {
      next_ts = nextTimestamp(_track->date[i], seconds);
      path.push_back(i);
    }
  }
}


void
TrackPath::
generateByHeading(float diff)
{
  setSpecifier(formatSpecifier("headingstep", diff));
  path.clear();
  heading_float last_heading;

  // Include the first and last points, and then skip points for which the
  // heading has changed less than the diff parameter.

  for (size_t i = begin; i < end; ++i)
  {
    double thdg;
    if (_track->getHeading(i, thdg))
    {
      heading_float heading = thdg;
      if (i == 0 || 
	  std::fabs(angle_difference(heading, last_heading)) >= diff ||
	  i == end-1)
      {
	path.push_back(i);
	last_heading = heading;
      }
    }
  }
}




/**
 * Generate a path beginning at the start of the clipping window and
 * containing at most maxpoints stepping through the track at interval @p
 * step.  So the last point in the path will not necessarily be the last
 * point in the window.
 **/
void
TrackPath::
generateByPointStep(int maxpoints, int step)
{
  setSpecifier(formatSpecifier("pointstep", step));
  path.clear();
  if (step < 1)
    step = 1;
  if (maxpoints < 0)
    maxpoints = 0;
  for (size_t i = begin; (i < end) && (--maxpoints >= 0); i += step)
  {
    path.push_back(i);
  }
}


void
TrackPath::
mergePath(const TrackPath& path)
{
  // The optimal way to do this might be to iterate through both paths and
  // either insert as we go or just push everything onto a new replacement
  // path.  However, that might be premature, and this is really safe since
  // it relies on the conditions already handled by addPoint().

  vector<int>::const_iterator it;
  for (it = path.path.begin(); it != path.path.end(); ++it)
  {
    addPoint(*it);
  }
}


void
TrackPath::
addPoint(int index)
{
  // Check if it's in the clipping window before inserting it.
  if (index >= (int)begin && index < (int)end)
  {
    // Lower bound returns an iterator to the first path point >= index,
    // which is where we want to insert this new path point.
    vector<int>::iterator it = lower_bound(path.begin(), path.end(), index);
    if (it == path.end() || (*it) != index)
    {
      path.insert(it, index);
    }
  }
}


PathStats
TrackPath::
getStats()
{
  PathStats stats;

  stats.npoints_track = track().npoints();
  stats.npoints_path = npoints();
  stats.path_duration = bpt::seconds(0);
  // Figure out the specifier which created this path.
  stats.specifier = _specifier;

  if (npoints())
  {
    stats.path_duration = _track->date[lastIndex()] - _track->date[path[0]];

    AircraftTrack::value_type minlat=_track->lat[path[0]];
    AircraftTrack::value_type maxlat=minlat;
    AircraftTrack::longitude_type minlon=_track->lon[path[0]];
    AircraftTrack::longitude_type maxlon=minlon;
    
    vector<int>::const_iterator it;
    for (it = path.begin(); it != path.end(); ++it)
    {
      minlat = std::min(minlat, _track->lat[*it]);
      minlon = std::min(minlon, _track->lon[*it]);
      maxlat = std::max(maxlat, _track->lat[*it]);
      maxlon = std::max(maxlon, _track->lon[*it]);
    }
    stats.bounding_box = PathRegion(PathPoint(minlat, minlon),
				    PathPoint(maxlat, maxlon));
  }
  return stats;
}


std::ostream&
PathStats::
toStream(std::ostream& out) const
{
  out << "path(" << specifier << "): "
      << "ntrack=" << npoints_track << "; "
      << "npath=" << npoints_path << "; "
      << "duration=" << path_duration << "; "
      << "bbox=" << bounding_box << ";";
  return out;
}


std::ostream&
PathRegion::
toStream(std::ostream& out) const
{
  out << "(" << sw.lon << "," << sw.lat << ","
      << ne.lon << "," << ne.lat << ")";
  return out;
}


