/*
-------------------------------------------------------------------------
OBJECT NAME:	PMSspex.h

DESCRIPTION:	Header file for PMSspec's files.

COPYRIGHT:	University Corporation for Atmospheric Research, 1998
-------------------------------------------------------------------------
*/

#ifndef PMSSPEX_H
#define PMSSPEX_H

#ifndef NULL
#define NULL		(0)
#endif

#define COMMENT		'#'


/* -------------------------------------------------------------------- */
/**
 * Parse a PMSspecs text file.  A PMSspecs file is a key, value paired
 * configuration file.  A file contains key, value pairs for many probes,
 * They are therefore blocked out by START/END keywords where the start is
 * also followed by the probes serial number.
 */
class PMSspex
{
public:
	PMSspex(const char fName[]);
	~PMSspex();

  /**
   * Given a probe serial number and a key name, return the value.
   */
  char 	*GetParameter(const char serialNumber[],  const char parameter[]);
  /**
   * Return list of serial numbers that were found in the file.
   */
  int 	GetSerialNumList(char *list[]);


private:
  char	**PMSfile;

};

#endif

/* END PMSSPEX.H */
