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

static int GetHeaderLong(int32_t *result, int32_t value);
static int GetHeaderString(char **result, char *value);


/* -------------------------------------------------------------------- */
int GetVersion(char **result)
{
  if (HAPI_HeaderInitialized)
    return(GetHeaderString(result, HAPI_header->version));
  else
    return(ERR);
}

/* -------------------------------------------------------------------- */
int GetProjectNumber(char **result)
{
  if (HAPI_HeaderInitialized)
    return(GetHeaderString(result, HAPI_header->prnum));
  else
    return(ERR);
}

/* -------------------------------------------------------------------- */
int GetFlightNumber(char **result)
{
  if (HAPI_HeaderInitialized)
    return(GetHeaderString(result, HAPI_header->fltnum));
  else
    return(ERR);
}

/* -------------------------------------------------------------------- */
int GetTapeNumber(char **result)
{
  if (HAPI_HeaderInitialized)
    return(GetHeaderString(result, HAPI_header->tpnum));
  else
    return(ERR);
}

/* -------------------------------------------------------------------- */
int GetHeaderDate(char **result)
{
  if (HAPI_HeaderInitialized)
    return(GetHeaderString(result, HAPI_header->date));
  else
    return(ERR);
}

/* -------------------------------------------------------------------- */
int GetHeaderTime(char **result)
{
  if (HAPI_HeaderInitialized)
    return(GetHeaderString(result, HAPI_header->time));
  else
    return(ERR);
}

/* -------------------------------------------------------------------- */
int GetTimeZone(char **result)
{
  if (HAPI_HeaderInitialized)
    return(GetHeaderString(result, HAPI_header->tzone));
  else
    return(ERR);
}

/* -------------------------------------------------------------------- */
int GetAircraft(char **result)
{
  if (HAPI_HeaderInitialized)
    return(GetHeaderString(result, HAPI_header->acraft));
  else
    return(ERR);
}

/* -------------------------------------------------------------------- */
int GetNumberItems(int32_t *result)
{
  if (HAPI_HeaderInitialized)
    return(GetHeaderLong(result, HAPI_header->n_items));
  else
    return(ERR);
}

/* -------------------------------------------------------------------- */
int get_lrlen(int32_t *result)
{
  if (HAPI_HeaderInitialized)
    return(GetHeaderLong(result, HAPI_header->lrlen));
  else
    return(ERR);
}

/* -------------------------------------------------------------------- */
int get_lrppr(int32_t *result)
{
  if (HAPI_HeaderInitialized)
    return(GetHeaderLong(result, HAPI_header->lrppr));
  else
    return(ERR);
}

/* -------------------------------------------------------------------- */
int get_thdrlen(int32_t *result)
{
  if (HAPI_HeaderInitialized)
    return(GetHeaderLong(result, HAPI_header->thdrlen));
  else
    return(ERR);
}

/* -------------------------------------------------------------------- */
static int GetHeaderString(char **result, char *value)
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
static int GetHeaderLong(int32_t *result, int32_t value)
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
