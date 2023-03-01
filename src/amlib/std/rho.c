/*
-------------------------------------------------------------------------
OBJECT NAME:	rho.c

FULL NAME:	Air Density

DESCRIPTION:	Template for AMLIB std & user directories.

COPYRIGHT:	University Corporation for Atmospheric Research, 2023
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void srho(DERTBL *varp)
{
  NR_TYPE	psx, atx, mr;

  psx = GetSample(varp, 0);
  atx = GetSample(varp, 1);
  mr = GetSample(varp, 2);

  NR_TYPE pair = psx * 100.0;
  NR_TYPE e = pair * mr / 1000.0 / 1000.0;
  NR_TYPE tair = atx + 273.15;
  NR_TYPE rho = (pair - e) / (Rd * tair) + e / (Rw * tair);

  PutSample(varp, rho);

}	/* END SRHO */

/* END RHO.C */
