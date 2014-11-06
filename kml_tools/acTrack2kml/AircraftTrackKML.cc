
#include "AircraftTrackKML.hh"

#include <sstream>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sys/stat.h> // chmod()
#include <sys/types.h>
#include <unistd.h>

using std::ostringstream;
using std::ofstream;
using std::cerr;
using std::endl;

using namespace boost::posix_time;

AircraftTrackKML::
AircraftTrackKML() :
  postProcessMode(false),
  _track(0)
{
}


/* -------------------------------------------------------------------- */
void
AircraftTrackKML::
WriteCurrentPositionKML(const std::string& finalfile)
{
  ofstream googleEarth;
  AircraftTrack& track = *_track;
  ProjectInfo& projInfo = track.projInfo;

  string file = finalfile + ".tmp";
  googleEarth.open(file.c_str());
  if (googleEarth.is_open() == false)
  {
    cerr << "Failed to open output file " << file << endl;
    return;
  }

  googleEarth
	<< "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
	<< "<kml xmlns=\"http://www.opengis.net/kml/2.2\"\n"
	<< " xmlns:gx=\"http://www.google.com/kml/ext/2.2\">\n"
	<< "<Document>\n"
	<< " <name>" << projInfo.projectName << " " << projInfo.flightNumber << "</name>\n"
	<< " <Style id=\"PM1\">\n"
	<< "  <IconStyle>\n"
	<< "   <scale>0.5</scale>\n"
	<< "   <Icon>\n"
	<< "    <href>http://" << cfg.webHost << "/flight_data/display/red.png</href>\n"
	<< "   </Icon>\n"
	<< "  </IconStyle>\n"
	<< " </Style>\n"

	<< " <Folder>\n"
	<< "  <name>Current Position</name>\n"
	<< "  <Placemark>\n"
	<< "   <name>" << projInfo.platform << "</name>\n"
	<< "   <styleUrl>#PM1</styleUrl>\n"
	<< "   <Point>\n"
	<< "    <altitudeMode>" << cfg.altMode << "</altitudeMode>\n"
	<< "    <coordinates>"
	<< last(track.lon) << "," << last(track.lat) << "," << (int)last(track.alt)
	<< "</coordinates>\n"
	<< "   </Point>\n"
	<< "  </Placemark>\n"
	<< " </Folder>\n"
	<< "</Document>\n"
	<< "</kml>\n";

  googleEarth.close();
  rename(file.c_str(), finalfile.c_str());
}

/* -------------------------------------------------------------------- */
void
AircraftTrackKML::
WriteLandmarksKML_Folder(ofstream& googleEarth)
{
  AircraftTrack& track = *_track;
  ProjectInfo& projInfo = track.projInfo;
  if (projInfo.landmarks.size() == 0)
    return;

  googleEarth
    << " <Folder>\n"
    << "  <name>Landmarks</name>\n";

  char buff[8192];
  strcpy(buff, projInfo.landmarks.c_str());
  char * p = strtok(buff, ",");

  while (p)
  {
    float lat, lon;
    char label[8192];

    sscanf(p, "%f %f %s", &lat, &lon, label);

    googleEarth
      << "  <Placemark>\n"
      << "   <name>" << label << "</name>\n"
      << "   <styleUrl>#PM1</styleUrl>\n"
      << "   <Point>\n"
      << "    <coordinates>"
      << lon << ","  << lat << "</coordinates>\n"
      << "   </Point>\n"
      << "  </Placemark>\n";

    p = strtok(0, ",");
  }

  googleEarth << " </Folder>\n";
}


/**
 * Given timestamp @current, compute the next timestamp following it which
 * falls on an even multiple of @p seconds.
 **/
ptime
nextTimestamp(ptime current, int mod_seconds)
{
  ptime basetime = ptime(current.date());
  int seconds = current.time_of_day().total_seconds();
  seconds = (seconds / mod_seconds + 1) * mod_seconds;
  return basetime + boost::posix_time::seconds(seconds);
}


