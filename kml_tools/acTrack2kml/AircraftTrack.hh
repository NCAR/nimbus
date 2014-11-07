
#ifndef _AircraftTrack_hh_
#define _AircraftTrack_hh_

#include <string>
#include <vector>

#include <boost/date_time/posix_time/posix_time.hpp>
#include "ProjectInfo.hh"

/**
 * Encapsulate the data which make up a generic aircraft track, especially
 * position, winds, and a few other state variables.
 **/
class AircraftTrack
{
public:
  typedef std::string string;
  typedef boost::posix_time::ptime ptime;
  typedef std::vector<boost::posix_time::ptime> vector_ptime;

  static const float missing_value = -32767.0;

  static
  std::string
  formatTimestamp(ptime pt, const std::string& format = "%Y-%m-%dT%H:%M:%SZ");

  AircraftTrack()
  {}

  /**
   * Clear all the data vectors.
   **/
  void
  clear();

  /**
   * Shortcut for the number of points in the track.
   **/
  size_t
  npoints()
  {
    return lat.size();
  }

  /**
   * Return the last timestamp in this track.  There must be points in the track.
   **/
  ptime
  lastTime()
  {
    return *date.rbegin();
  }

  /**
   * Reserve space for the given number of points, typically called just before 
   * filling the track with that many points.
   **/
  void
  reserve(size_t n);

  int
  barbSpeed(float ws)
  {
    return (int)((ws + 2.5) / 5) * 5;
  }

  bool
  isMissingValue(float target);

  /**
   * Fix all the longitude values to fit within the range [lon0, lon0+360),
   * where lon0 defaults to 0.
   **/
  void
  normalizeLongitude(float lon0 = 0);

  bool
  getHeading(int i, float& true_heading);

  vector_ptime date;
  std::vector<float> lat;
  std::vector<float> lon;
  std::vector<float> alt;
  std::vector<float> at;
  std::vector<float> dp;
  std::vector<float> tas;
  std::vector<float> ws;
  std::vector<float> wd;
  std::vector<float> wi;
  std::vector<float> thdg;

  ProjectInfo projInfo;

  /**
   * The track can be simplified to a subset of points.  This vector
   * identifies the members of the path.
   **/
  std::vector<bool> simple_path_point;
};


/**
 * Normalize an angular variable to within the range [origin, origin+360).
 **/
template <typename IT, typename T>
void
normalizeAngles(IT begin, IT end, T origin = 0)
{
  for (IT it = begin; it != end; ++it)
  {
    while (*it < origin)
      *it += 360.0;
    while (*it >= origin+360.0)
      *it -= 360.0;
  }
}

/**
 * A convenient template for getting a reference to the last element of a
 * vector.
 **/
template <typename T>
T&
last(std::vector<T>& v)
{
  return *v.rbegin();
}


#endif // _AircraftTrack_hh_
