/*
-------------------------------------------------------------------------
OBJECT NAME:	getpointer.c

ENTRY POINTS:	GetStructPointer()

DESCRIPTION:	

INPUT:		char *var

OUTPUT:		void Pointer to struct or NULL

REFERENCES:	_lookupvar()

REFERENCED BY:	User Level Applications

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "hdr_api.h"

#define VALID_VARS	ALL


/* -------------------------------------------------------------------- */
void *GetStructPointer(const char vn[])
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return((void *)NULL);

  return(HAPI_var[indx].ptr);

}	/* END GETSTRUCTPOINTER */

/* END GETPOINTER.C */
