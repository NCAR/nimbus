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

COPYRIGHT:	University Corporation for Atmospheric Research, 1995
-------------------------------------------------------------------------
*/

#include "nimbus.h"

#define MAX_PROBES	32

static int	nProbes = 0;
static char	*probeNames[MAX_PROBES];
static int	probeTypes[MAX_PROBES];

/* -------------------------------------------------------------------- */
void AddProbeToList(char name[], int type)
{
  if (nProbes >= MAX_PROBES)
    {
    fprintf(stderr, "Max probes exceeded, please call a programmer.\n");
    return;
    }

  probeNames[nProbes] = new char[strlen(name)+1];
  strcpy(probeNames[nProbes], name);
  probeTypes[nProbes] = type;

  probeNames[++nProbes] = NULL;

}	/* END ADDPROBETOLIST */

/* -------------------------------------------------------------------- */
void ResetProbeList()
{
  for (int i = 0; i < nProbes; ++i)
    delete [] probeNames[i];

  nProbes = 0;

}	/* END RESETPROBELIST */

/* -------------------------------------------------------------------- */
char **GetProbeList()
{
  return(probeNames);

}	/* END GETPROBELIST */

/* -------------------------------------------------------------------- */
unsigned long GetProbeType(char name[])
{
  for (int i = 0; i < nProbes; ++i)
    if (strcmp(name, probeNames[i]) == 0)
      return(probeTypes[i]);

  return(0);

}	/* END GETPROBETYPE */

/* END PROBE.C */
