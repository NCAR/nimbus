/*
-------------------------------------------------------------------------
OBJECT NAME:	thf.c

FULL NAME:	Aircraft true heading (yaw) (deg) for Litton 51

ENTRY POINTS:	sthf()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		

OUTPUT:		

REFERENCES:	angcr.c

REFERENCED BY:	compute.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

NR_TYPE	angcr(NR_TYPE, NR_TYPE, float, float);

/* -------------------------------------------------------------------- */
void sthf(DERTBL *varp)
{
  NR_TYPE	phdg, alpha, thi;

  phdg	= GetSample(varp, 0);
  alpha	= GetSample(varp, 1);
  thi	= GetSample(varp, 2);

  PutSample(varp, angcr(phdg + alpha, thi, (float)45.0, (float)0.0));

}	/* END STHF */

/* END THF.C */
