/*
-------------------------------------------------------------------------
OBJECT NAME:	getps.c

ENTRY POINTS:	GetParticleSpacing*()

DESCRIPTION:	

INPUT:		char *var, long *result

OUTPUT:		OK/ERR

REFERENCES:	_lookupvar()

REFERENCED BY:	User Level Applications

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "hdr_api.h"

#define VALID_VARS	PMS1V2


/* -------------------------------------------------------------------- */
int GetParticleSpacingStart(const char vn[], int32_t *result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  *result = ntohl(((Pms1v2 *)HAPI_var[indx].ptr)->ps_start);

  return(OK);

}	/* END GETPARTSPACESTART */

/* -------------------------------------------------------------------- */
int GetParticleSpacingLength(const char vn[], int32_t *result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  *result = ntohl(((Pms1v2 *)HAPI_var[indx].ptr)->ps_length);

  return(OK);

}	/* END GETPARTSPACELENGTH */

/* -------------------------------------------------------------------- */
int GetParticleSpacingGate(const char vn[], float *result)
{
  int	indx;
  int32_t  value;
  float *p = (float *)&value;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  value = ntohl( *(int *)&(((Pms1v2 *)HAPI_var[indx].ptr)->ps_gate) );
  *result = *p;

  return(OK);

}	/* END GETPARTSPACEGATE */

/* END GETPS.C */
