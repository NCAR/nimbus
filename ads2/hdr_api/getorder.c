/*
-------------------------------------------------------------------------
OBJECT NAME:	getorder.c

ENTRY POINTS:	GetOrder()

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
int GetOrder(const char vn[], int32_t *result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  *result = ntohl(((Sh *)HAPI_var[indx].ptr)->order);

  return(OK);

}	/* END GETORDER */

/* END GETORDER.C */
