/*
-------------------------------------------------------------------------
OBJECT NAME:	getvarlist.c

FULL NAME:	Get Variable List

ENTRY POINTS:	GetVariableList()

DESCRIPTION:	

INPUT:		none

OUTPUT:		char	**to list of var names

REFERENCES:	none

REFERENCED BY:	User Application Program

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "hdr_api.h"


/* -------------------------------------------------------------------- */
char **GetVariableList()
{
  if (!HAPI_HeaderInitialized)
    {
    taperr = NOINIT;
    return((char **)NULL);
    }

  return(HAPI_var_name);

}	/* END GETVARIABLELIST */

/* END GETVARLIST.C */
