/*
-------------------------------------------------------------------------
OBJECT NAME:	gethskp.c

ENTRY POINTS:	GetHouseKeepingNames()

DESCRIPTION:	

INPUT:		char *var, char **result

OUTPUT:		OK/ERR

REFERENCES:	_lookupvar()

REFERENCED BY:	User Level Applications

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "hdr_api.h"

#define VALID_VARS	(PMS1V2 | PMS2DH | MASP1)


/* -------------------------------------------------------------------- */
int GetHouseKeepingNames(const char vn[], char **result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  switch (HAPI_var[indx].type)
    {
    case MASP1:
      *result = (char *)((Masp *)HAPI_var[indx].ptr)->hsk_name;
      break;

    case PMS1V2:
      *result = (char *)((Pms1v2 *)HAPI_var[indx].ptr)->hsk_name;
      break;

    case PMS2DH:
      *result = (char *)((Pms2h *)HAPI_var[indx].ptr)->hsk_name;
      break;
    }

  return(OK);

}	/* END GETHOUSEKEEPINGNAMES */

/* END GETHSKP.C */
