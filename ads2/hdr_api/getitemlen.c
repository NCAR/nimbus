/*
-------------------------------------------------------------------------
OBJECT NAME:	getitemlen.c

ENTRY POINTS:	GetItemLength()

DESCRIPTION:	

INPUT:		char *var, long *result

OUTPUT:		OK/ERR

REFERENCES:	_lookupvar()

REFERENCED BY:	User Level Applications

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "hdr_api.h"

#define VALID_VARS	ALL


/* -------------------------------------------------------------------- */
int GetItemLength(const char vn[], long *result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  *result = ntohl(((Blk *)HAPI_var[indx].ptr)->item_len);

  return(OK);

}	/* END GETITEMLEN */

/* END GETITEMLEN.C */
