/*
-------------------------------------------------------------------------
OBJECT NAME:	getnbins.c

DESCRIPTION:	hdr_api routine to return PMS number of bins or channels.

COPYRIGHT:	University Corporation for Atmospheric Research, 2000
-------------------------------------------------------------------------
*/

#include "hdr_api.h"

#define VALID_VARS	(PMS1V3)


/* -------------------------------------------------------------------- */
int GetNumberBins(const char vn[], int32_t *result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  *result = ntohl(((Pms1v3 *)HAPI_var[indx].ptr)->bins);

  return(OK);

}	/* END GETINTERFACENUMBER */

/* END GETNBINS.C */
