/*
-------------------------------------------------------------------------
OBJECT NAME:	gethdr.c

ENTRY POINTS:	GetHeaderString()
		GetHeaderLong()
		GetADStype()
		

DESCRIPTION:	These two procedures are called by the macros:
		GetVersion()
		GetProjectNumber()
		etc. for all (struct Fl) values.

INPUT:		char **result, value

OUTPUT:		OK/ERR

REFERENCES:	none

REFERENCED BY:	Macros in tape.h

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "hdr_api.h"


/* -------------------------------------------------------------------- */
int GetHeaderString(char **result, char *value)
{
  if (!HAPI_HeaderInitialized)
    {
    taperr = NOINIT;
    return(ERR);
    }

  *result = value;

  return(OK);

}	/* END GETHEADERSTRING */

/* -------------------------------------------------------------------- */
long GetHeaderLong(long *result, long value)
{
  if (!HAPI_HeaderInitialized)
    {
    taperr = NOINIT;
    return(ERR);
    }

  *result = ntohl(value);

  return(OK);

}	/* END GETHEADERLONG */

/* -------------------------------------------------------------------- */
int GetADStype(char **result)
{
  if (!HAPI_HeaderInitialized)
    {
    taperr = NOINIT;
    return(ERR);
    }

  /* Proj 818 didn't make the 3.0 cut correctly, hack correct answer
   * here.
   */
  if (HAPI_version < 3.0 || strcmp("818", HAPI_header->prnum) == 0)
    *result = ADS_TYPE_1_STR;
  else
    *result = HAPI_header->ads_type;

  return(OK);

}	/* END GETADSTYPE */

/* END GETHDR.C */
