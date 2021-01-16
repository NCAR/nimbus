/*
-------------------------------------------------------------------------
OBJECT NAME:	getres.c

ENTRY POINTS:	GetResolution()

DESCRIPTION:	hdr_api routine to return PMS2D probe resolution.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "hdr_api.h"

#define VALID_VARS	PMS2D


/* -------------------------------------------------------------------- */
int GetResolution(const char vn[], short *result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  *result = ntohs(((Pms2 *)HAPI_var[indx].ptr)->resolution);

  return(OK);

}	/* END GETRESOLUTION */

/* END GETRES.C */
