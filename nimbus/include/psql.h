/*
-------------------------------------------------------------------------
OBJECT NAME:	psql.h

FULL NAME:	PostgreSQL output.

DESCRIPTION:	Produce RAF PostgreSQL flight file.

COPYRIGHT:	University Corporation for Atmospheric Research, 2004-05
-------------------------------------------------------------------------
*/

#ifndef PSQL_H
#define	PSQL_H

#include "nimbus.h"
#include "decode.h"

#include "UdpSocket.h"

#include <map>
#include <sstream>
#include <libpq-fe.h>


/* -------------------------------------------------------------------- */
typedef std::map<std::string, std::string> rateTableMap;
typedef std::map<int, std::string> rateTableList;

class sqlTransmit;

class PostgreSQL
{
public:
  PostgreSQL(std::string specifier);
  ~PostgreSQL();

  /**
   * @returns whether database was successfully initialized.
   */
  bool	isValid() const { return(_conn ? true : false); }

//@{
  /** 
   * Write LowRate (LRT) data into the database.
   */
  void	WriteSQL(const std::string & timeStamp);
  /** 
   * Write SampleRate (SRT) data into the database.
   * @returns usec of last sample written.
   */
  long	WriteSQLvolts(const std::string & timeStamp);

  /**
   * Generic function to submit a query/command to the database.
   * @param command is a valid PostgreSQL command.
   * @param xmitToGround is whether this command should be transmitted
   * to the ground in addition to being executed locally.
   */
  void	submitCommand(const std::string command, bool xmitToGround = false);

  /**
   * Close the connection to the database.
   */
  void	closeSQL();
//@}
//@{
  /**
   * Start SQL block command.  For performance reasons, send all the
   * particle data in a block to the database.
   */
  void	Start2dSQL();

  /**
   * Add 2D particle to the record string.  This does not actually
   * submit the data to the database.
   * @param rp variable table pointer.
   * @param time timestamp for the data.
   * @param msec is  millisecond of this particle.
   * @param p is the pointer to particle slices.
   * @param nSlices is how many slices make up this particle.
   * @see Submit2dSQL()
   */
  void	Write2dSQL(RAWTBL *rp, long time, long msec, ulong *p, int nSlices);

  /**
   * End SQL block command and send.  For performance reasons, send
   * all the particle data in a block to the database.
   */
  void	Submit2dSQL();
//@}

  const std::stringstream&
  GroundDBInitString() const
    { return _groundDBinitString; }

protected:
  /**
   * Database connection pointer.
   */
  PGconn *_conn;

  sqlTransmit *_ldm;

  std::stringstream _sqlString;

  /// Ground transimission sub-set of _sqlString.
  std::stringstream _groundDBinitString;

  std::stringstream _sql2d_str;

  /**
   * List of data rate tables (e.g. RAF_1sps, RAF_10sps, RAF_25sps)
   * created for this database.
   */
  rateTableList	_ratesTables;

//@{
  UdpSocket* _brdcst;

  static const int RT_UDP_PORT;

//@}
  static const std::string TIME_VARIABLE;
  static const std::string GLOBAL_ATTR_TABLE;
  static const std::string VARIABLE_LIST_TABLE;
  static const std::string CATEGORIES_TABLE;
  static const std::string LRT_TABLE;
  static const std::string RATE_TABLE_PREFIX;

  void dropAllTables();
  void createTables();
  void initializeGlobalAttributes();
  void initializeVariableList();

  std::string
  getGlobalAttribute(const char key[]) const;

  /**
   * Determine if this is the same flight as the previous instance of nimbus.
   * Desired real-time operation to determine whether nimbus has been
   * restarted therefore database should not be 'dropped' and re-created.
   */
  bool isSameFlight() const;

  /**
   * Low level add a variable with given params.
   */
  void
  addVariableToDataBase(const var_base* var, 
	size_t nDims, const int dims[],
	const std::vector<float>& cals, float missingValue);

  /**
   * Add a category variable-name pair to the categories table in the
   * database.  The category table is many to many.
   * @param entry target string to add the SQL command to.
   * @param var is the variable whose categories to create.
   */
  void
  addCategory(std::stringstream& entry, const var_base * var) const;

  /**
   * Add a varible to the list for it's Sample Rate group.  By
   * default this also creates the map of Sample Rate groups.
   * After the list is complete, iterate over the map and execute
   * the SQL commands that were created.
   * @param tableMap is the map being built.  A 'key' will be generated
   * based on the varibles sample rate, then the variable name will be
   * appended to that 'value'.
   * @param var is the variable to add.
   * @see createSampleRateTables()
   * @param addToSRTtable whether to add this to the SampleRate tables in
   * the DB.
   */
  void
  addVariableToTables(rateTableMap &tableMap, const var_base *var, bool addToSRTtable);


  /**
   * Execute the SQL commands from the map generated by the
   * addToSampleRateList() method.
   * @see addVariableToTables()
   */
  void
  createSampleRateTables(const rateTableMap &tableMap);


  /**
   * Add a value to the stringstream-s, used for building UDP broadcast and
   * SQL strings.  If the value is Nan or Inf, then MISSING_VALUE is
   * substituted.
   * @param value is the value to add.
   * @param addComma is whether to prepend a comma to the stream.
   */
  void
  addValueToAllStreams(NR_TYPE value, bool addComma = true);
  void
  addValue(std::stringstream& sqlStr, NR_TYPE value, bool addComma = true);

  /**
   * Add a vector of values to the stringstream-s, used for building UDP
   * broadcast and SQL strings.  If the value is Nan or Inf, then
   * MISSING_VALUE is substituted.
   * @param values is a pointer to the values to add.
   * @param nValues is the vector length.
   */
  void
  addVectorToAllStreams(const NR_TYPE *values, size_t nValues);

  /**
   * Remove any trailing spaces from the string @p target.
   */
  void
  remove_trailing_spaces(std::string & target) const;

  /**
   * Escape any characters requiring it before sending them to the
   * DataBase.  Specifically the single quote.
   * @returns input string if no characters are escaped else an escaped string.
   */
  std::string
  escape_string(const std::string & target) const;

  /**
   * Build the string to update the EndTime in the Database.  This
   * string, once submitted,  triggers a NOTIFY in the Database for
   * clients that new data is available.
   */
  std::string
  updateEndTimeString(const std::string & timeStamp, long usec) const;

  // Output ground database start packet to a file.
  void
  outputGroundDBInitPacket();

  // Launch script/program which feeds data to the ground (raf/groundfeed
  // code).  Currently unused.  Was called at end of ctor.
  void
  launchGroundFeed();

};	// END PGSQL.H

#endif
