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


/* -------------------------------------------------------------------- */
void ReadModuloVariables()
{
  int	index;
  char	*modvars[512], target[NAMELEN];
  int	val[2], ten_percent;
  MOD	*mp;


  ReadTextFile(MODVARS, modvars);

  for (int i = 0; modvars[i]; ++i)
    {
    sscanf(modvars[i], "%s %d %d", target, &val[0], &val[1]);
    mp = new MOD;

    ten_percent		= (int)((val[1] - val[0]) * 0.10);
    mp->value[0]	= val[0];
    mp->value[1]	= val[1];
    mp->diff		= val[1] - val[0];
    mp->bound[0]	= val[0] + ten_percent;
    mp->bound[1]	= val[1] - ten_percent;

  if ((index = SearchTable(sdi, target)) != ERR)
    sdi[index]->Modulo = mp;
  else
    if ((index = SearchTableSansLocation(raw, target)) != ERR)
      {
      raw[index]->Modulo = mp;

      strcat(target, "_");

      while (strncmp((char *)raw[++index], target, strlen(target)) == 0)
        raw[index]->Modulo = mp;
      }
    else
     if ((index = SearchTableSansLocation(derived, target)) != ERR)
      {
      derived[index]->Modulo = mp;

      strcat(target, "_");

      while (strncmp((char *)derived[++index],target,strlen(target)) == 0)
        derived[index]->Modulo = mp;
      }
    else
      delete mp;
    }

  FreeTextFile(modvars);

}	/* END READMODULOVARIABLES */

/* END RD_MOD.C */
