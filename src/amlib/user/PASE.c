/*
-------------------------------------------------------------------------
OBJECT NAME:	PASE.c

FULL NAME:

ENTRY POINTS:	sfunc()

STATIC FNS:  

DESCRIPTION:	Calcs for PASE, Steve Howell

COPYRIGHT:	University Corporation for Atmospheric Research, 2007
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

/* -------------------------------------------------------------------- */
void sScatter(DERTBL *varp)
{
  NR_TYPE  Vdc;

  Vdc = GetSample(varp, 0);

  PutSample(varp, pow(10.0, Vdc / 2.0));

}  /* END SFUNC */

/* END PASE.C */
