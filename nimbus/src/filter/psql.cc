/*
-------------------------------------------------------------------------
OBJECT NAME:    psql.cc (PostgreS)

FULL NAME:      PostgreSQL database routines.

COPYRIGHT:      University Corporation for Atmospheric Research, 2003-05
-------------------------------------------------------------------------
*/

#include "psql.h"
#include "transmit.h"
#include "ctape.h"
#include "vardb.h"

#include <algorithm>
#include <ctype.h>
#include <cmath>
#include <set>
#include <iomanip>

void GetPMS1DAttrsForSQL(RAWTBL *rp, char sql_buff[]);

const int PostgreSQL::RT_UDP_PORT = 2101;
const std::string PostgreSQL::GLOBAL_ATTR_TABLE = "Global_Attributes";
const std::string PostgreSQL::VARIABLE_LIST_TABLE = "Variable_List";
const std::string PostgreSQL::CATEGORIES_TABLE = "Categories";
const std::string PostgreSQL::LRT_TABLE = "RAF_LRT";
const std::string PostgreSQL::RATE_TABLE_PREFIX = "SampleRate";

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
    _ldm = new sqlTransmit();
  else
    _ldm = 0;

  dropAllTables();	// Remove existing tables, this is a reset.
  createTables();

  initializeGlobalAttributes();
  initializeVariableList();

  if (cfg.ProcessingMode() == Config::RealTime)
  {
    _brdcst = new UdpSocket(RT_UDP_PORT, "192.168.84.255");
    _brdcst->openSock(UDP_BROADCAST);
    submitCommand(
    "CREATE RULE update AS ON UPDATE TO global_attributes DO NOTIFY current", true);
  }

}	/* END INITSQL */

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
    std::string	temp;

    temp = "INSERT INTO global_attributes VALUES ('StartTime', '" + timeStamp + "')";
    submitCommand(temp, true);
    temp = "INSERT INTO global_attributes VALUES ('EndTime', '" + timeStamp + "')";
    submitCommand(temp, true);
  }


  /* The difference between SQL & UDP strings is the preamble and for SQL
   * the vectors must be encasulated with single quatations. e.g.
   * SQL = '{5, 6, 7, 8}'
   * UDP = {5, 6, 7, 8}
   */
  _sqlString.str("");
  _sqlString << "INSERT INTO " << LRT_TABLE << " VALUES ('" << timeStamp << "'";

  _broadcastString.str("");
  _broadcastString << "RAF-TS " << timeStamp << ' ';

  extern NR_TYPE	*AveragedData;

  /* Three loops again, analog, raw and derived.  This is analog.
   */
  for (size_t i = 0; i < sdi.size(); ++i)
    addValueToAllStreams(AveragedData[sdi[i]->LRstart], sdi[i]->Transmit);


  /* Three loops again, analog, raw and derived.  This is raw.
   */
  for (size_t i = 0; i < raw.size(); ++i)
  {
    if (raw[i]->Length > 1)	// PMS/vector data.
      addVectorToAllStreams(&AveragedData[raw[i]->LRstart], raw[i]->Length, raw[i]->Transmit);
    else
      addValueToAllStreams(AveragedData[raw[i]->LRstart], raw[i]->Transmit);
  }


  /* Three loops again, analog, raw and derived.  This is derived.
   */
  for (size_t i = 0; i < derived.size(); ++i)
  {
    if (derived[i]->Length > 1)
      addVectorToAllStreams(&AveragedData[derived[i]->LRstart], derived[i]->Length, derived[i]->Transmit);
    else
      addValueToAllStreams(AveragedData[derived[i]->LRstart], derived[i]->Transmit);
  }


  if (cfg.ProcessingMode() == Config::RealTime)
  {
    _broadcastString << '\n';
    _brdcst->writeSock(_broadcastString.str().c_str(), _broadcastString.str().length());
  }

  _sqlString << ");";
  submitCommand(_sqlString.str(), false);
  if (_ldm)
    _ldm->queueString(_transmitString.str());


  long usec = WriteSQLvolts(timeStamp);


  _sqlString.str("");
  _sqlString << "UPDATE global_attributes SET value='"
	<< timeStamp << '.' << usec << "' WHERE key='EndTime';";
  submitCommand(_sqlString.str(), true);


  if (++cntr % 3600 == 0)     // every hour
  {
fprintf(stderr, "Performing ANALYZE @ %s\n", timeStamp.c_str());

    _sqlString.str("");
    _sqlString << "ANALYZE " << LRT_TABLE << " (datetime)";

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
    tablesToDelete.insert(PQgetvalue(res, i, 0));

  PQclear(res);

  /* Add pms2d particle tables.
   */
  res = PQexec(_conn, "SELECT Name FROM PMS2D_list");

  for (int i = 0; i < PQntuples(res); ++i)
    tablesToDelete.insert(PQgetvalue(res, i, 0));

  PQclear(res);


  /* Loop & DROP.
   */
  std::set<std::string>::iterator it;
  for (it = tablesToDelete.begin(); it != tablesToDelete.end(); ++it)
  {
    std::string cmd("DROP TABLE ");
    cmd += it->c_str();

    submitCommand(cmd, true);
  }

  /*
   * Database seems to slow down after a number of runs without VACUUMing.
   *  It's not sufficient to just DROP all tables.
   */
  submitCommand("VACUUM FULL", true);

}	// END DROPTABLES

