/*
 * Program to read real-time flight database periodically (say 60 seconds)
 * and create a KML for google earth, an XML for google maps.
 */
#include <libpq-fe.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "netcdf.hh"

#include "boost/lexical_cast.hpp"

// Output directories for .xml & .kml files.  Ground
const std::string googleMapDataDir = "/net/www/docs/flight_data/pacdex/flights/";
const std::string googleEarthDataDir = "/net/www/docs/flight_data/pacdex/flights/";

// Onboard location
//const std::string googleMapDataDir = "/var/www/html/";
//const std::string googleEarthDataDir = "/var/www/html/";

// All datapoints are read from file, but only use every 'TimeStep' points.
// e.g. 15 would mean use 1 data point for every 15 seconds of data.
const int TimeStep = 10;

// True Airspeed cut-off.
const float TAS_CutOff = 20.0;

// Max out at this many points, don't want to swamp GoogleMap, though
// Vincent said he broke them up into segments, so we shouldn't have the
// old 600 point max.
const int maxGoogleMapPoints = 3000;

// Our raw data is coming from a PostGreSQL database.....
// The order of this enum should match the order of the variables
// being requested in the dataQuery string.
enum VariablePos { TIME=0, LON, LAT, ALT, AT, TAS, WS, WD, WI };
const std::string _dataQuerySuffix = ",at_a,tasx,wsc,wdc,wic FROM raf_lrt WHERE TASX > 20.0 ORDER BY datetime";


std::string projectName, flightNumber;
const char *status[] = { "Pre-flight", "In-flight", "Landed" };

std::vector<std::string> _date;
std::vector<float> _lat, _lon, _alt;
float firstAT = 0.0, firstTAS = -1.0, firstWS = 0.0, firstWD = 0.0, firstWI = 0.0;
float latestAT, latestTAS, latestWS, latestWD, latestWI;


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
  return extractPQString(res, 0, 0);
}

/* -------------------------------------------------------------------- */
/**
 * Read the 'coordinates' attribute form the database to get the names of
 * the lat, lon and alt variables.
 */
std::string buildDataQueryString(PGconn * conn)
{
  std::string dataQuery = "SELECT datetime,";

  std::string coords = getGlobalAttribute(conn, "coordinates");

  int first_space = coords.find_first_of(' ');
  int second_space = coords.find_first_of(' ', first_space+1);
  int third_space = coords.find_first_of(' ', second_space+1);

  std::string lon = coords.substr(0, first_space);
  std::string lat = coords.substr(first_space+1, second_space - first_space - 1);
  std::string alt = coords.substr(second_space+1, third_space - second_space - 1);

  dataQuery += lon + "," + lat + "," + alt + _dataQuerySuffix;

  return dataQuery;
}

/* -------------------------------------------------------------------- */
std::string
startBubbleCDATA()
{
  std::stringstream s;
  std::string startTime = _date[0].substr(_date[0].find(' '));

  s	<< "<![CDATA[Take off :" << startTime << "<br>Alt : " << _alt[0]
	<< "<br>Temp : " << firstAT << "<br>WS : " << firstWS << "<br>WD : "
	<< firstWD << "<br>WI : " << firstWI << "]]>";

  return s.str();
}

/* -------------------------------------------------------------------- */
std::string
endBubbleCDATA(int status_id)
{
  std::stringstream e;
  std::string s = _date[_date.size()-1];
  std::string endTime = s.substr(s.find(' '));

  e << "<![CDATA[";

  if (status_id == 3)	// Landed.
    e << "Landed :";
  else
    e << "Current :";

  e	<< endTime << "<br>Alt : " << _alt[_alt.size()-1] << "<br>Temp : "
	<< latestAT << "<br>WS : " << latestWS << "<br>WD : "
	<< latestWD << "<br>WI : " << latestWI << "]]>";

  return e.str();
}

