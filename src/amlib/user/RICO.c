/*
-------------------------------------------------------------------------
OBJECT NAME:	RICO.c

FULL NAME:	

ENTRY POINTS:	sfunc()

STATIC FNS:	

DESCRIPTION:	Functions for the RICO project

COPYRIGHT:	University Corporation for Atmospheric Research, 2004
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void sCINext(DERTBL *varp)
{
  NR_TYPE	cinb, cinf, CINext;

  cinf = GetSample(varp, 0);
  cinb = GetSample(varp, 1);

  CINext = (93.3 * cinf) + (18.9 * cinb);

  PutSample(varp, CINext);

}	/* END SFUNC */

/* END RICO.C */