/* -------------------------------------------------------------------- */
void
AircraftTrackKML::
WriteTimeStampsKML_Folder(ofstream& googleEarth)
{
  AircraftTrack& track = *_track;
  ProjectInfo& projInfo = track.projInfo;
  if (track.npoints() == 0)
    return;

  googleEarth
    << " <Folder>\n"
    << "  <name>Time Stamps</name>\n";

  // The idea is to generate a timestamp placemark every cfg.ts_Freq
  // minutes, including the first and last point.
  //
  // Time format: 2014-01-19T00:27:38Z
  //
  ptime next_ts = track.date[0];
  for (size_t i = 0; i < track.date.size(); i++)
  {
    if ( (i == 0) || (track.date[i] >= next_ts) || (i == track.date.size()-1) )
    {
      next_ts = nextTimestamp(track.date[i], cfg.ts_Freq*60);
      struct tm tm = to_tm(track.date[i]);
      char label[64];
      strftime(label, sizeof(label), "%H:%M", &tm);

      googleEarth
        << "  <Placemark>\n"
        << "   <name>" << label << " " << projInfo.platform << "</name>\n"
	<< "   <description>" << midBubbleCDATA(i) << "</description>\n"
        << "   <TimeStamp>\n"
        << "    <when>" << track.formatTimestamp(track.date[i]) << "</when>\n"
        << "   </TimeStamp>\n"
        << "   <styleUrl>#PM1</styleUrl>\n"
        << "   <Point>\n"
	<< "    <altitudeMode>" << cfg.altMode << "</altitudeMode>\n"
        << "    <coordinates>"
        << track.lon[i] << "," << track.lat[i] << "," << (int)track.alt[i]
	<< "</coordinates>\n"
        << "   </Point>\n"
        << "  </Placemark>\n";
    }
  }

  googleEarth << " </Folder>\n";
}

/* -------------------------------------------------------------------- */
void
AircraftTrackKML::
WriteWindBarbsKML_Folder(ofstream& googleEarth)
{
  AircraftTrack& track = *_track;
  ProjectInfo& projInfo = track.projInfo;
  if (track.npoints() == 0)
    return;

  googleEarth
    << " <Folder>\n"
    << "  <name>Wind Barbs</name>\n";

  ptime next_ts = track.date[0];
  for (size_t i = 0; i < track.date.size(); i++)
  {
    if ( (i == 0) || (track.date[i] >= next_ts) || (i == track.date.size()-1) )
    {
      next_ts = nextTimestamp(track.date[i], cfg.barb_Freq*60);
      int iws = track.barbSpeed(track.ws[i]);	// make sure to pass in knots.
      int iwd = (int)track.wd[i];
      char url[8192];

      if (iws < 0) iws = 0;
      if (iws > 200) iws = 200;	// we only have barbs to 200 Knots.

      // Force wind direction into 0-360.
      normalizeAngles(&iwd, &iwd+1, 0);

      sprintf(url, "<href>http://%s/flight_data/display/windbarbs/%03d/wb_%03d_%03d.png</href>\n",
		cfg.webHost.c_str(), iws, iws, iwd);

      struct tm tm = to_tm(track.date[i]);
      char label[64];
      strftime(label, sizeof(label), "%H:%M", &tm);

      googleEarth
        << "  <Placemark>\n"
        << "   <name>" << label << " " << projInfo.platform << "</name>\n"
	<< "   <description>" << midBubbleCDATA(i) << "</description>\n"
        << "   <Style>\n"
        << "    <IconStyle>\n"
        << "     <scale>3</scale>\n"
        << "     <Icon>" << url << "</Icon>\n"
        << "    </IconStyle>\n"
        << "   </Style>\n"
        << "   <Point>\n"
	<< "    <altitudeMode>" << cfg.altMode << "</altitudeMode>\n"
        << "    <coordinates>"
        << track.lon[i] << "," << track.lat[i] << "," << (int)track.alt[i] << "</coordinates>\n"
        << "   </Point>\n"
        << "  </Placemark>\n";
    }
  }

  googleEarth << " </Folder>\n";
}

