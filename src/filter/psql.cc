/*
-------------------------------------------------------------------------
OBJECT NAME:    psql.cc (PostgreS)

FULL NAME:      PostgreSQL database routines.

COPYRIGHT:      University Corporation for Atmospheric Research, 2003-06
-------------------------------------------------------------------------
*/

#include "psql.h"
#include "transmit.h"
#include "vardb.h"

#include <algorithm>
#include <cctype>
#include <set>
#include <iomanip>

void GetPMS1DAttrsForSQL(RAWTBL *rp, char sql_buff[]);

const int PostgreSQL::RT_UDP_PORT = 58800;
const std::string PostgreSQL::TIME_VARIABLE = "datetime";
const std::string PostgreSQL::GLOBAL_ATTR_TABLE = "Global_Attributes";
const std::string PostgreSQL::VARIABLE_LIST_TABLE = "Variable_List";
const std::string PostgreSQL::CATEGORIES_TABLE = "Categories";
const std::string PostgreSQL::LRT_TABLE = "RAF_LRT";
const std::string PostgreSQL::RATE_TABLE_PREFIX = "SampleRate";

std::string readLandmarks();

/* -------------------------------------------------------------------- */
PostgreSQL::PostgreSQL(std::string specifier, bool transmitToGround)
{
  _conn = PQconnectdb(specifier.c_str());

  /* check to see that the backend connection was successfully made
   */
  if (PQstatus(_conn) == CONNECTION_BAD)
  {
    fprintf(stderr, "Connection to database failed, check environment variables.\n");
    fprintf(stderr, "%s", PQerrorMessage(_conn));
    PQfinish(_conn);
    _conn = 0;
    return;
  }

  PQsetnonblocking(_conn, true);

  if (cfg.TransmitToGround())
  {
   if (cfg.Aircraft() == Config::HIAPER)
      _ldm = new sqlTransmit("G5");
   if (cfg.Aircraft() == Config::C130)
      _ldm = new sqlTransmit("C130");
  }
  else
    _ldm = 0;

  // Don't recreate database if this is the same flight.
  if (isSameFlight() == false)
  {
    LogMessage("Not the same flight as previous startup, dropping databases.");
    dropAllTables();	// Remove existing tables, this is a reset.
    createTables();
    sleep(3);
    initializeGlobalAttributes();
    initializeVariableList();
    submitCommand(
    "CREATE RULE update AS ON UPDATE TO global_attributes DO NOTIFY current", true);
    if (_ldm)
      _ldm->setTimeInterval(5);
  }

#ifdef BCAST_DATA
  if (cfg.ProcessingMode() == Config::RealTime)
  {
    _brdcst = new UdpSocket(RT_UDP_PORT, "192.168.84.255");
    _brdcst->openSock(UDP_BROADCAST);
  }
#endif

  if (cfg.TransmitToGround())
    LaunchGroundFeed();

}	/* END CTOR */

/* -------------------------------------------------------------------- */
inline std::string
PostgreSQL::updateEndTimeString(const std::string & timeStamp, long usec) const
{
  std::stringstream temp;
  temp << "UPDATE global_attributes SET value='"
        << timeStamp << '.' << usec << "' WHERE key='EndTime';";

  return temp.str();
}

