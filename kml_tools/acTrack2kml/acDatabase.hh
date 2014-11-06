
#include "Config.hh"
#include "ProjectInfo.hh"
#include "AircraftTrack.hh"

#include <string>
#include <map>
#include <vector>

class db_private_t;
struct pg_result;

/*
 * Enumerate the types of variables to be loaded from the database and
 * translated into a track, independent of their various names in the
 * database table.
 */
enum VariableType { TIME=0, LON, LAT, ALT, AT, DP, TAS, WS, WD, WI, THDG };

struct Variable
{
  Variable() :
    column_index(-1)
  {}

  Variable(const std::string& name_in, const std::string& units_in = "") :
    name(name_in),
    units(units_in),
    column_index(-1)
  {}

  Variable&
  setIndex(int ix, VariableType vtype_in)
  {
    column_index = ix;
    vtype = vtype_in;
    return *this;
  }

  std::string name;
  std::string units;
  int column_index;
  VariableType vtype;
};

class acDatabase
{
public:

  acDatabase();

  ~acDatabase();

  void
  setConfig(const Config& config)
  {
    cfg = config;
  }

  void
  fillProjectInfo(ProjectInfo& projInfo);

  bool
  openDatabase();

  void
  closeDatabase();

  /**
   * Query the database for fresh data.  If this is the first request for
   * data, then get everything in the database at the moment.  Return true
   * if the track changes, either because the track is invalidated or
   * points are added, otherwise false.  If a new track is replacing an old
   * one, then this method returns true and track.npoints() > 0.
   */
  bool
  fillAircraftTrack(AircraftTrack& track);

  void
  dump(std::ostream& out);

private:

  template <typename T>
  T
  getResult(pg_result* res, int indx, VariableType vtype);

  std::string
  getVariableUnits(const std::string& name);

  std::string
  getGlobalAttribute(const std::string& key);

  std::string
  buildDataQueryString(AircraftTrack& track);

  void
  setupColumns();

  void
  clearColumns();

  void
  loadGlobalAttributes();

  void
  loadDirectory();

  Variable*
  lookupDirectory(const std::string& name);

  void
  addResult(AircraftTrack& track, pg_result *res, int indx);

  void
  addColumn(const std::string& name, VariableType vtype);

  Variable*
  variable(VariableType vtype);

  Config cfg;

  typedef std::map<std::string, std::string> attr_map_t;
  attr_map_t _global_attributes;

  db_private_t* db;

  // Two data structures for the database columns: a vector in order of the
  // columns in the database query, and a map from variable type to the
  // column index.
  typedef std::map<VariableType, unsigned int> variables_t;
  variables_t variables;
  std::vector<Variable> columns;

  typedef std::map<std::string, Variable> dir_t;
  dir_t _directory;
};
