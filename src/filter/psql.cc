/*
-------------------------------------------------------------------------
OBJECT NAME:    psql.cc (PostgreS)

FULL NAME:      PostgreSQL database routines.

COPYRIGHT:      University Corporation for Atmospheric Research, 2003-06
-------------------------------------------------------------------------
*/

#include "psql.h"

#include <cctype>
#include <ctime>
#include <set>
#include <iomanip>
#include <zlib.h>
#include <sys/param.h>

void GetPMS1DAttrsForSQL(RAWTBL *rp, char sql_buff[]);

const std::string PostgreSQL::TIME_VARIABLE = "datetime";
const std::string PostgreSQL::GLOBAL_ATTR_TABLE = "Global_Attributes";
const std::string PostgreSQL::VARIABLE_LIST_TABLE = "Variable_List";
const std::string PostgreSQL::CATEGORIES_TABLE = "Categories";
const std::string PostgreSQL::LRT_TABLE = "RAF_LRT";
const std::string PostgreSQL::RATE_TABLE_PREFIX = "SampleRate";

std::string readLandmarks();

/* -------------------------------------------------------------------- */
PostgreSQL::PostgreSQL(std::string specifier)
{
  ILOG(("PostgreSQL ctor, spec = %s", specifier.c_str()));

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
    _groundDBinitString.str("");
    _groundDBinitString << cfg.AircraftString() << '\n';
    _groundDBinitString << "CREATE OR REPLACE FUNCTION pg_sleep(int) RETURNS int AS '/lib/libc.so.6', 'sleep' LANGUAGE 'C' STRICT;\n";
    _groundDBinitString << "NOTIFY killiit;\n";
    _groundDBinitString << "SELECT pg_sleep(2);\n";
  }

  // Don't recreate database if this is the same flight.
  if (isSameFlight() == false)
  {
    LogMessage("Not the same flight as previous startup, dropping database tables.");
    dropAllTables();	// Remove existing tables, this is a reset.
    createTables();
    sleep(3);
    initializeGlobalAttributes();
    initializeVariableList();
    grantSelectToTables("guest");
    submitCommand(
    "CREATE RULE update AS ON UPDATE TO global_attributes DO NOTIFY current;", true);
    createNotifyInsertsTrigger();

    submitCommand("INSERT INTO global_attributes VALUES ('checksum', '" + cfg.Checksum() + "');", cfg.TransmitToGround());
  }
}	/* END CTOR */

/* -------------------------------------------------------------------- */
inline std::string
PostgreSQL::updateEndTimeString(const std::string & timeStamp, int32_t usec) const
{
  std::stringstream temp;
  temp << "UPDATE global_attributes SET value='"
        << timeStamp << '.' << usec << "' WHERE key='EndTime';";

  return temp.str();
}