/* -------------------------------------------------------------------- */
void
PostgreSQL::WriteSQL(const std::string timeStamp)
{
  static size_t	cntr = 0;

  /* Set global attributes start and end time to the timeStamp of the
   * first record to arrive.
   */
  if (cntr == 0)
  {
    _sqlString.str("");
    _sqlString << "INSERT INTO global_attributes VALUES ('StartTime', '"
	<< timeStamp << "'); INSERT INTO global_attributes VALUES ('EndTime', '"
	<< timeStamp << "');";
    submitCommand(_sqlString.str(), true);

    // transmit of 1 second data removed from nimbus...
    _ldm = 0;
  }


  /* The difference between SQL & UDP strings is the preamble and for SQL
   * the vectors must be encasulated with single quatations. e.g.
   * SQL = '{5, 6, 7, 8}'
   * UDP = {5, 6, 7, 8}
   */
  _sqlString.str("");
  _sqlString << "INSERT INTO " << LRT_TABLE << " VALUES ('" << timeStamp << "'";

#ifdef BCAST_DATA
  _broadcastString.str("");
  _broadcastString << "RAF-TS " << timeStamp << ' ';
#endif

  _transmitString.str("");
  _transmitString << _sqlString.str();

  extern NR_TYPE	*AveragedData;


  /* Two loops again, raw and derived.  This is raw.
   */
  for (size_t i = 0; i < raw.size(); ++i)
  {
    if (raw[i]->Length > 1)	// PMS/vector data.
      addVectorToAllStreams(&AveragedData[raw[i]->LRstart], raw[i]->Length, raw[i]->Transmit);
    else
      addValueToAllStreams(AveragedData[raw[i]->LRstart], raw[i]->Transmit);
  }


  /* Two loops again, raw and derived.  This is derived.
   */
  for (size_t i = 0; i < derived.size(); ++i)
  {
    if (derived[i]->Length > 1)
      addVectorToAllStreams(&AveragedData[derived[i]->LRstart], derived[i]->Length, derived[i]->Transmit);
    else
      addValueToAllStreams(AveragedData[derived[i]->LRstart], derived[i]->Transmit);
  }


#ifdef BCAST_DATA
  if (cfg.ProcessingMode() == Config::RealTime)
  {
    _broadcastString << '\n';
    _brdcst->writeSock(_broadcastString.str().c_str(), _broadcastString.str().length());
  }
#endif

  _sqlString << ");";
  _transmitString << ");";
  submitCommand(_sqlString.str(), false);

  if (_ldm)
  {
    if (_ldm->oneCommandLeft())
      _transmitString << updateEndTimeString(timeStamp, 0);
    _ldm->queueString(_transmitString.str());
  }


  long usec = WriteSQLvolts(timeStamp);


  submitCommand(updateEndTimeString(timeStamp, usec), false);


  if (++cntr % 3600 == 0)     // every hour
  {
fprintf(stderr, "Performing ANALYZE @ %s\n", timeStamp.c_str());

    _sqlString.str("");
    _sqlString << "ANALYZE " << LRT_TABLE << " (" << TIME_VARIABLE << ");";

    submitCommand(_sqlString.str(), true);
  }

}	/* END WRITESQL */

/* -------------------------------------------------------------------- */
void
PostgreSQL::dropAllTables()
{
  std::set<std::string> tablesToDelete;

  tablesToDelete.insert(GLOBAL_ATTR_TABLE);
  tablesToDelete.insert(VARIABLE_LIST_TABLE);
  tablesToDelete.insert(CATEGORIES_TABLE);
  tablesToDelete.insert(LRT_TABLE);
  tablesToDelete.insert("PMS1D_list");
  tablesToDelete.insert("PMS2D_list");

  PGresult* res;

  /* Add SampleRate* tables.
   */
  res = PQexec(_conn, "SELECT sampleratetable FROM Variable_List");

  for (int i = 0; i < PQntuples(res); ++i)
  {
    std::string s(PQgetvalue(res, i, 0));
    remove_trailing_spaces(s);
    if (s.length() > 0)
      tablesToDelete.insert(s);
  }

  PQclear(res);

  /* Add pms2d particle tables.
   */
  res = PQexec(_conn, "SELECT Name FROM PMS2D_list");

  for (int i = 0; i < PQntuples(res); ++i)
  {
    std::string s(PQgetvalue(res, i, 0));
    remove_trailing_spaces(s);
    if (s.length() > 0)
      tablesToDelete.insert(s);
  }

  PQclear(res);


  /* Loop & DROP.
   */
  std::set<std::string>::iterator it;
  for (it = tablesToDelete.begin(); it != tablesToDelete.end(); ++it)
  {
    _sqlString.str("");
    _sqlString << "DROP TABLE " << *it << ';';
    submitCommand(_sqlString.str(), true);
  }

  /*
   * Database seems to slow down after a number of runs without VACUUMing.
   * It's not sufficient to just DROP all tables.
   */
  submitCommand("VACUUM FULL;", true);

}	// END DROPTABLES

