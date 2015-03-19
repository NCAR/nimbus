
#ifndef _AircraftTrack_hh_
#define _AircraftTrack_hh_

#include <string>
#include <vector>
#include "angle.hh"

#include <boost/date_time/posix_time/posix_time.hpp>
#include "ProjectInfo.hh"

/**
 * The type-specific data members that make up a track are in a base class
 * by themselves.
 **/
template <typename T>
struct AircraftTrackT
{
  typedef boost::posix_time::ptime ptime;
  typedef std::vector<boost::posix_time::ptime> vector_ptime;
  typedef T value_type;
  typedef longitude<T> longitude_type;

  vector_ptime date;
  std::vector<T> lat;
  std::vector< longitude<T> > lon;
  std::vector<T> alt;
  std::vector<T> at;
  std::vector<T> dp;
  std::vector<T> tas;
  std::vector<T> ws;
  std::vector<T> wd;
  std::vector<T> wi;
  std::vector<T> thdg;
};


/**
 * Encapsulate the data which make up a generic aircraft track, especially
 * position, winds, and a few other state variables.
 **/
class AircraftTrack : public AircraftTrackT<double>
{
public:
  typedef std::string string;

  static const float missing_value = -32767.0;

  static
  std::string
  formatTimestamp(ptime pt, const std::string& format = "%Y-%m-%dT%H:%M:%SZ");

  typedef enum { NOSTATUS=0, UPDATED, NOCHANGE, ERROR } StatusFlag;

  AircraftTrack();

  /**
   * A track can have a status to indicate errors and changes made by the
   * routines which modify the track.  Clear the status with clearStatus()
   * and set it with setStatus().  The AircraftTrack is the data model, so
   * the data model incorporates errors and status related to updating the
   * model.
   **/
  void
  setStatus(StatusFlag flag, const std::string& msg);

  void
  clearStatus();

  /**
   * Return true only if no error status has been set on the track.
   **/
  bool
  ok()
  {
    return status != ERROR;
  }

  /**
   * Return true only if the track has been changed since the status was
   * last cleared.
   **/
  bool
  updated()
  {
    return status == UPDATED;
  }

  /**
   * Clear all the data vectors.
   **/
  void
  clear();

  /**
   * Shortcut for the number of points in the track.
   **/
  size_t
  npoints() const
  {
    return lat.size();
  }

  /**
   * Return the last timestamp in this track.  There must be points in the
   * track.
   **/
  ptime
  lastTime() const
  {
    return *date.rbegin();
  }

  /**
   * Return the largest index into the track whose timestamp is less than
   * or equal to the given time @p when.  If there is no such index,
   * meaning the time precedes all points in the track, then return -1.  To
   * use this to find the point just past this time, such as to set the end
   * index for a clipping window, pass @p ending as true.
   **/
  int
  findTime(ptime when, bool ending=false);

  /**
   * Reserve space for the given number of points, typically called just before 
   * filling the track with that many points.
   **/
  void
  reserve(size_t n);

  int
  barbSpeed(double ws)
  {
    return (int)((ws + 2.5) / 5) * 5;
  }

  bool
  isMissingValue(double target);

  /**
   * Always returning heading as a double means callers do not have to be
   * templates to account for the possible heading types.
   **/
  bool
  getHeading(int i, double& true_heading);

  /**
   * The longitudes in an AircraftTrack are always normalized to [-180,
   * 180), so this method allows renderers to compute a longitude offset so
   * that longitudes do not change sign across the international date line
   * (antimeridian).
   *
   * The algorithm computes the bounding box for the whole track, then if
   * the west longitude is greater than the east longitude, the offset is
   * set to 360.  The "fixed" longitude can be computed by adding the
   * offset to the values less than zero, or else assigning the longitude
   * to type longitude_positive_degrees<>.
   **/
  void
  computeLongitudeOffset();

  inline value_type
  offsetLongitude(int i)
  {
    if (offset_longitude)
    {
      value_type plon = lon[i].value();
      longitude_positive_degrees<value_type>::normalize(plon);
      return plon;
    }
    return lon[i];
  }

  bool offset_longitude;

  StatusFlag status;
  std::string statusMessage;


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
