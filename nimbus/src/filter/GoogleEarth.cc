#include "config.h"
#include "GoogleEarth.h"

#include <sstream>

const size_t GoogleEarthKML::TimeStep = 10;

extern Config cfg;

/* -------------------------------------------------------------------- */
GoogleEarthKML::GoogleEarthKML(std::string nc_file)
{
  // Strip extension off outputfile name, then add .kml.
  std::string out_file = nc_file.substr(0, nc_file.find_last_of("/"));

  out_file += "/";
  out_file += cfg.ProjectNumber() + cfg.FlightNumber();
  out_file += ".kml";

  _headerWritten = false;

  _outFile.open(out_file.c_str());
  if (_outFile.is_open() == false)
  {
    fprintf(stderr, "GoogleEarthKML: failed to open output file %s\n", out_file.c_str());
    return;
  }
    fprintf(stderr, "GoogleEarth KML output file = %s\n", out_file.c_str());
}

/* -------------------------------------------------------------------- */
GoogleEarthKML::~GoogleEarthKML()
{
  if (_outFile.is_open() == false)
    return;

  createFooter();
  _outFile.close();
}

/* -------------------------------------------------------------------- */
std::string
GoogleEarthKML::
startBubbleCDATA()
{
  std::stringstream s;

  s	<< "<![CDATA[Take off :" << cfg.FlightDate() << " " << _startTime
	<< "<br>Alt : " << _latestALT << "]]>";

  return s.str();
}

/* -------------------------------------------------------------------- */
std::string
GoogleEarthKML::
endBubbleCDATA()
{
  std::stringstream e;

  e << "<![CDATA[Landed: " << _endTime << "<br>Alt : " << _latestALT << "]]>";

  return e.str();
}

/* -------------------------------------------------------------------- */
void
GoogleEarthKML::
createHeader()
{
  _outFile	<< "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		<< "<kml xmlns=\"http://earth.google.com/kml/2.0\">\n"
		<< "<Document>\n"
		<< "  <Style id=\"PM1\">\n"
		<< "    <IconStyle>\n"
		<< "      <scale>0.5</scale>\n"
		<< "      <Icon>\n"
		<< "        <href>http://www3/rt_flights/images/red.png</href>\n"
		<< "      </Icon>\n"
		<< "    </IconStyle>\n"
		<< "  </Style>\n"
		<< "  <Style id=\"PM2\">\n"
		<< "    <IconStyle>\n"
		<< "      <scale>0.5</scale>\n"
		<< "      <Icon>\n"
		<< "        <href>http://www3/rt_flights/images/white.png</href>\n"
		<< "      </Icon>\n"
		<< "    </IconStyle>\n"
		<< "  </Style>\n"
		<< "  <Style id=\"TRACK1\">\n"
		<< "   <LineStyle>\n"
		<< "    <color>ff00ffff</color>\n"
		<< "   </LineStyle>\n"
		<< "   <PolyStyle>\n"
		<< "    <color>7f00ff00</color>\n"
		<< "   </PolyStyle>\n"
		<< "  </Style>\n"
		<< "<Folder>\n"
		<< "<Placemark>\n"
		<< "  <name>" << cfg.FlightNumber() << "</name>\n"
		<< "  <LookAt>\n"
		<< "   <longitude>" << _latestLON << "</longitude>\n"
		<< "   <latitude>" << _latestLAT << "</latitude>\n"
		<< "   <range>500000</range>\n"
		<< "   <tilt>55</tilt>\n"
		<< "   <heading>0</heading>\n"
		<< "  </LookAt>\n"
		<< "  <visibility>1</visibility>\n"
		<< "  <open>0</open>\n"
		<< "  <Style>\n"
		<< "   <LineStyle>\n"
		<< "    <color>ff00ffff</color>\n"
		<< "   </LineStyle>\n"
		<< "   <PolyStyle>\n"
		<< "    <color>7f00ff00</color>\n"
		<< "   </PolyStyle>\n"
		<< "  </Style>\n"
		<< "  <LineString>\n"
		<< "   <extrude>1</extrude>\n"
		<< "   <tessellate>1</tessellate>\n"
		<< "   <altitudeMode>absolute</altitudeMode>\n"
		<< "  <Point>\n"
		<< "    <altitudeMode>absolute</altitudeMode>\n"
		<< "    <coordinates>" << _latestLON << ", " << _latestLAT << ", " << _latestALT << "</coordinates>\n"
		<< "  </Point>\n"
		<< "   <coordinates>\n";

  _headerWritten = true;
}

/* -------------------------------------------------------------------- */
void
GoogleEarthKML::
update()
{
  if (_outFile.is_open() == false)
    return;

  if (_latestTAS > 30.0)
  {
    if (!_headerWritten)
      createHeader();

    if (++_timeCounter % TimeStep == 0)
      _outFile << "    " << _latestLON << "," << _latestLAT << "," << _latestALT << "\n";
  }
}

/* -------------------------------------------------------------------- */
void
GoogleEarthKML::
createFooter()
{
  _outFile	<< "   </coordinates>\n"
		<< "  </LineString>\n"
		<< " </Placemark>\n"
		<< " <Placemark>\n"
		<< "  <name>Plane</name>\n"
		<< "  <description>" << endBubbleCDATA() << "</description>\n"
		<< "  <styleUrl>#PM1</styleUrl>\n"
		<< "  <Point>\n"
		<< "   <altitudeMode>absolute</altitudeMode>\n"
		<< "    <coordinates>" << _latestLON << ", " << _latestLAT << ", " << _latestALT << "</coordinates>\n"
		<< "  </Point>\n"
		<< " </Placemark>\n"
		<< " <Placemark>\n"
		<< "  <name>Take off</name>\n"
		<< "  <description>" << startBubbleCDATA() << "</description>\n"
		<< "  <styleUrl>#PM2</styleUrl>\n"
		<< " </Placemark>\n"
		<< " </Folder>\n"
		<< "</Document>\n"
		<< "</kml>\n";
}
