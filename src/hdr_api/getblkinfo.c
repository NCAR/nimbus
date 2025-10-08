/*
-------------------------------------------------------------------------
OBJECT NAME:	getblkinfo.c

DESCRIPTION:	hdr_api routines to return block/instrument struct pointer,
		type, and length.

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

/* -------------------------------------------------------------------- */
int GetItemType(const char vn[], char **result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  *result = ((Blk *)HAPI_var[indx].ptr)->item_type;

  return(OK);

}	/* END GETITEMTYPE */

/* -------------------------------------------------------------------- */
int GetItemLength(const char vn[], int32_t *result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  *result = ntohl(((Blk *)HAPI_var[indx].ptr)->item_len);

  return(OK);

}	/* END GETITEMLEN */

/* END GETBLKINFO.C */
