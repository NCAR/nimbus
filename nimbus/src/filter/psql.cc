/*
-------------------------------------------------------------------------
OBJECT NAME:    psql.c (PostgreS)

FULL NAME:      PostgreSQL database routines.

ENTRY POINTS:   InitSQL()
		WriteSQL()
		Write2dSQL()

STATIC FNS:     none

DESCRIPTION:

REFERENCES:     none

REFERENCED BY:  rtloop.c

NOTES:		

COPYRIGHT:      University Corporation for Atmospheric Research, 2003
-------------------------------------------------------------------------
*/

#include "psql.h"
#include "ctape.h"
#include "vardb.h"

#include <ctype.h>


void GetPMS1DAttrsForSQL(RAWTBL *rp, char sql_buff[]);

const int PostgreSQL::RT_UDP_PORT = 2101;


/* -------------------------------------------------------------------- */
PostgreSQL::PostgreSQL(std::string specifier)
{
  _conn = PQconnectdb("");

  /* check to see that the backend connection was successfully made
   */
  if (PQstatus(_conn) == CONNECTION_BAD)
  {
    fprintf(stderr, "Connection to database failed, check environment variables.\n");
    fprintf(stderr, "%s", PQerrorMessage(_conn));
    PQfinish(_conn);
    _conn = NULL;
    return;
  }

  _brdcst = new UdpSocket(RT_UDP_PORT, "128.117.84.255");

  if (Mode == REALTIME)
    _brdcst->openSock(UDP_BROADCAST);

  PQsetnonblocking(_conn, true);

  dropAllTables();
  createTables();

  initializeGlobalAttributes();
  initializeVariableList();

  if (Mode == REALTIME)
  {
    submitCommand(
    "CREATE RULE update AS ON UPDATE TO global_attributes DO NOTIFY current");
  }

  // Clear out an residual shit after restarting the DB.  This causes long
  // delay at start-up......
//  submitCommand("VACUUM FULL"));

}	/* END INITSQL */

/* -------------------------------------------------------------------- */
void PostgreSQL::Start2dSQL()
{
  _sql2d_str = "BEGIN;";

}	/* END START2DSQL */

/* -------------------------------------------------------------------- */
void PostgreSQL::Write2dSQL(RAWTBL *rp, long time, long msec, ulong *p, int nSlices)
{
  int	i;
  char	name[16], temp[128];

  if (nSlices < 2)
    return;

  strcpy(name, rp->name);
  name[strlen(name)-1] = '\0';

  sprintf(temp, "INSERT INTO %s VALUES ('%02d:%02d:%02d', %u, %d, '{%lu", name, time/3600, (time%3600)/60, time%60, msec, nSlices, p[0]);
  _sql2d_str += temp;

  for (i = 1; i < nSlices; ++i)
    {
    sprintf(temp, ", %d", p[i]);
    _sql2d_str += temp;
    }

  _sql2d_str += "}');";

}	/* END WRITE2DSQL */

/* -------------------------------------------------------------------- */
void PostgreSQL::Submit2dSQL()
{
  _sql2d_str += "COMMIT;";
  submitCommand(_sql2d_str);

}	/* END SUBMIT2DSQL */

