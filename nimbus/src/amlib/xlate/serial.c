/*
-------------------------------------------------------------------------
OBJECT NAME:	serial.c

FULL NAME:	

ENTRY POINTS:	xlserial()

STATIC FNS:	none

DESCRIPTION:	Code for User Serial blocks.  This code is to be changed
		whenever a user comes along with this kind of interface.
		Old projects are given the tough shit status, and previous
		Serial blocks get decoded according to the current project.

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	rec_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void xlserial(RAWTBL *varp, void *input, NR_TYPE *output)
{
  for (size_t i = 0; i < varp->SampleRate; ++i)
    output[i] = atof(&((char *)input)[i*8]);

}	/* END XLSERIAL */

/* END SERIAL.C */
