/*
-------------------------------------------------------------------------
OBJECT NAME:	getrate.c

DESCRIPTION:	hdr_api routint to return sample rate.

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
int GetRate(const char vn[], int32_t *result)
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
