/*
-------------------------------------------------------------------------
OBJECT NAME:	probe.c

FULL NAME:	Probe List

ENTRY POINTS:	AddProbeToList()
		ResetProbeList()
		GetProbeList()
		GetProbeType()

STATIC FNS:	none

DESCRIPTION:	Routines for the Toggle Probe Menu item.

REFERENCES:	none

REFERENCED BY:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1995-2005
-------------------------------------------------------------------------
*/

#include "nimbus.h"

#include <map>

std::map<std::string, unsigned long> probeMap;


/* -------------------------------------------------------------------- */
void AddProbeToList(char name[], unsigned long type)
{
  probeMap[name] = type;

}	/* END ADDPROBETOLIST */

/* -------------------------------------------------------------------- */
void ResetProbeList()
{
  probeMap.clear();

}	/* END RESETPROBELIST */

/* -------------------------------------------------------------------- */
std::vector<std::string> GetProbeList()
{
  std::vector<std::string> list;
  std::map<std::string, unsigned long>::const_iterator it;

  for (it = probeMap.begin(); it != probeMap.end(); ++it)
    list.push_back(it->first);

  return(list);

}	/* END GETPROBELIST */

/* -------------------------------------------------------------------- */
unsigned long GetProbeType(std::string& name)
{
  std::map<std::string, unsigned long>::const_iterator it;

  it = probeMap.find(name);

  if (it == probeMap.end())
    return(0);
  else
    return(it->second);

}	/* END GETPROBETYPE */

/* END PROBE.C */
