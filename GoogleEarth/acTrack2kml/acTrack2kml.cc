/*
 * Program to read real-time flight database periodically (say 60 seconds)
 * and create a KML for google earth.
 */
#include <libpq-fe.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <cmath>
#include <cstdio>
#include <cstring>

#include <netcdf.hh>

#include "boost/lexical_cast.hpp"

// Output directories for .xml & .kml files.  Ground.
static const std::string grnd_googleEarthDataDir = "/net/www/docs/flight_data/";
static const std::string grnd_webHost = "www.eol.ucar.edu";

// Output directories for .xml & .kml files.  Onboard.
static const std::string onboard_googleEarthDataDir = "/var/www/html/flight_data/";
static const std::string onboard_webHost = "acserver.raf.ucar.edu";

static std::string	googleEarthDataDir, webHost;

// All datapoints are read from file, but only use every 'TimeStep' points.
// e.g. 15 would mean use 1 data point for every 15 seconds of data.
static int TimeStep = 15;

// True Airspeed cut-off (take-off and landing speed).
static const float TAS_CutOff = 20.0;

// Frequency of Time Stamps (in minutes).
static int ts_Freq = 10;

// Frequency of Wind Barbs (in minutes).
static int barb_Freq = 7;

static std::string netCDFinputFile, outputKML, database_host, platform, dbname;

static const float missing_value = -32767.0;

static bool PostProcessMode = false;

// Our raw data is coming from a PostGreSQL database.....
// The order of this enum should match the order of the variables
// being requested in the dataQuery string.
enum VariablePos { TIME=0, LON, LAT, ALT, AT, TAS, WS, WD, WI };
static const std::string _dataQuerySuffix = ",atx,tasx,wsc,wdc,wic FROM raf_lrt WHERE TASX > ";


class _projInfo
{
public:
  _projInfo() : groundFeedDataRate(1) { };

  std::string platform;
  std::string projectName;
  std::string flightNumber;
  std::string landmarks;
  int groundFeedDataRate;
} projectInfo;

const char *status[] = { "Pre-flight", "In-flight", "Landed" };

std::vector<std::string> _date;
std::vector<float> _lat, _lon, _alt, _at, _tas, _ws, _wd, _wi;
float firstAT = 0.0, firstTAS = -1.0, firstWS = 0.0, firstWD = 0.0, firstWI = 0.0;
//float latestAT, latestTAS, latestWS, latestWD, latestWI;


/* -------------------------------------------------------------------- */
bool
isMissingValue(float target)
{
  if (isnan(target) || target == missing_value)
    return true;

  return false;
}

/* -------------------------------------------------------------------- */
template <typename T>
T
extractPQvalue(const char* cpqstring)
{
  if (! cpqstring)
  {
    std::cerr << "extractPQvalue: string is null!\n";
    return T();
  }
  try
  {
    return boost::lexical_cast<T>(cpqstring);
  }
  catch (const boost::bad_lexical_cast& badcast)
  {
    std::cerr << "could not parse '" << cpqstring << "': "
                << badcast.what() << std::endl;
  }
  return T();
}

/* -------------------------------------------------------------------- */
std::string
extractPQString(PGresult *result, int tuple, int field)
{
  const char* pqstring = PQgetvalue(result, tuple, field);
  if (! pqstring)
    return "";

  int len = strlen(pqstring);
  while (len > 0 &&
          isascii(pqstring[len-1]) &&
          isspace(pqstring[len-1]))
    len--;

  return std::string(pqstring, len);
}

/* -------------------------------------------------------------------- */
std::string getGlobalAttribute(PGconn * conn, std::string attr)
{
  std::string query = "SELECT value FROM global_attributes WHERE key='";
  query += attr + "'";
  PGresult * res = PQexec(conn, query.c_str());

  int ntuples = PQntuples(res);

  if (ntuples == 0)
  {
    std::cerr << "No global attribute " << attr << "!\n";
    return "";
  }
  std::string s = extractPQString(res, 0, 0);
  PQclear(res);
  return s;
}

/* -------------------------------------------------------------------- */
/**
 * Read the 'coordinates' attribute form the database to get the names of
 * the lat, lon and alt variables.
 */
std::string buildDataQueryString(PGconn * conn)
{
  std::string dataQuery = "SELECT datetime,";

  std::string lat = getGlobalAttribute(conn, "latitude_coordinate");
  std::string lon = getGlobalAttribute(conn, "longitude_coordinate");
  std::string alt = getGlobalAttribute(conn, "zaxis_coordinate");

  dataQuery += lon + "," + lat + "," + alt + _dataQuerySuffix;

  char tmp[100];
  sprintf(tmp, "%f ORDER BY datetime", TAS_CutOff);
  dataQuery += tmp;

  return dataQuery;
}

