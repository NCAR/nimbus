/*
-------------------------------------------------------------------------
OBJECT NAME:	getstartlen.c

ENTRY POINTS:	GetStart()
		GetLength()

DESCRIPTION:	

INPUT:		char *var, long *result

OUTPUT:		OK/ERR

REFERENCES:	_lookupvar()

REFERENCED BY:	User Level Applications

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "hdr_api.h"

#define VALID_VARS (SH | PMS1D | INS | IRS | DME | EVENT | BLK | PMS1V2 | UVHYG | SERIAL | PMS2DH | MASP1 | PMS1V3)


/* -------------------------------------------------------------------- */
int GetStart(char vn[], long *result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  *result = ntohl(((Blk *)HAPI_var[indx].ptr)->start);

  return(OK);

}	/* END GETSTART */

/* -------------------------------------------------------------------- */
int GetLength(char vn[], long *result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  *result = ntohl(((Blk *)HAPI_var[indx].ptr)->length);

  return(OK);

}	/* END GETLENGTH */

/* END GETSTARTLEN.C */
