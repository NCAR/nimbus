/*
-------------------------------------------------------------------------
OBJECT NAME:	psql.h

FULL NAME:	PostgreSQL output.

DESCRIPTION:	Produce RAF PostgreSQL flight file.

COPYRIGHT:	University Corporation for Atmospheric Research, 2004
-------------------------------------------------------------------------
*/

#ifndef PSQL_H
#define	PSQL_H

#include "nimbus.h"
#include "decode.h"

#include "UdpSocket.h"

#include <sstream>
#include <libpq-fe.h>


/* -------------------------------------------------------------------- */
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
  void	WriteSQL(const std::string timeStamp);

  /**
   * Generic function to submit a query/command to the database.
   * @param command is a valid PostgreSQL command.
   */
  void	submitCommand(const std::string command);

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

private:
  /**
   * Database connection pointer.
   */
  PGconn   *_conn;

  std::string	_sql2d_str;

//@{
  UdpSocket*	_brdcst;

  static const int RT_UDP_PORT;
//@}
  void	dropAllTables();
  void	createTables();
  void	initializeGlobalAttributes();
  void	initializeVariableList();

  /**
   * Low level add a variable with given params.
   */
  void	addVariableToDataBase(const std::string& name, const std::string& units,
	const std::string& longName, const int sampleRate, const int nDims,
	const int dims[], const float missingValue,
	const std::string& dataQuality);

  /**
   * Add a category variable-name pair to the categories table in the
   * database.  The category table is many to many.
   * @param varName is the variable name.
   * @param category and it's corresponding category.
   */
  void addCategory(std::string varName, std::string category);

  /**
   * Add a value to the stringstream-s, used for building UDP broadcast and
   * SQL strings.  If the value is Nan or Inf, then MISSING_VALUE is
   * substituted.
   * @param sql is the SQL stringstream to add the value.
   * @param udp is the UDP stringstream to add the value.
   * @param value is the value to add.
   * @param addComma is whether to add a comma to the stream.
   */
  void	addValue(std::stringstream& sqlStr, std::stringstream& brdStr, const NR_TYPE value, const bool addComma);

  /**
   * Add a vector of values to the stringstream-s, used for building UDP
   * broadcast and SQL strings.  If the value is Nan or Inf, then
   * MISSING_VALUE is substituted.
   * @param sql is the SQL stringstream to add the value.
   * @param udp is the UDP stringstream to add the value.
   * @param values is a pointer to the values to add.
   * @param nValues is the vector length.
   * @param addComma is whether to add a comma to the stream.
   */
  void	addVector(std::stringstream& sql, std::stringstream& udp, const NR_TYPE *values, int nValues, const bool addComma);

};	// END POSTGRESQL

#endif