/* -------------------------------------------------------------------- */
std::string
startBubbleCDATA()
{
  std::stringstream s;
  std::string startTime = _date[0].substr(_date[0].find('T')+1);

  s     << "<![CDATA[" << startTime
        << "<br>Alt : " << _alt[0]
        << " feet<br>Temp : " << firstAT
        << " C<br>WS : " << firstWS
        << " m/s<br>WD : " << firstWD
        << " degree_T<br>WI : " << firstWI << " m/s]]>";

  return s.str();
}

/* -------------------------------------------------------------------- */
std::string
endBubbleCDATA()
{
  std::stringstream e;
  std::string s = _date[_date.size()-1];
  std::string endTime = s.substr(s.find('T')+1);

  e << "<![CDATA[";

  e	<< endTime << "<br>" 
	<< " Lat : " << _lat[_lat.size()-1]
	<< " deg_N<br>Lon : " << _lon[_lon.size()-1]
	<< " deg_E<br>Alt : " << _alt[_alt.size()-1]
	<< " feet<br>Temp : " << _at[_at.size()-1]
	<< " C<br>WS : " << _ws[_ws.size()-1]
	<< " m/s<br>WD : " << _wd[_wd.size()-1]
	<< " degree_T<br>WI : " << _wi[_wi.size()-1] << " m/s]]>";

  return e.str();
}

/* -------------------------------------------------------------------- */
std::string
midBubbleCDATA(int i)
{
  std::stringstream e;
  std::string s = _date[i];
  std::string endTime = s.substr(s.find('T')+1);

  e << "<![CDATA[";

  e	<< " Lat : " << _lat[i]
	<< " deg_N<br>Lon : " << _lon[i]
	<< " deg_E<br>Alt : " << _alt[i]
	<< " feet<br>Temp : " << _at[i]
	<< " C<br>WS : " << _ws[i]
	<< " m/s<br>WD : " << _wd[i]
	<< " degree_T<br>WI : " << _wi[i] << " m/s]]>";

  return e.str();
}

/* -------------------------------------------------------------------- */
int barbSpeed(float ws)
{
  int iws = 0;

  if (ws > 2.5 && ws < 7.5) iws = 5;
  if (ws >= 7.5 && ws < 12.5) iws = 10;
  if (ws >= 12.5 && ws < 17.5) iws = 15;
  if (ws >= 17.5 && ws < 22.5) iws = 20;
  if (ws >= 22.5 && ws < 27.5) iws = 25;
  if (ws >= 27.5 && ws < 32.5) iws = 30;
  if (ws >= 32.5 && ws < 37.5) iws = 35;
  if (ws >= 37.5 && ws < 42.5) iws = 40;
  if (ws >= 42.5 && ws < 47.5) iws = 45;
  if (ws >= 47.5 && ws < 52.5) iws = 50;
  if (ws >= 52.5 && ws < 57.5) iws = 55;
  if (ws >= 57.5 && ws < 62.5) iws = 60;
  if (ws >= 62.5 && ws < 67.5) iws = 65;
  if (ws >= 67.5 && ws < 72.5) iws = 70;
  if (ws >= 72.5 && ws < 77.5) iws = 75;
  if (ws >= 77.5 && ws < 82.5) iws = 80;
  if (ws >= 82.5 && ws < 87.5) iws = 85;
  if (ws >= 87.5 && ws < 92.5) iws = 90;
  if (ws >= 92.5 && ws < 97.5) iws = 95;
  if (ws >= 97.5 && ws < 102.5) iws = 100;
  if (ws >= 102.5 && ws < 105.5) iws = 105;

  return iws;
}

