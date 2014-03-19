// Acquire the aircraft data from a database or from a cache if enabled.

#include "acDatabase.hh"

#include <string>
#include <iostream>
#include <iterator>
#include <algorithm>

#include <libpq-fe.h>
#include "boost/lexical_cast.hpp"

using std::string;
using std::cerr;
using std::endl;

using namespace boost::posix_time;

// Our raw data is coming from a PostGreSQL database.....
// The order of this enum should match the order of the variables
// being requested in the dataQuery string.
enum VariablePos { TIME=0, LON, LAT, ALT, AT, DP, TAS, WS, WD, WI };

struct db_private_t
{
  PGconn* _conn;
};

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


acDatabase::
acDatabase()
{
  db = new db_private_t();
  db->_conn = 0;
}


acDatabase::
~acDatabase()
{
  closeDatabase();
  delete db;
}


void
acDatabase::
loadDirectory(std::vector<std::string>& names)
{
  std::ostringstream query;

  _directory.clear();
  query << "SELECT name, units from variable_list where name in (";
  for (int i = 0; i < int(names.size()); ++i)
  {
    if (i > 0)
      query << ",";
    query << "'" << names[i] << "'";
  }
  query << ");";

  if (cfg.verbose)
    cerr << query.str() << endl;
  PGresult* res = PQexec(db->_conn, query.str().c_str());
  int ntuples = PQntuples(res);

  for (int j = 0; j < ntuples; ++j)
  {
    std::string name = PQgetvalue(res, j, 0);
    std::string units = PQgetvalue(res, j, 1);
    _directory[name] = Variable(name, units);
  }
  PQclear(res);
}


void
acDatabase::
loadGlobalAttributes()
{
  std::ostringstream query;

  _global_attributes.clear();
  // Load all of the attributes at once, and stash them in a map to look
  // them up later as needed.
  query << "SELECT key, value FROM global_attributes;";

  if (cfg.verbose)
    cerr << query.str() << endl;
  PGresult* res = PQexec(db->_conn, query.str().c_str());
  int ntuples = PQntuples(res);

  for (int j = 0; j < ntuples; ++j)
  {
    std::string key = PQgetvalue(res, j, 0);
    std::string value = PQgetvalue(res, j, 1);
    _global_attributes[key] = value;
  }
  PQclear(res);
}


void
acDatabase::
dump(std::ostream& out)
{
  out << "Global attributes:\n";
  for (attr_map_t::iterator it = _global_attributes.begin();
       it != _global_attributes.end(); ++it)
  {
    out << "   " << it->first << "='" << it->second << "'\n";
  }
  out << "Variable directory:\n";
  for (dir_t::iterator di = _directory.begin(); 
       di != _directory.end(); ++di)
  {
    out << "   " << di->first << " (" << di->second.units << ")\n";
  }
}


void
acDatabase::
addColumn(const std::string& name)
{
  // Add this name as a variable column.  If it's known in the directory,
  // then copy that Variable instance to get the units.  If not, then just
  // create a default.
  if (cfg.verbose)
    cerr << "adding column " << name << endl;
  dir_t::iterator it = _directory.find(name);
  if (it != _directory.end())
  {
    variables.push_back(it->second.setIndex(variables.size()));
  }
  else
  {
    variables.push_back(Variable(name).setIndex(variables.size()));
  }
}


void
acDatabase::
setupVariables()
{
  // These are added in a standard order, but they don't all have standard
  // names in the database.
  static const char* aoc[] = { "TA","TD","TAS","WS","WD","UWZ" };
  static const char* raf[] = { "ATX","DPXC","TASX","WSC","WDC","WIC" };
  static const char* altnames[] = { "PALTF", "PALT" };
  //  static const char* locnames[] = { "GGALT", "GGLAT", "GGLON" };

  if (cfg.verbose)
    cerr << "Loading variables and global attributes from the database." 
	 << endl;

  // We know the maximum list of variables we're interested in, so load all
  // of them now.
  std::vector<std::string> lookupnames;
  std::back_insert_iterator< std::vector<std::string> > it(lookupnames);
  std::copy(aoc, aoc+6, it);
  std::copy(raf, raf+6, it);
  std::copy(altnames, altnames+2, it);

  loadDirectory(lookupnames);
  loadGlobalAttributes();
  if (cfg.verbose)
    dump(cerr);

  const char** pvars;
  variables.clear();
  addColumn("DATETIME");
  string lat = getGlobalAttribute("latitude_coordinate");
  string lon = getGlobalAttribute("longitude_coordinate");
  string alt;

  // Search for pressure altitude first, zaxis_coord is typically GPS alt.
  if (_directory.find("PALTF") != _directory.end())
    alt = "PALTF";
  else if (_directory.find("PALT") != _directory.end())
    alt = "PALT";
  else
    alt = getGlobalAttribute("zaxis_coordinate");
  
  if (getVariableUnits(alt) == "m")
    cfg.convertToFeet = 3.2808;	// feet per meter
  else
    cfg.convertToFeet = 1.0;	// no-op (feet to feet)

  addColumn(lon);
  addColumn(lat);
  addColumn(alt);
  if (cfg.platform == "N42RF" || 
      cfg.platform == "N43RF" ||
      cfg.platform == "N49RF")
  {
    pvars = aoc;
  }
  else
  {
    pvars = raf;
  }
  for (int j = 0; j < 6; ++j)
  {
    addColumn(pvars[j]);
  }
}



