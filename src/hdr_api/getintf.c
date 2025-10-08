/*
-------------------------------------------------------------------------
OBJECT NAME:	getintf.c

DESCRIPTION:	hdr_api routines to return PMS interface channel numbers

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "hdr_api.h"

#define VALID_VARS	(PMS1V2 | PMS1V3 | PMS2D)


/* -------------------------------------------------------------------- */
int GetInterfaceNumber(const char vn[], int32_t *result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  if (HAPI_var[indx].type == PMS1V2)
    *result = ntohl(((Pms1v2 *)HAPI_var[indx].ptr)->intf_num);
  else
    *result = 0;

  return(OK);

}	/* END GETINTERFACENUMBER */

/* -------------------------------------------------------------------- */
int GetInterfaceChannel(const char vn[], int32_t *result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  if (HAPI_var[indx].type == PMS1V2)
    *result = ntohl(((Pms1v2 *)HAPI_var[indx].ptr)->intf_chan);
  else
  if (HAPI_var[indx].type == PMS1V3)
    *result = ntohl(((Pms1v3 *)HAPI_var[indx].ptr)->intf_chan);
  else
  if (HAPI_var[indx].type == PMS2D)
    *result = ntohl(((Pms2 *)HAPI_var[indx].ptr)->intf_chan);

  return(OK);

}	/* END GETINTERFACECHANNEL */

/* END GETINTF.C */
