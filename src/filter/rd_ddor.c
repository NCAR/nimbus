/*
-------------------------------------------------------------------------
OBJECT NAME:	rd_ddor.c

FULL NAME:	Read DDORLR file

ENTRY POINTS:	ReadDefaultDerivedOutputRate()

DESCRIPTION:	This file is of consequence only for a HighRate run.
		The variables in the file will default to a LowRate
		OutputRate.

INPUT:		${PROJ_DIR}/defaults/DDORLR

OUTPUT:		DERTBL Default_HR_OR updated.

REFERENCES:	SearchTable(), AccessProjectFile(), ReadTextFile()

REFERENCED BY:	hdr_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1994
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"


/* -------------------------------------------------------------------- */
void ReadDefaultDerivedOutputRate()
{
  int	i, index;
  char	*ddor[512];

  if (AccessProjectFile(DEFDEROR, "r") == FALSE)
    return;

  ReadTextFile(DEFDEROR, ddor);

  for (i = 0; ddor[i]; ++i)
    if ((index = SearchTable(derived, ddor[i])) != ERR)
      derived[index]->Default_HR_OR = LOW_RATE;

  FreeTextFile(ddor);

}	/* END READDEFAULTDERIVEDOUTPUTRATE */

/* END RD_DDOR.C */