/* -------------------------------------------------------------------- */
void
AircraftTrackKML::
renamefile(string file, string outFile)
{
  rename(file.c_str(), outFile.c_str());
}


void
AircraftTrackKML::
compressKML(const std::string& file)
{
  std::string kmzfile = file;

  size_t ext = kmzfile.rfind(".kml");
  if (ext != string::npos && kmzfile.length() - ext == 4)
  {
    kmzfile.replace(ext, string::npos, ".kmz");
  }
  else
  {
    kmzfile = kmzfile + ".kmz";
  }
  
  std::string tfile = kmzfile + ".tmp";
  ostringstream command;
  command << "zip '" << tfile << "' '" << file << "'";
  system(command.str().c_str());
  chmod(tfile.c_str(), 0664);
  rename(tfile.c_str(), kmzfile.c_str());
}

/* -------------------------------------------------------------------- */
void
AircraftTrackKML::
WriteGoogleEarthKML(const std::string& finalfile)
{
  AircraftTrack& track = *_track;
  ProjectInfo& projInfo = track.projInfo;
  if (track.npoints() == 0)
  {
    return;
  }

  if (cfg.verbose)
  {
    cerr << "WriteGoogleEarthKML(" << finalfile << ", projectInfo);\n";
  }
  ofstream googleEarth;

  string file = finalfile + ".tmp";
  googleEarth.open(file.c_str());
  if (googleEarth.is_open() == false)
  {
    cerr << "Failed to open output file " << file << endl;
    return;
  }

  googleEarth
	<< "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
	<< "<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n"
	<< "<Document>\n"
	<< " <name>" << projInfo.projectName << " " << projInfo.flightNumber << "</name>\n"
	<< " <Style id=\"PM1\">\n"
	<< "  <IconStyle>\n"
	<< "   <scale>0.5</scale>\n"
	<< "   <Icon>\n"
	<< "    <href>http://" << cfg.webHost << "/flight_data/display/red.png</href>\n"
	<< "   </Icon>\n"
	<< "  </IconStyle>\n"
	<< " </Style>\n"
	<< " <Style id=\"PM2\">\n"
	<< "  <IconStyle>\n"
	<< "   <scale>0.5</scale>\n"
	<< "   <Icon>\n"
	<< "    <href>http://" << cfg.webHost << "/flight_data/display/white.png</href>\n"
	<< "   </Icon>\n"
	<< "  </IconStyle>\n"
	<< " </Style>\n"
	<< " <Style id=\"BOUND\">\n"
	<< "  <LineStyle>\n"
	<< "   <color>ffff0000</color>\n"
	<< "   <width>1</width>\n"
	<< "  </LineStyle>\n"
	<< " </Style>\n"

	<< " <Style id=\"TRACK_RED\">\n"
	<< "  <LineStyle>\n"
	<< "   <color>ff0000aa</color>\n"
	<< "   <width>2</width>\n"
	<< "  </LineStyle>\n"
	<< "  <PolyStyle>\n"
	<< "   <color>7f00ff00</color>\n"
	<< "  </PolyStyle>\n"
	<< " </Style>\n"

	<< " <Style id=\"TRACK_YELLOW\">\n"
	<< "  <LineStyle>\n"
	<< "   <color>ff00ffff</color>\n"
	<< "   <width>2</width>\n"
	<< "  </LineStyle>\n"
	<< "  <PolyStyle>\n"
	<< "   <color>7f00ff00</color>\n"
	<< "  </PolyStyle>\n"
	<< " </Style>\n"

	<< "<LookAt>\n"
	<< "  <range>1500000</range>\n"
	<< "  <longitude>" << last(track.lon) << "</longitude>\n"
	<< "  <latitude>" << last(track.lat) << "</latitude>\n"
	<< "</LookAt>\n"

	<< " <Folder>\n"
	<< "  <name>" << projInfo.flightNumber << "</name>\n"
	<< "  <open>1</open>\n"

/* Greg Stossmeister asked to have this removed.  Only useful to GoogleEarth.
 * Perhaps move to the wrapper KML RealTimeGV.kml if still desirable.

	<< "  <ScreenOverlay>\n"
	<< "    <name>Last Camera Image</name>\n"
	<< "    <Icon>\n"
	<< "      <href>http://www.eol.ucar.edu/flight_data/" << platform << "/images/latest_camera.jpg</href>\n"
	<< "    </Icon>\n"
	<< "    <overlayXY x=\"0\" y=\"0\" xunits=\"fraction\" yunits=\"fraction\" />\n"
	<< "    <screenXY x=\"0\" y=\"0\" xunits=\"fraction\" yunits=\"fraction\" />\n"
	<< "    <rotationXY x=\"0\" y=\"0\" xunits=\"fraction\" yunits=\"fraction\" />\n"
	<< "    <size x=\"0\" y=\"0\" xunits=\"fraction\" yunits=\"fraction\" />\n"
	<< "  </ScreenOverlay>\n";
*/;

  // Add all the points but the last hour of the track are added in yellow.
  ptime hourbreak = last(track.date) - hours(1);
  int i = 0;

  // I don't know what this was supposed to do...
  int step = cfg.TimeStep / projInfo.groundFeedDataRate;
  ptime next_ts = track.date[0];

  if (track.date[0] < hourbreak)
  {
    googleEarth
	<< "  <Placemark>\n"
	<< "   <name>Track, yellow</name>\n"
	<< "   <visibility>1</visibility>\n"
	<< "   <open>1</open>\n"
	<< "   <styleUrl>#TRACK_YELLOW</styleUrl>\n"
	<< "   <LineString>\n"
	<< "    <extrude>1</extrude>\n"
	<< "    <altitudeMode>" << cfg.altMode << "</altitudeMode>\n"
	<< "    <coordinates>\n";

    for (i = 0; i < track.npoints() && track.date[i] < hourbreak; ++i)
    {
      if (track.date[i] < next_ts)
	continue;
      next_ts = nextTimestamp(track.date[i], cfg.TimeStep);
      googleEarth << track.lon[i] << "," 
		  << track.lat[i] << "," 
		  << (int)track.alt[i] << "\n";
    }

    googleEarth
	<< "    </coordinates>\n"
	<< "   </LineString>\n"
	<< "  </Placemark>\n";
  }

  googleEarth
	<< "  <Placemark>\n"
	<< "   <name>Track, last hour</name>\n"
	<< "   <visibility>1</visibility>\n"
	<< "   <open>1</open>\n"
	<< "   <styleUrl>#TRACK_RED</styleUrl>\n"
	<< "   <LineString>\n"
	<< "    <extrude>1</extrude>\n"
	<< "    <altitudeMode>" << cfg.altMode << "</altitudeMode>\n"
	<< "    <coordinates>\n";

  // Output last hour of track, in red.
  if ((i -= step) < 0) i = 0;
  next_ts = track.date[i];
  for (; i < track.npoints(); ++i)
  {
    if (track.date[i] < next_ts)
      continue;
    next_ts = nextTimestamp(track.date[i], cfg.TimeStep);
    googleEarth << track.lon[i] << "," 
		<< track.lat[i] << "," 
		<< (int)track.alt[i] << "\n";
  }
  googleEarth	<< last(track.lon) << "," << last(track.lat) << ","
		<< (int)last(track.alt) << "\n";

  googleEarth
	<< "    </coordinates>\n"
	<< "   </LineString>\n"
	<< "  </Placemark>\n"
	<< "  <Placemark>\n"
	<< "   <name>" << projInfo.platform << "</name>\n"
	<< "   <description>" << endBubbleCDATA() << "</description>\n"
	<< "   <styleUrl>#PM1</styleUrl>\n"
	<< "   <Point>\n"
	<< "    <altitudeMode>" << cfg.altMode << "</altitudeMode>\n"
	<< "    <coordinates>" 
	<< last(track.lon) << "," << last(track.lat) << "," << last(track.alt)
	<< "</coordinates>\n"
	<< "   </Point>\n"
	<< "  </Placemark>\n"
	<< "  <Placemark>\n"
	<< "   <name>Take off</name>\n"
	<< "   <description>" << startBubbleCDATA() << "</description>\n"
	<< "   <styleUrl>#PM2</styleUrl>\n"
	<< "   <Point>\n"
	<< "    <altitudeMode>" << cfg.altMode << "</altitudeMode>\n"
	<< "    <coordinates>" << track.lon[0] << "," << track.lat[0] << "," << track.alt[0] << "</coordinates>\n"
	<< "   </Point>\n"
	<< "  </Placemark>\n";

  WriteLandmarksKML_Folder(googleEarth);
  WriteWindBarbsKML_Folder(googleEarth);
  WriteTimeStampsKML_Folder(googleEarth);

  googleEarth
		<< "</Folder>\n"
		<< "</Document>\n"
		<< "</kml>\n";


  googleEarth.close();
  renamefile(file, finalfile);
  if (! postProcessMode)
  {
    compressKML(finalfile);
  }
}

