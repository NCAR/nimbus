/*
 * Program to read real-time flight database periodically (say 60 seconds)
 * and create a KML for google earth.
 *
 * COPYRIGHT: University Corporation for Atmospheric Research, 2005-2012
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

using namespace std;

// Output directories for .xml & .kml files.  Ground.
static const string grnd_googleEarthDataDir = "/net/www/docs/flight_data/";
static const string grnd_webHost = "www.eol.ucar.edu";

// Output directories for .xml & .kml files.  Onboard.
static const string onboard_googleEarthDataDir = "/var/www/html/flight_data/";
static const string onboard_webHost = "acserver.raf.ucar.edu";

static const float missing_value = -32767.0;

class Config
{
public:
  Config() : TimeStep(15), TAS_CutOff(20.0), ts_Freq(2000), barb_Freq(5), altMode("absolute"), postProcessMode(false), convertToFeet(1.0), showit(true)
  { }


  string	googleEarthDataDir, webHost;

  // All datapoints are read from file, but only use every 'TimeStep' points.
  // e.g. 15 would mean use 1 data point for every 15 seconds of data.
  int TimeStep;

  // True Airspeed cut-off (take-off and landing speed).
  float TAS_CutOff;

  // Frequency of Time Stamps (in minutes).
  int ts_Freq;

  // Frequency of Wind Barbs (in minutes).
  int barb_Freq;

  // Should we use KML clampToGround or ABSOLUTE.
  string altMode;

  string netCDFinputFile, outputKML, database_host, platform, dbname;


  bool postProcessMode;

  // If alt is in meters this gets changed to 3.28.
  float convertToFeet;

  bool showit;
};

static Config cfg;


// Our raw data is coming from a PostGreSQL database.....
// The order of this enum should match the order of the variables
// being requested in the dataQuery string.
enum VariablePos { TIME=0, LON, LAT, ALT, AT, DP, TAS, WS, WD, WI };
static const string _dataQuerySuffix = ",atx,dpxc,tasx,wsc,wdc,wic FROM raf_lrt WHERE TASX > ";



class _projInfo
{
public:
  _projInfo() : groundFeedDataRate(1) { };

  string platform;
  string projectName;
  string flightNumber;
  string landmarks;
  int groundFeedDataRate;
} projectInfo;

const char *status[] = { "Pre-flight", "In-flight", "Landed" };

vector<string> _date;
vector<float> _lat, _lon, _alt, _at, _dp, _tas, _ws, _wd, _wi;
float firstAT = 0.0, firstDP = 0.0, firstTAS = -1.0, firstWS = 0.0, firstWD = 0.0, firstWI = 0.0;
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
extractPQvalue(const char *cpqstring)
{
  if (! cpqstring)
  {
    cerr << "extractPQvalue: string is null!\n";
    return T();
  }
  try
  {
    return boost::lexical_cast<T>(cpqstring);
  }
  catch (const boost::bad_lexical_cast& badcast)
  {
    cerr << "could not parse '" << cpqstring << "': "
                << badcast.what() << endl;
  }
  return T();
}

/* -------------------------------------------------------------------- */
string
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

  return string(pqstring, len);
}

/* -------------------------------------------------------------------- */
string getGlobalAttribute(PGconn *conn, string attr)
{
  string query = "SELECT value FROM global_attributes WHERE key='";
  query += attr + "'";
  PGresult * res = PQexec(conn, query.c_str());

  int ntuples = PQntuples(res);

  if (ntuples == 0)
  {
    cerr << "No global attribute " << attr << "!\n";
    PQclear(res);
    return "";
  }
  string s = extractPQString(res, 0, 0);
  PQclear(res);
  return s;
}

/* -------------------------------------------------------------------- */
string getVariableUnits(PGconn *conn, string name)
{
  string query = "SELECT units FROM variable_list WHERE name='";
  query += name + "'";
  PGresult * res = PQexec(conn, query.c_str());

  int ntuples = PQntuples(res);

  if (ntuples == 0)
  {
    cerr << "No variable named: " << name << "!\n";
    PQclear(res);
    return "";
  }
  string s = extractPQString(res, 0, 0);
  PQclear(res);
  return s;
}

/* -------------------------------------------------------------------- */
/**
 * Read the 'coordinates' attribute form the database to get the names of
 * the lat, lon and alt variables.
 */