/* -------------------------------------------------------------------- */
void
PostgreSQL::createTables()
{
  std::string lm = readLandmarks();
  _sqlString.str("");

  int size = std::max((size_t)80, lm.size()+2);	// +2 cause I'm old fashioned.

  _sqlString << "CREATE TABLE Global_Attributes (key char(20) PRIMARY KEY, value char(" << size << "));";
  _sqlString << "CREATE TABLE Variable_List (Name char(20) PRIMARY KEY, Units char(16), Uncalibrated_Units char(16), long_name char(80), SampleRateTable char(16), nDims int, dims int[], nCals int, poly_cals float[], missing_value float, data_quality char(16));";
  _sqlString << "CREATE TABLE Categories (variable char(20), category char(20));";

  /*
   * PMS tables.
   */
  _sqlString << "CREATE TABLE PMS1D_list (Name char(20), SerialNumber char(16), SampleRateTable char(16), FirstBin INT, LastBin INT, CellSizes FLOAT[]);";
  _sqlString << "CREATE TABLE PMS2D_list (Name char(20), SerialNumber char(16));";

  submitCommand(_sqlString.str(), true);

}	// END CREATETABLES

/* -------------------------------------------------------------------- */
void
PostgreSQL::initializeGlobalAttributes()
{
  std::string coords = cfg.CoordinateVariables();
  coords.replace(coords.find("Time"), 4, TIME_VARIABLE);
  extern char dateProcessed[];	// From netcdf.c

  _sqlString.str("");

  /* Add Global Attributes/Flight Data.
   */
  _sqlString << "INSERT INTO global_attributes VALUES ('Source', 'NCAR Research Aviation Facility');";
  _sqlString << "INSERT INTO global_attributes VALUES ('Address', 'P.O. Box 3000, Boulder, CO 80307-3000');";
  _sqlString << "INSERT INTO global_attributes VALUES ('Phone', '(303) 497-1030');";
  _sqlString << "INSERT INTO global_attributes VALUES ('ProjectName', '" << cfg.ProjectName() << "');";
  _sqlString << "INSERT INTO global_attributes VALUES ('Platform', '" << cfg.TailNumber() << "');";
  _sqlString << "INSERT INTO global_attributes VALUES ('ProjectNumber', '" << cfg.ProjectNumber() << "');";
  _sqlString << "INSERT INTO global_attributes VALUES ('FlightNumber', '" << cfg.FlightNumber() << "');";
  _sqlString << "INSERT INTO global_attributes VALUES ('DateProcessed', '" << dateProcessed << "');";
  _sqlString << "INSERT INTO global_attributes VALUES ('coordinates', '" << coords << "');";
  _sqlString << "INSERT INTO global_attributes VALUES ('wind_field', '" << cfg.WindFieldVariables() << "');";
  _sqlString << "INSERT INTO global_attributes VALUES ('landmarks', '" << readLandmarks() << "');";

  submitCommand(_sqlString.str(), true);

}	// END INITIALIZEGLOBALATTRIBUTES

