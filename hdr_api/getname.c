/*
-------------------------------------------------------------------------
OBJECT NAME:	getname.c

ENTRY POINTS:	GetName()

DESCRIPTION:	

INPUT:		char *var, char **result

OUTPUT:		OK/ERR

REFERENCES:	_lookupvar()

REFERENCED BY:	User Level Applications

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "hdr_api.h"

#define VALID_VARS	(SH | PMS1D | PMS2D | ASYNC | PMS1V2 | SERIAL | PMS2DH | PMS1V3)


/* -------------------------------------------------------------------- */
int GetName(const char vn[], char **result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);


  switch (HAPI_var[indx].type)
    {
    case SH:
      *result = ((Sh *)HAPI_var[indx].ptr)->name;
      break;

    case PMS1D:
      *result = ((Pms1 *)HAPI_var[indx].ptr)->name;
      break;

    case PMS1V2:
      *result = ((Pms1v2 *)HAPI_var[indx].ptr)->name;
      break;

    case PMS1V3:
      *result = ((Pms1v3 *)HAPI_var[indx].ptr)->name;
      break;

    case PMS2D:
      *result = ((Pms2 *)HAPI_var[indx].ptr)->name;
      break;

    case ASYNC:
      *result = ((Asy *)HAPI_var[indx].ptr)->name;
      break;

    case SERIAL:
      *result = ((Serial *)HAPI_var[indx].ptr)->name;

    case PMS2DH:
      *result = ((Pms2h *)HAPI_var[indx].ptr)->name;

    default:
      taperr = BADTYPE;
      return(ERR);
    }


  {	/* Clean up bug in xbuild that allowed spaces to be stored as part
  * of the variable name.
  */
  char	*p, *t;

  for (p = t = *result; *p; ++p)
    if (*p != ' ')
      *t++ = *p;

  *t = *p;
  }

  return(OK);

}	/* END GETNAME */

/* END GETNAME.C */
