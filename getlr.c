/*
-------------------------------------------------------------------------
OBJECT NAME:	getlr.c

FULL NAME:	Get Logical Record *

ENTRY POINTS:	GetLRLength()
		GetLRPPR()

DESCRIPTION:	

INPUT:		char *var, long *result

OUTPUT:		OK/ERR

REFERENCES:	_lookupvar()

REFERENCED BY:	User Level Applications

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "hdr_api.h"

#define VALID_VARS	(PMS2D | ASYNC)


/* -------------------------------------------------------------------- */
int GetLRLength(char vn[], long *result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);


  switch (HAPI_var[indx].type)
    {
    case PMS2D:
      *result = ntohl(((Pms2 *)HAPI_var[indx].ptr)->lrlen);
      break;

    case ASYNC:
      *result = ntohl(((Asy *)HAPI_var[indx].ptr)->lrlen);
      break;

    default:
      taperr = BADTYPE;
      return(ERR);
    }

  return(OK);

}	/* END GETLRLENGTH */

/* -------------------------------------------------------------------- */
int GetLRPPR(char vn[], long *result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);


  switch (HAPI_var[indx].type)
    {
    case PMS2D:
      *result = (long)ntohs(((Pms2 *)HAPI_var[indx].ptr)->lrppr);
      break;

    case ASYNC:
      *result = ntohl(((Asy *)HAPI_var[indx].ptr)->lrppr);
      break;

    default:
      taperr = BADTYPE;
      return(ERR);
    }

  return(OK);

}	/* END GETLRPPR */

/* END GETLR.C */
