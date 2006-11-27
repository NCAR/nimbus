/*
-------------------------------------------------------------------------
OBJECT NAME:	vdbdump.c

FULL NAME:	VarDB dump

DESCRIPTION:	This program does a basic text dump of a Variable DataBase
		file.

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h> // htonl macros.

#include "vardb.h"

extern long	VarDB_nRecords;
bool tracker = false;

/* -------------------------------------------------------------------- */
main(int argc, char *argv[])
{
  int		i = 1;

  if (argc < 2)
  {
    fprintf(stderr, "Usage: vdbdump [-a] [proj_num | VarDB_filename]\n");
    return(1);
  }

  if (strcmp(argv[i], "-a") == 0)
  {
     tracker = true;
     i++;
  }

  if (InitializeVarDB(argv[i]) == ERR)
  {
    fprintf(stderr, "vdbdump: Initialize failure.\n");
    return(1);
  }


  printf("Version %d, with %d records.\n", ntohl(VarDB_Hdr.Version),
						VarDB_nRecords);

  for (i = 0; i < VarDB_nRecords; ++i)
  {
    if (tracker)
      printf("%-12.12s%-8.8s%-60.60s", ((struct var_v2 *)VarDB)[i].Name,
               ((struct var_v2 *)VarDB)[i].Units, ((struct var_v2 *)VarDB)[i].Title);
    else
      printf("%-12.12s %-12.12s %-50.50s", ((struct var_v2 *)VarDB)[i].Name,
               ((struct var_v2 *)VarDB)[i].Units, ((struct var_v2 *)VarDB)[i].Title);

    if (ntohl(((struct var_v2 *)VarDB)[i].type) == FLOATING)
    {
      ulong	p;
      float	*rc = (float *)&p;

      p = ntohl(*((int *)&(((struct var_v2 *)VarDB)[i].FloatRange)));
      if (!tracker) {
         printf("%6.1f\n", *rc);
      }
    }
    else
    {
      ulong	p0, p1;
      float	*rc0 = (float *)&p0, *rc1 = (float *)&p1;

      p0 = ntohl(*((int *)&(((struct var_v2 *)VarDB)[i].FixedRange[0])));
      p1 = ntohl(*((int *)&(((struct var_v2 *)VarDB)[i].FixedRange[1])));
      if (!tracker)
         printf("%6.1f %6.1f\n", *rc0, *rc1);
    }

    if (tracker)
       printf("\n");
  }

  ReleaseVarDB();

  return(0);

}	/* END MAIN */

/* END VDBDUMP.C */