/* -------------------------------------------------------------------- */
void
PostgreSQL::createTables()
{
  submitCommand(
  "CREATE TABLE Variable_List (Name char(20) PRIMARY KEY, Units char(16), Uncalibrated_Units char(16), long_name char(80), SampleRateTable char(16), nDims int, dims int[], nCals int, poly_cals float[], missing_value float, data_quality char(16))", true);

  submitCommand(
  "CREATE TABLE Categories (variable char(20), category char(20))", true);

  /*
   * PMS tables.
   */
  submitCommand(
  "CREATE TABLE PMS1D_list (Name char(20), SerialNumber char(16), SampleRateTable char(16), FirstBin INT, LastBin INT, CellSizes FLOAT[])", true);
  submitCommand(
  "CREATE TABLE PMS2D_list (Name char(20), SerialNumber char(16))", true);

}	// END CREATETABLES

/* -------------------------------------------------------------------- */
void
PostgreSQL::initializeGlobalAttributes()
{
  submitCommand(
  "CREATE TABLE Global_Attributes (key char(20) PRIMARY KEY, value char(120))", true);

  /* Add Global Attributes/Flight Data.
   */
  submitCommand(
  "INSERT INTO global_attributes VALUES ('Source', 'NCAR Research Aviation Facility')", true);
  submitCommand(
  "INSERT INTO global_attributes VALUES ('Address', 'P.O. Box 3000, Boulder, CO 80307-3000')", true);
  submitCommand(
  "INSERT INTO global_attributes VALUES ('Phone', '(303) 497-1030')", true);

  char	*p, temp[100];
  extern char	dateProcessed[];	// From netcdf.c

  sprintf(temp, "INSERT INTO global_attributes VALUES ('ProjectName', '%s')", ProjectName);
  submitCommand(temp, true);

  GetAircraft(&p);
  sprintf(temp, "INSERT INTO global_attributes VALUES ('Platform', '%s')", p);
  submitCommand(temp, true);

  GetProjectNumber(&p);
  sprintf(temp, "INSERT INTO global_attributes VALUES ('ProjectNumber', '%s')", p);
  submitCommand(temp, true);

  GetFlightNumber(&p);
  sprintf(temp, "INSERT INTO global_attributes VALUES ('FlightNumber', '%s')", p);
  submitCommand(temp, true);

  sprintf(temp, "INSERT INTO global_attributes VALUES ('DateProcessed', '%s')", dateProcessed);
  submitCommand(temp, true);

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


  /* 3 big loops here for analog, raw and derived.  This is analog.
   */
  for (size_t i = 0; i < sdi.size(); ++i)
  {
    std::string alt_units("V");

    if (sdi[i]->type[0] == 'C')	// Pulse Counters.
      alt_units = "count";

    addVariableToDataBase(sdi[i]->name, VarDB_GetUnits(sdi[i]->name), alt_units,
	VarDB_GetTitle(sdi[i]->name), sdi[i]->SampleRate, nDims, dims,
	sdi[i]->cof, MISSING_VALUE, "Preliminary", sdi[i]->Transmit);

    addCategory(sdi[i]->name, "Analog");

    addVariableToTables(rateTableMap, sdi[i], true);
  }


  std::vector<float> noCals;

  /* 3 big loops here for analog, raw and derived.  This is raw.
   */
  for (size_t i = 0; i < raw.size(); ++i)
  {
    std::string        name;

    if (isdigit(raw[i]->name[0]))       // Can't support vars starting with number.
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
	<< "', '" << raw[i]->SerialNumber << "')";
      submitCommand(_sqlString.str());

      _sqlString.str("");
      _sqlString << "CREATE TABLE " << name
	<< " (datetime time (3) PRIMARY KEY, nSlices int, particle int[])";
      submitCommand(_sqlString.str());
    }

    /* If PMS1D then add to the PMS1D table.
     */
    if (raw[i]->Length > 1)
    {
      std::stringstream	temp;
      std::string name(&raw[i]->name[1]);

      _sqlString.str("");
      _sqlString << "INSERT INTO PMS1D_list VALUES ('" << name << "', '" <<
	raw[i]->SerialNumber << "', '" << RATE_TABLE_PREFIX <<
	raw[i]->SampleRate << '\'';

      GetPMS1DAttrsForSQL(raw[i], buffer);
      _sqlString << buffer;
      _sqlString << ')';
      submitCommand(_sqlString.str(), true);
    }

    if (raw[i]->Length > 1)
    {
      nDims = 2;
      dims[1] = raw[i]->Length;
    }
    else
      nDims = 1;

    addVariableToDataBase(raw[i]->name, VarDB_GetUnits(raw[i]->name), "",
	VarDB_GetTitle(raw[i]->name), raw[i]->SampleRate, nDims, dims,
	noCals, MISSING_VALUE, "Preliminary", raw[i]->Transmit);

    addCategory(raw[i]->name, "Raw");

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
      dims[1] = derived[i]->Length;
    }
    else
      nDims = 1;

    addVariableToDataBase(derived[i]->name, VarDB_GetUnits(derived[i]->name), "",
	VarDB_GetTitle(derived[i]->name), derived[i]->SampleRate, nDims, dims,
	noCals, MISSING_VALUE, "Preliminary", derived[i]->Transmit);

    addCategory(derived[i]->name, "Derived");

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

  extern NR_TYPE	*SampledData, *SRTvolts;

  _sqlString.str("");

  for (it = _ratesTables.begin(); it != _ratesTables.end(); ++it)
  {
    /* @todo should the LRT_TABLE be in the rateTableMap?
     */
    if (it->second == LRT_TABLE)
      continue;

    maxRate = std::max(maxRate, it->first);

    for (int i = 0; i < it->first; ++i)
    {
      _sqlString << "INSERT INTO " << it->second << " VALUES ('" << timeStamp
	<< "." << std::setfill('0') << std::setw(3) << 1000 / it->first * i << "'";

      for (size_t j = 0; j < sdi.size(); ++j)
        if (sdi[j]->SampleRate == (size_t)it->first)
          addValue(_sqlString, SRTvolts[sdi[j]->SRstart+i]);

      for (size_t j = 0; j < raw.size(); ++j)
        if (raw[j]->SampleRate == (size_t)it->first && raw[j]->Length == 1)
          addValue(_sqlString, SampledData[raw[j]->SRstart+i]);

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

}	/* END START2DSQL */

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

}	/* END WRITE2DSQL */