/* -------------------------------------------------------------------- */
void PostgreSQL::WriteSQL(const std::string timeStamp)
{
  int	i, j, len;
  std::string	temp;

  static int	cntr = 0;

//printf("WriteSQL\n");

  /* Set global attributes start and end time to the timeStamp of the
   * first record to arrive.
   */
  if (cntr == 0)
  {
    temp = "INSERT INTO global_attributes VALUES ('StartTime', '" + timeStamp + "')";
    submitCommand(temp);

    temp = "INSERT INTO global_attributes VALUES ('EndTime', '" + timeStamp + "')";
    submitCommand(temp);
  }


  /* The difference between SQL & UDP strings is the preamble and for SQL
   * the vectors must be encasulated with single quatations.
   * SQL = '{5, 6, 7, 8}'
   * UDP = {5, 6, 7, 8}
   */
  std::stringstream	sqlStr;
  sqlStr << "INSERT INTO RAF_1hz VALUES ('" << timeStamp << "', ";

  std::stringstream	brdStr;
  brdStr << "RAF-TS " << timeStamp << " ";

  extern NR_TYPE	*AveragedData;

  /* Three loops again, analog, raw and derived.  This is analog.
   */
  for (i = 0; i < nsdi; ++i)
    addValue(sqlStr, brdStr, AveragedData[sdi[i]->LRstart], true);


  /* Three loops again, analog, raw and derived.  This is raw.
   */
  for (i = 0; i < nraw; ++i)
  {
    if (raw[i]->Length > 1)	// PMS/vector data.
      addVector(sqlStr, brdStr, &AveragedData[raw[i]->LRstart], raw[i]->Length, true);
    else
      addValue(sqlStr, brdStr, AveragedData[raw[i]->LRstart], true);
  }


  /* Three loops again, analog, raw and derived.  This is derived.
   */
  for (i = 0; i < nderive; ++i)
  {
    bool addComma = (i == nderive-1) ? false : true;

    if (derived[i]->Length > 1)
      addVector(sqlStr, brdStr, &AveragedData[derived[i]->LRstart], derived[i]->Length, addComma);
    else
      addValue(sqlStr, brdStr, AveragedData[derived[i]->LRstart], addComma);
  }



  sqlStr << "); UPDATE global_attributes SET value='";
  sqlStr << timeStamp;
  sqlStr << "' WHERE key='EndTime';";

  submitCommand(sqlStr.str());

  if (Mode == REALTIME)
  {
    brdStr << "\n";
    _brdcst->writeSock(brdStr.str().c_str(), brdStr.str().size());
  }

//  if (++cntr < 10900 && cntr % 3600 == 0)     // every hour, 1st 3 hours
  if (++cntr % 3600 == 0)     // every hour
  {
fprintf(stderr, "Performing ANALYZE @ %s\n", timeStamp.c_str());
    submitCommand("ANALYZE raf_1hz (datetime)");
  }

}	/* END WRITESQL */

/* -------------------------------------------------------------------- */
void PostgreSQL::submitCommand(const std::string command)
{
//printf("submitCommand\n");

  if (_conn == NULL)
    return;

  PGresult* res;

  while ( (res = PQgetResult(_conn)) )
    PQclear(res);

  PQsendQuery(_conn, command.c_str());
  fprintf(stderr, "%s", PQerrorMessage(_conn));

  while ( (res = PQgetResult(_conn)) )
    PQclear(res);

}	/* END SUBMITCOMMAND */

/* -------------------------------------------------------------------- */
void PostgreSQL::dropAllTables()
{
  submitCommand("DROP TABLE Global_Attributes");
  submitCommand("DROP TABLE Variable_List");
  submitCommand("DROP TABLE Categories");
  submitCommand("DROP TABLE PMS1D_list");
  submitCommand("DROP TABLE PMS2D_list");
  submitCommand("DROP TABLE RAF_1hz");

}	// END DROPTABLES

/* -------------------------------------------------------------------- */
void PostgreSQL::createTables()
{
  submitCommand(
  "CREATE TABLE Variable_List (Name char(20) PRIMARY KEY, Units char(16), long_name char(80), SampleRateTable char(16), nDims int, dims int[], missing_value float, data_quality char(16))");

  submitCommand(
  "CREATE TABLE Categories (variable char(20), category char(20))");

  /*
   * PMS tables.
   */
  submitCommand(
  "CREATE TABLE PMS1D_list (Name char(20), SerialNumber char(16), SampleRateTable char(16), FirstBin INT, LastBin INT, CellSizes FLOAT[])");
  submitCommand(
  "CREATE TABLE PMS2D_list (Name char(20), SerialNumber char(16))");

}	// END CREATETABLES

