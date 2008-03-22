/*
-------------------------------------------------------------------------
OBJECT NAME:	climet.c

FULL NAME:	

ENTRY POINTS:	sClimet()

STATIC FNS:	

DESCRIPTION:	Template for AMLIB std & user directories.

COPYRIGHT:	University Corporation for Atmospheric Research, 2002
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


static NR_TYPE	conc;

/* -------------------------------------------------------------------- */
void sClimet(DERTBL *varp)
{
  int		i;
  NR_TYPE	*actual, *concentration, psx, atx,
		sampleVolume;

  /* These 'GetSample()'s must match the same order as the variables
   * are listed in the DependTable.
   */
  actual = GetVector(varp, 0);
  psx = GetSample(varp, 1);
  atx = GetSample(varp, 2);

  concentration = &AveragedData[varp->LRstart];

  sampleVolume = 4.7195e-5;
  sampleVolume *= (1013.25 / psx) * (atx + Kelvin) / 294.26;

  conc = 0.0;

  for (i = 0; i < 7; ++i)
    {
    concentration[i] = actual[i] / sampleVolume;
    conc += concentration[i];
//printf("%d ", (int)actual[i]);
    }

//printf("  %f\n", sampleVolume);

}	/* END SFUNC */

/* -------------------------------------------------------------------- */
void sconct(DERTBL *varp)
{
  PutSample(varp, conc);

}

/* END CLIMET.C */
