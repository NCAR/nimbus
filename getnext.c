/*
-------------------------------------------------------------------------
OBJECT NAME:	getnext.c

ENTRY POINTS:	GetNext()
		GetFirst()

DESCRIPTION:	

INPUT:		none

OUTPUT:		char	*ptr to variable name/NULL for End O List

REFERENCES:	none

REFERENCED BY:	User Application Program

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "hdr_api.h"


/* -------------------------------------------------------------------- */
char *GetFirst()
{
  if (!HAPI_HeaderInitialized)
    {
    taperr = NOINIT;
    return((char *)NULL);
    }

  return(HAPI_var_name[(HAPI_name_ptr = 0)]);

}	/* END GETFIRST */

/* -------------------------------------------------------------------- */
char *GetNext()
{
  if (!HAPI_HeaderInitialized)
    {
    taperr = NOINIT;
    return((char *)NULL);
    }

  if (HAPI_var_name[HAPI_name_ptr])
    return(HAPI_var_name[++HAPI_name_ptr]);
  else
    return((char *)NULL);

}	/* END GETNEXT */

/* END GETNEXT.C */