/* -------------------------------------------------------------------- */
void
AircraftTrackKML::
WriteGoogleEarthAnimatedKML(const std::string& finalfile)
{
  AircraftTrack& track = *_track;
  ProjectInfo& projInfo = track.projInfo;
  ofstream googleEarth;

  string file = finalfile + ".tmp";
  googleEarth.open(file.c_str());
  if (googleEarth.is_open() == false)
  {
    cerr << "Failed to open output file " << file << endl;
    return;
  }

  googleEarth
	<< "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
	<< "<kml xmlns=\"http://www.opengis.net/kml/2.2\"\n"
	<< " xmlns:gx=\"http://www.google.com/kml/ext/2.2\">\n"
	<< "<Document>\n"
	<< " <name>" << projInfo.projectName << " " << projInfo.flightNumber << "</name>\n"
	<< " <Style id=\"PM1\">\n"
	<< "  <IconStyle>\n"
	<< "   <scale>0.5</scale>\n"
	<< "   <Icon>\n"
	<< "    <href>http://" << cfg.webHost << "/flight_data/display/red.png</href>\n"
	<< "   </Icon>\n"
	<< "  </IconStyle>\n"
	<< " </Style>\n"
	<< " <Style id=\"PM2\">\n"
	<< "  <IconStyle>\n"
	<< "   <scale>0.5</scale>\n"
	<< "   <Icon>\n"
	<< "    <href>http://" << cfg.webHost << "/flight_data/display/white.png</href>\n"
	<< "   </Icon>\n"
	<< "  </IconStyle>\n"
	<< " </Style>\n"
	<< " <Style id=\"BOUND\">\n"
	<< "  <LineStyle>\n"
	<< "   <color>ffff0000</color>\n"
	<< "   <width>1</width>\n"
	<< "  </LineStyle>\n"
	<< " </Style>\n"
	<< " <Style id=\"TRACK\">\n"
	<< "  <LineStyle>\n"
	<< "   <color>ff0000aa</color>\n"
	<< "   <width>2</width>\n"
	<< "  </LineStyle>\n"
	<< "  <PolyStyle>\n"
	<< "   <color>7f00ff00</color>\n"
	<< "  </PolyStyle>\n"
	<< " </Style>\n"

	<< "   <LookAt>\n"
	<< "     <gx:TimeStamp>\n"
	<< "       <when>" << track.formatTimestamp(last(track.date)) << "</when>\n"
	<< "     </gx:TimeStamp>\n"
	<< "     <range>1500000</range>\n"
	<< "     <longitude>" << last(track.lon) << "</longitude>\n"
	<< "     <latitude>" << last(track.lat) << "</latitude>\n"
	<< "   </LookAt>\n"

	<< " <Folder>\n"
	<< "  <name>" << projInfo.flightNumber << "</name>\n"
	<< "  <open>1</open>\n";

  for (size_t i = 0; i < track.date.size(); )
  {
    googleEarth
	<< "  <Placemark>\n"
	<< "   <name>Track</name>\n"
	<< "   <styleUrl>#TRACK</styleUrl>\n"
	<< "   <visibility>1</visibility>\n"
	<< "   <open>1</open>\n"
	<< "   <LineString>\n"
	<< "    <extrude>1</extrude>\n"
	<< "    <altitudeMode>" << cfg.altMode << "</altitudeMode>\n"
	<< "    <coordinates>\n";

    ptime start = track.date[i];
    for (size_t j = 0; i < track.date.size() && j < 120; j += cfg.TimeStep, i += cfg.TimeStep)
      googleEarth << track.lon[i] << "," << track.lat[i] << "," << (int)track.alt[i] << "\n";

    ptime end = track.date[i >= track.date.size() ? track.date.size()-1 : i];

    googleEarth
	<< "    </coordinates>\n"
	<< "   </LineString>\n"
	<< "   <TimeSpan>\n"
	<< "     <begin>" << track.formatTimestamp(start) << "</begin>"
	<< "     <end>" << track.formatTimestamp(end) << "</end>\n"
	<< "   </TimeSpan>\n"
	<< "  </Placemark>\n";

     if (i < track.date.size())
       i -= cfg.TimeStep;
  }

  googleEarth
	<< "  <Placemark>\n"
	<< "   <name>" << projInfo.platform << "</name>\n"
	<< "   <description>" << endBubbleCDATA() << "</description>\n"
	<< "   <styleUrl>#PM1</styleUrl>\n"
	<< "   <Point>\n"
	<< "    <altitudeMode>" << cfg.altMode << "</altitudeMode>\n"
	<< "    <coordinates>"
	<< last(track.lon) << "," << last(track.lat) << "," << last(track.alt)
	<< "</coordinates>\n"
	<< "   </Point>\n"
	<< "  </Placemark>\n"
	<< "  <Placemark>\n"
	<< "   <name>Take off</name>\n"
	<< "   <description>" << startBubbleCDATA() << "</description>\n"
	<< "   <styleUrl>#PM2</styleUrl>\n"
	<< "   <Point>\n"
	<< "    <altitudeMode>" << cfg.altMode << "</altitudeMode>\n"
	<< "    <coordinates>" << track.lon[0] << "," << track.lat[0] << "," << track.alt[0]
	<< "</coordinates>\n"
	<< "   </Point>\n"
	<< "  </Placemark>\n";

  WriteLandmarksKML_Folder(googleEarth);
  WriteTimeStampsKML_Folder(googleEarth);
  WriteWindBarbsKML_Folder(googleEarth);

  googleEarth
		<< "</Folder>\n"
		<< "</Document>\n"
		<< "</kml>\n";


  googleEarth.close();
  renamefile(file, finalfile);
  if (! postProcessMode)
  {
    compressKML(finalfile);
  }
}


