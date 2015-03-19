
#ifndef _TrackPath_hh_
#define _TrackPath_hh_

#include <string>
#include <vector>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iosfwd>

/**
 * Given timestamp @current, compute the next timestamp following it which
 * falls on an even multiple of @p seconds.  Note that @p mod_seconds is an
 * integer, so it is not possible to advance by fractional seconds with
 * this function.  If @p mod_seconds < 1, then the @p current time is
 * returned.
 **/
boost::posix_time::ptime
nextTimestamp(boost::posix_time::ptime current, int mod_seconds);

class AircraftTrack;

struct PathPoint
{
  PathPoint(double lat_=0, longitude_double lon_=0):
    lat(lat_), lon(lon_)
  {}

  double lat;
  longitude_double lon;
};

struct PathRegion
{
  PathRegion()
  {}

  PathRegion(const PathPoint& ll, const PathPoint& ur):
    sw(ll), ne(ur)
  {}

  std::ostream&
  toStream(std::ostream& out) const;

  PathPoint sw;
  PathPoint ne;
};


inline std::ostream&
operator<< (std::ostream& out, const PathRegion& pr)
{
  return pr.toStream(out);
}


/**
 * Aggregate statistics about a generated TrackPath, such as these:
 *
 * number of points in the whole track
 * number of points in the path
 * time span of points
 * lat/lon coordinates of whole track
 * lat/lon coordinates of (50 km?) region around track start
 **/
struct PathStats
{
  PathStats() :
    npoints_track(0),
    npoints_path(0),
    path_duration(boost::posix_time::seconds(0))
  {}

  std::ostream&
  toStream(std::ostream&) const;

  int npoints_track;
  int npoints_path;
  boost::posix_time::time_duration path_duration;
  std::string specifier;
  PathRegion bounding_box;
  PathRegion takeoff;
};


inline std::ostream&
operator<< (std::ostream& out, const PathStats& stats)
{
  return stats.toStream(out);
}

/**
 * A path is a subset of the points in an AircraftTrack.  It can be
 * generated using different algorithms and criteria and then rendered for
 * formats like KML.  The implementation is just a vector of indices into
 * the full AircraftTrack, so it is only valid until the underlying
 * AircraftTrack changes.
 *
 * A TrackPath can have a clipping window which limits all paths to the
 * region from the begin index to just before the end.  By default there is
 * no clipping window.
 *
 * When a path through the track is generated with one of the path
 * generator methods (generateByTimeInterval() and generateByHeading()),
 * the path will begin and end in the clipping window.  It is possible to
 * generate a path with zero points or only one point, so the caller must
 * account for this if iterating through a track and expecting each path to
 * advance some minimum number of points through the track.
 **/
class TrackPath
{
public:

  /**
   * Create a TrackPath based on reference @p track_, optionally setting
   * the clipping window with parameters @p begin_ and @p end_.  The
   * default clipping window is the entire track.
   **/
  TrackPath(AircraftTrack& track_, int begin_=0, int end_=-1);

  /**
   * Create a TrackPath based on the AircraftTrack referenced by pointer @p
   * trackp.  If a TrackPath instance does not refer to an AircraftTrack,
   * then all of the other methods save setTrack() will cause memory
   * errors.
   **/
  TrackPath(AircraftTrack* trackp=0, int begin_=0, int end_=-1);

  /**
   * Reset this instance and base it on the given AircraftTrack @p track_.
   **/
  void
  resetTrack(AircraftTrack* trackp=0, int begin_=0, int end_=-1);

  /**
   * Set the clipping region.  The default arguments set the clipping
   * window to the entire track.  The track clipping window includes the
   * point at index @p begin up but not including the point at index @p
   * end.  Therefore setting clipping to (0, track.npoints()) includes the
   * entire track.  When @p end is passed as -1, it is converted to
   * track.npoints().
   **/
  void
  setClipping(int begin=0, int end=-1);

  void
  setTimeStep(int timestep);

  int
  getTimeStep()
  {
    return _timestep;
  }

