/*
-------------------------------------------------------------------------
OBJECT NAME:	probe.c

FULL NAME:	Probe List

DESCRIPTION:	Routines for the Toggle Probe Menu item.

COPYRIGHT:	University Corporation for Atmospheric Research, 1995-2010
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "gui.h"


/* These probes have a known/fixed suffix.  We will be able to search and
 * use this list directly.
 */
static const struct probelist knownProbes[] = {
	{ "IRIG Hskp",	"IRIG" },
	{ "A2D Temps",	"A2DTEMP" },
	{ "Second IRS",	"_IRS2" },
	{ "VCSEL",	"_VXL" },
	{ "ADC",	"_A" },
	{ "Avionics GPS", "_G" },
	{ "Water CN",	"_WCN" },
	{ "Gust Pod",	"_GP" },
	{ "Garmin",	"_GMN" },
	{ NULL,		NULL } };

/* The PMS probes will have unique suffixes (locations).  We will search
 * based on the second string below, and replace that with the suffix of
 * the discovered parameter.  We will add the suffix to the label before
 * pushing it on the vector.
 */
static const struct probelist pmsList[] = {
	{ "CDP",	"ACDP" },
	{ "S100",	"AS100" },
	{ "S200",	"AS200" },
	{ "S300",	"AS300" },
	{ "UHSAS",	"AUHSAS" },
	{ "Fast2DC",	"A1DC" },
	{ "2D-P",	"A1DP" },
	{ NULL,		NULL } };

std::vector<struct probelist> probeList;


/* -------------------------------------------------------------------- */
void GenerateProbeListADS3()
{
  for (size_t i = 0; knownProbes[i].label; ++i) {
    for (size_t j = 0; j < raw.size(); ++j) {
      if (strstr(raw[j]->name, knownProbes[i].suffix)) {
        probeList.push_back(knownProbes[i]);
        break;	// Don't keep looking for this suffix.
      }
    }
  }

  for (size_t i = 0; pmsList[i].label; ++i) {
    for (size_t j = 0; j < raw.size(); ++j) {
      if (strstr(raw[j]->name, pmsList[i].suffix)) {
        char *p = strchr(raw[j]->name, '_');

        if (p) {
          struct probelist t;
          t.label = new char [strlen(pmsList[i].label)+strlen(p)+1];
          strcpy(t.label, pmsList[i].label);
          strcat(t.label, p);
          t.suffix = new char [strlen(p)+1];
          strcpy(t.suffix, p);
          probeList.push_back(t);
        }
      }
    }
  }

  // We could a search for all remaining probes/locations.....
}

/* -------------------------------------------------------------------- */
void ResetProbeList()
{
  probeList.clear();
}

/* END PROBE.C */
