/*
-------------------------------------------------------------------------
OBJECT NAME:	gettype.c

ENTRY POINTS:	GetType()

DESCRIPTION:	

INPUT:		char *var, char **result

OUTPUT:		OK/ERR

REFERENCES:	_lookupvar()

REFERENCED BY:	User Level Applications

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "hdr_api.h"

#define VALID_VARS	SH

/* -------------------------------------------------------------------- */
int GetType(char vn[], char **result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  *result = ((Sh *)HAPI_var[indx].ptr)->type;

  return(OK);

}	/* END GETTYPE */

/* END GETTYPE.C */
