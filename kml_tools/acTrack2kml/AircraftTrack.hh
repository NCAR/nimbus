
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
  int
  npoints()
  {
    return int(lat.size());
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

  ProjectInfo projInfo;
};


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
