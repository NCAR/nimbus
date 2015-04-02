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

struct db_private_t
{
  PGconn* _conn;
};

/* -------------------------------------------------------------------- */
template <typename T>
T
extractPQvalue(PGresult *result, int tuple, int field)
{
  const char *cpqstring = PQgetvalue(result, tuple, field);
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
template <>
std::string
extractPQvalue<std::string>(PGresult *result, int tuple, int field)
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
  // This is 12 hours of 1-second data, so really a database should never
  // have more points than this in the last 12 hours.
  _max_flight_seconds = 43200;
}


acDatabase::
~acDatabase()
{
  closeDatabase();
  delete db;
}


void
acDatabase::
loadDirectory()
{
  std::ostringstream query;

  _directory.clear();
  _directory["DATETIME"] = Variable("DATETIME");
  query << "SELECT name, units from variable_list;";

  if (cfg.verbose > 1)
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

  if (cfg.verbose > 1)
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


Variable*
acDatabase::
lookupDirectory(const std::string& name)
{
  Variable* found = 0;
  dir_t::iterator it = _directory.find(name);
  if (it != _directory.end())
  {
    found = &it->second;
  }
  return found;
}


void
acDatabase::
addColumn(const std::string& name, VariableType vtype)
{
  // Add this name as a variable column.  If it's known in the directory,
  // then copy that Variable instance to get the units.  If not, then just
  // create a default.
  if (cfg.verbose > 1)
    cerr << "adding column " << name << endl;
  Variable* ventry = lookupDirectory(name);
  unsigned int ix = columns.size();
  if (ventry)
  {
    columns.push_back(ventry->setIndex(ix, vtype));
    variables[vtype] = ix;
  }
  else
  {
    // A variable not in the directory may indicate a problem.
    if (cfg.verbose)
    {
      cerr << "Database column " << name
	   << " is not in the variable list!  Adding a plain "
	   << "variable without units." << endl;
    }
    columns.push_back(Variable(name).setIndex(ix, vtype));
    variables[vtype] = ix;
  }
}


void
acDatabase::
clearColumns()
{
  variables.clear();
  columns.clear();
}

#define ALEN(v) (sizeof(v)/sizeof(v[0]))
#define AEND(v) (v + ALEN(v))

void
acDatabase::
setupColumns()
{
  // These are listed in the order of the VariableType enum, so each name
  // can be mapped to the right enum value when added as a query column.
  static const char* aoc[] = { "TA","TD","TAS","WS","WD","UWZ" };
  static const char* raf[] = { "ATX","DPXC","TASX","WSC","WDC","WIC" };

  if (cfg.verbose)
    cerr << "Loading variables and global attributes from the database." 
	 << endl;
  loadGlobalAttributes();
  if (cfg.verbose > 1)
    dump(cerr);
  loadDirectory();

  string lat = getGlobalAttribute("latitude_coordinate");
  string lon = getGlobalAttribute("longitude_coordinate");
  string alt = getGlobalAttribute("zaxis_coordinate");

  // Now start naming the columns to query from the database.
  clearColumns();
  addColumn("DATETIME", TIME);

  // Search for pressure altitude first, zaxis_coord is typically GPS alt.
  if (lookupDirectory("PALTF"))
    alt = "PALTF";
  else if (lookupDirectory("PALT"))
    alt = "PALT";
  lat = cfg.getLatitudeVariable(lat);
  lon = cfg.getLongitudeVariable(lon);
  alt = cfg.getAltitudeVariable(alt);
  cfg.setAltitudeUnits(getVariableUnits(alt));

  addColumn(lon, LON);
  addColumn(lat, LAT);
  addColumn(alt, ALT);
  string thdg = "THDG";
  if (cfg.platform == "N42RF" || 
      cfg.platform == "N43RF" ||
      cfg.platform == "N49RF")
  {
    for (const char** pvar = aoc; pvar != AEND(aoc); ++pvar)
    {
      addColumn(*pvar, static_cast<VariableType>(AT+(pvar-aoc)));
    }
    thdg = "THDGREF";
  }
  else
  {
    for (const char** pvar = raf; pvar != AEND(raf); ++pvar)
    {
      addColumn(*pvar, static_cast<VariableType>(AT+(pvar-raf)));
    }
  }
  // Add a column for THDG only if it is in the directory.
  if (lookupDirectory(thdg))
  {
    addColumn(thdg, THDG);
  }
}



/* -------------------------------------------------------------------- */
bool
acDatabase::
openDatabase()
{
  if (! db->_conn)
  {
    char conn_str[8192];

    sprintf(conn_str, "host='%s' dbname='%s' user ='ads'", 
	    cfg.database_host.c_str(), cfg.dbname.c_str());
    if (cfg.verbose > 1)
    {
      cerr << "Connect string : [" << conn_str << "]" << endl;
    }
    db->_conn = PQconnectdb(conn_str);

    /* check to see that the backend connection was successfully made
     */
    if (PQstatus(db->_conn) == CONNECTION_BAD)
    {
      PQfinish(db->_conn);
      db->_conn = 0;
      if (!cfg.check || cfg.verbose)
	cerr << "PQconnectdb: Connection failed." << endl;
    }
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
    if (cfg.verbose > 1)
    {
      cerr << "PQ database connection closed." << endl;
    }
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


Variable*
acDatabase::
variable(VariableType vtype)
{
  Variable* found = 0;
  variables_t::iterator it;
  it = variables.find(vtype);
  if (it != variables.end())
  {
    found = &(columns[it->second]);
  }
  return found;
}


/* -------------------------------------------------------------------- */
string
acDatabase::
buildDataQueryString(AircraftTrack& track)
{
  std::ostringstream query;
  
  query << "SELECT ";

  for (int i = 0; i < int(columns.size()); ++i)
  {
    if (i > 0)
      query << ",";
    query << columns[i].name;
  }
  query << " from raf_lrt where ";

  // TAS cutoff deprecated in favor of loading the whole track and
  // letting the renderer clip as necessary.
  if (false && variable(TAS))
  {
    query << variable(TAS)->name << " > " << cfg.TAS_CutOff << " AND ";
  }
  query << variable(LAT)->name << " <> 0.0 ";
  query << " AND " << variable(LON)->name << " <> 0.0 ";
  query << " AND " << variable(LAT)->name
	<< " <> " << AircraftTrack::missing_value;
  query << " AND " << variable(LON)->name << " <> " 
	<< AircraftTrack::missing_value;
  // It does not hurt to always limit the number of points retrieved.
  query << " AND " << variable(TIME)->name << " > "
	<< "(SELECT MAX(datetime) - interval '12 hours' from raf_lrt)";
  if (cfg.verbose > 1)
  {
    cerr << "selecting only the most recent "
	 << _max_flight_seconds/3600.0 << " hours of points." << endl;
  }
#ifdef notdef
  if (track.npoints() == 0)
  {
    // This is the initial query to fill a track.  Limit the request to the
    // most recent flight by a 12-hour time period, in case the database
    // contains data from past flights.
    string qdate = "SELECT max(datetime) from raf_lrt;";
    PGresult *res = PQexec(db->_conn, qdate.c_str());
    if (!cfg.check || cfg.verbose)
      cerr << PQerrorMessage(db->_conn);
    int ntuples = PQntuples(res);
    if (ntuples > 0)
    {
      ptime latest = time_from_string(extractPQvalue<string>(res, 0, 0));
      ptime cutoff = latest - hours(12);
      query << " AND " << variable(TIME)->name << " > " 
	    << AircraftTrack::formatTimestamp(cutoff,
					      "'%Y-%m-%d %H:%M:%S'");
      if (cfg.verbose)
      {
	cerr << "selecting only the most recent 12 hours, from "
	     << AircraftTrack::formatTimestamp(cutoff) << " to " 
	     << AircraftTrack::formatTimestamp(latest) << endl;
      }
    }
    PQclear(res);
  }
#endif
  if (track.npoints() > 0)
  {
    // 2014-01-28 23:54:15
    query << " AND " << variable(TIME)->name << " > " 
	  << AircraftTrack::formatTimestamp(track.lastTime(),
					    "'%Y-%m-%d %H:%M:%S'");
  }
  if (cfg.altMode == "absolute")
  {
    query << " AND " << variable(ALT)->name << " <> " 
	  << AircraftTrack::missing_value;
  }
  query << " ORDER BY " << variable(TIME)->name;
  // This is just a failsafe to avoid requesting too many points if the
  // other checks fail.  Note that if this takes effect, then it will cut
  // off the most recent points since the rows are selected in order of
  // datetime.  Someday this could select rows in reverse order, then set
  // them in the track beginning with the last point and working backwards
  // to the first point.
  query << " LIMIT " << _max_flight_seconds;
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
  if (cfg.verbose > 1)
    cerr << "groundFeedDataRate set to " << dataRate << endl;
}


template <typename T>
T
acDatabase::
getResult(PGresult* res, int indx, VariableType vtype)
{
  unsigned int icol = variable(vtype)->column_index;
  return extractPQvalue<T>(res, indx, icol);
}


/* -------------------------------------------------------------------- */
void
acDatabase::
addResult(AircraftTrack& track, PGresult *res, int indx)
{
  // 2014-01-29 00:02:10
  track.date.push_back(time_from_string(getResult<string>(res, indx, TIME)));
  track.lon.push_back(getResult<float>(res, indx, LON));
  track.lat.push_back(getResult<float>(res, indx, LAT));

  float alt = 0.0;
  if (cfg.altMode == "absolute")
  {
    alt = getResult<float>(res, indx, ALT);
    if (alt != AircraftTrack::missing_value)
      alt *= cfg.convertToFeet;
  }
  track.alt.push_back(alt);

  track.at.push_back(getResult<float>(res, indx, AT));
  track.dp.push_back(getResult<float>(res, indx, DP));
  track.tas.push_back(getResult<float>(res, indx, TAS));
  float ws = getResult<float>(res, indx, WS);
  if (ws != AircraftTrack::missing_value)
    ws *= 1.9438;	// convert to knots.
  track.ws.push_back(ws);
  
  track.wd.push_back(getResult<float>(res, indx, WD));
  track.wi.push_back(getResult<float>(res, indx, WI));

  float thdg = AircraftTrack::missing_value;
  if (variable(THDG))
  {
    thdg = getResult<float>(res, indx, THDG);
  }
  track.thdg.push_back(thdg);
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
  if (track.npoints() == 0 && cfg.verbose)
  {
    cerr << "Filling an empty track." << endl;
  }
  else if (cfg.verbose)
  {
    cerr << "Fetching track points after "
	 << AircraftTrack::formatTimestamp(track.lastTime(),
					   "'%Y-%m-%d %H:%M:%S'")
	 << endl;
  }
  if (track.npoints() > 0 && 
      track.lastTime() < second_clock::universal_time() - hours(12))
  {
    if (cfg.verbose)
    {
      cerr << "Track more than 12 hours old, fetching variables again." << endl;
    }
    clearColumns();
  }

  if (columns.empty())
  {
    // Only static information is used from the ProjectInfo and global
    // attributes, so they do not need to be refreshed every time the
    // database connection is opened, or queried for new points.  Instead
    // they are refreshed if the track is invalidated.
    setupColumns();
  }

  string query = buildDataQueryString(track);
  int ntuples;

  if (cfg.verbose > 1)
  {
    cerr << "        dataQuery: " << query << endl;
    cerr << "              alt: " << variable(ALT)->name
	 << " (" << getVariableUnits(variable(ALT)->name) << ")" << endl;
    cerr << "cfg.convertToFeet: " << cfg.convertToFeet << endl;
  }
  PGresult *res;
  res = PQexec(db->_conn, query.c_str());
  if (!cfg.check || cfg.verbose)
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
    ptime next = time_from_string(extractPQvalue<string>(res, 0, TIME));
    if (next - track.lastTime() > seconds(_max_flight_seconds))
    {
      if (cfg.verbose)
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
  if (cfg.verbose)
  {
    cerr << track.npoints() << " points in track." << endl;
  }
  PQclear(res);

  // This does not fetch anything from the database, it just translates
  // global attributes into the ProjectInfo settings, so it's safe to do
  // whenever.
  fillProjectInfo(track.projInfo);

  return changed || ntuples > 0;
}



void
acDatabase::
updateTrack(AircraftTrack& track)
{
  std::ostringstream msg;
  ptime now = second_clock::universal_time();
  if (cfg.verbose)
  {
    cerr << now << ": updating track from database " 
	 << cfg.database_host << "..." << endl;
  }
  if (openDatabase())
  {
    bool rewrite = fillAircraftTrack(track) && track.npoints() > 0;
    closeDatabase();
    if (rewrite)
    {
      msg << "Track updated to " << track.npoints() 
	  << " at " << now;
      track.setStatus(AircraftTrack::UPDATED, msg.str());
    }
    else
    {
      msg << "Track still at " << track.npoints() << " points as of "
	  << now;
      track.setStatus(AircraftTrack::NOCHANGE, msg.str());
    }
  }
  else
  {
    msg << "Database connection failed: "
	<< cfg.database_host;
    track.setStatus(AircraftTrack::ERROR, msg.str());
  }
}

