/*
-------------------------------------------------------------------------
OBJECT NAME:	getintf.c

ENTRY POINTS:	GetInterface*()

DESCRIPTION:	

INPUT:		char *var, long *result

OUTPUT:		OK/ERR

REFERENCES:	_lookupvar()

REFERENCED BY:	User Level Applications

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "hdr_api.h"

#define VALID_VARS	(PMS1V2 | PMS1V3 | PMS2D)


/* -------------------------------------------------------------------- */
int GetInterfaceNumber(char vn[], long *result)
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
int GetInterfaceChannel(char vn[], long *result)
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