/* -------------------------------------------------------------------- */
bool
acDatabase::
openDatabase()
{
  char	conn_str[8192];

  sprintf(conn_str, "host='%s' dbname='%s' user ='ads'", 
          cfg.database_host.c_str(), cfg.dbname.c_str());
  if (cfg.verbose)
    cerr << "Connect string : [" << conn_str << "]" << endl;
  db->_conn = PQconnectdb(conn_str);

  /* check to see that the backend connection was successfully made
   */
  if (PQstatus(db->_conn) == CONNECTION_BAD)
  {
    PQfinish(db->_conn);
    db->_conn = 0;
    cerr << "PQconnectdb: Connection failed." << endl;
  }
  return db->_conn != 0;
}


void
acDatabase::
closeDatabase()
{
  if (db->_conn)
  {
    PQfinish(db->_conn);
    cerr << "PQ database connection closed." << endl;
    db->_conn = 0;
  }
}
    


/* -------------------------------------------------------------------- */
string
acDatabase::
getGlobalAttribute(const std::string& attr)
{
  attr_map_t::iterator it = _global_attributes.find(attr);
  if (it != _global_attributes.end())
    return it->second;
  return "";
}


/* -------------------------------------------------------------------- */
string
acDatabase::
getVariableUnits(const std::string& name)
{
  dir_t::iterator it = _directory.find(name);
  string units;
  if (it != _directory.end())
  {
    units = it->second.units;
  }
  return units;
}


/* -------------------------------------------------------------------- */
string
acDatabase::
buildDataQueryString(AircraftTrack& track)
{
  std::ostringstream query;
  
  query << "SELECT ";

  bool first = true;
  for (int i = 0; i < int(variables.size()); ++i, first=false)
  {
    if (!first)
      query << ",";
    query << variables[i].name;
  }
  query << " from raf_lrt where ";
  query << variables[TAS].name << " > " << cfg.TAS_CutOff;
  query << " AND " << variables[LAT].name << " <> 0.0 ";
  query << " AND " << variables[LON].name << " <> 0.0 ";
  query << " AND " << variables[LAT].name << " <> " << AircraftTrack::missing_value;
  query << " AND " << variables[LON].name << " <> " << AircraftTrack::missing_value;
  if (track.npoints())
  {
    // 2014-01-28 23:54:15
    query << " AND " << variables[TIME].name << " > " 
	  << AircraftTrack::formatTimestamp(track.lastTime(), "'%Y-%m-%d %H:%M:%S'");
  }
  if (cfg.altMode == "absolute")
  {
    query << " AND " << variables[ALT].name << " <> " << AircraftTrack::missing_value;
  }
  query << " ORDER BY " << variables[TIME].name;
  return query.str();
}


/* -------------------------------------------------------------------- */
void
acDatabase::
fillProjectInfo(ProjectInfo& projInfo)
{
  projInfo.flightNumber = getGlobalAttribute("FlightNumber");
  projInfo.projectName = getGlobalAttribute("ProjectName");
  projInfo.platform = getGlobalAttribute("Platform");
  projInfo.landmarks = getGlobalAttribute("landmarks");
  if (projInfo.flightNumber.size() == 0)
    projInfo.flightNumber = "noflight";

  // Get the rate at which data are being shipped from platform to ground.
  string dr = getGlobalAttribute("DataRate");
  int dataRate = atoi(dr.c_str());
  if (dataRate <= 0)
    dataRate = 5;	// 5 second data is the default rate onboard.
  projInfo.groundFeedDataRate = dataRate;
  if (cfg.verbose)
    cerr << "groundFeedDataRate set to " << dataRate << endl;
}



