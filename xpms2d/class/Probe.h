/*
-------------------------------------------------------------------------
OBJECT NAME:	Probe.h

FULL NAME:	Probe Information

COPYRIGHT:	University Corporation for Atmospheric Research, 1997
-------------------------------------------------------------------------
*/

#ifndef PROBE_H
#define PROBE_H

#include <define.h>

#include <raf/hdrAPI.h>

/* -------------------------------------------------------------------- */
class Probe {

public:
  Probe(Header *hdr, const Pms2 *p, int cnt);
  Probe(const char hdr[], int recSize);

  std::string Name() const	{ return _name; }
  std::string SerialNum() const	{ return _serialNumber; }

  const char * Code() const		{ return _code; }
  /// @returns Logical-record length.  Exabyte hangover.
  long lrLen() const		{ return _lrLen; }
  /// @returns Logical-records per physical record.  Exabyte hangover.
  int lrPpr() const		{ return _lrPpr; }

  /// @returns Probe resolution per diode.
  int Resolution() const	{ return _resolution; }

  bool Display() const		{ return _displayed; }
  void setDisplay(bool b)	{ _displayed = b; }

private:
  std::string	_name;
  std::string	_serialNumber;

  char	_code[4];
  long	_lrLen;
  int	_lrPpr;
  int	_resolution;

  bool	_displayed;

};	/* END PROBE.H */

#endif
