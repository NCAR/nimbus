/*
-------------------------------------------------------------------------
OBJECT NAME:	getoffset.c

ENTRY POINTS:	GetSampleOffset()

DESCRIPTION:	

INPUT:		char *var, long *result

OUTPUT:		OK/ERR

REFERENCES:	_lookupvar()

REFERENCED BY:	User Level Applications

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "hdr_api.h"

#define VALID_VARS	SH


/* -------------------------------------------------------------------- */
int GetSampleOffset(const char vn[], int32_t *result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  *result = ntohl(((Sh *)HAPI_var[indx].ptr)->offset);

  return(OK);

}	/* END GETSAMPLEOFFSET */

/* END GETOFFSET.C */
