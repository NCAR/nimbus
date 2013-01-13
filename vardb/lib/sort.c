/*
-------------------------------------------------------------------------
OBJECT NAME:	init.c

FULL NAME:	Initialize

ENTRY POINTS:	SortVarDB()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		FileName

OUTPUT:		OK/ERR

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include <stdio.h>
#include <string.h>
#include "vardb.h"

extern long	VarDB_nRecords;

/* -------------------------------------------------------------------- */
void SortVarDB()
{
  int		i, j;
  int		swap;
  struct var_v2	temp;

  for (i = VarDB_nRecords - 1; i >= 0; --i)
    {
    swap = FALSE;

    for (j = VarDB_nRecords - 1; j >= 0; --j)
      if (strcmp(((struct var_v2 *)VarDB)[i].Name,
		 ((struct var_v2 *)VarDB)[j].Name) > 0)
      {
      swap = TRUE;

      memcpy(&temp, &((struct var_v2 *)VarDB)[j], sizeof(struct var_v2));

      memcpy(&((struct var_v2 *)VarDB)[j], &((struct var_v2 *)VarDB)[i],
						sizeof(struct var_v2));

      memcpy(&((struct var_v2 *)VarDB)[i], &temp, sizeof(struct var_v2));
      }

    if (swap == FALSE)
      break;
    }

}	/* END SORTVARDB */

/* END SORT.C */
