/*
-------------------------------------------------------------------------
OBJECT NAME:	rd_sum.c

FULL NAME:	Read sumvars file.

ENTRY POINTS:	ReadSumVariables()

DESCRIPTION:	

INPUT:		${PROJ_DIR}/defaults/sumvars

OUTPUT:		[sp|rp|dp]->Average updated.

REFERENCES:	SearchTable()

REFERENCED BY:	hdr_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"


/* -------------------------------------------------------------------- */
void ReadSumVariables()
{
  int	index;
  char	*sumvar[512], target[NAMELEN];


  ReadTextFile(SUMVARS, sumvar);

  for (int i = 0; sumvar[i]; ++i)
    {
    strcpy(target, sumvar[i]);

    if ((index = SearchTable(sdi, target)) != ERR)
      {
      sdi[index]->type[0] = 'C';
      sdi[index]->Average = (void (*) (...))Sum;
      }
    else
    if ((index = SearchTableSansLocation(raw, target)) != ERR)
      {
      raw[index]->Average = (void (*) (...))Sum;

      strcat(target, "_");

      while (strncmp((char *)raw[++index], target, strlen(target)) == 0)
        raw[index]->Average = (void (*) (...))Sum;
      }
    else
      continue;
    }

  FreeTextFile(sumvar);

}	/* END READSUMVARIABLES */

/* END RD_SUM.C */
