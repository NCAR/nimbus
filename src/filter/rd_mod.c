/*
-------------------------------------------------------------------------
OBJECT NAME:	rd_mod.c

FULL NAME:	Read modvars file.

ENTRY POINTS:	ReadModuloVariables()

DESCRIPTION:	

INPUT:		${PROJ_DIR}/defaults/modvars

OUTPUT:		[sp|rp|dp]->Modulo & mod_vals updated.

REFERENCES:	SearchTable()

REFERENCED BY:	hdr_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"


void
reset_modulo(var_base* varp, MOD* mp)
{
  if (varp->Modulo)
  {
    WLOG(("changing existing Modulo pointer for variable %s", varp->name));
    delete varp->Modulo;
    varp->Modulo = 0;
  }
  varp->Modulo = mp;
}


/* -------------------------------------------------------------------- */
void ReadModuloVariables()
{
  int	index;
  char	*modvars[512], target[NAMELEN];
  int	val[2], ten_percent;
  MOD	*mp;

  ReadTextFile(MODVARS, modvars);

  for (size_t i = 0; modvars[i]; ++i)
    {
    sscanf(modvars[i], "%s %d %d", target, &val[0], &val[1]);
    mp = new MOD;

    ten_percent		= (int)((val[1] - val[0]) * 0.10);
    mp->value[0]	= val[0];
    mp->value[1]	= val[1];
    mp->diff		= val[1] - val[0];
    mp->bound[0]	= val[0] + ten_percent;
    mp->bound[1]	= val[1] - ten_percent;

  // Each raw and derived variable gets its own new copy of the Modulo
  // struct, so each instance can delete its copy when it gets destroyed.
  if ((index = SearchTableSansLocation(raw, target)) != ERR)
    {
      reset_modulo(raw[index], mp);

    strcat(target, "_");

    while (++index < (int)raw.size() &&
	strncmp((char *)raw[index]->name, target, strlen(target)) == 0)
      reset_modulo(raw[index], new MOD(*mp));
    }
  else
  if ((index = SearchTableSansLocation(derived, target)) != ERR)
    {
      reset_modulo(derived[index], mp);

    strcat(target, "_");

    while (++index < (int)derived.size() &&
	strncmp((char *)derived[index]->name,target,strlen(target)) == 0)
      reset_modulo(derived[index], new MOD(*mp));
    }
  else
    delete mp;
  }

  FreeTextFile(modvars);

}	/* END READMODULOVARIABLES */

/* END RD_MOD.C */