/* -------------------------------------------------------------------- */
/* Copies verbatim from a text file called "include.kml", this allows for
 * user define stuff to be added.
 */
void
AircraftTrackKML::
WriteSpecialInclude(ofstream& googleEarth)
{
  std::ifstream inc;

  inc.open("include.kml");
  if (inc.is_open())
  {
    char buff[8192];
    while (!inc.eof())
    {
      inc.getline(buff, 8192);
      googleEarth << buff;
    }
    inc.close();
  }
}

/* -------------------------------------------------------------------- */
std::string
AircraftTrackKML::
startBubbleCDATA()
{
  AircraftTrack& track = *_track;
  std::ostringstream s;

  s << "<![CDATA[" << track.formatTimestamp(track.date[0], "%H:%M:%S")
    << std::fixed
    << " Lat : " << track.lat[0]
    << " deg_N<br>Lon : " << track.lon[0] << " deg_E<br>";
  s.precision(0);
  if (track.alt[0] != track.missing_value) 
    s << "Alt : " << track.alt[0] << " feet<br>";
  s.precision(2);
  if (track.at[0] != track.missing_value) 
    s << "Temp : " << track.at[0] << " C<br>";
  if (track.dp[0] != track.missing_value) 
    s << "DP : " << track.dp[0] << " C<br>";
  if (track.ws[0] != track.missing_value) 
    s << "WS : " << track.ws[0] << " knots<br>";
  if (track.wd[0] != track.missing_value) 
    s << "WD : " << track.wd[0] << " degree_T<br>";
  if (track.wi[0] != track.missing_value) 
    s << "WI : " << track.wi[0] << " m/s";
  s << "]]>";
  return s.str();
}

