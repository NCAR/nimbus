/*
-------------------------------------------------------------------------
OBJECT NAME:	wd.c

FULL NAME:	Wind Direction

ENTRY POINTS:	swd()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	compute.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

/* -------------------------------------------------------------------- */
void swd(DERTBL *varp)
{
  NR_TYPE	wd, ui, vi;

  ui = GetSample(varp, 0);
  vi = GetSample(varp, 1);

  if (ui == 0.0 && vi == 0.0)
/*  If wind is "calm," output a direction of 0 degrees instead of 360 degrees.
          Ron Ruth  9 November 2001  */
    {
    wd = 0.;
    }
  else
    {
    wd = (NR_TYPE)atan2((double)ui, (double)vi) * RAD_DEG + 180.0;

    while (wd < 0.0)
    wd += 360.0;
    }

  PutSample(varp, wd);

}	/* END SWD */

/* END WD.C */