/* -------------------------------------------------------------------- */
void
PostgreSQL::WriteSQL(const std::string & timeStamp)
{
  static size_t	cntr = 0;

  /* Set global attributes start and end time to the timeStamp of the
   * first record to arrive.
   */
  if (cntr++ == 0)
  {
    _sqlString.str("");
    _sqlString << "INSERT INTO global_attributes VALUES ('StartTime', '"
	<< timeStamp << "');INSERT INTO global_attributes VALUES ('EndTime', '"
	<< timeStamp << "');";
    submitCommand(_sqlString.str(), true);

    if (cfg.TransmitToGround())
      outputGroundDBInitPacket();
  }


  /* The difference between SQL & UDP strings is the preamble and for SQL
   * the vectors must be encasulated with single quatations. e.g.
   * SQL = '{5, 6, 7, 8}'
   * UDP = {5, 6, 7, 8}
   */
  _sqlString.str("");
  _sqlString << "INSERT INTO " << LRT_TABLE << " VALUES ('" << timeStamp << "'";


  extern NR_TYPE	*AveragedData;

  /* Two loops again, raw and derived.
   */
  for (size_t i = 0; i < raw.size(); ++i)
  {
    if (raw[i]->Length > 1)	// PMS/vector data.
    {
      /* Start at 1 to eliminate unused 0th bin.  See also GetPMS1DAttrsForSQL().
       * and intializeVariableList().  Change to '= 0' when we remove legacy 0th bin.
       */
      size_t start = raw[i]->LRstart;
      size_t len = raw[i]->Length;
#ifdef ZERO_BIN
      if ((raw[i]->ProbeType & PROBE_PMS1D) || (raw[i]->ProbeType & PROBE_PMS2D))
        { ++start; --len; }
#endif
      addVectorToAllStreams(&AveragedData[start], len);
    }
    else
      addValueToAllStreams(AveragedData[raw[i]->LRstart]);
  }


  for (size_t i = 0; i < derived.size(); ++i)
  {
    if (derived[i]->Length > 1)
    {
      /* Start at 1 to eliminate unused 0th bin.  See also GetPMS1DAttrsForSQL().
       * and intializeVariableList().  Change to '= 0' when we remove legacy 0th bin.
       */
      size_t start = derived[i]->LRstart;
      size_t len = derived[i]->Length;
#ifdef ZERO_BIN
      if ((derived[i]->ProbeType & PROBE_PMS1D) || (derived[i]->ProbeType & PROBE_PMS2D))
        { ++start; --len; }
#endif
      addVectorToAllStreams(&AveragedData[start], len);
    }
    else
      addValueToAllStreams(AveragedData[derived[i]->LRstart]);
  }


  _sqlString << ");";
  submitCommand(_sqlString.str(), false);

  int32_t usec = WriteSQLvolts(timeStamp);

  submitCommand(updateEndTimeString(timeStamp, usec), false);

  /*
   * Should ANALYZE and VACUUM intervals be based on time or on number of
   * records inserted?  When loading a database directly from a raw data
   * file, the records are inserted as fast as possible, and so the ANALYZE
   * and VACUUM will happen much more frequently, perhaps too frequently.
   */

  // Defaults to 1500, which is supposed to be about every hour, but really
  // is 25 minutes.
  size_t interval = cfg.AnalyzeInterval();
  if (interval && (cntr % interval == 0))
  {
    char msg[128];
    sprintf(msg, "psql.cc: Performing ANALYZE @ %s.", timeStamp.c_str());

    ILOG((msg));
    fprintf(stderr, "%s\n", msg);

    _sqlString.str("");
    _sqlString << "ANALYZE " << LRT_TABLE << " (" << TIME_VARIABLE << ");";

    submitCommand(_sqlString.str(), true);
  }

  // Defaults to 1000, which is supposed to be about every half-hour, but
  // really is about 17 minutes.
  interval = cfg.VacuumInterval();
  if (interval && (cntr % interval == 0))
  {
    char msg[128];
    sprintf(msg, "psql.cc: Performing VACUUM @ %s.", timeStamp.c_str());

    ILOG((msg));
    fprintf(stderr, "%s\n", msg);

    _sqlString.str("VACUUM;");
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
  tablesToDelete.insert("parcels");

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


  /* DROP tables.  Do NOT try to make this one DROP TABLE statement.
   * If one of the tables does not exist, then the whole statement fails
   * and none of them get dropped.
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

  _sqlString << "CREATE TABLE Global_Attributes (key text PRIMARY KEY, value text);";
  _sqlString << "CREATE TABLE Variable_List (Name text PRIMARY KEY, Units text, Uncalibrated_Units text, long_name text, SampleRateTable text, nDims int, dims int[], nCals int, poly_cals float[], missing_value float, data_quality text);";
  _sqlString << "CREATE TABLE Categories (variable text, category text);";
  _sqlString << "CREATE TABLE parcels (id text, datetime timestamp);";

  /*
   * PMS tables.
   */
  _sqlString << "CREATE TABLE PMS1D_list (Name text, SerialNumber text, SampleRateTable text, FirstBin INT, LastBin INT, CellSizes FLOAT[]);";
  _sqlString << "CREATE TABLE PMS2D_list (Name text, SerialNumber text);";

  submitCommand(_sqlString.str(), true);

}	// END CREATETABLES

/* -------------------------------------------------------------------- */
void
PostgreSQL::grantSelectToTables(const std::string user)
{
  _sqlString.str("");

  _sqlString << "GRANT SELECT ON Global_Attributes TO " << user << ";";
  _sqlString << "GRANT SELECT ON Variable_List TO " << user << ";";
  _sqlString << "GRANT SELECT ON Categories TO " << user << ";";

  /*
   * PMS tables.
   */
  _sqlString << "GRANT SELECT ON PMS1D_list TO " << user << ";";
  _sqlString << "GRANT SELECT ON PMS2D_list TO " << user << ";";


  /*
   * Low rate table
   */
  _sqlString << "GRANT SELECT ON " << LRT_TABLE << " TO " << user << ";";

  /*
   * TODO: if we ever make read only users (ads) on non-ground db
   *       we will need to grant access to all Sample rate tables
   */

  submitCommand(_sqlString.str(), true);

}	// END GRANTSELECTTOTABLES

/* -------------------------------------------------------------------- */
void
PostgreSQL::createNotifyInsertsTrigger()
{
  _groundDBinitString << R"(

    --
    -- based on https://gist.github.com/colophonemes/9701b906c5be572a40a84b08f4d2fa4e
    --

    --
    -- notify_inserts((channel_name, optional):
    --   Trigger notification for messaging to PG Notify
    --
    --   If channel_name is not specified, defaults to table_name + '_inserts'
    --
    --   e.g.
    --
    --   CREATE TRIGGER notify_inserts AFTER INSERT ON raf_lrt
    --     FOR EACH ROW EXECUTE PROCEDURE notify_inserts();
    --     => NOTIFY on channel 'raf_lrt_inserts'

    --   CREATE TRIGGER notify_inserts AFTER INSERT ON raf_lrt
    --     FOR EACH ROW EXECUTE PROCEDURE notify_inserts('foo');
    --     => NOTIFY on channel 'foo'

    --   Notifies channel w/ JSON hash:
    --     - column name and value as key-value pairs
    --     - e.g.
    --        INSERT INTO raf_lrt(datetime, gglat, gglon,ggalt,atx) VALUES (NOW(), 4, -15,1000,32.2)
    --
    --        =>  {"datetime":"2019-05-07 00:28:16.149046","gglat":"4","gglon":"-15","ggalt":"1000","atx":"32.2"}
    --

    CREATE OR REPLACE FUNCTION notify_inserts() RETURNS trigger AS $trigger$
      DECLARE
        rec RECORD;
        payload TEXT;
        _column_name TEXT;
        column_value TEXT;
        payload_items TEXT[];
        channel_name TEXT;
      BEGIN
        CASE TG_OP
        WHEN 'INSERT' THEN
          rec := NEW;
        ELSE
          RAISE EXCEPTION 'Action TG_OP: "%" is not supported by this function', TG_OP;
        END CASE;
        -- Get required fields
        FOR _column_name IN SELECT column_name FROM information_schema.columns WHERE table_schema=TG_TABLE_SCHEMA AND table_name=TG_TABLE_NAME LOOP
          EXECUTE format('SELECT $1.%I::TEXT', _column_name)
          INTO column_value
          USING rec;
          payload_items := array_append(payload_items, '"' || _column_name || '":"' || replace(column_value, '"', '\"') || '"');
        END LOOP;

        -- Build the payload
        payload :=  '{' || array_to_string(payload_items, ',') || '}';

        -- Notify the channel
        IF TG_ARGV[0] IS NOT NULL THEN
          channel_name := TG_ARGV[0];
        ELSE
          channel_name := TG_TABLE_NAME || '_inserts';
        END IF;
        PERFORM pg_notify(channel_name, payload);

        RETURN rec;
      END;
    $trigger$ LANGUAGE plpgsql;
  )";

  // DROP TRIGGER IF EXISTS notify_inserts ON raf_lrt;
  // CREATE TRIGGER notify_inserts AFTER INSERT ON raf_lrt
  //   FOR EACH ROW EXECUTE PROCEDURE notify_inserts();

  _groundDBinitString << "DROP TRIGGER IF EXISTS notify_inserts ON " << LRT_TABLE << ";";
  _groundDBinitString << "CREATE TRIGGER notify_inserts AFTER INSERT ON " << LRT_TABLE;
  _groundDBinitString << "  FOR EACH ROW EXECUTE PROCEDURE notify_inserts();";

} // END CREATENOTIFYINSERTSTRIGGER

void
PostgreSQL::initializeGlobalAttributes()
{
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
  _sqlString << "INSERT INTO global_attributes VALUES ('latitude_coordinate', '" << cfg.CoordinateLatitude() << "');";
  _sqlString << "INSERT INTO global_attributes VALUES ('longitude_coordinate', '" << cfg.CoordinateLongitude() << "');";
  _sqlString << "INSERT INTO global_attributes VALUES ('zaxis_coordinate', '" << cfg.CoordinateAltitude() << "');";
  _sqlString << "INSERT INTO global_attributes VALUES ('time_coordinate', '" << TIME_VARIABLE << "');";
  _sqlString << "INSERT INTO global_attributes VALUES ('wind_field', '" << cfg.WindFieldVariables() << "');";
  _sqlString << "INSERT INTO global_attributes VALUES ('landmarks', '" << readLandmarks() << "');";
  _sqlString << "INSERT INTO global_attributes VALUES ('DataRate', '" << cfg.GroundFeedDataRate() << "');";

  submitCommand(_sqlString.str(), true);

  // Update data rate onboard to be 1, not the ground feed data rate.
  _sqlString.str("UPDATE global_attributes SET value='1' WHERE key='DataRate'");
  submitCommand(_sqlString.str());

}	// END INITIALIZEGLOBALATTRIBUTES

/* -------------------------------------------------------------------- */
void
PostgreSQL::initializeVariableList()
{
  int	nDims, dims[3];

  rateTableMap		rateTableMap;
  std::stringstream	groundLrtTable;
  std::vector<float>	noCals;

  nDims = 1;
  dims[0] = 1;


  /* 2 big loops here for raw and derived.  This is raw.
   */
  for (size_t i = 0; i < raw.size(); ++i)
  {
    RAWTBL	*rp = raw[i];

    if (isdigit(rp->name[0])) // Can't support vars starting with number.
    {
      fprintf(stderr,
	"psql.cc: PostGreSQL does not support variables starting with a numeric [%s]\n",
	rp->name);
      exit(1);
    }

    /* If PMS2D then add to the PMS2D table.
     */
    if ((rp->ProbeType & PROBE_PMS2D) && rp->Length > 1)
    {
      std::string name(&rp->name[1]);

      _sqlString.str("");
      _sqlString << "INSERT INTO PMS2D_list VALUES ('" << name
	<< "', '" << rp->SerialNumber << "');";
      _sqlString << "CREATE TABLE " << name
	<< " (" << TIME_VARIABLE << " time (3) PRIMARY KEY, nSlices int, particle int[]);";
      submitCommand(_sqlString.str());
    }

    /* If PMS1D then add to the PMS1D table.
     */
    if ((rp->ProbeType & PROBE_PMS1D) && rp->Length > 1)
    {
      std::string name(&rp->name[1]);

      _sqlString.str("");
      _sqlString << "INSERT INTO PMS1D_list VALUES ('" << name << "', '" <<
	rp->SerialNumber << "', '" << RATE_TABLE_PREFIX <<
	rp->SampleRate << '\'';

      GetPMS1DAttrsForSQL(rp, buffer);
      _sqlString << buffer;
      _sqlString << ");";
      submitCommand(_sqlString.str(), true);
    }

    if (rp->Length > 1)
    {
      nDims = 2;
      dims[1] = rp->Length;
#ifdef ZERO_BIN
      // Subtract 1, since we don't put 0th bin into SQL database.
      // See addVectorToAllStreams() & pms1d.c:GetPMS1DAttrsForSQL()
      // Remove the '-1' when we compensate for in FIRST_BIN/LAST_BIN.
      if ((rp->ProbeType & PROBE_PMS1D) || (rp->ProbeType & PROBE_PMS2D))
        dims[1] = rp->Length - 1;
#endif
    }
    else
      nDims = 1;

    addVariableToDataBase(rp, nDims, dims, noCals, MISSING_VALUE);

    /* Don't add/duplicate rate 1. Don't add vectors for the time being.
     */
    if (rp->SampleRate > 1 && rp->Length == 1)
      addVariableToTables(rateTableMap, groundLrtTable, rp, true);
    else
      addVariableToTables(rateTableMap, groundLrtTable, rp, false);
  }



  for (size_t i = 0; i < derived.size(); ++i)
  {
    DERTBL *dp = derived[i];

    if (isdigit(dp->name[0])) // Can't support vars starting with number.
    {
      fprintf(stderr,
	"psql.cc: PostGreSQL does not support variables starting with a numeric [%s]\n",
	dp->name);
      exit(1);
    }

    if (dp->Length > 1)
    {
      nDims = 2;
      dims[1] = dp->Length;
#ifdef ZERO_BIN
      // Subtract 1, since we don't put 0th bin into SQL database.
      // See addVectorToAllStreams() & pms1d.c:GetPMS1DAttrsForSQL()
      // Remove the '-1' when we compensate for in FIRST_BIN/LAST_BIN.
      if ((dp->ProbeType & PROBE_PMS1D) || (dp->ProbeType & PROBE_PMS2D))
        dims[1] = dp->Length - 1;
#endif
    }
    else
      nDims = 1;

    addVariableToDataBase(dp, nDims, dims, noCals, MISSING_VALUE);
    addVariableToTables(rateTableMap, groundLrtTable, dp, false);
  }

  _groundDBinitString << groundLrtTable.str();
  /* Send commands to create the "SampleRate*" tables.
   */
  createSampleRateTables(rateTableMap);

}	// END INITIALIZEVARIABLELIST

/* -------------------------------------------------------------------- */
int32_t
PostgreSQL::WriteSQLvolts(const std::string & timeStamp)
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

  /* Do not write srt during research flights.  The db VACUUM starts causing
   * delays after several hours of flight.  Avoiding the addition of sample
   * rate data helps keep the db smaller.
   */
  if (!flying)
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
PostgreSQL::Write2dSQL(RAWTBL *rp, int32_t time, int32_t msec, uint32_t *p, int nSlices)
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

  if (std::isnan(value) || std::isinf(value))
    sql << MISSING_VALUE;
  else
    sql << value;

}	// END ADDVALUE

