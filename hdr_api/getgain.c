/*
-------------------------------------------------------------------------
OBJECT NAME:	getgain.c

ENTRY POINTS:	GetChannelGain()
		GetChannelOffset()

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

#define VALID_VARS	(SH)


/* -------------------------------------------------------------------- */
int GetChannelOffset(char vn[], long *result)
{
  int	indx;

  if (HAPI_version < 3.0)
    {
    taperr = BADVER;
    return(ERR);
    }

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  taperr = 0;

  *result = ntohl(((Sh *)HAPI_var[indx].ptr)->ch_offset);

  return(OK);

}	/* END GETCHANNELOFFSET */

/* -------------------------------------------------------------------- */
int GetChannelGain(char vn[], long *result)
{
  int	indx;

  if (HAPI_version < 3.0)
    {
    taperr = BADVER;
    return(ERR);
    }

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  taperr = 0;

  *result = ntohl(((Sh *)HAPI_var[indx].ptr)->ch_gain);

  return(OK);

}	/* END GETCHANNELGAIN */

/* END GETGAIN.C */