/* -------------------------------------------------------------------- */
void
PostgreSQL::initializeVariableList()
{
  int	nDims, dims[3];

  rateTableMap		rateTableMap;

  // Use _transmitString to build the ground version of rateTableMap[1].
  _transmitString.str("");

  nDims = 1;
  dims[0] = 1;


  std::vector<float> noCals;

  /* 2 big loops here for raw and derived.  This is raw.
   */
  for (size_t i = 0; i < raw.size(); ++i)
  {
    std::string        name;

    if (isdigit(raw[i]->name[0])) // Can't support vars starting with number.
    {	// Use 'H' for Houskeeping..... ?
      name = "H" + std::string(raw[i]->name);
    }
    else
      name = raw[i]->name;

    /* If PMS2D then add to the PMS2D table.
     */
    if (strncmp(raw[i]->name, "A1D", 3) == 0)
    {
      std::string name(raw[i]->name);
      name = name.substr(0, name.length()-1);

      _sqlString.str("");
      _sqlString << "INSERT INTO PMS2D_list VALUES ('" << name
	<< "', '" << raw[i]->SerialNumber << "');";
      _sqlString << "CREATE TABLE " << name
	<< " (" << TIME_VARIABLE << " time (3) PRIMARY KEY, nSlices int, particle int[]);";
      submitCommand(_sqlString.str());
    }

    /* If PMS1D then add to the PMS1D table.
     */
    if (raw[i]->Length > 1)
    {
      std::string name(&raw[i]->name[1]);

      _sqlString.str("");
      _sqlString << "INSERT INTO PMS1D_list VALUES ('" << name << "', '" <<
	raw[i]->SerialNumber << "', '" << RATE_TABLE_PREFIX <<
	raw[i]->SampleRate << '\'';

      GetPMS1DAttrsForSQL(raw[i], buffer);
      _sqlString << buffer;
      _sqlString << ");";
      submitCommand(_sqlString.str(), true);
    }

    if (raw[i]->Length > 1)
    {
      nDims = 2;
      // Subtract 1, since we don't put 0th bin into SQL database.
      // See addVectorToAllStreams() & pms1d.c:GetPMS1DAttrsForSQL()
      dims[1] = raw[i]->Length-1;
    }
    else
      nDims = 1;

    addVariableToDataBase(raw[i], nDims, dims, noCals, MISSING_VALUE);

    /* Don't add/duplicate rate 1. Don't add vectors for the time being.
     */
    if (raw[i]->SampleRate > 1 && raw[i]->Length == 1)
      addVariableToTables(rateTableMap, raw[i], true);
    else
      addVariableToTables(rateTableMap, raw[i], false);
  }


  /* 3 big loops here for analog, raw and derived.  This is derived.
   */
  for (size_t i = 0; i < derived.size(); ++i)
  {
    if (derived[i]->Length > 1)
    {
      nDims = 2;
      // Subtract 1, since we don't put 0th bin into SQL database.
      // See addVectorToAllStreams() & pms1d.c:GetPMS1DAttrsForSQL()
      dims[1] = derived[i]->Length-1;
    }
    else
      nDims = 1;

    addVariableToDataBase(derived[i], nDims, dims, noCals, MISSING_VALUE);
    addVariableToTables(rateTableMap, derived[i], false);
  }

  /* Send commands to create the "SampleRate*" tables.
   */
  createSampleRateTables(rateTableMap);

}	// END INITIALIZEVARIABLELIST

/* -------------------------------------------------------------------- */
long
PostgreSQL::WriteSQLvolts(const std::string timeStamp)
{
  rateTableList::iterator it;
  int	maxRate = 1;

  extern NR_TYPE *SRTvolts;

  _sqlString.str("");

  for (it = _ratesTables.begin(); it != _ratesTables.end(); ++it)
  {
    /// @todo should the LRT_TABLE be in the rateTableMap?
    if (it->second == LRT_TABLE)
      continue;

    maxRate = std::max(maxRate, it->first);

    for (int i = 0; i < it->first; ++i)
    {
      _sqlString << "INSERT INTO " << it->second << " VALUES ('" << timeStamp
	<< "." << std::setfill('0') << std::setw(3) << 1000 / it->first * i << "'";

      for (size_t j = 0; j < raw.size(); ++j)
        if (raw[j]->SampleRate == (size_t)it->first && raw[j]->Length == 1)
          addValue(_sqlString, SRTvolts[raw[j]->SRstart+i]);

      _sqlString << ");";
    }
  }

  submitCommand(_sqlString.str());

  // Return lastest timestamp written in usec.
  return (1000 / maxRate) * (maxRate-1);

}	// END WRITESQLVOLTS