/* -------------------------------------------------------------------- */
void PostgreSQL::initializeGlobalAttributes()
{
  submitCommand(
  "CREATE TABLE Global_Attributes (key char(20) PRIMARY KEY, value char(120))");

  /* Add Global Attributes/Flight Data.
   */
  submitCommand(
  "INSERT INTO global_attributes VALUES ('Source', 'NCAR Research Aviation Facility')");
  submitCommand(
  "INSERT INTO global_attributes VALUES ('Address', 'P.O. Box 3000, Boulder, CO 80307-3000')");
  submitCommand(
  "INSERT INTO global_attributes VALUES ('Phone', '(303) 497-1030')");

  char	*p, temp[100];
  extern char	dateProcessed[];	// From netcdf.c

  GetAircraft(&p);
  sprintf(temp, "INSERT INTO global_attributes VALUES ('ProjectName', '%s')", p);
  PQclear(PQexec(_conn, temp));
  GetProjectNumber(&p);
  sprintf(temp, "INSERT INTO global_attributes VALUES ('ProjectNumber', '%s')", p);
  PQclear(PQexec(_conn, temp));
  GetFlightNumber(&p);
  sprintf(temp, "INSERT INTO global_attributes VALUES ('FlightNumber', '%s')", p);
  PQclear(PQexec(_conn, temp));
  sprintf(temp, "INSERT INTO global_attributes VALUES ('DateProcessed', '%s')", dateProcessed);
  submitCommand(temp);

}	// END INTIALIZEGLOBALATTRIBUTES

/* -------------------------------------------------------------------- */
void PostgreSQL::initializeVariableList()
{
  int	nVars = 0, nDims, dims[3];

  std::stringstream	sqlStr;

printf("InitializeVariableList\n");

  nDims = 1;
  dims[0] = 1;

  sqlStr << "CREATE TABLE RAF_1hz (datetime timestamp PRIMARY KEY, ";

  /* 3 big loops here for analog, raw and derived.  This is analog.
   */
  for (int i = 0; i < nsdi; ++i)
    {
    sqlStr << sdi[i]->name << " FLOAT, ";
    ++nVars;

    addVariableToDataBase(sdi[i]->name, VarDB_GetUnits(sdi[i]->name),
	VarDB_GetTitle(sdi[i]->name), sdi[i]->SampleRate, nDims, dims,
	MISSING_VALUE, "Preliminary");

    addCategory(sdi[i]->name, "Analog");
    addCategory(sdi[i]->name, VarDB_GetCategoryName(sdi[i]->name));
    }


  /* 3 big loops here for analog, raw and derived.  This is raw.
   */
  for (int i = 0; i < nraw; ++i)
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
      name = name.substr(0, name.size()-1);

      std::string	temp;
      temp = "INSERT INTO PMS2D_list VALUES ('" + name + "', '" +
		raw[i]->SerialNumber + "')";
      submitCommand(temp);

      temp = "DROP TABLE " + name;
      submitCommand(temp);

      temp = "CREATE TABLE " + name +
		" (datetime time, msec int, nSlices int, particle int[])";
      submitCommand(temp);
    }

    /* If PMS1D then add to the PMS1D table.
     */
    if (raw[i]->Length > 1)
    {
      std::stringstream	temp;
      std::string name(&raw[i]->name[1]);

      temp << "INSERT INTO PMS1D_list VALUES ('" << name << "', '" <<
	raw[i]->SerialNumber << "', 'RAF_" << raw[i]->SampleRate << "hz'";

      GetPMS1DAttrsForSQL(raw[i], buffer);
      temp << buffer;
      temp << ")";
      submitCommand(temp.str());
    }

    sqlStr << name;

    if (raw[i]->Length > 1)
    {
      nDims = 2;
      dims[1] = raw[i]->Length;
      sqlStr << " FLOAT[], ";
    }
    else
    {
      nDims = 1;
      sqlStr << " FLOAT, ";
    }

    ++nVars;

    addVariableToDataBase(raw[i]->name, VarDB_GetUnits(raw[i]->name),
	VarDB_GetTitle(raw[i]->name), raw[i]->SampleRate, nDims, dims,
	MISSING_VALUE, "Preliminary");

    addCategory(raw[i]->name, "Raw");
    addCategory(raw[i]->name, VarDB_GetCategoryName(raw[i]->name));
  }


  /* 3 big loops here for analog, raw and derived.  This is derived.
   */
  for (int i = 0; i < nderive; ++i)
  {
    sqlStr << derived[i]->name;

    if (derived[i]->Length > 1)
    {
      nDims = 2;
      dims[1] = derived[i]->Length;
      sqlStr << " FLOAT[]";
    }
    else
    {
      nDims = 1;
      sqlStr << " FLOAT";
    }

    if (i != nderive-1)
      sqlStr << ", ";
    ++nVars;

    addVariableToDataBase(derived[i]->name, VarDB_GetUnits(derived[i]->name),
	VarDB_GetTitle(derived[i]->name), derived[i]->OutputRate, nDims, dims,
	MISSING_VALUE, "Preliminary");

    addCategory(derived[i]->name, "Derived");
  }

  /* Send command to create the "RAF_1hz" table.
   */
  sqlStr << ")";
  submitCommand(sqlStr.str());

