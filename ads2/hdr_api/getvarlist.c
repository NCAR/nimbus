/*
-------------------------------------------------------------------------
OBJECT NAME:	getvarlist.c

DESCRIPTION:	hdr_api routine to return list of variables in the header.
		hdr_api rountes for looping through block/instrument list.

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

/* END GETVARLIST.C */
