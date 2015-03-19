

#include "osm.hh"

#include <cmath>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <errno.h>

OSM::
OSM()
{}


std::string
OSM::
formatPositionJSON(AircraftTrack& track, int i)
{
  if (i == -1)
  {
    i = track.npoints()-1;
  }
  double thdg;
  if (!track.getHeading(i, thdg))
  {
    if (_config.verbose)
    {
      std::cerr << "Not enough points in track to write JSON." << std::endl;
    }
    return "";
  }

  // Grab copies of lat and lon so we can normalize them as needed for the
  // position file.
  double lat = track.lat[i];
  longitude_double lon = track.lon[i];

  char output[1024] = "";
  sprintf(output,
	  "{\"timestamp\":\"%s\",\"alt\":\"%.1f\",\"lat\":\"%f\","
	  "\"head\":\"%s\",\"declination\":\"0\",\"lon\":\"%f\"}",
	  track.formatTimestamp(track.date[i], "%Y-%m-%d %H:%M:%S").c_str(),
	  track.alt[i], lat, heading_double(thdg).format("%.1f").c_str(), 
	  lon.value());
  return output;
}


void
OSM::
writePositionJSON(AircraftTrack& track, const std::string& filename)
{
  std::string content = formatPositionJSON(track);
  if (_config.verbose)
  {
    std::cerr << "JSON: " << content << std::endl;
  }
  if (content.length())
  {
    FILE *fp;
    int errnum = 0;

    // Maybe better to write the whole contents once instead of two writes,
    // in case that hangs up NFS.
    content.append("\n");
    if (_config.verbose)
      std::cerr << "writing " << filename << "..." << std::endl;
    if ( (fp = fopen(filename.c_str(), "w+")) )
    {
      if (fprintf(fp, content.c_str()) < 0)
      {
	errnum = errno;
      }
      fclose(fp);
      if (_config.verbose)
	std::cerr << "Done writing " << filename << std::endl;
    }
    else
    {
      errnum = errno;
    }
    if (errnum != 0)
    {
      std::ostringstream oss;
      oss << "osm: error " << errnum << ": " << strerror(errnum)
	  << ": " << filename;
      std::cerr << oss.str() << std::endl;
    }
  }
}