/* -------------------------------------------------------------------- */
void
WriteGoogleMapXML(std::string& projName, std::string& flightNum, int status_id)
{
  // Open a temperary file, write to that, rename to what to want.

  std::ofstream googleMap;
  int flightNumInt = atoi(&flightNum.c_str()[2]);

  std::string file = googleMapDataDir + "flight_" + flightNum + ".xml";
  std::string temp = googleMapDataDir + "tmp_latest" + ".xml";
  googleMap.open(temp.c_str());

  if (googleMap.is_open() == false)
  {
    std::cerr << "Failed to open output file " << temp << std::endl;
    return;
  }

  googleMap << "<?xml version='1.0' standalone='yes'?>\n"
	<< "<flight>\n  <id>" << flightNumInt << "</id>\n"
	<< "  <status>" << status[status_id-1] << "</status>\n"
	<< "  <status_id>" << status_id << "</status_id>\n"
	<< "  <desc>Flight " << flightNum << "</desc>\n"
	<< "  <start lat=\"" << _lat[0] << "\" lng=\"" << _lon[0] << "\">"
	<< startBubbleCDATA() << "</start>\n"
	<< "  <end lat=\"" << _lat[_lat.size()-1] << "\" lng=\""
	<< _lon[_lon.size()-1] << "\">";

  googleMap << endBubbleCDATA(status_id) << "</end>\n  <points>\n";

  size_t step = (_lat.size() / maxGoogleMapPoints) + 1;

  for (size_t i = 0; i < _lat.size(); i += step)
  {
    googleMap	<< "    <point lat=\"" << _lat[i] << "\" lng=\"" << _lon[i]
		<< "\" alt=\"" << (int)_alt[i] << "\"/>\n";
//		<< "\" alt=\"" << (int)_alt[i] << "\" dt=\"" << _date[i] << "\"/>\n";
  }

  googleMap << "  </points>\n</flight>\n";
  googleMap.close();
  rename(temp.c_str(), file.c_str());
}

