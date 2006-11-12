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
  char	*list[512];

  if (AccessProjectFile(XMIT_VARS, "r") == false)
    return;

  ReadTextFile(XMIT_VARS, list);

  for (int i = 0; list[i]; ++i)
  {
    int r_idx, d_idx;

    if ((r_idx = SearchTable(raw, list[i])) != ERR)
      raw[r_idx]->Transmit = true;

    if ((d_idx = SearchTable(derived, list[i])) != ERR)
      derived[d_idx]->Transmit = true;

    if (r_idx == ERR && d_idx == ERR)
    {
      char msg[100];
      sprintf(msg, "rd_xmit.c: Variable [%s] not found, ignoring.\n", list[i]);
      LogMessage(msg);
    }
  }

  FreeTextFile(list);


  // Reproduce above file in the order which nimbus will be placing them in
  // the packet.
  FILE *fp;
  std::string fileName(XMIT_VARS);
  fileName += ".rt";
  if ((fp = OpenProjectFile(fileName, "w", RETURN)) == NULL)
  {
    MakeProjectFileName(buffer, XMIT_VARS);
    fprintf(stderr, "ReadGroundVarsFile: can't open %s for writing.\n", buffer);
    exit(1);
  }


  bool oneOut = false;
  for (size_t i = 0; i < raw.size(); ++i)
  {
    if (raw[i]->Transmit)
    {
      if (oneOut)
        fprintf(fp, ", %s", raw[i]->name);
      else
        fprintf(fp, raw[i]->name);

      oneOut = true;
    }
  }

  for (size_t i = 0; i < derived.size(); ++i)
  {
    if (derived[i]->Transmit)
    {
      if (oneOut)
        fprintf(fp, ", %s", derived[i]->name);
      else
        fprintf(fp, derived[i]->name);

      oneOut = true;
    }
  }

  fclose(fp);

}	/* END READGROUNDVARSFILE */

/* END RD_XMIT.C */
