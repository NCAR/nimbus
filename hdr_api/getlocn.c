/*
-------------------------------------------------------------------------
OBJECT NAME:	getlocn.c

ENTRY POINTS:	GetLocation()
		GetDSMlocation()

DESCRIPTION:	

INPUT:		char *var, char **result

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

#define VALID_VARS	(BLK | UVHYG | IRS | EVENT | PMS1D | PMS1V2 | PMS2D | PMS2DH | MASP1 | PMS1V3)


/* -------------------------------------------------------------------- */
int GetLocation(const char vn[], char **result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  taperr = 0;

  /* Currently locn for both structs is in the same place
   */
  switch (HAPI_var[indx].type)
    {
    case PMS1D:
      *result = ((Pms1 *)HAPI_var[indx].ptr)->locn;
      break;

    case PMS1V2:
      *result = ((Pms1v2 *)HAPI_var[indx].ptr)->locn;
      break;

    case PMS1V3:
      *result = ((Pms1v3 *)HAPI_var[indx].ptr)->locn;
      break;

    case IRS:
    case EVENT:
      *result = ((Irs *)HAPI_var[indx].ptr)->locn;
      break;

    case MASP1:
      *result = ((Masp *)HAPI_var[indx].ptr)->locn;
      break;

    case PMS2D:
      *result = ((Pms2 *)HAPI_var[indx].ptr)->locn;
      break;

    case PMS2DH:
      *result = ((Pms2h *)HAPI_var[indx].ptr)->locn;
      break;

    case BLK:
      if (HAPI_version < 3.0)
        taperr = BADVER;

      *result = ((Blk *)HAPI_var[indx].ptr)->locn;
      break;

    case UVHYG:
      if (HAPI_version < 3.0)
        taperr = BADVER;

      *result = ((Uvhyg *)HAPI_var[indx].ptr)->locn;
      break;

    default:
      taperr = BADTYPE;
    }

  if (taperr)
    {
    *result = NULL;
    return(ERR);
    }
  else
    return(OK);

}	/* END GETLOCATION */

/* -------------------------------------------------------------------- */
int GetDSMlocation(const char vn[], char **result)
{
  int	indx;

  if (HAPI_version < 3.0)
    {
    taperr = BADVER;
    *result = NULL;
    return(ERR);
    }

  if ((indx = HAPI_lookupvar(vn, ALL)) == ERR)
    return(ERR);

  taperr = 0;

  /* Currently locn for both structs is in the same place
   */
  switch (HAPI_var[indx].type)
    {
    case SH:
      *result = ((Sh *)HAPI_var[indx].ptr)->dsm_locn;
      break;

    case DME:
      *result = ((Dme *)HAPI_var[indx].ptr)->dsm_locn;
      break;

    case MASP1:
      *result = ((Masp *)HAPI_var[indx].ptr)->dsm_locn;
      break;

    case PMS1D:
      *result = ((Pms1 *)HAPI_var[indx].ptr)->dsm_locn;
      break;

    case PMS1V2:
      *result = ((Pms1v2 *)HAPI_var[indx].ptr)->dsm_locn;
      break;

    case PMS1V3:
      *result = ((Pms1v3 *)HAPI_var[indx].ptr)->dsm_locn;
      break;

    case INS:
      *result = ((Ins *)HAPI_var[indx].ptr)->dsm_locn;
      break;

    case IRS:
      *result = ((Irs *)HAPI_var[indx].ptr)->dsm_locn;
      break;

    case EVENT:
      *result = ((Evt *)HAPI_var[indx].ptr)->dsm_locn;
      break;

    case PMS2D:
      *result = ((Pms2 *)HAPI_var[indx].ptr)->dsm_locn;
      break;

    case PMS2DH:
      *result = ((Pms2h *)HAPI_var[indx].ptr)->dsm_locn;
      break;

    case BLK:
      *result = ((Blk *)HAPI_var[indx].ptr)->dsm_locn;
      break;

    case UVHYG:
      *result = ((Blk *)HAPI_var[indx].ptr)->dsm_locn;
      break;

    case ASYNC:
      *result = ((Asy *)HAPI_var[indx].ptr)->dsm_locn;
      break;

    case SERIAL:
      *result = ((Serial *)HAPI_var[indx].ptr)->dsm_locn;
      break;
    }

  return(OK);

}	/* END GETDSMLOCATION */

/* END GETLOCN.C */
