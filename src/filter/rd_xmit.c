/*
-------------------------------------------------------------------------
OBJECT NAME:	rd_xmit.c

FULL NAME:	Read 'groundvars' file

ENTRY POINTS:	ReadGroundVarsFile()

DESCRIPTION:	If we are transmitting data to ground, read the list
		of variables to transmit and toggle the Transmit bool
		in var_base.

INPUT:		${PROJ_DIR}/defaults/###/groundvars

OUTPUT:		var_base Transmit updated.

REFERENCES:	SearchTable(), AccessProjectFile(), ReadTextFile()

REFERENCED BY:	hdr_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 2006
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"


/* -------------------------------------------------------------------- */
void ReadGroundVarsFile()
{
  int	i, index;
  char	*list[512];

  if (AccessProjectFile(XMIT_VARS, "r") == false)
    return;

  ReadTextFile(XMIT_VARS, list);

  for (i = 0; list[i]; ++i)
  {
    if ((index = SearchTable(raw, list[i])) != ERR)
      raw[index]->Transmit = true;

    if ((index = SearchTable(derived, list[i])) != ERR)
      derived[index]->Transmit = true;
  }

  FreeTextFile(list);

}	/* END READGROUNDVARSFILE */

/* END RD_XMIT.C */
