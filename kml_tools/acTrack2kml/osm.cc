

#include "osm.hh"

#include <cmath>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <errno.h>

OSM::
OSM()
{}


void
OSM::
writePositionJSON(AircraftTrack& track, const std::string& filename)
{
  char output[1024] = "";

  // We need at least two points in the track.
  if (track.npoints() < 2)
  {
    if (_config.verbose)
    {
      std::cerr << "Not enough points in track to write "
		<< filename << std::endl;
    }
    return;
  }

  // Point to the last two points.  This works as long as the track vectors
  // are not resized in the meanwhile.
  float* lat = &last(track.lat) - 1;
  float* lon = &last(track.lon) - 1;
  float alt = last(track.alt);

  if (! (lat[0] < -90 || lat[0] > 90 || lon[0] < -180 || lon[0] > 180 ||
	 lat[1] < -90 || lat[1] > 90 || lon[1] < -180 || lon[1] > 180))
  {
    double rlon[2], rlat[2];
    rlat[0] = lat[0] * M_PI / 180.0;
    rlon[0] = lon[0] * M_PI / 180.0;
    rlat[1] = lat[1] * M_PI / 180.0;
    rlon[1] = lon[1] * M_PI / 180.0;
    double y = sin(rlon[1] - rlon[0]) * cos(rlat[1]);
    double x = cos(rlat[0]) * sin(rlat[1]) - sin(rlat[0]) * cos(rlat[1]) * cos(rlon[1]-rlon[0]);
    double thdg = int( (atan2(y, x) * 180.0 / M_PI + 360.0) + 0.5);

    // while (thdg < 0.0) thdg += 360.0;
    // while (thdg >= 360.0) thdg -= 360.0;

    sprintf(output,
	    "{\"alt\":\"%.1f\",\"lat\":\"%f\",\"head\":\"%.1f\",\"declination\":\"0\",\"lon\":\"%f\"}", 
	    alt, lat[1], thdg, lon[1]);
    if (_config.verbose)
    {
      std::cerr << "JSON: " << output << std::endl;
    }
  }
  else
  {
    std::cerr << "lat or lon are out of bounds: " 
	      << "(" << lon[0] << "," << lat[0] << ") - "
	      << "(" << lon[1] << "," << lat[1] << ")" << std::endl;
  }

  if ( strlen(output) )
  {
    FILE *fp;
    int errnum = 0;

    // Maybe better to write the whole contents once instead of two writes,
    // in case that hangs up NFS.
    strcat(output, "\n");
    if (_config.verbose)
      std::cerr << "writing " << filename << "..." << std::endl;
    if ( (fp = fopen(filename.c_str(), "w+")) )
    {
      if (fprintf(fp, output) < 0)
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

