
#ifndef _AircraftTrackKML_hh_
#define _AircraftTrackKML_hh_

#include <string>
#include <fstream>

#include "AircraftTrack.hh"
#include "ProjectInfo.hh"
#include "Config.hh"

/**
 * Generate a KML representation of an AircraftTrack.
 **/
class AircraftTrackKML
{
public:
  typedef std::string string;

  struct Coordinates
  {
    Coordinates(AircraftTrack& rtrack, int i_) :
      track(rtrack),
      i(i_)
    {}

    std::ostream&
    asKML(std::ostream&) const;

    AircraftTrack& track;
    int i;
  };

  AircraftTrackKML();

  void
  setConfig(Config& config);

  void
  setTrack(AircraftTrack* track_in);

  void
  WriteGoogleEarthAnimatedKML(const std::string& file);

  void
  WriteGoogleEarthKML(const std::string& file);

  void
  renamefile(string file, string outFile);

  /**
   * Return true if this file exists and is more recent than the track.
   */
  bool
  checkFile(const std::string& file);

  /**
   * Producing kmz files may not by useful for mission coordinator.  kmz is
   * uncompressed server side and sent to client, so there is no bandwidth
   * savings to using for web applications.  GoogleEarth can take advantage
   * of it, but we are mostly web based at this time.  cjw 5/31/2013.
   */
  void
  compressKML(const std::string& file);

  void
  WriteWindBarbsKML_Folder(std::ofstream& googleEarth);

  void
  WriteTimeStampsKML_Folder(std::ofstream& googleEarth);

  void
  WriteLandmarksKML_Folder(std::ofstream& googleEarth);

  void
  WriteCurrentPositionKML(const std::string& outfile);

  void
  WriteSpecialInclude(std::ofstream& googleEarth);

  string
  startBubbleCDATA();

  string
  endBubbleCDATA();

  string
  midBubbleCDATA(int i);

  size_t
  writeCoordinates(std::ostream& out, size_t& i, 
		   boost::posix_time::ptime& end_ts);

  /// Don't compress files for netCDF post-processing.
  bool postProcessMode;

private:

  AircraftTrack* _track;
  Config cfg;

};


inline std::ostream&
operator<<(std::ostream& out, const AircraftTrackKML::Coordinates& coords)
{
  return coords.asKML(out);
}


#endif // _AircraftTrackKML_hh_
