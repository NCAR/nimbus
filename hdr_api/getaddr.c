/*
-------------------------------------------------------------------------
OBJECT NAME:	getaddr.c

ENTRY POINTS:	GetPrimaryAddr()
		GetSecondaryAddr()

DESCRIPTION:	

INPUT:		

OUTPUT:		OK/ERR

REFERENCES:	_lookupvar()

REFERENCED BY:	User Application Programs

NOTE:		Both procedures are assuming that the addresses are
		located in the same place for all variable structures
		present.  If any of the address locations change withen 
		their structures, this code will need to be modified.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "hdr_api.h"

#define VALID_VARS_P	(SH | PMS1D | INS)
#define VALID_VARS_S	(PMS1D | INS)


/* -------------------------------------------------------------------- */
int GetPrimaryAddr(const char vn[], long *result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS_P)) == ERR)
    return(ERR);

  /* Currently adsaddr & adsadr1 for all structs is in the same
   * place.
   */
  *result = ntohl(((Ins *)HAPI_var[indx].ptr)->adsadr1);

  return(OK);

}	/* END GETADSADDRESS */

/* -------------------------------------------------------------------- */
int GetSecondaryAddr(const char vn[], long *result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS_S)) == ERR)
    return(ERR);

  /* Currently auxaddr & adsadr2 for all structs is in the same
   * place.
   */
  *result = ntohl(((Ins *)HAPI_var[indx].ptr)->adsadr2);

  return(OK);

}	/* END GETADSADDRESS */

/* END GETADDR.C */
