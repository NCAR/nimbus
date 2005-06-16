/*
-------------------------------------------------------------------------
OBJECT NAME:	getsn.c

ENTRY POINTS:	GetSerialNumber()

DESCRIPTION:	

INPUT:		char *var, char *result

OUTPUT:		OK/ERR

REFERENCES:	HAPI_lookupvar()

REFERENCED BY:	User Level Applications

NOTE:		This proc is assuming that all rate fields are located
		in the same position in all structs.  If this assumption
		changes, then you need to modify the code to reflect the
		change.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "hdr_api.h"

#define VALID_VARS	(PMS1V2 | PMS2D | PMS2DH | UVHYG | MASP1 | PMS1V3)


/* -------------------------------------------------------------------- */
int GetSerialNumber(const char vn[], char **result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  switch (HAPI_var[indx].type)
    {
    case MASP1:
      *result = ((Masp *)HAPI_var[indx].ptr)->serial_num;
      break;

    case PMS1V2:
      *result = ((Pms1v2 *)HAPI_var[indx].ptr)->serial_num;
      break;

    case PMS1V3:
      *result = ((Pms1v3 *)HAPI_var[indx].ptr)->serial_num;
      break;

    case PMS2D:
      *result = ((Pms2 *)HAPI_var[indx].ptr)->serial_num;
      break;

    case PMS2DH:
      *result = ((Pms2h *)HAPI_var[indx].ptr)->serial_num;
      break;

    case UVHYG:
      *result = ((Uvhyg *)HAPI_var[indx].ptr)->serial_num;
      break;
    }


  if (HAPI_version > 2.09)
    return(OK);
  else
    {
    taperr = BADVER;
    return(ERR);
    }

}	/* END GETSERIALNUMBER */

/* END GETSN.C */