/* -------------------------------------------------------------------- */
void
PostgreSQL::Start2dSQL()
{
  _sql2d_str.str("BEGIN;");

}	// END START2DSQL

/* -------------------------------------------------------------------- */
void
PostgreSQL::Write2dSQL(RAWTBL *rp, long time, long msec, ulong *p, int nSlices)
{
  if (nSlices < 2)
    return;

  std::string name(rp->name);
  name = name.substr(0, name.length()-1);

  _sql2d_str << "INSERT INTO " << name << " VALUES ('";

//  sprintf(temp, "%02d:%02d:%02d.%03u', %d, '{",
//	time/3600, (time%3600)/60, time%60, msec, nSlices);

  char  pf = _sql2d_str.fill('0');
  int   pw = _sql2d_str.width(2);

  _sql2d_str << time/3600 << ':' << (time%3600)/60 << ':' << time%60 << '.';
  _sql2d_str.width(3);
  _sql2d_str << msec << "', " << nSlices << ", '{" << p[0];

  _sql2d_str.fill(pf);
  _sql2d_str.width(pw);

  for (int i = 1; i < nSlices; ++i)
    _sql2d_str << ',' << p[i];

  _sql2d_str << "}');";

}	// END WRITE2DSQL

/* -------------------------------------------------------------------- */
void
PostgreSQL::Submit2dSQL()
{
  _sql2d_str << "COMMIT;";
  submitCommand(_sql2d_str.str());

}	// END SUBMIT2DSQL

/* -------------------------------------------------------------------- */
inline void
PostgreSQL::addValue(std::stringstream& sql, NR_TYPE value, bool addComma)
{
  if (addComma)
    sql << ',';

  if (isnan(value) || isinf(value))
    sql << MISSING_VALUE;
  else
    sql << value;

}	// END ADDVALUE

/* -------------------------------------------------------------------- */
inline void
PostgreSQL::addValueToAllStreams(NR_TYPE value, bool xmit, bool addComma)
{
  static const char commaFormat[] = ",%e";
  static const char normalFormat[] = "%e";

  const char *format = commaFormat;
  char value_ascii[32];

  if (!addComma)
    format = normalFormat;

  if (isnan(value) || isinf(value))
    value = MISSING_VALUE;

  sprintf(value_ascii, format, value);

  _sqlString << value_ascii;
#ifdef BCAST_DATA
  _broadcastString << value_ascii;
#endif
  if (xmit)
    _transmitString << value_ascii;

}	// END ADDVALUE

/* -------------------------------------------------------------------- */
inline void
PostgreSQL::addVectorToAllStreams(const NR_TYPE *value, size_t nValues, bool xmit)
{
  _sqlString << ",'{";
#ifdef BCAST_DATA
  _broadcastString << ",{";
#endif
  if (xmit)
    _transmitString << ",'{";

  /* Start at 1 to eliminate unused 0th bin.  See also GetPMS1DAttrsForSQL().
   * and intializeVariableList().
   */
  size_t start = 1;
  for (size_t j = start; j < nValues; ++j)
  {
    if (j != start)
      addValueToAllStreams(value[j], xmit);
    else
      addValueToAllStreams(value[j], xmit, false);
  }

  _sqlString << "}'";
#ifdef BCAST_DATA
  _broadcastString << '}';
#endif
  if (xmit)
    _transmitString << "}'";

}	// END ADDVECTOR