printf("nVars = %d\n", nVars);

}	// END INITIALIZEVARIABLELIST

/* -------------------------------------------------------------------- */
void PostgreSQL::addVariableToDataBase(
		const std::string& name,
		const std::string& units,
		const std::string& longName,
		const int sampleRate,
		const int nDims,
		const int dims[],
		const float missingValue,
		const std::string& dataQuality)
{
  std::stringstream   entry;

  entry << "INSERT INTO Variable_List VALUES ('" <<
	name		<< "', '" <<
	units		<< "', '" <<
	longName	<< "', '" <<
	"RAF_" << sampleRate << "hz', '" <<
	nDims		<< "', '{";

  for (int i = 0; i < nDims; ++i)
  {
    if (i > 0)
      entry << ",";

    entry << dims[i];
  }

  entry << "}', '" <<
	missingValue	<< "', '" <<
	dataQuality	<< "')";

  submitCommand(entry.str());
  addCategory(name, VarDB_GetCategoryName(name.c_str()));

}	// END ADDVARIABLETODATABASE

/* -------------------------------------------------------------------- */
void PostgreSQL::addValue(std::stringstream& sql, std::stringstream& udp, const NR_TYPE value, const bool addComma)
{
  if (isnan(value) || isinf(value))
  {
    sql << MISSING_VALUE;
    udp << MISSING_VALUE;
  }
  else
  {
    sql << value;
    udp << value;
  }

  if (addComma)
  {
    sql << ",";
    udp << ",";
  }

}	// END ADDVALUE

/* -------------------------------------------------------------------- */
void PostgreSQL::addVector(std::stringstream& sql, std::stringstream& udp, const NR_TYPE *value, const int nValues, const bool addComma)
{
  sql << "'{";
  udp << "{";

  for (int j = 0; j < nValues; ++j)
  {
    if (j != nValues-1)
      addValue(sql, udp, value[j], true);
    else
      addValue(sql, udp, value[j], false);
  }

  sql << "}'";
  udp << "}";

  if (addComma)
  {
    sql << ",";
    udp << ",";
  }

}	// END ADDVECTOR

/* -------------------------------------------------------------------- */
void PostgreSQL::addCategory(std::string varName, std::string category)
{
  if (category.length() == 0)
    return;

  std::string entry = "INSERT INTO categories VALUES ('" + varName +
			"', '" + category + "')";

  submitCommand(entry);

}	// END ADDCATEGORY

/* -------------------------------------------------------------------- */
void PostgreSQL::closeSQL()
{
  if (_conn)
    PQfinish(_conn);

}	// END CLOSESQL

// END PSQL.CC
