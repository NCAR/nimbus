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
  int	i, index;
  char	*sumvar[512], target[NAMELEN];


  ReadTextFile(SUMVARS, sumvar);

  for (i = 0; sumvar[i]; ++i)
    {
    strcpy(target, sumvar[i]);

    if ((index = SearchTable((char **)sdi, nsdi, target)) != ERR)
      {
      sdi[index]->type[0] = 'C';
      sdi[index]->Average = (void (*) (NR_TYPE *, NR_TYPE *, void *))SumSDI;
      }
    else
    if ((index = SearchTableSansLocation((char **)raw, nraw, target)) != ERR)
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
