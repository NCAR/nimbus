/*
-------------------------------------------------------------------------
OBJECT NAME:	async.c

FULL NAME:	

ENTRY POINTS:	xlAsync()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	rec_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1995
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void xlAsync(RAWTBL *varp, void *input, NR_TYPE *output)
{
  output[0] = output[1] = output[2] = 0.0;

}	/* END XLASYNC */

/* END ASYNC.C */
