/*
-------------------------------------------------------------------------
OBJECT NAME:	getrate.c

ENTRY POINTS:	GetRate()

DESCRIPTION:	

INPUT:		char *var, long *result

OUTPUT:		OK/ERR

REFERENCES:	_lookupvar()

REFERENCED BY:	User Level Applications

NOTE:		This proc is assuming that all rate fields are located
		in the same position in all structs.  If this assumption
		changes, then you need to modify the code to reflect the
		change.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "hdr_api.h"

#define VALID_VARS	(SH | INS | DME | PMS1D | PMS1V2 | SERIAL | MASP1 | PMS1V3)


/* -------------------------------------------------------------------- */
int GetRate(char vn[], long *result)
{
  int	indx;

  if (HAPI_version < 3.45)
    {
    if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
      return(ERR);
    }
  else
    {
    if ((indx = HAPI_lookupvar(vn, VALID_VARS | BLK)) == ERR)
      return(ERR);
    }

  /* Currently the rate field for all structs is in the same place
   */
  if (HAPI_var[indx].type == BLK)
    *result = ntohl(((Blk *)HAPI_var[indx].ptr)->rate);
  else
    *result = ntohl(((Sh *)HAPI_var[indx].ptr)->rate);

  return(OK);

}	/* END GETRATE */

/* END GETRATE.C */