/* -------------------------------------------------------------------- */
void
PostgreSQL::addVariableToDataBase(
		const var_base *var,
		size_t nDims,
		const int dims[],
		const std::vector<float>& cals,
		float missingValue)
{
  std::stringstream entry;

  entry << "INSERT INTO Variable_List VALUES ('" <<
	var->name	<< "', '" <<
	escape_string(var->Units)	<< "', '" <<
	escape_string(var->AltUnits)	<< "', '" <<
	escape_string(var->LongName)	<< "', '";

  if (var->SampleRate > 0)
    entry << RATE_TABLE_PREFIX << var->SampleRate;

  entry << "', '" << nDims << "', '{";

  for (size_t i = 0; i < nDims; ++i)
  {
    if (i > 0)
      entry << ',';

    entry << dims[i];
  }
  entry << "}', '" << cals.size() << "', '{";

  for (size_t i = 0; i < cals.size(); ++i)
  {
    if (i > 0)
      entry << ',';

    entry << cals[i];
  }

  entry << "}', '" << missingValue << "', '" << var->DataQuality << "');";

  addCategory(entry, var);
  submitCommand(entry.str(), var->Transmit);

}	// END ADDVARIABLETODATABASE

/* -------------------------------------------------------------------- */
void
PostgreSQL::addCategory(std::stringstream& entry, const var_base * var) const
{
  /** @todo Must check  the category for a ' (single quote), appears
   * we have one with one.
   */
  for (size_t i = 0; i < var->CategoryList.size(); ++i)
    entry << "INSERT INTO categories VALUES ('" << var->name << "', '"
	  << escape_string(var->CategoryList[i]) << "');";

}	// END ADDCATEGORY

/* -------------------------------------------------------------------- */
void
PostgreSQL::addVariableToTables(rateTableMap &tableMap, const var_base *var,
  bool addToSRTtable)
{
  std::vector<int> rates;

  /* Add to LRT_TABLE.
   */
  rates.push_back(1);

  if (addToSRTtable)
    rates.push_back(var->SampleRate);

  for (size_t i = 0; i < rates.size(); ++i)
  {
    // Set up raw tables.
    std::stringstream preamble, rt;

    if (i == 0)
      rt << LRT_TABLE;
    else
      rt << RATE_TABLE_PREFIX << rates[i];

    _ratesTables[rates[i]] = rt.str();

    preamble << "CREATE TABLE " << rt.str() << " (" << TIME_VARIABLE << " timestamp ";

    if (rates[i] > 1)
      preamble << "(3) ";

    preamble << "PRIMARY KEY, ";

    /* We don't have a map entry for the ground transmit subset of
     * variables.  Build it in _transmitString, initialize here.
     */
    if (i == 0 && var->Transmit) // Only do this for LRT table.
    {
      if (_transmitString.str().length() == 0)
        _transmitString << preamble.str();
      else
        _transmitString << ',';
    }

    if (tableMap[preamble.str()].length() > 0)
      tableMap[preamble.str()] += ',';

    std::string build = var->name;
    build += " FLOAT";
    if (var->Length > 1)	// array.
      build += "[]";

    tableMap[preamble.str()] += build;
    if (i == 0 && var->Transmit)
      _transmitString << build;
  }

}	// END ADDVARIABLETOTABLES

/* -------------------------------------------------------------------- */
void
PostgreSQL::createSampleRateTables(const rateTableMap &tableMap)
{
  rateTableMap::const_iterator it;

  _sqlString.str("");
  for (it = tableMap.begin(); it != tableMap.end(); ++it)
    _sqlString << it->first << it->second << ");";

  submitCommand(_sqlString.str());

  // Send the subset'ed RAF_LRT table creation to ground.
  if (_ldm)
  {
    _transmitString << ");";
    _ldm->queueString(_transmitString.str());
  }
}	// END CREATESAMPLERATETABLES

