/*
-------------------------------------------------------------------------
OBJECT NAME:	ACconfig.h

DESCRIPTION:	Header file for ACconfig's files.

COPYRIGHT:	University Corporation for Atmospheric Research, 1998-2004
-------------------------------------------------------------------------
*/

#ifndef ACCONFIG_H
#define ACCONFIG_H

#include <string>
#include <map>
#include <vector>

#define COMMENT		'#'


/* -------------------------------------------------------------------- */
namespace libraf {

/**
 * Read and parse an aircraft configuration file 'ACconfig'.  Generally
 * this file can be found in $PROJ_DIR/defaults/
 */
class ACconfig
{
public:
	ACconfig(std::string fName);

  static const std::string NO_KEY, NO_TAILNUM, NO_SERVER;

  /**
   * @returns a list/vector of tail number strings.
   */
  std::vector<std::string> GetTailNumberList()	{ return(_tailNumList); }

  /**
   * Locate a specific key for a specific aircraft/tail number.
   * @returns the value associated with the key or NO_KEY or NO_TAILNUMBER
   */
  std::string	GetParameter(std::string tailNumber, std::string target);

  /**
   * Given a computer hostname, locate the tail number associated with
   * it.
   * @returns string tail number or NO_SERVER if not located.
   */
  std::string	LocateMyTailNumber(std::string server);


private:
  std::vector<std::string>	_tailNumList;
  std::map<std::string, std::string>  _attrs;

};

}

#endif // END ACCONFIG.H
