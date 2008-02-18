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
Probe::Probe(const char xml_entry[], int recSize)
{
  _lrLen = recSize;
  _lrPpr = 1;
  _displayed = false;

  std::string id = getAttribute(xml_entry, "id");
  strcpy(_code, id.c_str());

  _name = getAttribute(xml_entry, "type");
  _name += getAttribute(xml_entry, "suffix");

  _resolution = atoi(getAttribute(xml_entry, "resolution").c_str());

printf("Probe:: id=%s, name=%s, resolution=%d\n", _code, _name.c_str(), _resolution);
}

/* -------------------------------------------------------------------- */
Probe::Probe(const char name[])
{
  _name.push_back(name[0]);
  _name.push_back(name[1]);
  _name.push_back('\0');
  strcpy(_code, _name.c_str());

  _lrLen = 4116;
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

/* -------------------------------------------------------------------- */
std::string Probe::getAttribute(const char s[], const char target[]) const
{
  char *start, *end;
  start = strstr(s, target);
  start = strchr(start, '\"') + 1;
  end = strchr(start, '\"') - 1;

  int len = end-start+1;
  char output[1024];
  memcpy(output, start, len);
  output[len] = '\0';

  std::string result(output);
  return result;
}

// END PROBE.CC
