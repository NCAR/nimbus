/*
-------------------------------------------------------------------------
OBJECT NAME:	getitemtype.c

ENTRY POINTS:	GetItemType()

DESCRIPTION:	

INPUT:		char *var, char **result

OUTPUT:		OK/ERR

REFERENCES:	_lookupvar()

REFERENCED BY:	User Level Applications

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "hdr_api.h"

#define VALID_VARS	ALL

/* -------------------------------------------------------------------- */
int GetItemType(const char vn[], char **result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  *result = ((Blk *)HAPI_var[indx].ptr)->item_type;

  return(OK);

}	/* END GETITEMTYPE */

/* END GETITEMTYPE.C */
