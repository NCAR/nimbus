/*
-------------------------------------------------------------------------
OBJECT NAME:	getxvert.c

ENTRY POINTS:	GetConversionFactor()
		GetConversionOffset()

DESCRIPTION:	

INPUT:		char *var, float *result

OUTPUT:		OK/ERR

REFERENCES:	_lookupvar()

REFERENCED BY:	User Level Applications

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "hdr_api.h"

#define VALID_VARS	SH


/* -------------------------------------------------------------------- */
int GetConversionFactor(const char vn[], float *result)
{
  int	indx;
  long	value;
  float	*p = (float *)&value;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  value = ntohl( *(int *)&(((Sh *)HAPI_var[indx].ptr)->convert) );
  *result = *p;

  return(OK);

}	/* END GETCONVERSIONFACTOR */

/* -------------------------------------------------------------------- */
int GetConversionOffset(const char vn[], long *result)
{
  int	indx;

  if (HAPI_version < 3.09)
    {
    taperr = BADVER;
    return(ERR);
    }

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  taperr = 0;

  *result = ntohl(((Sh *)HAPI_var[indx].ptr)->convert_offset);

  return(OK);

}	/* END GETCONVERSIONOFFSET */

/* END GETXVERT.C */
