/*
-------------------------------------------------------------------------
OBJECT NAME:	getstartlen.c

DESCRIPTION:	hdr_api routine to return data block start and length values.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "hdr_api.h"

#define VALID_VARS (SH | PMS1D | INS | IRS | DME | EVENT | BLK | PMS1V2 | UVHYG | SERIAL | PMS2DH | MASP1 | PMS1V3)


/* -------------------------------------------------------------------- */
int GetStart(const char vn[], int32_t *result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  *result = ntohl(((Blk *)HAPI_var[indx].ptr)->start);

  return(OK);

}	/* END GETSTART */

/* -------------------------------------------------------------------- */
int GetLength(const char vn[], int32_t *result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  *result = ntohl(((Blk *)HAPI_var[indx].ptr)->length);

  return(OK);

}	/* END GETLENGTH */

/* END GETSTARTLEN.C */
