/*
-------------------------------------------------------------------------
OBJECT NAME:	reference.c

DESCRIPTION:	This function is used for straight assignment of one variable
		to another.  Allows a second backup parameter to be passed in
		also.  If the primary parameter is nan, then the backup is used.

COPYRIGHT:	University Corporation for Atmospheric Research, 1996-2010
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void sRefer(DERTBL *varp)
{
  NR_TYPE x = GetSample(varp, 0);

  // If we have a secondary, use it if primary is nan.
  if (isnan(x) && varp->ndep > 1)
    x = GetSample(varp, 1);

  PutSample(varp, x);

}	/* END SREFER */

/* END REFERENCE.C */