string buildDataQueryString(PGconn *conn)
{
  string dataQuery = "SELECT datetime,";

  string lat = getGlobalAttribute(conn, "latitude_coordinate");
  string lon = getGlobalAttribute(conn, "longitude_coordinate");
  string alt;

  // Search for pressure altitude first, zaxis_coord is typically GPS alt.
  if (getVariableUnits(conn, "PALTF") != "")
    alt = "PALTF";
  else
  if (getVariableUnits(conn, "PALT") != "")
    alt = "PALT";
  else
    alt = getGlobalAttribute(conn, "zaxis_coordinate");
  
  if (getVariableUnits(conn, alt) == "m")
    cfg.convertToFeet = 3.2808;	// feet per meter
  else
    cfg.convertToFeet = 1.0;	// no-op (feet to feet)


  dataQuery += lon + "," + lat + "," + alt + _dataQuerySuffix;

  char tmp[100];
  sprintf(tmp, "%f ORDER BY datetime", cfg.TAS_CutOff);
  dataQuery += tmp;

  if (cfg.showit) {
    cfg.showit = 0;
    cout << "dataQuery: " << dataQuery << endl;
    cout << "alt: " << alt << " units: " << getVariableUnits(conn, alt) << endl;
    cout << "cfg.convertToFeet: " << cfg.convertToFeet << endl;
  }
  return dataQuery;
}

/* -------------------------------------------------------------------- */
string
startBubbleCDATA()
{
  stringstream s;
  string startTime = _date[0].substr(_date[0].find('T')+1);

  s << "<![CDATA[" << startTime << fixed
	<< " Lat : " << _lat[0]
	<< " deg_N<br>Lon : " << _lon[0] << " deg_E<br>";
  s.precision(0);
  if (_alt[0] != missing_value) s << "Alt : " << _alt[0] << " feet<br>";
  s.precision(2);
  if (firstAT != missing_value) s << "Temp : " << firstAT << " C<br>";
  if (firstDP != missing_value) s << "DP : " << firstDP << " C<br>";
  if (firstWS != missing_value) s << "WS : " << firstWS << " knots<br>";
  if (firstWD != missing_value) s << "WD : " << firstWD << " degree_T<br>";
  if (firstWI != missing_value) s << "WI : " << firstWI << " m/s";
  s << "]]>";

  return s.str();
}

/* -------------------------------------------------------------------- */
string
endBubbleCDATA()
{
  stringstream e;
  string s = _date[_date.size()-1];
  string endTime = s.substr(s.find('T')+1);

  e	<< "<![CDATA[" << endTime << "<br>" << fixed
	<< " Lat : " << _lat[_lat.size()-1]
	<< " deg_N<br>Lon : " << _lon[_lon.size()-1] << " deg_E<br>";
  e.precision(0);
  if (_alt[_alt.size()-1] != missing_value) e << "Alt : " << _alt[_alt.size()-1] << " feet<br>";
  e.precision(2);
  if (_at[_at.size()-1] != missing_value) e << "Temp : " << _at[_at.size()-1] << " C<br>";
  if (_dp[_dp.size()-1] != missing_value) e << "DP : " << _dp[_dp.size()-1] << " C<br>";
  if (_ws[_ws.size()-1] != missing_value) e << "WS : " << _ws[_ws.size()-1] << " knots<br>";
  if (_wd[_wd.size()-1] != missing_value) e << "WD : " << _wd[_wd.size()-1] << " degree_T<br>";
  if (_wi[_wi.size()-1] != missing_value) e << "WI : " << _wi[_wi.size()-1] << " m/s";
  e << "]]>";

  return e.str();
}

/* -------------------------------------------------------------------- */
string
midBubbleCDATA(int i)
{
  stringstream e;

  e	<< fixed << "<![CDATA[" << " Lat : " << _lat[i]
	<< " deg_N<br>Lon : " << _lon[i] << " deg_E<br>";
  e.precision(0);
  if (_alt[i] != missing_value) e << "Alt : " << _alt[i] << " feet<br>";
  e.precision(2);
  if (_at[i] != missing_value) e << "Temp : " << _at[i] << " C<br>";
  if (_dp[i] != missing_value) e << "DP : " << _dp[i] << " C<br>";
  if (_ws[i] != missing_value) e << "WS : " << _ws[i] << " knots<br>";
  if (_wd[i] != missing_value) e << "WD : " << _wd[i] << " degree_T<br>";
  if (_wi[i] != missing_value) e << "WI : " << _wi[i] << " m/s";
  e << "]]>";


  return e.str();
}

