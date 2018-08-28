/*
-------------------------------------------------------------------------
OBJECT NAME:	pms1_data.c

FULL NAME:	Translate Climet to new record format.

ENTRY POINTS:	xlClimet()

STATIC FNS:	none

DESCRIPTION:	This sums up the data bins from SampleRate to 1hz.

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	rec_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 2002
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void xlClimetPOF(RAWTBL *varp, void *input, NR_TYPE *np)
{
  Climet_blk *p = (Climet_blk *)input;

  *np = ntohs(p->pof);

}

/* -------------------------------------------------------------------- */
void xlClimet(RAWTBL *varp, void *input, NR_TYPE *np)
{
  Climet_blk *p = (Climet_blk *)input;

  for (size_t i = 1; i < varp->Length; ++i)
    np[i] = ntohs(p->counts[i-1]);

}	/* END XLCLIMET */

/* END CLIMET.C */
