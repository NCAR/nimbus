/*
-------------------------------------------------------------------------
OBJECT NAME:	event.c

FULL NAME:	Translate EVENT blocks

ENTRY POINTS:	xlevent()

DESCRIPTION:	Cast event data (short) into a float.

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
void xlevent(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = (NR_TYPE)ntohl(*((long *)input));
}

/* END EVENT.C */
