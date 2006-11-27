/*
-------------------------------------------------------------------------
OBJECT NAME:	catdump.c

FULL NAME:	VarDB dump w/ Categories

DESCRIPTION:	Similar to vdbdump, but output iff category & standard_name

COPYRIGHT:	University Corporation for Atmospheric Research, 1999-2006
-------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h> // htonl macros.

#include "vardb.h"

extern long	VarDB_nRecords;

/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  int		i;

  if (argc != 2)
  {
    fprintf(stderr, "Usage: catdump [proj_num | VarDB_filename]\n");
    exit(1);
  }

  if (InitializeVarDB(argv[1]) == ERR)
  {
    fprintf(stderr, "catdump: Initialize failure.\n");
    exit(1);
  }


  printf("Version %d, with %d records.\n", ntohl(VarDB_Hdr.Version),
						VarDB_nRecords);

  for (i = 0; i < VarDB_nRecords; ++i)
  {
    if (VarDB_GetStandardName(((struct var_v2 *)VarDB)[i].Name) != 0)
      printf("%-12.12s %-40.40s %-15.15s %d %s\n",
		((struct var_v2 *)VarDB)[i].Name,
		((struct var_v2 *)VarDB)[i].Title,
		VarDB_GetCategoryName(((struct var_v2 *)VarDB)[i].Name),
		VarDB_GetStandardName(((struct var_v2 *)VarDB)[i].Name),
		VarDB_GetStandardNameName(((struct var_v2 *)VarDB)[i].Name));
  }

  ReleaseVarDB();

  return(0);

}	/* END MAIN */

/* END CATDUMP.C */
