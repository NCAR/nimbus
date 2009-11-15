/*
-------------------------------------------------------------------------
OBJECT NAME:	getdmeaddr.c

ENTRY POINTS:	GetReadAddr1()
		GetReadAddr2()
		GetWriteAddr()

DESCRIPTION:	

INPUT:		char *var, long *result

OUTPUT:		OK/ERR

REFERENCES:	_lookupvar()

REFERENCED BY:	User Level Applications

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "hdr_api.h"

#define VALID_VARS	DME


/* -------------------------------------------------------------------- */
int GetReadAddr1(const char vn[], int32_t *result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  *result = ntohl(((Dme *)HAPI_var[indx].ptr)->rdadr1);

  return(OK);

}	/* END GETREADADDR1 */

/* -------------------------------------------------------------------- */
int GetReadAddr2(const char vn[], int32_t *result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  *result = ntohl(((Dme *)HAPI_var[indx].ptr)->rdadr2);

  return(OK);

}	/* END GETREADADDR2 */

/* -------------------------------------------------------------------- */
int GetWriteAddr(const char vn[], int32_t *result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  *result = ntohl(((Dme *)HAPI_var[indx].ptr)->wradr);

  return(OK);

}	/* END GETWRITEADDR */

/* END GETDMEADDR.C */