/* -------------------------------------------------------------------- */
void WriteCurrentPositionKML(const _projInfo& projInfo)
{
  std::ofstream googleEarth;
  std::string file;

  file = googleEarthDataDir + "current_pos.kml";

  std::string temp = googleEarthDataDir + "latest" + ".kml";

  googleEarth.open(temp.c_str());
  if (googleEarth.is_open() == false)
  {
    std::cerr << "Failed to open output file " << temp << std::endl;
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
	<< "    <href>http://" << webHost << "/flight_data/display/red.png</href>\n"
	<< "   </Icon>\n"
	<< "  </IconStyle>\n"
	<< " </Style>\n"

	<< " <Folder>\n"
	<< "  <name>Current Position</name>\n"
	<< "  <Placemark>\n"
	<< "   <name>GV</name>\n"
	<< "   <styleUrl>#PM1</styleUrl>\n"
	<< "   <Point>\n"
	<< "    <altitudeMode>absolute</altitudeMode>\n"

	<< "    <coordinates>"
		<< _lon[_lon.size()-1] << ","
		<< _lat[_lat.size()-1] << ","
		<< (int)_alt[_alt.size()-1]
	<< "</coordinates>\n"
	<< "   </Point>\n"
	<< "  </Placemark>\n"
	<< " </Folder>\n"
	<< "</Document>\n"
	<< "</kml>\n";

  googleEarth.close();
  rename(temp.c_str(), file.c_str());
}

/* -------------------------------------------------------------------- */
void WriteLandmarksKML_Folder(std::ofstream & googleEarth, const _projInfo& projInfo)
{
  if (projInfo.landmarks.size() == 0)
    return;

  googleEarth
    << " <Folder>\n"
    << "  <name>Landmarks</name>\n";

  char buff[1028];
  strcpy(buff, projInfo.landmarks.c_str());
  char * p = strtok(buff, ",");

  while (p)
  {
    float lat, lon;
    char label[256];

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

/* -------------------------------------------------------------------- */
void WriteTimeStampsKML_Folder(std::ofstream & googleEarth)
{
  googleEarth
    << " <Folder>\n"
    << "  <name>Time Stamps</name>\n";

  int this_ts, curr_ts, last_ts = -1;
  for (size_t i = 0; i < _date.size(); i++)
  {
    std::string hour   = _date[i].substr(11, 2);
    std::string minute = _date[i].substr(14, 2);
    std::string second = _date[i].substr(17, 2);
    this_ts = atoi(hour.c_str())*60 + atoi(minute.c_str());
    curr_ts = this_ts / ts_Freq;
    if ( (i == 0) || (curr_ts != last_ts) || (i == _date.size()-1) ) {
      last_ts = curr_ts;
      std::string label = _date[i].substr(11, 5);
      googleEarth
        << "  <Placemark>\n"
        << "   <name>" << label << "</name>\n"
	<< "   <description>" << midBubbleCDATA(i) << "</description>\n"
        << "   <TimeStamp>\n"
        << "    <when>" << _date[i] << "Z</when>\n"
        << "   </TimeStamp>\n"
        << "   <styleUrl>#PM1</styleUrl>\n"
        << "   <Point>\n"
	<< "    <altitudeMode>absolute</altitudeMode>\n"
        << "    <coordinates>"
        << _lon[i] << "," << _lat[i] << "," << (int)_alt[i] << "</coordinates>\n"
        << "   </Point>\n"
        << "  </Placemark>\n";
    }
  }

  googleEarth << " </Folder>\n";
}

/* -------------------------------------------------------------------- */
void WriteWindBarbsKML_Folder(std::ofstream & googleEarth)
{
  googleEarth
    << " <Folder>\n"
    << "  <name>Wind Barbs</name>\n";

  int this_ts, curr_ts, last_ts = -1;
  for (size_t i = 0; i < _date.size(); i++)
  {
    std::string hour   = _date[i].substr(11, 2);
    std::string minute = _date[i].substr(14, 2);
    std::string second = _date[i].substr(17, 2);
    this_ts = atoi(hour.c_str())*60 + atoi(minute.c_str());
    curr_ts = this_ts / barb_Freq;
    if ( (i == 0) || (curr_ts != last_ts) || (i == _date.size()-1) ) {
      last_ts = curr_ts;
      int iws = barbSpeed(_ws[i] * 1.9438);	// Knots.
      int iwd = (int)_wd[i];
      char url[512];

      sprintf(url, "<href>http://%s/flight_data/display/windbarbs/%03d/wb_%03d_%03d.png</href>\n",
		webHost.c_str(), iws, iws, iwd);

      googleEarth
        << "  <Placemark>\n"
	<< "   <description><![CDATA[WD: " << _wd[i] << " deg<br>WS: " << _ws[i] << " knots]]></description>\n"
        << "   <Style>\n"
        << "    <IconStyle>\n"
        << "     <scale>3</scale>\n"
        << "     <Icon>" << url << "</Icon>\n"
        << "    </IconStyle>\n"
        << "   </Style>\n"
        << "   <Point>\n"
	<< "    <altitudeMode>absolute</altitudeMode>\n"
        << "    <coordinates>"
        << _lon[i] << "," << _lat[i] << "," << (int)_alt[i] << "</coordinates>\n"
        << "   </Point>\n"
        << "  </Placemark>\n";
    }
  }

  googleEarth << " </Folder>\n";
}

/* -------------------------------------------------------------------- */
/* Copies verbatum from a text file called "include.kml", this allows for
 * user define stuff to be added.
 */
void WriteSpecialInclude(std::ofstream & googleEarth)
{
  std::ifstream inc;

  inc.open("include.kml");
  if (inc.is_open())
  {
    char buff[1028];
    while (!inc.eof())
    {
      inc.getline(buff, 1028);
      googleEarth << buff;
    }
    inc.close();
  }
}

/* -------------------------------------------------------------------- */
void
renamefile(std::string file, std::string outFile)
{
  if (PostProcessMode)	// Don't compress files for netCDF post-processing.
    return;

  char buffer[1024];
  std::string tmptmp(googleEarthDataDir); tmptmp += "tmp.kmz";

  sprintf(buffer, "zip %s %s", tmptmp.c_str(), file.c_str());
  system(buffer);

  sprintf(buffer, "chmod g+w %s", tmptmp.c_str());
  system(buffer);

  std::string temp = googleEarthDataDir + outFile + ".kmz";
  rename(tmptmp.c_str(), temp.c_str());
}

/* -------------------------------------------------------------------- */
void WriteGoogleEarthKML(std::string & file, const _projInfo& projInfo)
{
  std::ofstream googleEarth;

  googleEarth.open(file.c_str());
  if (googleEarth.is_open() == false)
  {
    std::cerr << "Failed to open output file " << file << std::endl;
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
	<< "    <href>http://" << webHost << "/flight_data/display/red.png</href>\n"
	<< "   </Icon>\n"
	<< "  </IconStyle>\n"
	<< " </Style>\n"
	<< " <Style id=\"PM2\">\n"
	<< "  <IconStyle>\n"
	<< "   <scale>0.5</scale>\n"
	<< "   <Icon>\n"
	<< "    <href>http://" << webHost << "/flight_data/display/white.png</href>\n"
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
	<< "  <longitude>" << _lon[_lon.size()-1] << "</longitude>\n"
	<< "  <latitude>" << _lat[_lat.size()-1] << "</latitude>\n"
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

  int oneHour = 3600 / projectInfo.groundFeedDataRate;
  int i = 0, n = _date.size() - oneHour;
  int step = TimeStep / projectInfo.groundFeedDataRate;

  if (n > 0)
  {
    googleEarth
	<< "  <Placemark>\n"
	<< "   <name>Track, yellow</name>\n"
	<< "   <visibility>1</visibility>\n"
	<< "   <open>1</open>\n"
	<< "   <styleUrl>#TRACK_YELLOW</styleUrl>\n"
	<< "   <LineString>\n"
	<< "    <extrude>1</extrude>\n"
	<< "    <altitudeMode>absolute</altitudeMode>\n"
	<< "    <coordinates>\n";

    for (i = 0; i < n; i += step)
    {
      googleEarth << _lon[i] << "," << _lat[i] << "," << (int)_alt[i] << "\n";
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
	<< "    <altitudeMode>absolute</altitudeMode>\n"
	<< "    <coordinates>\n";

  // Output last hour of track, in red.
  if ((i -= step) < 0) i = 0;
  for (; i < _date.size(); i += step)
  {
    googleEarth << _lon[i] << "," << _lat[i] << "," << (int)_alt[i] << "\n";
  }
  googleEarth	<< _lon[_date.size()-1] << ","
		<< _lat[_date.size()-1] << ","
		<< (int)_alt[_date.size()-1] << "\n";

  googleEarth
	<< "    </coordinates>\n"
	<< "   </LineString>\n"
	<< "  </Placemark>\n"
	<< "  <Placemark>\n"
	<< "   <name>" << projectInfo.platform << "</name>\n"
	<< "   <description>" << endBubbleCDATA() << "</description>\n"
	<< "   <styleUrl>#PM1</styleUrl>\n"
	<< "   <Point>\n"
	<< "    <altitudeMode>absolute</altitudeMode>\n"
	<< "    <coordinates>" << _lon[_lon.size()-1] << "," << _lat[_lat.size()-1] << "," << _alt[_alt.size()-1] << "</coordinates>\n"
	<< "   </Point>\n"
	<< "  </Placemark>\n"
	<< "  <Placemark>\n"
	<< "   <name>Take off</name>\n"
	<< "   <description>" << startBubbleCDATA() << "</description>\n"
	<< "   <styleUrl>#PM2</styleUrl>\n"
	<< "   <Point>\n"
	<< "    <altitudeMode>absolute</altitudeMode>\n"
	<< "    <coordinates>" << _lon[0] << "," << _lat[0] << "," << _alt[0] << "</coordinates>\n"
	<< "   </Point>\n"
	<< "  </Placemark>\n";

  WriteLandmarksKML_Folder(googleEarth, projInfo);
  WriteTimeStampsKML_Folder(googleEarth);
  WriteWindBarbsKML_Folder(googleEarth);

  googleEarth
		<< "</Folder>\n"
		<< "</Document>\n"
		<< "</kml>\n";


  googleEarth.close();
  renamefile(file, "real-time");
}

/* -------------------------------------------------------------------- */
void WriteGoogleEarthAnimatedKML(std::string & file, const _projInfo& projInfo)
{
  std::ofstream googleEarth;

  googleEarth.open(file.c_str());
  if (googleEarth.is_open() == false)
  {
    std::cerr << "Failed to open output file " << file << std::endl;
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
	<< "    <href>http://" << webHost << "/flight_data/display/red.png</href>\n"
	<< "   </Icon>\n"
	<< "  </IconStyle>\n"
	<< " </Style>\n"
	<< " <Style id=\"PM2\">\n"
	<< "  <IconStyle>\n"
	<< "   <scale>0.5</scale>\n"
	<< "   <Icon>\n"
	<< "    <href>http://" << webHost << "/flight_data/display/white.png</href>\n"
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
	<< "       <when>" << _date[_date.size()-1] << "Z</when>\n"
	<< "     </gx:TimeStamp>\n"
	<< "     <range>1500000</range>\n"
	<< "     <longitude>" << _lon[_lon.size()-1] << "</longitude>\n"
	<< "     <latitude>" << _lat[_lat.size()-1] << "</latitude>\n"
	<< "   </LookAt>\n"

	<< " <Folder>\n"
	<< "  <name>" << projInfo.flightNumber << "</name>\n"
	<< "  <open>1</open>\n";

  for (size_t i = 0; i < _date.size(); )
  {
    googleEarth
	<< "  <Placemark>\n"
	<< "   <name>Track</name>\n"
	<< "   <styleUrl>#TRACK</styleUrl>\n"
	<< "   <visibility>1</visibility>\n"
	<< "   <open>1</open>\n"
	<< "   <LineString>\n"
	<< "    <extrude>1</extrude>\n"
	<< "    <altitudeMode>absolute</altitudeMode>\n"
	<< "    <coordinates>\n";

    std::string start = _date[i];
    for (size_t j = 0; i < _date.size() && j < 120; j += TimeStep, i += TimeStep)
      googleEarth << _lon[i] << "," << _lat[i] << "," << (int)_alt[i] << "\n";

    std::string end = _date[i >= _date.size() ? _date.size()-1 : i];
    start.replace(10, 1, "T");
    end.replace(10, 1, "T");

    googleEarth
	<< "    </coordinates>\n"
	<< "   </LineString>\n"
	<< "   <TimeSpan>\n"
	<< "     <begin>" << start << "Z</begin><end>" << end << "Z</end>\n"
	<< "   </TimeSpan>\n"
	<< "  </Placemark>\n";

     if (i < _date.size())
       i -= TimeStep;
  }

  std::string st = _date[0];
  st.replace(10, 1, "T");
  std::string end = _date[_date.size()-1];
  end.replace(10, 1, "T");

  googleEarth
	<< "  <Placemark>\n"
	<< "   <name>" << projectInfo.platform << "</name>\n"
	<< "   <description>" << endBubbleCDATA() << "</description>\n"
	<< "   <styleUrl>#PM1</styleUrl>\n"
	<< "   <Point>\n"
	<< "    <altitudeMode>absolute</altitudeMode>\n"
	<< "    <coordinates>" << _lon[_lon.size()-1] << "," << _lat[_lat.size()-1] << "," << _alt[_alt.size()-1] << "</coordinates>\n"
	<< "   </Point>\n"
	<< "  </Placemark>\n"
	<< "  <Placemark>\n"
	<< "   <name>Take off</name>\n"
	<< "   <description>" << startBubbleCDATA() << "</description>\n"
	<< "   <styleUrl>#PM2</styleUrl>\n"
	<< "   <Point>\n"
	<< "    <altitudeMode>absolute</altitudeMode>\n"
	<< "    <coordinates>" << _lon[0] << "," << _lat[0] << "," << _alt[0] << "</coordinates>\n"
	<< "   </Point>\n"
	<< "  </Placemark>\n";

  WriteLandmarksKML_Folder(googleEarth, projInfo);
  WriteTimeStampsKML_Folder(googleEarth);
  WriteWindBarbsKML_Folder(googleEarth);

  googleEarth
		<< "</Folder>\n"
		<< "</Document>\n"
		<< "</kml>\n";


  googleEarth.close();
  renamefile(file, "animated-track");
}

/* -------------------------------------------------------------------- */
void updateData(PGresult * res, int indx)
{
  if (_lat.size() == 0)
  {
    firstTAS = extractPQvalue<float>(PQgetvalue(res, indx, TAS));
    firstAT = extractPQvalue<float>(PQgetvalue(res, indx, AT));
    firstWS = extractPQvalue<float>(PQgetvalue(res, indx, WS));
    firstWD = extractPQvalue<float>(PQgetvalue(res, indx, WD));
    firstWI = extractPQvalue<float>(PQgetvalue(res, indx, WI));
  }

  /*latestTAS = extractPQvalue<float>(PQgetvalue(res, indx, TAS));
  latestAT = extractPQvalue<float>(PQgetvalue(res, indx, AT));
  latestWS = extractPQvalue<float>(PQgetvalue(res, indx, WS));
  latestWD = extractPQvalue<float>(PQgetvalue(res, indx, WD));
  latestWI = extractPQvalue<float>(PQgetvalue(res, indx, WI));*/

  float lat, lon, alt;
  lat = extractPQvalue<float>(PQgetvalue(res, indx, LAT));
  lon = extractPQvalue<float>(PQgetvalue(res, indx, LON));
  alt = extractPQvalue<float>(PQgetvalue(res, indx, ALT));

  if (lat == -32767 || lon == -32767 || alt == -32767)
    return;

  std::string tm(extractPQString(res, indx, TIME)); 
  tm.replace(10, 1, "T");
  _date.push_back(tm);
  _lon.push_back( extractPQvalue<float>(PQgetvalue(res, indx, LON)) );
  _lat.push_back( extractPQvalue<float>(PQgetvalue(res, indx, LAT)) );
  _alt.push_back( extractPQvalue<float>(PQgetvalue(res, indx, ALT)) * 3.2808);

  _at.push_back( extractPQvalue<float>(PQgetvalue(res, indx, AT)) );
  _tas.push_back( extractPQvalue<float>(PQgetvalue(res, indx, TAS)) );
  _ws.push_back( extractPQvalue<float>(PQgetvalue(res, indx, WS)) );
  _wd.push_back( extractPQvalue<float>(PQgetvalue(res, indx, WD)) );
  _wi.push_back( extractPQvalue<float>(PQgetvalue(res, indx, WI)) );
}

/*-------------------------------------------------------------------- */
/**
 * Query the database for fresh data.  If this is the first request for
 * data, then get everything in the database at the moment.
 */
void GetNewData(PGconn * conn, std::string query)
{
  PGresult * res;
  int ntuples;

// This was originally written to accrete new data, but it didn't keep
// track correctly.  So for now just refetch all data.
_lat.clear(); _lon.clear(); _alt.clear(); _date.clear();
_at.clear(); _tas.clear(); _ws.clear(); _wd.clear(); _wi.clear();

  if (_lat.size() == 0)	// First time.
  {
    res = PQexec(conn, query.c_str());

    std::cerr << PQerrorMessage(conn);

    ntuples = PQntuples(res);

    firstAT = 0.0, firstTAS = -1.0, firstWS = 0.0, firstWD = 0.0, firstWI = 0.0;

    if (ntuples > 0)
    {
      for (int i = 0; i < ntuples; ++i)
        updateData(res, i);

      updateData(res, ntuples-1);
    }
  }
  else
  {
    static std::string prevEndTime;

    std::string newEndTime = getGlobalAttribute(conn, "EndTime");

    query += " WHERE datetime = '";
    query += newEndTime.substr(0, 19);
    query += "' ORDER BY datetime";

    res = PQexec(conn, query.c_str());
    ntuples = PQntuples(res);

    if (PQresultStatus(res) == PGRES_COMMAND_OK)
    {
      if (newEndTime != prevEndTime && ntuples > 0)
        updateData(res, 0);

      prevEndTime = newEndTime;
    }
    else
      fprintf(stderr, "Command failed: %s", PQerrorMessage(conn));
  }

  PQclear(res);
}

/* -------------------------------------------------------------------- */
PGconn * openDataBase()
{
  char	conn_str[1024];

  sprintf(conn_str, "host='%s' dbname='%s' user ='ads'", 
          database_host.c_str(), dbname.c_str());
  std::cout << "Connect string : [" << conn_str << "]" << std::endl;
  PGconn *conn = PQconnectdb(conn_str);


  /* check to see that the backend connection was successfully made
   */
  if (PQstatus(conn) == CONNECTION_BAD)
  {
    PQfinish(conn);
    std::cerr << "PQconnectdb: Connection failed." << std::endl;
    return 0;
  }

  return conn;
}

/* -------------------------------------------------------------------- */
void ReadDataFromNetCDF(const std::string & fileName)
{
  std::cout << "ReadDataFromNetCDF: " << fileName << std::endl;
  NcFile file(fileName.c_str());

  if (file.is_valid() == false)
  {
    std::cerr << "Failed to open, or invalid netCDF file.\n";
    return;
  }

  NcAtt* attr;
  attr = file.get_att("FlightNumber");
  projectInfo.flightNumber = attr->as_string(0);

  attr = file.get_att("ProjectName");
  projectInfo.projectName = attr->as_string(0);

  attr = file.get_att("Platform");
  projectInfo.platform = attr->as_string(0);

  attr = file.get_att("landmarks");
  projectInfo.landmarks = attr->as_string(0);

  attr = file.get_att("longitude_coordinate");
  char *lon = attr->as_string(0);

  attr = file.get_att("latitude_coordinate");
  char *lat = attr->as_string(0);

  attr = file.get_att("zaxis_coordinate");
  char *alt = attr->as_string(0);

  attr = file.get_att("time_coordinate");
  char *tim = attr->as_string(0);

  NcVar* tim_v = file.get_var(tim);
  NcVar* tas_v = file.get_var("TASX");
  NcVar* lat_v = file.get_var(lat);
  NcVar* lon_v = file.get_var(lon);
  NcVar* alt_v = file.get_var(alt);
  NcVar* atx_v = file.get_var("ATX");
  NcVar* ws_v = file.get_var("WSC");
  NcVar* wi_v = file.get_var("WIC");
  NcVar* wd_v = file.get_var("WDC");

  NcValues *tim_vals = tim_v->values();
  NcValues *tas_vals = tas_v->values();
  NcValues *lat_vals = lat_v->values();
  NcValues *lon_vals = lon_v->values();
  NcValues *alt_vals = alt_v->values();
  NcValues *atx_vals = atx_v->values();
  NcValues *ws_vals = ws_v->values();
  NcValues *wi_vals = wi_v->values();
  NcValues *wd_vals = wd_v->values();

  attr = tim_v->get_att("units");
  struct tm tm;
  strptime(attr->as_string(0), "seconds since %F %T +0000", &tm);
  time_t t = mktime(&tm);

  size_t n = tim_vals->num();
  for (size_t i = 0; i < n; ++i)
  {
    for (; i < n && tas_vals->as_float(i) < TAS_CutOff; ++i)
      ;

    for (; i < n &&
		isMissingValue(lat_vals->as_float(i)) ||
		isMissingValue(lon_vals->as_float(i)) ||
		isMissingValue(alt_vals->as_float(i)); ++i)
      ;

    if (i >= n)
      break;

    char buffer[60];
    time_t thist = t + tim_vals->as_int(i);
    strftime(buffer, 60, "%FT%T", gmtime(&thist));
    _date.push_back( buffer );
    _lon.push_back( lon_vals->as_float(i) );
    _lat.push_back( lat_vals->as_float(i) );
    _alt.push_back( alt_vals->as_float(i) * 3.2808 );
    _at.push_back( atx_vals->as_float(i) );
    _ws.push_back( ws_vals->as_float(i) );
    _wi.push_back( wi_vals->as_float(i) );
    _wd.push_back( wd_vals->as_float(i) );
  }

  firstAT = _at[0];
  firstWS = _ws[0];
  firstWD = _wd[0];
  firstWI = _wi[0];
}

/* -------------------------------------------------------------------- */
int usage(const char* argv0)
{
  std::cerr
	<< "Usage: has two forms, one for real-time use and the other to scan\n"
	<< "	a netCDF file in post-processing mode.\n\n"
	<< "Real-time onboard form:\n"
	<< "	acTrack2kml -o [-h database_host]\n\n"
	<< "Real-time ground form:\n"
	<< "	acTrack2kml [-h database_host] -p platform\n\n"
	<< "Post-processing:\n"
	<< "	acTrack2kml infile.nc outfile.kml\n";   

  return 1;
}

/* -------------------------------------------------------------------- */
int parseRunstring(int argc, char** argv)
{
  extern char *optarg;       /* set by getopt() */
  extern int optind;       /* "  "     "     */
  int opt_char;     /* option character */

  enum {isnt, yes, no} ground_selected = isnt;

  while ((opt_char = getopt(argc, argv, "p:h:s:t:o")) != -1)
  {
    switch (opt_char)
    {
    case 'p':	// platform selection, used to select dbname on ground.
      if ( ground_selected == no )
      {
        std::cerr << "\n\tDo not select a platform when running onboard (-o).\n\n";
        return usage(argv[0]);
      }
      ground_selected = yes;
      platform = optarg;
      // TODO query the platforms DB to get a list of these...
      if ( platform.compare("GV") &&
           platform.compare("P3") &&
           platform.compare("C130") )
      {
        std::cerr << "\n\tplatform must be GV, C130, or P3\n\n";
        return usage(argv[0]);
      }
      dbname = "real-time-"+platform;
      googleEarthDataDir = grnd_googleEarthDataDir+platform+"/GE/";
      webHost            = grnd_webHost;
      break;

    case 'h':	// PGHOST over-ride.
      database_host = optarg;
      break;

    case 's':	// Time-step, default is 30 seconds.
      TimeStep = atoi(optarg);
      break;

    case 't':	// Time Stamp Frequency, default is 10 minutes.
      ts_Freq = atoi(optarg);
      break;

    case 'o':	// onboard.  Modify some defaults if this is set.
      if ( ground_selected == yes )
      {
        std::cerr << "\n\tDo not select a platform when running onboard (-o).\n\n";
        return usage(argv[0]);
      }
      ground_selected = no;
      dbname = "real-time";
      googleEarthDataDir = onboard_googleEarthDataDir + "/GE/";
      webHost            = onboard_webHost;
      break;

    case '?':
      return usage(argv[0]);
      break;
    }
  }

  if ((optind+2) == argc)	// netCDF mode.
  {
    netCDFinputFile = argv[optind];
    outputKML = argv[optind+1];
  }

  if ((optind+1) == argc)	// netCDF mode, but they left off output file.
    return usage(argv[0]);

  return 0;
}

/* -------------------------------------------------------------------- */
void getGlobalAttrData(PGconn * conn)
{
  projectInfo.flightNumber = getGlobalAttribute(conn, "FlightNumber");
  projectInfo.projectName = getGlobalAttribute(conn, "ProjectName");
  projectInfo.platform = getGlobalAttribute(conn, "Platform");
  projectInfo.landmarks = getGlobalAttribute(conn, "landmarks");
  if (projectInfo.flightNumber.size() == 0)
    projectInfo.flightNumber = "noflight";

  // Get the data rate at which data is being shipped from platform to ground.
  std::string dr = getGlobalAttribute(conn, "DataRate");
  int dataRate = atoi(dr.c_str());
  if (dataRate <= 0)
    dataRate = 5;	// 5 second data is the default rate onboard.
  projectInfo.groundFeedDataRate = dataRate;

  if (TimeStep < dataRate)
    TimeStep = dataRate;
}

/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  int rc;

  char *p = getenv("PGHOST");
  if (p) database_host = p;

  if (argc == 1 || strstr(argv[1], "usage") || strstr(argv[1], "help"))
    return usage(argv[0]);

  if (argc > 1)
  {
    if ((rc = parseRunstring(argc,argv)) != 0)
      return rc;
  }

  if (netCDFinputFile.length() > 0)
  {
    PostProcessMode = true;
    ReadDataFromNetCDF(netCDFinputFile);
    if (_lat.size() > 0)
    {
      std::cout << "WriteGoogleEarthKML(" << outputKML << ", projectInfo);\n";
      WriteGoogleEarthKML(outputKML, projectInfo);
    }
    return 0;
  }


  std::cout << "\n  Using database host : " << database_host << std::endl;
  std::cout << "\n  Output directory : " << googleEarthDataDir << std::endl;
  std::cout << std::endl;


  // Real-time mode.

  PGconn * conn;

  while (1)
  {
    while ((conn = openDataBase()) == 0)
      sleep(3);

    getGlobalAttrData(conn);

    std::string dataQuery = buildDataQueryString(conn);

    GetNewData(conn, dataQuery);
    PQfinish(conn);


    if (_lat.size() > 0)
    {
      std::string outFile;

      outFile = googleEarthDataDir + "latest" + ".kml";

      WriteGoogleEarthKML(outFile, projectInfo);
      WriteGoogleEarthAnimatedKML(outFile, projectInfo);
      WriteCurrentPositionKML(projectInfo);
    }
    sleep(30);
  }

  return 0;
}
