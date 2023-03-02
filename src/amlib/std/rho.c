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
  NR_TYPE	psx, atx, mr, rho;

  psx = GetSample(varp, 0) * 100.0;
  atx = GetSample(varp, 1) + Kelvin;
/*
  mr = GetSample(varp, 2);

  NR_TYPE e = psx * mr / 1000.0 / 1000.0;
  rho = (psx - e) / (Rd * atx) + e / (Rw * tair);
*/
  rho = psx / (Rd * atx);

  PutSample(varp, rho);

}	/* END SRHO */

/* END RHO.C */
