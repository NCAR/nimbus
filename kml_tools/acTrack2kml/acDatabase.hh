
#include "Config.hh"
#include "ProjectInfo.hh"
#include "AircraftTrack.hh"

#include <string>
#include <map>
#include <vector>

class db_private_t;
struct pg_result;

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
  setIndex(int ix)
  {
    column_index = ix;
    return *this;
  }

  std::string name;
  std::string units;
  int column_index;
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

  std::string
  getVariableUnits(const std::string& name);

  std::string
  getGlobalAttribute(const std::string& key);

  std::string
  buildDataQueryString(AircraftTrack& track);

  void
  setupVariables();

  void
  loadGlobalAttributes();

  void
  loadDirectory(std::vector<std::string>& names);

  void
  addResult(AircraftTrack& track, pg_result *res, int indx);

  void
  addColumn(const std::string& name);

  Config cfg;

  typedef std::map<std::string, std::string> attr_map_t;
  attr_map_t _global_attributes;

  db_private_t* db;

  std::vector<Variable> variables;

  typedef std::map<std::string, Variable> dir_t;
  dir_t _directory;
};