/* -------------------------------------------------------------------- */
int barbSpeed(float ws)
{
  return (int)((ws + 2.5) / 5) * 5;
}

/* -------------------------------------------------------------------- */
void WriteCurrentPositionKML(const _projInfo& projInfo)
{
  ofstream googleEarth;
  string file;

  file = cfg.googleEarthDataDir + "current_pos.kml";

  string temp = cfg.googleEarthDataDir + "latest" + ".kml";

  googleEarth.open(temp.c_str());
  if (googleEarth.is_open() == false)
  {
    cerr << "Failed to open output file " << temp << endl;
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
	<< "   <name>GV</name>\n"
	<< "   <styleUrl>#PM1</styleUrl>\n"
	<< "   <Point>\n"
	<< "    <altitudeMode>" << cfg.altMode << "</altitudeMode>\n"
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
void WriteLandmarksKML_Folder(ofstream& googleEarth, const _projInfo& projInfo)
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
void WriteTimeStampsKML_Folder(ofstream& googleEarth)
{
  googleEarth
    << " <Folder>\n"
    << "  <name>Time Stamps</name>\n";

  int this_ts, curr_ts, last_ts = -1;
  for (size_t i = 0; i < _date.size(); i++)
  {
    string hour   = _date[i].substr(11, 2);
    string minute = _date[i].substr(14, 2);
    string second = _date[i].substr(17, 2);
    this_ts = atoi(hour.c_str())*60 + atoi(minute.c_str());
    curr_ts = this_ts / cfg.ts_Freq;
    if ( (i == 0) || (curr_ts != last_ts) || (i == _date.size()-1) ) {
      last_ts = curr_ts;
      string label = _date[i].substr(11, 5);
      googleEarth
        << "  <Placemark>\n"
        << "   <name>" << label << "</name>\n"
	<< "   <description>" << midBubbleCDATA(i) << "</description>\n"
        << "   <TimeStamp>\n"
        << "    <when>" << _date[i] << "Z</when>\n"
        << "   </TimeStamp>\n"
        << "   <styleUrl>#PM1</styleUrl>\n"
        << "   <Point>\n"
	<< "    <altitudeMode>" << cfg.altMode << "</altitudeMode>\n"
        << "    <coordinates>"
        << _lon[i] << "," << _lat[i] << "," << (int)_alt[i] << "</coordinates>\n"
        << "   </Point>\n"
        << "  </Placemark>\n";
    }
  }

  googleEarth << " </Folder>\n";
}

/* -------------------------------------------------------------------- */
void WriteWindBarbsKML_Folder(ofstream& googleEarth)
{
  googleEarth
    << " <Folder>\n"
    << "  <name>Wind Barbs</name>\n";

  int this_ts, curr_ts, last_ts = -1;
  for (size_t i = 0; i < _date.size(); i++)
  {
    string hour   = _date[i].substr(11, 2);
    string minute = _date[i].substr(14, 2);
    string second = _date[i].substr(17, 2);
    this_ts = atoi(hour.c_str())*60 + atoi(minute.c_str());
    curr_ts = this_ts / cfg.barb_Freq;
    if ( (i == 0) || (curr_ts != last_ts) || (i == _date.size()-1) ) {
      last_ts = curr_ts;
      int iws = barbSpeed(_ws[i]);	// make sure to pass in knots.
      int iwd = (int)_wd[i];
      char url[512];

      if (iws < 0) iws = 0;
      if (iwd < 0 || iwd > 360) iwd = 0;

      sprintf(url, "<href>http://%s/flight_data/display/windbarbs/%03d/wb_%03d_%03d.png</href>\n",
		cfg.webHost.c_str(), iws, iws, iwd);
      string label = _date[i].substr(11, 5);

      googleEarth
        << "  <Placemark>\n"
        << "   <name>" << label << "</name>\n"
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
void WriteSpecialInclude(ofstream& googleEarth)
{
  ifstream inc;

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
renamefile(string file, string outFile)
{
  if (cfg.postProcessMode)	// Don't compress files for netCDF post-processing.
    return;

  // Rename kml first.
  string temp = cfg.googleEarthDataDir + outFile + ".kml";
  rename(file.c_str(), temp.c_str());

  /**
   * Producing kmz files may not by useful for mission coordinator.  kmz is uncompressed
   * server side and sent to client, so there is no bandwidth savings to using for web
   * applications.  GoogleEarth can take advantage of it, but we are mostly web based
   * at this time.  cjw 5/31/2013.
   */
  if (1)	// Compress to kmz.
  {
    char buffer[1024];
    string tmptmp(cfg.googleEarthDataDir); tmptmp += "tmp.kmz";

    sprintf(buffer, "zip %s %s", tmptmp.c_str(), file.c_str());
    system(buffer);

    sprintf(buffer, "chmod g+w %s", tmptmp.c_str());
    system(buffer);

    temp = cfg.googleEarthDataDir + outFile + ".kmz";
    rename(tmptmp.c_str(), temp.c_str());
  }
}

/* -------------------------------------------------------------------- */
void WriteGoogleEarthKML(string& file, const _projInfo& projInfo)
{
  ofstream googleEarth;

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
  int step = cfg.TimeStep / projectInfo.groundFeedDataRate;

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
	<< "    <altitudeMode>" << cfg.altMode << "</altitudeMode>\n"
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
	<< "    <altitudeMode>" << cfg.altMode << "</altitudeMode>\n"
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
	<< "    <altitudeMode>" << cfg.altMode << "</altitudeMode>\n"
	<< "    <coordinates>" << _lon[_lon.size()-1] << "," << _lat[_lat.size()-1] << "," << _alt[_alt.size()-1] << "</coordinates>\n"
	<< "   </Point>\n"
	<< "  </Placemark>\n"
	<< "  <Placemark>\n"
	<< "   <name>Take off</name>\n"
	<< "   <description>" << startBubbleCDATA() << "</description>\n"
	<< "   <styleUrl>#PM2</styleUrl>\n"
	<< "   <Point>\n"
	<< "    <altitudeMode>" << cfg.altMode << "</altitudeMode>\n"
	<< "    <coordinates>" << _lon[0] << "," << _lat[0] << "," << _alt[0] << "</coordinates>\n"
	<< "   </Point>\n"
	<< "  </Placemark>\n";

  WriteLandmarksKML_Folder(googleEarth, projInfo);
  WriteWindBarbsKML_Folder(googleEarth);
  WriteTimeStampsKML_Folder(googleEarth);

  googleEarth
		<< "</Folder>\n"
		<< "</Document>\n"
		<< "</kml>\n";


  googleEarth.close();
  renamefile(file, "real-time");
}

/* -------------------------------------------------------------------- */
void WriteGoogleEarthAnimatedKML(string& file, const _projInfo& projInfo)
{
  ofstream googleEarth;

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
	<< "    <altitudeMode>" << cfg.altMode << "</altitudeMode>\n"
	<< "    <coordinates>\n";

    string start = _date[i];
    for (size_t j = 0; i < _date.size() && j < 120; j += cfg.TimeStep, i += cfg.TimeStep)
      googleEarth << _lon[i] << "," << _lat[i] << "," << (int)_alt[i] << "\n";

    string end = _date[i >= _date.size() ? _date.size()-1 : i];
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
       i -= cfg.TimeStep;
  }

  string st = _date[0];
  st.replace(10, 1, "T");
  string end = _date[_date.size()-1];
  end.replace(10, 1, "T");

  googleEarth
	<< "  <Placemark>\n"
	<< "   <name>" << projectInfo.platform << "</name>\n"
	<< "   <description>" << endBubbleCDATA() << "</description>\n"
	<< "   <styleUrl>#PM1</styleUrl>\n"
	<< "   <Point>\n"
	<< "    <altitudeMode>" << cfg.altMode << "</altitudeMode>\n"
	<< "    <coordinates>" << _lon[_lon.size()-1] << "," << _lat[_lat.size()-1] << "," << _alt[_alt.size()-1] << "</coordinates>\n"
	<< "   </Point>\n"
	<< "  </Placemark>\n"
	<< "  <Placemark>\n"
	<< "   <name>Take off</name>\n"
	<< "   <description>" << startBubbleCDATA() << "</description>\n"
	<< "   <styleUrl>#PM2</styleUrl>\n"
	<< "   <Point>\n"
	<< "    <altitudeMode>" << cfg.altMode << "</altitudeMode>\n"
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
void updateData(PGresult *res, int indx)
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

  if (lat == missing_value || lon == missing_value || alt == missing_value)
    return;

  float value;
  string tm(extractPQString(res, indx, TIME)); 
  tm.replace(10, 1, "T");
  _date.push_back(tm);
  _lon.push_back( extractPQvalue<float>(PQgetvalue(res, indx, LON)) );
  _lat.push_back( extractPQvalue<float>(PQgetvalue(res, indx, LAT)) );
  value = extractPQvalue<float>(PQgetvalue(res, indx, ALT));
  if (value != missing_value)
    value *= cfg.convertToFeet;
  _alt.push_back( value );

  _at.push_back( extractPQvalue<float>(PQgetvalue(res, indx, AT)) );
  _dp.push_back( extractPQvalue<float>(PQgetvalue(res, indx, DP)) );
  _tas.push_back( extractPQvalue<float>(PQgetvalue(res, indx, TAS)) );
  value = extractPQvalue<float>(PQgetvalue(res, indx, WS));
  if (value != missing_value)
    value *= 1.9438;	// convert to knots.
  _ws.push_back(value);
  
  _wd.push_back( extractPQvalue<float>(PQgetvalue(res, indx, WD)) );
  _wi.push_back( extractPQvalue<float>(PQgetvalue(res, indx, WI)) );
}

/*-------------------------------------------------------------------- */
/**
 * Query the database for fresh data.  If this is the first request for
 * data, then get everything in the database at the moment.
 */
void GetNewData(PGconn *conn, string query)
{
  PGresult *res;
  int ntuples;

// This was originally written to accrete new data, but it didn't keep
// track correctly.  So for now just refetch all data.
_lat.clear(); _lon.clear(); _alt.clear(); _date.clear();
_at.clear(); _dp.clear(); _tas.clear(); _ws.clear(); _wd.clear(); _wi.clear();

  if (_lat.size() == 0)	// First time.
  {
    res = PQexec(conn, query.c_str());

    cerr << PQerrorMessage(conn);

    ntuples = PQntuples(res);

    firstAT = 0.0, firstDP = 0.0, firstTAS = -1.0, firstWS = 0.0, firstWD = 0.0, firstWI = 0.0;

    if (ntuples > 0)
    {
      for (int i = 0; i < ntuples; ++i)
        updateData(res, i);

      updateData(res, ntuples-1);
    }
  }
  else
  {
    static string prevEndTime;

    string newEndTime = getGlobalAttribute(conn, "EndTime");

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
PGconn *openDataBase()
{
  char	conn_str[1024];

  sprintf(conn_str, "host='%s' dbname='%s' user ='ads'", 
          cfg.database_host.c_str(), cfg.dbname.c_str());
  cout << "Connect string : [" << conn_str << "]" << endl;
  PGconn *conn = PQconnectdb(conn_str);


  /* check to see that the backend connection was successfully made
   */
  if (PQstatus(conn) == CONNECTION_BAD)
  {
    PQfinish(conn);
    cerr << "PQconnectdb: Connection failed." << endl;
    return 0;
  }

  return conn;
}

/* -------------------------------------------------------------------- */
NcVar *getNetcdfVariable(NcFile& file, const char var_name[])
{
  NcVar *var;

  if ((var = file.get_var(var_name)) == 0)
  {
    cerr << "acTrack2kml: unable to find variable " << var << " in netCDF file, fatal.\n";
    exit(1);
  }

  return var;
}

/* -------------------------------------------------------------------- */
void ReadDataFromNetCDF(const string & fileName)
{
  cout << "ReadDataFromNetCDF: " << fileName << endl;
  NcFile file(fileName.c_str());

  if (file.is_valid() == false)
  {
    cerr << "Failed to open, or invalid netCDF file.\n";
    return;
  }

  NcError err(NcError::silent_nonfatal);

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

  attr = file.get_att("time_coordinate");
  char *tim = attr->as_string(0);

  NcVar *tim_v = getNetcdfVariable(file, tim);
  NcVar *tas_v = getNetcdfVariable(file, "TASX");
  NcVar *lat_v = getNetcdfVariable(file, lat);
  NcVar *lon_v = getNetcdfVariable(file, lon);
  NcVar *atx_v = getNetcdfVariable(file, "ATX");
  NcVar *dp_v = getNetcdfVariable(file, "DPXC");
  NcVar *ws_v = getNetcdfVariable(file, "WSC");
  NcVar *wi_v = getNetcdfVariable(file, "WIC");
  NcVar *wd_v = getNetcdfVariable(file, "WDC");

  // Pressure altitutide is preferred.
  NcVar *alt_v;
  const char *alt = "PALTF";
  if ((alt_v = file.get_var(alt)) == 0)
  {
    cfg.convertToFeet = 3.2808;
    alt = "PALT";
    if ((alt_v = file.get_var(alt)) == 0)
    {
      attr = file.get_att("zaxis_coordinate");
      alt = attr->as_string(0);

      alt_v = getNetcdfVariable(file, alt);
    }
  }

  NcValues *tim_vals = tim_v->values();
  NcValues *tas_vals = tas_v->values();
  NcValues *lat_vals = lat_v->values();
  NcValues *lon_vals = lon_v->values();
  NcValues *alt_vals = alt_v->values();
  NcValues *atx_vals = atx_v->values();
  NcValues *dp_vals = dp_v->values();
  NcValues *ws_vals = ws_v->values();
  NcValues *wi_vals = wi_v->values();
  NcValues *wd_vals = wd_v->values();

  attr = alt_v->get_att("units");
  if (strcmp(attr->as_string(0), "m") == 0)
    cfg.convertToFeet = 3.2808; // feet per meter

  cout << "alt: " << alt << " units: " << attr->as_string(0) << endl;
  cout << "cfg.convertToFeet: " << cfg.convertToFeet << endl;

  attr = tim_v->get_att("units");
  struct tm tm;
  strptime(attr->as_string(0), "seconds since %F %T +0000", &tm);
  time_t t = mktime(&tm);

  size_t n = tim_vals->num();
  for (size_t i = 0; i < n; ++i)
  {
    for (; i < n && tas_vals->as_float(i) < cfg.TAS_CutOff; ++i)
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
    float value;
    strftime(buffer, 60, "%FT%T", gmtime(&thist));
    _date.push_back( buffer );
    _lon.push_back( lon_vals->as_float(i) );
    _lat.push_back( lat_vals->as_float(i) );
    value = alt_vals->as_float(i);
    if (value != missing_value)
      value *= cfg.convertToFeet;
    _alt.push_back( value );
    _at.push_back( atx_vals->as_float(i) );
    _dp.push_back( dp_vals->as_float(i) );
    value = ws_vals->as_float(i);
    if (value != missing_value)
      value *= 1.9438;		// conver to knots.
    _ws.push_back( value );
    _wi.push_back( wi_vals->as_float(i) );
    _wd.push_back( wd_vals->as_float(i) );
  }

  firstAT = _at[0];
  firstAT = _dp[0];
  firstWS = _ws[0];
  firstWD = _wd[0];
  firstWI = _wi[0];
}

/* -------------------------------------------------------------------- */
int usage(const char *argv0)
{
  cerr
	<< "Usage: has two forms, one for real-time use and the other to scan\n"
	<< "	a netCDF file in post-processing mode.\n\n"
	<< "Real-time onboard form:\n"
	<< "	acTrack2kml -o [-h database_host] [-p platform]\n\n"
	<< "Real-time ground form:\n"
	<< "	acTrack2kml [-h database_host] -p platform\n\n"
	<< "Post-processing:\n"
	<< "	acTrack2kml infile.nc outfile.kml\n\n"
	<< "Options:\n"
	<< "  -p platform		Platform name, valid values (C130, DC8, GV, P3).\n"
	<< "  -h database_host	Database server host with data.\n"
	<< "  -o			Run onboard, changes webhost to onboard server.\n"
	<< "  -b barb_freq		Frequency of wind barbs in minutes, default is 5.\n"
	<< "  -c			Clamp track to ground, ignore altitude.\n"
	<< "  -t ts_freq		Frequency of time stamps in minutes, default is 2000 (off).\n"
	<< "  -s time_step		Time interval of data points for track in seconds, default is 30.\n";

  return 1;
}

/* -------------------------------------------------------------------- */
int parseRunstring(int argc, char** argv)
{
  extern char *optarg;	/* set by getopt() */
  extern int optind;	/* "  "     "     */
  int opt_char;		/* option character */

  // Default to ground, -p and netCDF mode.
  cfg.webHost = grnd_webHost;

  while ((opt_char = getopt(argc, argv, "p:h:b:s:t:o:c")) != -1)
  {
    switch (opt_char)
    {
    case 'p':	// platform selection, used to select dbname
      cfg.platform = optarg;
      // TODO query the platforms DB to get a list of these...
      if ( cfg.platform.compare("GV") &&
           cfg.platform.compare("P3") &&
           cfg.platform.compare("G4") &&
           cfg.platform.compare("DC8") &&
           cfg.platform.compare("A10") &&
           cfg.platform.compare("C130") )
      {
        cerr << "\n\tplatform must be GV, C130, DC8, or P3\n\n";
        return usage(argv[0]);
      }
      cfg.dbname = "real-time-"+cfg.platform;
      cfg.googleEarthDataDir = grnd_googleEarthDataDir+cfg.platform+"/GE/";
      break;

    case 'h':	// PGHOST over-ride.
      cfg.database_host = optarg;
      break;

    case 's':	// Time-step, default is 30 seconds.
      cfg.TimeStep = atoi(optarg);
      if (cfg.TimeStep < 5) cfg.TimeStep = 5;
      break;

    case 'b':	// Windbarb Frequency, default is 5 minutes.
      cfg.barb_Freq = atoi(optarg);
      if (cfg.barb_Freq < 1) cfg.barb_Freq = 2000; // 24 hours, basically turn off.
      break;

    case 't':	// Time Stamp Frequency, default is 20 minutes.
      cfg.ts_Freq = atoi(optarg);
      if (cfg.ts_Freq < 1) cfg.ts_Freq = 2000; // 24 hours, basically turn off.
      break;

    case 'o':	// onboard.  Modify some defaults if this is set.
      if ( strlen( cfg.platform.c_str() ) ) {
        cfg.dbname = "real-time-" + cfg.platform;
        cfg.googleEarthDataDir = onboard_googleEarthDataDir + cfg.platform + "/GE/";
      } else {
        cfg.dbname = "real-time";
        cfg.googleEarthDataDir = onboard_googleEarthDataDir + "/GE/";
      }
      cfg.webHost            = onboard_webHost;
      break;

    case 'c':	// Clamp to ground
      cfg.altMode = "clampToGround";
      break;

    case '?':
      return usage(argv[0]);
      break;
    }
  }

  if ((optind+2) == argc)	// netCDF mode.
  {
    cfg.netCDFinputFile = argv[optind];
    cfg.outputKML = argv[optind+1];
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
  string dr = getGlobalAttribute(conn, "DataRate");
  int dataRate = atoi(dr.c_str());
  if (dataRate <= 0)
    dataRate = 5;	// 5 second data is the default rate onboard.
  projectInfo.groundFeedDataRate = dataRate;

  if (cfg.TimeStep < dataRate)
    cfg.TimeStep = dataRate;
}

/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  int rc;

  char *p = getenv("PGHOST");
  if (p) cfg.database_host = p;

  if (argc == 1 || strstr(argv[1], "usage") || strstr(argv[1], "help"))
    return usage(argv[0]);

  if (argc > 1)
  {
    if ((rc = parseRunstring(argc,argv)) != 0)
      return rc;
  }

  if (cfg.netCDFinputFile.length() > 0)
  {
    cfg.postProcessMode = true;
    ReadDataFromNetCDF(cfg.netCDFinputFile);
    if (_lat.size() > 0)
    {
      cout << "WriteGoogleEarthKML(" << cfg.outputKML << ", projectInfo);\n";
      WriteGoogleEarthKML(cfg.outputKML, projectInfo);
    }
    return 0;
  }


  cout << "\n  Using database host : " << cfg.database_host << endl;
  cout << "\n  Output directory : " << cfg.googleEarthDataDir << endl;
  cout << endl;


  // Real-time mode.

  PGconn *conn;

  while (1)
  {
    while ((conn = openDataBase()) == 0)
      sleep(3);

    getGlobalAttrData(conn);

    string dataQuery = buildDataQueryString(conn);

    GetNewData(conn, dataQuery);
    PQfinish(conn);


    if (_lat.size() > 0)
    {
      string outFile;

      outFile = cfg.googleEarthDataDir + "latest" + ".kml";

      WriteGoogleEarthKML(outFile, projectInfo);
      WriteGoogleEarthAnimatedKML(outFile, projectInfo);
      WriteCurrentPositionKML(projectInfo);
    }
    sleep(30);
  }

  return 0;
}