/* -------------------------------------------------------------------- */
void WriteCurrentPositionKML(std::string& projName, std::string& flightNum)
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
	<< "<kml xmlns=\"http://earth.google.com/kml/2.1\">\n"
	<< "<Document>\n"
	<< " <name>" << projName << " " << flightNum << "</name>\n"
	<< " <Style id=\"PM1\">\n"
	<< "  <IconStyle>\n"
	<< "   <scale>0.5</scale>\n"
	<< "   <Icon>\n"
	<< "    <href>http://www.eol.ucar.edu/flight_data/images/red.png</href>\n"
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
void WriteLandmarksKML(std::ofstream & googleEarth)
{
  std::ifstream landmarks;
  std::string file("landmarks");

  landmarks.open(file.c_str());
  if (landmarks.is_open() == false)
  {
    std::cerr << "Failed to locate/open landmarks file " << file << std::endl;
    return;
  }

  googleEarth
    << " <Folder>\n"
    << "  <name>Landmarks</name>\n";

  char buff[1028];
  while (!landmarks.eof())
  {
    float lat, lon;
    std::string label;

    landmarks >> lat >> lon >> label;

    googleEarth
      << "  <Placemark>\n"
      << "   <name>" << label << "</name>\n"
      << "   <styleUrl>#PM1</styleUrl>\n"
      << "   <Point>\n"
      << "    <coordinates>"
      << lon << ","  << lat << "</coordinates>\n"
      << "   </Point>\n"
      << "  </Placemark>\n";
  }

  landmarks.close();

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
void WriteGoogleEarthKML(std::string& projName, std::string& flightNum, int status_id)
{
  std::ofstream googleEarth;
  std::string file;

  if (status_id == 3)	// if landed.
    file = googleEarthDataDir + "flight_" + flightNum + ".kml";
  else
    file = googleEarthDataDir + "latest" + ".kml";

  googleEarth.open(file.c_str());
  if (googleEarth.is_open() == false)
  {
    std::cerr << "Failed to open output file " << file << std::endl;
    return;
  }

  googleEarth
	<< "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
	<< "<kml xmlns=\"http://earth.google.com/kml/2.1\">\n"
	<< "<Document>\n"
	<< " <name>" << projName << " " << flightNum << "</name>\n"
	<< " <Style id=\"PM1\">\n"
	<< "  <IconStyle>\n"
	<< "   <scale>0.5</scale>\n"
	<< "   <Icon>\n"
	<< "    <href>http://www.eol.ucar.edu/flight_data/images/red.png</href>\n"
	<< "   </Icon>\n"
	<< "  </IconStyle>\n"
	<< " </Style>\n"
	<< " <Style id=\"PM2\">\n"
	<< "  <IconStyle>\n"
	<< "   <scale>0.5</scale>\n"
	<< "   <Icon>\n"
	<< "    <href>http://www.eol.ucar.edu/flight_data/images/white.png</href>\n"
	<< "   </Icon>\n"
	<< "  </IconStyle>\n"
	<< " </Style>\n"
	<< " <Style id=\"BOUND\">\n"
	<< "  <LineStyle>\n"
	<< "   <color>ffff0000</color>\n"
	<< "  </LineStyle>\n"
	<< " </Style>\n"
	<< " <Style id=\"TRACK\">\n"
	<< "  <LineStyle>\n"
	<< "   <color>ff00ffff</color>\n"
	<< "  </LineStyle>\n"
	<< "  <PolyStyle>\n"
	<< "   <color>7f00ff00</color>\n"
	<< "  </PolyStyle>\n"
	<< " </Style>\n"
	<< " <LookAt>\n"
	<< "  <longitude>" << _lon[_lon.size()-1] << "</longitude>\n"
	<< "  <latitude>" << _lat[_lat.size()-1] << "</latitude>\n"
	<< "  <range>100000</range>\n"
	<< "  <tilt>55</tilt>\n"
	<< "  <heading>0</heading>\n"
	<< " </LookAt>\n"
	<< " <Folder>\n"
	<< "  <name>" << flightNum << "</name>\n"
	<< "  <open>1</open>\n"
	<< "  <Placemark>\n"
	<< "   <name>Track</name>\n"
	<< "   <styleUrl>#TRACK</styleUrl>\n"
	<< "   <visibility>1</visibility>\n"
	<< "   <open>1</open>\n"
	<< "   <LineString>\n"
	<< "    <extrude>1</extrude>\n"
	<< "    <tessellate>1</tessellate>\n"
	<< "    <altitudeMode>absolute</altitudeMode>\n"
	<< "    <coordinates>\n";

  for (size_t i = 0; i < _lat.size(); ++i)
  {
    googleEarth << _lon[i] << "," << _lat[i] << "," << (int)_alt[i] << "\n";
  }

  googleEarth
	<< "    </coordinates>\n"
	<< "   </LineString>\n"
	<< "  </Placemark>\n"
	<< "  <Placemark>\n"
	<< "   <name>Plane</name>\n"
	<< "   <description>" << endBubbleCDATA(status_id) << "</description>\n"
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

  WriteLandmarksKML(googleEarth);

  googleEarth
		<< "</Folder>\n"
		<< "</Document>\n"
		<< "</kml>\n";

  googleEarth.close();
  if (status_id != 3)	// if !landed.
  {
    char buffer[1024];
    std::string temp = googleEarthDataDir + "real-time.kmz";
    sprintf(buffer, "rm %s; zip %s %s", file.c_str(), file.c_str(), temp.c_str());
    system(buffer);
  }
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

  _date.push_back( extractPQString(res, indx, TIME) );
  _lon.push_back( extractPQvalue<float>(PQgetvalue(res, indx, LON)) );
  _lat.push_back( extractPQvalue<float>(PQgetvalue(res, indx, LAT)) );
  _alt.push_back( extractPQvalue<float>(PQgetvalue(res, indx, ALT)) * 3.2808);
  latestTAS = extractPQvalue<float>(PQgetvalue(res, indx, TAS));
  latestAT = extractPQvalue<float>(PQgetvalue(res, indx, AT));
  latestWS = extractPQvalue<float>(PQgetvalue(res, indx, WS));
  latestWD = extractPQvalue<float>(PQgetvalue(res, indx, WD));
  latestWI = extractPQvalue<float>(PQgetvalue(res, indx, WI));
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

  if (_lat.size() == 0)	// First time.
  {
    res = PQexec(conn, query.c_str());
    ntuples = PQntuples(res);

    firstAT = 0.0, firstTAS = -1.0, firstWS = 0.0, firstWD = 0.0, firstWI = 0.0;

    if (ntuples > 0)
    {
      for (int i = 0; i < ntuples; i += TimeStep)
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

     if (newEndTime != prevEndTime && ntuples > 0)
      updateData(res, 0);

    prevEndTime = newEndTime;
  }

  PQclear(res);
}

/* -------------------------------------------------------------------- */
int flightStatus()
{
  int status = 2;	// Default to 'in-flight'.

  if (firstTAS == -1)
    status = 1;		// Pre flight.
  else
  if (latestTAS < TAS_CutOff)
    status = 3;		// Landed.

  return status;
}

/* -------------------------------------------------------------------- */
PGconn * openDataBase()
{
//  PGconn *conn = PQconnectdb("host = 'orion.atd.ucar.edu' dbname = 'real-time' user = 'ads'");
  PGconn *conn = PQconnectdb("");	// Use environment variables.

  /* check to see that the backend connection was successfully made
   */
  if (PQstatus(conn) == CONNECTION_BAD)
  {
    PQfinish(conn);
    std::cerr << "SourceSQL: Connection failed:\n";
    return 0;
  }

  return conn;
}

/* -------------------------------------------------------------------- */
void ReadDataFromNetCDF(char *fileName)
{
  NcFile file(fileName);

  if (file.is_valid() == false)
    return;

  NcAtt* attr;
  attr = file.get_att("FlightNumber");
  flightNumber = attr->as_string(0);

  attr = file.get_att("ProjectName");
  projectName = attr->as_string(0);

  attr = file.get_att("coordinates");
  char *coords = attr->as_string(0);

  char lon[32], lat[32], alt[32], tim[32];
  sscanf(coords, "%s %s %s %s", lon, lat, alt, tim);

  NcVar* tim_v = file.get_var(tim);
  NcVar* tas_v = file.get_var("TASX");
  NcVar* lat_v = file.get_var(lat);
  NcVar* lon_v = file.get_var(lon);
  NcVar* alt_v = file.get_var(lat);

  NcValues *tim_vals = tim_v->values();
  NcValues *tas_vals = tas_v->values();
  NcValues *lat_vals = lat_v->values();
  NcValues *lon_vals = lon_v->values();
  NcValues *alt_vals = alt_v->values();

  attr = tim_v->get_att("units");
  struct tm tm;
  strptime(attr->as_string(0), "seconds since %F %T +0000", &tm);
  time_t t = mktime(&tm);

  for (int i = 0; i < tim_vals->num(); i += TimeStep)
  {
    for (; tas_vals->as_float(i) < TAS_CutOff; ++i)
      ;

    if (i >= tim_vals->num())
      break;

    char buffer[60];
    time_t thist = t + tim_vals->as_int(i);
    strftime(buffer, 60, "%F %T", gmtime(&thist));
    _date.push_back( buffer );
    _lon.push_back( lon_vals->as_float(i) );
    _lat.push_back( lat_vals->as_float(i) );
    _alt.push_back( alt_vals->as_float(i) * 3.2808 );
  }
}

/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  if (argc > 1)
  {
    ReadDataFromNetCDF(argv[1]);
    if (_lat.size() > 0)
    {
//      WriteGoogleMapXML(projectName, flightNumber, 3);
      WriteGoogleEarthKML(projectName, flightNumber, 3);
    }
    return 0;
  }


  // Real-time mode.

  PGconn * conn;

  while (1)
  {
    while ((conn = openDataBase()) == 0)
      sleep(3);

    flightNumber = getGlobalAttribute(conn, "FlightNumber");
    projectName = getGlobalAttribute(conn, "ProjectName");
    if (flightNumber.size() == 0)
      flightNumber = "noflight";

    std::string dataQuery = buildDataQueryString(conn);

    GetNewData(conn, dataQuery);
    PQfinish(conn);


    if (_lat.size() > 0)
    {
//      WriteGoogleMapXML(projectName, flightNumber, flightStatus());
      WriteGoogleEarthKML(projectName, flightNumber, flightStatus());
      WriteCurrentPositionKML(projectName, flightNumber);
    }
    sleep(TimeStep*2);
  }

  return 0;
}
