/*
-------------------------------------------------------------------------
OBJECT NAME:	getnbins.c

ENTRY POINTS:	GetNumberBins()

DESCRIPTION:	

INPUT:		char *var, long *result

OUTPUT:		OK/ERR

REFERENCES:	_lookupvar()

REFERENCED BY:	User Level Applications

COPYRIGHT:	University Corporation for Atmospheric Research, 2000
-------------------------------------------------------------------------
*/

#include "hdr_api.h"

#define VALID_VARS	(PMS1V3)


/* -------------------------------------------------------------------- */
int GetNumberBins(char vn[], long *result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  *result = ntohl(((Pms1v3 *)HAPI_var[indx].ptr)->bins);

  return(OK);

}	/* END GETINTERFACENUMBER */

/* END GETNBINS.C */
