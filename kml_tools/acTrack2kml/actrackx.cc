/*
 * COPYRIGHT: University Corporation for Atmospheric Research, 2005-2015
 */

#include "ACTrackDriver.hh"

using std::cout;
using std::endl;

std::string
insert_kml_suffix(std::string path, const std::string& suffix)
{
  path.insert(path.length()-4, std::string("-")+suffix);
  return path;
}


void
report_heading_path(TrackPath& path, float degrees)
{
  path.generateByHeading(degrees);
  cout << "Path with points where heading steps by " << degrees << ":\n";
  cout << path.getStats() << "\n";
}


/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  ACTrackDriver driver;

  // Given a track input source on the command line, load the track and
  // generate paths for it using different algorithms and compare the paths
  // and their KML output.

  Config config = driver.getConfig();
  config.run_once = true;
  config.verbose = 2;
  // It turns out it is not necessary to use a relative URL for things like
  // the icons and wind barbs, it works to load them from the EOL web site.
  //config.flightDataURL = "flight_data";
  //config.outputKML = "UNSET";
  driver.setConfig(config);

  if (driver.parseConfig(argc, argv) != 0)
  {
    return 1;
  }
  config = driver.getConfig();

  // Load the track and report some statistics about it.
  driver.loadTrack();
  AircraftTrack& track = driver.getTrack();
  if (! track.ok())
  {
    return 1;
  }
  cout << "Loaded track with " << track.npoints() << " points for "
       << "airspeed cutoff at " << config.TAS_CutOff << endl;

  // Derive a better KML output name if the default above was not
  // explicitly overridden by the command line.
  if (config.outputKML == "UNSET")
  {
    std::ostringstream buf;
    ProjectInfo& proj = track.projInfo;
    buf << proj.projectName << "-" << proj.flightNumber << "-real-time.kml";
    config.outputKML = buf.str();
  }

  // Try the same track with a different cutoff.
  config.TAS_CutOff = 0;
  config.compressKML = false;
  driver.setConfig(config);
  track.clear();
  driver.loadTrack();
  if (! track.ok())
  {
    return 1;
  }
  cout << "Loaded track with " << track.npoints() << " points for "
       << "airspeed cutoff at " << config.TAS_CutOff << endl;

  // Now compare paths on the longer track with no cutoff.
  TrackPath path(track);
  path.generateByTimeStep(config.TimeStep);
  cout << "Path with points every " << config.TimeStep << " seconds:\n"
       << path.getStats() << "\n";

  report_heading_path(path, 1);
  report_heading_path(path, 0.25);
  report_heading_path(path, 0.1);

  // Now dump this path to KML, first with default and then using heading.
  //driver.setConfig(config);

  AircraftTrackKML kml;
  config.color = "ffff0000";
  config.path_method = "timestep:15";
  kml.setConfig(config);
  kml.setTrack(&track);
  kml.WriteGoogleEarthKML(insert_kml_suffix(config.outputKML, "timestep"));

  config.color = "ff00ff00";
  config.path_method = "headingstep:1.0";
  kml.setConfig(config);
  kml.setTrack(&track);
  kml.WriteGoogleEarthKML(insert_kml_suffix(config.outputKML, "headingstep"));

  return 0;
}
