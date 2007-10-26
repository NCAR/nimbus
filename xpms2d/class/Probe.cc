/*
-------------------------------------------------------------------------
OBJECT NAME:	Probe.cc

FULL NAME:	Probe Class

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1997
-------------------------------------------------------------------------
*/

#include "Probe.h"


/* -------------------------------------------------------------------- */
Probe::Probe(const char name[], int recSize)
{
  _name.push_back(name[0]);
  _name.push_back(name[1]);
  _name.push_back('\0');
  strcpy(_code, _name.c_str());

  _lrLen = recSize;
  _lrPpr = 1;
  _displayed = false;

  if (_name[0] == 'C')
    _resolution = 25;

  if (_name[0] == 'P')
    _resolution = 200;

printf("Probe:: %s, resolution = %d\n", _name.c_str(), _resolution);
}

/* -------------------------------------------------------------------- */
Probe::Probe(Header * hdr, const Pms2 * p, int cnt)
{
  // Extract stuff from Header.
  _name = hdr->VariableName(p);
  _name += "_";
  _name += hdr->AircraftLocation(p);
  _serialNumber = hdr->SerialNumber(p);

  _code[0] = _name[3]; _code[1] = cnt + '0'; _code[2] = '\0';

  _lrLen = hdr->lrLength(p);
  _lrPpr = hdr->lrPpr(p);
  _resolution = hdr->Resolution(p);
  _displayed = false;

printf("Probe:: %s - %s\n", _name.c_str(), _code);
}

/* END PROBE.CC */