/* -------------------------------------------------------------------- */
void
PostgreSQL::Submit2dSQL()
{
  _sql2d_str << "COMMIT;";
  submitCommand(_sql2d_str.str());

}	/* END SUBMIT2DSQL */

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
  _broadcastString << value_ascii;
  if (xmit)
    _transmitString << value_ascii;

}	// END ADDVALUE

/* -------------------------------------------------------------------- */
inline void
PostgreSQL::addVectorToAllStreams(const NR_TYPE *value, int nValues, bool xmit)
{
  _sqlString << ",'{";
  _broadcastString << ",{";
  if (xmit)
    _transmitString << ",{";

  for (int j = 0; j < nValues; ++j)
  {
    if (j != 0)
      addValueToAllStreams(value[j], xmit);
    else
      addValueToAllStreams(value[j], xmit, false);
  }

  _sqlString << "}'";
  _broadcastString << '}';
  if (xmit)
    _transmitString << '}';

}	// END ADDVECTOR

/* -------------------------------------------------------------------- */
void
PostgreSQL::addVariableToDataBase(
		const std::string& name,
		const std::string& units,
		const std::string& uncaled_units,
		const std::string& longName,
		int sampleRate,
		size_t nDims,
		const int dims[],
		const std::vector<float>& cals,
		float missingValue,
		const std::string& dataQuality,
		bool beingTransmitted)
{
  std::stringstream   entry;

  entry << "INSERT INTO Variable_List VALUES ('" <<
	name		<< "', '" <<
	units		<< "', '" <<
	uncaled_units	<< "', '" <<
	longName	<< "', '";

  if (sampleRate > 0)
    entry << RATE_TABLE_PREFIX << sampleRate;

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

  entry << "}', '" << missingValue << "', '" << dataQuality << "')";

  submitCommand(entry.str(), beingTransmitted);
  if (beingTransmitted)
    addCategory(name, VarDB_GetCategoryName(name.c_str()));

}	// END ADDVARIABLETODATABASE

