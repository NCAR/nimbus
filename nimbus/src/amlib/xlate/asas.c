/*
-------------------------------------------------------------------------
OBJECT NAME:	asas.c

FULL NAME:	Translate PMS1D to new record format (including Version 2)

ENTRY POINTS:	xlaact()

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
void xlaact(RAWTBL *varp, void *p, NR_TYPE *np)
{
  NR_TYPE	reset;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
    reset = (NR_TYPE)ntohs(((P16_blk *)p)[i].aux[4]);

    if (reset < 0)
      reset += 65536;

    np[i] = (reset * (NR_TYPE)ntohs(((P16_blk *)p)[i].aux[2]) / 1024.0) * 0.52;
    }

}	/* END XLAACT */

/* END ASAS.C */