/* -------------------------------------------------------------------- */
std::string
AircraftTrackKML::
endBubbleCDATA()
{
  AircraftTrack& track = *_track;
  ostringstream e;
  // Time format: 07:26:27
  ptime endTime = last(track.date);

  e	<< "<![CDATA[" << track.formatTimestamp(endTime, "%H:%M:%S")
	<< "<br>" << std::fixed
	<< " Lat : " << last(track.lat)
	<< " deg_N<br>Lon : " << last(track.lon) << " deg_E<br>";
  e.precision(0);
  if (last(track.alt) != track.missing_value) 
    e << "Alt : " << last(track.alt) << " feet<br>";
  e.precision(2);
  if (last(track.at) != track.missing_value) 
    e << "Temp : " << last(track.at) << " C<br>";
  if (last(track.dp) != track.missing_value) 
    e << "DP : " << last(track.dp) << " C<br>";
  if (last(track.ws) != track.missing_value) 
    e << "WS : " << last(track.ws) << " knots<br>";
  if (last(track.wd) != track.missing_value) 
    e << "WD : " << last(track.wd) << " degree_T<br>";
  if (last(track.wi) != track.missing_value) 
    e << "WI : " << last(track.wi) << " m/s";
  e << "]]>";

  return e.str();
}

/* -------------------------------------------------------------------- */
std::string
AircraftTrackKML::
midBubbleCDATA(int i)
{
  AircraftTrack& track = *_track;
  ostringstream e;

  e	<< std::fixed << "<![CDATA[" << " Lat : " << track.lat[i]
	<< " deg_N<br>Lon : " << track.lon[i] << " deg_E<br>";
  e.precision(0);
  if (track.alt[i] != track.missing_value) e << "Alt : " << track.alt[i] << " feet<br>";
  e.precision(2);
  if (track.at[i] != track.missing_value) e << "Temp : " << track.at[i] << " C<br>";
  if (track.dp[i] != track.missing_value) e << "DP : " << track.dp[i] << " C<br>";
  if (track.ws[i] != track.missing_value) e << "WS : " << track.ws[i] << " knots<br>";
  if (track.wd[i] != track.missing_value) e << "WD : " << track.wd[i] << " degree_T<br>";
  if (track.wi[i] != track.missing_value) e << "WI : " << track.wi[i] << " m/s";
  e << "]]>";


  return e.str();
}



void
AircraftTrackKML::
setTrack(AircraftTrack* track_in)
{
  _track = track_in;
  AircraftTrack& track = *_track;

  if (cfg.TimeStep < track.projInfo.groundFeedDataRate)
  {
    cfg.TimeStep = track.projInfo.groundFeedDataRate;
  }
  if (cfg.verbose > 1)
  {
    cerr << "KML.TimeStep = " << cfg.TimeStep << endl;
    cerr << "KML: Normalizing longitude to 0-360." << endl;
  }
  track.normalizeLongitude(-180.0);
}


void
AircraftTrackKML::
setConfig(Config& config)
{
  cfg = config;
}



bool
AircraftTrackKML::
checkFile(const std::string& file)
{
  // The file does not need to be rewritten if either the track has no
  // points or the file exists and mtime is after the last point.  Return
  // false if the file needs to be updated.
  AircraftTrack& track = *_track;
  bool recent = false;
  struct stat sbuf;
  if (stat(file.c_str(), &sbuf) == 0)
  {
    ptime pt = from_time_t(sbuf.st_mtime);
    recent = (track.npoints() > 0) && (pt > *track.date.rbegin());
  }
  return recent || (track.npoints() == 0);
}