/* -------------------------------------------------------------------- */
void
PostgreSQL::addCategory(std::string varName, std::string category)
{
  if (category.length() == 0)
    return;

  /* @todo Must check  the category for a ' (single quote), appears
   * we have one with one.
   */
  std::string entry = "INSERT INTO categories VALUES ('" + varName +
			"', '" + category + "')";

  submitCommand(entry, true);

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

    preamble << "CREATE TABLE " << rt.str() << " (datetime timestamp ";

    if (rates[i] > 1)
      preamble << "(3) ";

    preamble << "PRIMARY KEY, ";

    /* We don't have a map entry for the ground transmit subset of
     * variables.  Build it in _transmitString, initialize here.
     */
    if (var->Transmit && _transmitString.str().length() == 0)
      _transmitString << preamble;
    else
      _transmitString << ',';

    if (tableMap[preamble.str()].length() > 0)
      tableMap[preamble.str()] += ',';

    std::string build = var->name;
    build += " FLOAT";
    if (var->Length > 1)	// array.
      build += "[]";

    tableMap[preamble.str()] += build;
    if (var->Transmit)
      _transmitString << build;
  }

}	// END ADDVARIABLETOTABLES

/* -------------------------------------------------------------------- */
void
PostgreSQL::createSampleRateTables(const rateTableMap &tableMap)
{
  rateTableMap::const_iterator it;

  for (it = tableMap.begin(); it != tableMap.end(); ++it)
  {
    std::stringstream  cmd;
    cmd << it->first << it->second << ')';

    submitCommand(cmd.str());
  }

  // Send the subset'ed RAF_LRT table creation to ground.
  if (_ldm)
    _ldm->sendString(_transmitString.str());
    
}	// END CREATESAMPLERATETABLES

/* -------------------------------------------------------------------- */
void
PostgreSQL::submitCommand(const std::string command, bool xmit)
{
  if (_conn == 0)
    return;

  PGresult* res;

  while ( (res = PQgetResult(_conn)) )
    PQclear(res);

  PQsendQuery(_conn, command.c_str());
  fprintf(stderr, "%s", PQerrorMessage(_conn));

  if (_ldm && xmit)
    _ldm->sendString(command);

  while ( (res = PQgetResult(_conn)) )
    PQclear(res);

}	/* END SUBMITCOMMAND */

/* -------------------------------------------------------------------- */
void PostgreSQL::closeSQL()
{
  if (_conn)
    PQfinish(_conn);

}	// END CLOSESQL

// END PSQL.CC