/* -------------------------------------------------------------------- */
inline void
PostgreSQL::addValueToAllStreams(NR_TYPE value, bool addComma)
{
  static const char commaFormat[] = ",%e";
  static const char normalFormat[] = "%e";

  const char *format = commaFormat;
  char value_ascii[32];

  if (!addComma)
    format = normalFormat;

  if (std::isnan(value) || std::isinf(value))
    value = MISSING_VALUE;

  sprintf(value_ascii, format, value);

  _sqlString << value_ascii;

}	// END ADDVALUE

/* -------------------------------------------------------------------- */
inline void
PostgreSQL::addVectorToAllStreams(const NR_TYPE *value, size_t nValues)
{
  _sqlString << ",'{";

  size_t start = 0;

  for (size_t j = start; j < nValues; ++j)
  {
    if (j != start)
      addValueToAllStreams(value[j]);
    else
      addValueToAllStreams(value[j], false);
  }

  _sqlString << "}'";

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

  /* Names need to have standard case since column names in RAF_lrt
   * will be lower case dictated by PostGreSQL.
   */
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
PostgreSQL::addVariableToTables(rateTableMap &tableMap, std::stringstream &groundLrtTable, const var_base *var,
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
     * variables.  Build it in _groundDBinitString, initialize here.
     */
    if (i == 0 && var->Transmit) // Only do this for LRT table.
    {
      if (groundLrtTable.str().length() == 0)
        groundLrtTable << preamble.str();
      else
        groundLrtTable << ',';
    }

    if (tableMap[preamble.str()].length() > 0)
      tableMap[preamble.str()] += ',';

    std::string build = var->name;
    build += " FLOAT";
    if (var->Length > 1)    // array.
      build += "[]";

    tableMap[preamble.str()] += build;
    if (i == 0 && var->Transmit)
      groundLrtTable << build;
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

  _groundDBinitString << ");";

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
PostgreSQL::isSameFlight()
{
  flying = false;

  // Anything not starting with pre-ordained prefix is not the same flight.
  // FlightNumber="hanger" being the most obvious.
  if (cfg.FlightNumber().compare(0, 2, "rf") != 0 &&
      cfg.FlightNumber().compare(0, 2, "tf") != 0 &&
      cfg.FlightNumber().compare(0, 2, "ff") != 0)
    return false;

  flying = true;

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

  if (xmit)
    _groundDBinitString << command << '\n';

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
    str = target.substr(0, pos) + "''" + target.substr(pos + 1);
  else
    str = target;

  return str;
}

/* -------------------------------------------------------------------- */
static const std::string PGHOST = "acserver";
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
void
PostgreSQL::outputGroundDBInitPacket()
{
  char fName[MAXPATHLEN], *dir;

  if ((dir = getenv("XMIT_DIR")) == 0)
  {
    fprintf(stderr, "env XMIT_DIR undefined, fatal for '-x'\n");
    quit();
  }

  time_t t = time(0);
  char timeStamp[64];
  strftime(timeStamp, sizeof(timeStamp), ISO8601_T, gmtime(&t));
  sprintf(fName, "%s/%s_nimbus_start_%s.gz", dir, cfg.AircraftString().c_str(), timeStamp);

  gzFile gzfd = gzopen(fName, "wbx");
  gzwrite(gzfd, _groundDBinitString.str().c_str(), _groundDBinitString.str().length());
  gzclose(gzfd);

  //launch sendSQL script - will send init packet until checksums match.
  pid_t pid = fork();

  if (pid == 0)
  {
    const char * command = "/home/local/Systems/scripts/sendSQL";

    if ( execlp(command, command, (const char *)fName, (const char *)0) == -1 )
    {
      fprintf(stderr, "nimbus:psql.cc: Failed to execute '%s', errno = %d\n",
        command, errno);
      _exit(1);
    }
    _exit(0);
  }
}

/* -------------------------------------------------------------------- */
static const char feedCommand[] =
		"/home/local/raf/groundfeed/bin/Groundfeed.sh";
void
PostgreSQL::launchGroundFeed()
{
  if (cfg.GroundFeedType() == Config::UDP)
    return;  // Means we are using UDP broadcast instead.

  pid_t pid = fork();

  if (pid == 0)
  {
    execl(feedCommand, feedCommand, cfg.ProjectNumber().c_str(), (char*)0);
    fprintf(stderr, "nimbus: failed to exec %s\n", feedCommand);
    _exit(1);
  }
}

// END PSQL.CC
