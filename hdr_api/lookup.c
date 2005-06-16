/*
-------------------------------------------------------------------------
OBJECT NAME:	lookup.c

ENTRY POINTS:	_lookupvar()

TYPE:		

DESCRIPTION:	

INPUT:		Var name to lookup

OUTPUT:		Index into var_name/var_list or ERR

REFERENCES:	none

REFERENCED BY:	Internal to the header API.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "hdr_api.h"


/* -------------------------------------------------------------------- */
int HAPI_lookupvar(const char vn[], const long valid_vars)
{
  int	i;

  if (!HAPI_HeaderInitialized)
    {
    fprintf(stderr, "hdr_api: not initialized.\n");
    taperr = NOINIT;
    return(ERR);
    }


  /* If it is not the current var then do a linear search
  */
  if (HAPI_var_name[HAPI_name_ptr] == NULL ||
      strcmp(vn, HAPI_var_name[HAPI_name_ptr]) != 0)
    {
    for (i = 0; HAPI_var_name[i]; ++i)
      if (strcmp(HAPI_var_name[i], vn) == 0)
        break;
    }
  else
    i = HAPI_name_ptr;


  /* If we have a match && it is of a desired type, then
  * return the var, else error
  */
  if (HAPI_var_name[i])
    {
    if (HAPI_var[i].type & valid_vars)
      return(i);
    else
      {
      taperr = BADTYPE;
      return(ERR);
      }
    }
  else
    {
/*    fprintf(stderr, "hdr_api: %s not found.\n", vn); */
    taperr = BADVAR;
    return(ERR);
    }

}	/* END LOOKUPVAR */

/* END LOOKUP.C */
