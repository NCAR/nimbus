/*
-------------------------------------------------------------------------
OBJECT NAME:	despike.c

FULL NAME:	Despiking object routines

ENTRY POINTS:	ReadDespikeFile()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	hdr_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1995
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"

/* -------------------------------------------------------------------- */
void ReadDespikeFile()
{
  int	index;
  float	slope;
  char	*spikes[512], target[NAMELEN];

  if (AccessProjectFile(SPIKE, "r") == FALSE)
    return;

  ReadTextFile(SPIKE, spikes);

  for (int i = 0; spikes[i]; ++i)
    {
    sscanf(spikes[i], "%s %f", target, &slope);

    if ((index = SearchTable(raw, target)) != ERR)
      raw[index]->SpikeSlope = fabs((double)slope);
    else
      {
      sprintf(buffer, "ReadDespikeFile: non-existent variable %s specified, continuing.\n", target);
      LogMessage(buffer);
      }
    }

  FreeTextFile(spikes);

}	/* END READDESPIKEFILE */

/* END RD_SPIKE.C */