/* -------------------------------------------------------------------- */
void
acDatabase::
addResult(AircraftTrack& track, PGresult *res, int indx)
{
  // 2014-01-29 00:02:10
  track.date.push_back(time_from_string(extractPQString(res, indx, TIME)));
  track.lon.push_back(extractPQvalue<float>(PQgetvalue(res, indx, LON)));
  track.lat.push_back(extractPQvalue<float>(PQgetvalue(res, indx, LAT)));

  float alt = 0.0;
  if (cfg.altMode == "absolute")
  {
    alt = extractPQvalue<float>(PQgetvalue(res, indx, ALT));
    if (alt != AircraftTrack::missing_value)
      alt *= cfg.convertToFeet;
  }
  track.alt.push_back( alt );

  track.at.push_back( extractPQvalue<float>(PQgetvalue(res, indx, AT)) );
  track.dp.push_back( extractPQvalue<float>(PQgetvalue(res, indx, DP)) );
  track.tas.push_back( extractPQvalue<float>(PQgetvalue(res, indx, TAS)) );
  float ws = extractPQvalue<float>(PQgetvalue(res, indx, WS));
  if (ws != AircraftTrack::missing_value)
    ws *= 1.9438;	// convert to knots.
  track.ws.push_back(ws);
  
  track.wd.push_back( extractPQvalue<float>(PQgetvalue(res, indx, WD)) );
  track.wi.push_back( extractPQvalue<float>(PQgetvalue(res, indx, WI)) );
}



/*-------------------------------------------------------------------- */
bool
acDatabase::
fillAircraftTrack(AircraftTrack& track)
{
  bool changed = false;

  // If track has data in it, then we attempt to only append new points to
  // it.  If the track is too old, then assume this is a new flight and
  // everything must be refreshed.
  if (track.npoints() == 0)
  {
    cerr << "Filling an empty track." << endl;
  }
  else
  {
    cerr << "Fetching track points after "
	 << AircraftTrack::formatTimestamp(track.lastTime(), "'%Y-%m-%d %H:%M:%S'")
	 << endl;
  }
  if (track.npoints() > 0 && 
      track.lastTime() < second_clock::universal_time() - hours(12))
  {
    cerr << "Track more than 12 hours old, fetching variables again." << endl;
    variables.clear();
  }

  if (variables.empty())
  {
    // Only static information is used from the ProjectInfo and global
    // attributes, so they do not need to be refreshed every time the
    // database connection is opened, or queried for new points.  Instead
    // they are refreshed if the track is invalidated.
    setupVariables();
  }

  string query = buildDataQueryString(track);
  int ntuples;

  if (cfg.verbose)
  {
    cerr << "        dataQuery: " << query << endl;
    cerr << "              alt: " << variables[ALT].name
	 << " (" << getVariableUnits(variables[ALT].name) << ")" << endl;
    cerr << "cfg.convertToFeet: " << cfg.convertToFeet << endl;
  }
  PGresult *res;
  res = PQexec(db->_conn, query.c_str());
  cerr << PQerrorMessage(db->_conn);
  ntuples = PQntuples(res);
  if (cfg.verbose)
    cerr << ntuples << " result rows." << endl;

  // Check the first result here and see if it is much newer than the
  // existing track, and if so invalidate the current track.  The problem
  // is that we cannot build the query string without calling
  // setupVariables() first, but the variables need to be synchronized if
  // the track is outdated.  So do the cache check in two stages: if the
  // current track is old, then fetch the variables before building the
  // query.  Then if the query returns a new track, clear the current
  // track.  It's like the variables and the track are two separate caches.
  if (ntuples > 0 && track.npoints() > 0)
  {
    ptime next = time_from_string(extractPQString(res, 0, TIME));
    if (next - track.lastTime() > hours(12))
    {
      cerr << "Replacing cached track more than 12 hours old." << endl;
      changed = true;
      track.clear();
    }
  }

  track.reserve(track.npoints() + ntuples);
  for (int i = 0; i < ntuples; ++i)
  {
    addResult(track, res, i);
  }
  cerr << track.npoints() << " points in track." << endl;
  PQclear(res);

  // This does not fetch anything from the database, it just translates
  // global attributes into the ProjectInfo settings, so it's safe to do
  // whenever.
  fillProjectInfo(track.projInfo);

  return changed || ntuples > 0;
}

