/*
-------------------------------------------------------------------------
OBJECT NAME:	reference.c

FULL NAME:	

ENTRY POINTS:	sRefer()

STATIC FNS:	

DESCRIPTION:	This function is used for straight assignment of one variable
		to another.

INPUT:		

OUTPUT:		

REFERENCES:	

REFERENCED BY:	compute.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1996
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void sRefer(DERTBL *varp)
{
  PutSample(varp, GetSample(varp, 0));

}	/* END SREFER */

/* END REFERENCE.C */
