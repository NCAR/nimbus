/*
-------------------------------------------------------------------------
OBJECT NAME:	pms1_ps.c

FULL NAME:	Translate PMS1D Particle Spacing to new record format

ENTRY POINTS:	xlpartspace()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	rec_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void xlpartspace(RAWTBL *varp, void *input, NR_TYPE *np)
{
  P1dSpc_blk	*p = (P1dSpc_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    for (size_t j = 0; j < varp->Length; ++j)
      np[(i*varp->Length)+j] = ntohs(p[i].bins[j]);

}	/* END XLPARTSPACE */

/* END PMS1_PS.C */