/* -------------------------------------------------------------------- */
std::string
PostgreSQL::getGlobalAttribute(const char key[]) const
{
  char temp[128];
  std::string resultStr;

  sprintf(temp, "SELECT value FROM global_attributes WHERE key='%s';", key);
  PGresult *res = PQexec(_conn, temp);

  if (PQntuples(res) > 0)
  {
    resultStr = PQgetvalue(res, 0, 0);
    remove_trailing_spaces(resultStr);
  }
  PQclear(res);

  return resultStr;
}

/* -------------------------------------------------------------------- */
bool
PostgreSQL::isSameFlight() const
{
  // Anything not starting with pre-ordained prefix is not the same flight.
  // FlightNumber="hangar" being the most obvious.
  if (cfg.FlightNumber().compare(0, 2, "rf") != 0 &&
      cfg.FlightNumber().compare(0, 2, "tf") != 0 &&
      cfg.FlightNumber().compare(0, 2, "ff") != 0)
    return false;

  // Lab test flight is 161 rf09.  Restarting is considered new flight.
  if (cfg.ProjectNumber().compare("161") == 0 &&
      cfg.FlightNumber().compare("rf09") == 0)
    return false;

  if (cfg.ProjectName() != getGlobalAttribute("ProjectName"))
    return false;

  if (cfg.FlightNumber() != getGlobalAttribute("FlightNumber"))
    return false;

  return true;

}	// END ISSAMEFLIGHT

/* -------------------------------------------------------------------- */
void
PostgreSQL::submitCommand(const std::string command, bool xmit)
{
  if (_conn == 0)
    return;

  // Clear any outstanding 'results'.
  PGresult* res;
  while ( (res = PQgetResult(_conn)) )
    PQclear(res);

  // Submit command asynchronously.
  PQsendQuery(_conn, command.c_str());
  fprintf(stderr, "%s", PQerrorMessage(_conn));

  if (_ldm && xmit)
    _ldm->queueString(command);

  while ( (res = PQgetResult(_conn)) )
    PQclear(res);

}	/* END SUBMITCOMMAND */

/* -------------------------------------------------------------------- */
void
PostgreSQL::closeSQL()
{
  if (_conn)
    PQfinish(_conn);
  _conn = 0;
}

/* -------------------------------------------------------------------- */
PostgreSQL::~PostgreSQL()
{
  closeSQL();
}

/* -------------------------------------------------------------------- */
void
PostgreSQL::remove_trailing_spaces(std::string & target) const
{
  while (target[target.size()-1] == ' ')
    target.resize(target.size()-1);
}

/* -------------------------------------------------------------------- */
std::string
PostgreSQL::escape_string(const std::string & target) const
{
  int pos;
  std::string str;
  if ((pos = target.find_first_of("'")) > 0)
    str = target.substr(0, pos) + "\\'" + target.substr(pos + 1);
  else
    str = target;

  return str;
}

/* -------------------------------------------------------------------- */
static const std::string PGHOST = "ac-server";
static const std::string PGDATABASE = "real-time";
static const std::string PGUSER = "ads";

std::string
BuildPGspecString()
{
  std::string specifier;
  char *p;

  specifier = "host=";
  if ((p = getenv("PGHOST")) == 0)
    specifier += PGHOST;
  else
    specifier += p;

  specifier += " dbname=";
  if ((p = getenv("PGDATABASE")) == 0)
    specifier += PGDATABASE;
  else
    specifier += p;

  specifier += " user=";
  if ((p = getenv("PGUSER")) == 0)
    specifier += PGUSER;
  else
    specifier += p;

  return specifier;
}

/* -------------------------------------------------------------------- */
static const char feedCommand[] =
		"/home/local/raf/groundfeed/bin/Groundfeed.sh";
void
PostgreSQL::LaunchGroundFeed()
{
  pid_t pid = fork();

  if (pid == 0)
  {
    execl(feedCommand, feedCommand, cfg.ProjectNumber().c_str(), (char*)0);
    fprintf(stderr, "nimbus: failed to exec %s\n", feedCommand);
    _exit(1);
  }
}

// END PSQL.CC