  void
  setHeadingStep(int headingstep);

  float
  getHeadingStep()
  {
    return _headingstep;
  }

  /**
   * Path algorithm specifiers take the form <method>[:<parameter>], where
   * method can be 'timestep' or 'headingstep' or 'pointstep', and the
   * parameter is the corresponding integral interval.
   **/
  void
  setSpecifier(const std::string& spec);

  const std::string&
  getMethod()
  {
    return _method;
  }

  /**
   * Set the clipping window on this path to the first and last points
   * above the given airspeed threshold @p cutoff.
   **/
  void
  clipTAS(float cutoff);

  /**
   * Set the clipping window to include the times from @p first to @p last.
   * If the first time is not in the track exactly, then the window starts
   * with the last time which precedes it.  For the @p last time, the
   * window includes the last point which equals or does not succeed the
   * last time.
   **/
  void
  clipTimes(boost::posix_time::ptime first, boost::posix_time::ptime last);

  /**
   * Return the number of points in the generated path, not in the track
   * and not in the clipping window.  The number of points in the clipping
   * window is (end-begin).
   **/
  size_t
  npoints() const
  {
    return path.size();
  }

  /**
   * Return the last track point in the current path, or if the path is
   * empty, return zero.  This can be used to query where a succeeding path
   * should start if it's going to connect with this path.  If the
   * preceding path is empty, then the succeeding path should start at the
   * beginning.  Note that if zero is returned, it may be outside of the
   * clipping window, so when the succeeding path is generated it may not
   * actually start at track point zero.
   **/
  size_t
  lastIndex() const
  {
    if (path.size() > 0)
      return *path.rbegin();
    return 0;
  }

  /**
   * Generate a path using the given specifir, or if empty use the method
   * and parameters passed to setSpecifier().  When the specifier has not
   * been set or cannot be parsed, then the path will be the whole track.
   * Therefore it is up to the caller to make sure the defaults are set
   * appropriately before calling this method.
   **/
  void
  generate(const std::string& specifier = "");

  /**
   * Generate a path containing the first and last points in the window,
   * and then only the first point in each successive interval of @p
   * seconds.  This only works on intervals of integral seconds, it is not
   * possible to iterate by fractional seconds.  If that is ever needed,
   * then there needs to be a counterpart method which accepts integral
   * microseconds or floating point seconds.  The former is preferable.
   **/
  void
  generateByTimeStep(int seconds);

  /**
   * Generate a path containing the first and last points in the window and
   * all successive points whose heading differs by more than @p diff
   * degrees from the heading of the previous point in the path.
   **/
  void
  generateByHeading(float diff);

  /**
   * Generate a path beginning at the start of the clipping window and
   * containing at most maxpoints stepping through the track at interval @p
   * step.  So the last point in the path will not necessarily be the last
   * point in the window.
   **/
  void
  generateByPointStep(int npoints, int step=1);

  /**
   * Merge TrackPath @p path into this TrackPath.  All the points in @p
   * path which are inside this path's clipping window are inserted into
   * this path.  This is useful when generating tracks which should include
   * points selected by other path algorithms.  For example, one path may
   * pick off points for labels and another for lines, so merging those
   * paths into the line path ensures that the label points will fall on
   * the line path.
   *
   * There is no confirmation that this path and the merged path are both
   * based on the same AircraftTrack instance.
   **/
  void
  mergePath(const TrackPath& path);

  /**
   * Manually add track point at @p index to this path.  If the point is
   * within the clipping window, then it will be inserted into the path.
   * It is inserted (rather than appended for example) so the path points
   * are always in the same order as the track points.  If the point is
   * aleady in the path then this does nothing.
   **/
  void
  addPoint(int index);

  PathStats
  getStats();

  const AircraftTrack&
  track()
  {
    return *_track;
  }

  std::vector<int> path;
  size_t begin;
  size_t end;

private:

  AircraftTrack* _track;
  std::string _specifier;
  std::string _method;
  int _timestep;
  float _headingstep;
  int _pointstep;

};


#endif // _TrackPath_hh_
