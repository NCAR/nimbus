/*
-------------------------------------------------------------------------
OBJECT NAME:	vdbdump.c

FULL NAME:	VarDB dump

ENTRY POINTS:	main()

STATIC FNS:	none

DESCRIPTION:	This program does a basic text dump of a Variable DataBase
		file.

INPUT:		FileName

OUTPUT:		Stuff

REFERENCES:	libVarDB.a

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h> // htonl macros.

#include "vardb.h"

extern long	VarDB_nRecords;

/* -------------------------------------------------------------------- */
main(int argc, char *argv[])
{
  int		i;

  if (argc != 2)
    {
    fprintf(stderr, "Usage: vdbdump [proj_num | VarDB_filename]\n");
    exit(1);
    }

  if (InitializeVarDB(argv[1]) == ERR)
    {
    fprintf(stderr, "vdbdump: Initialize failure.\n");
    exit(1);
    }


  printf("Version %d, with %d records.\n", ntohl(VarDB_Hdr.Version),
						VarDB_nRecords);

  for (i = 0; i < VarDB_nRecords; ++i)
    {
    printf("%-12.12s %-6.6s %-50.50s",	((struct var_v2 *)VarDB)[i].Name,
	((struct var_v2 *)VarDB)[i].Units, ((struct var_v2 *)VarDB)[i].Title);

    if (ntohl(((struct var_v2 *)VarDB)[i].type) == FLOATING)
      {
      ulong	p;
      float	*rc = (float *)&p;

      p = ntohl(*((int *)&(((struct var_v2 *)VarDB)[i].FloatRange)));

      printf("%6.1f\n", *rc);
      }
    else
      {
      ulong	p0, p1;
      float	*rc0 = (float *)&p0, *rc1 = (float *)&p1;

      p0 = ntohl(*((int *)&(((struct var_v2 *)VarDB)[i].FixedRange[0])));
      p1 = ntohl(*((int *)&(((struct var_v2 *)VarDB)[i].FixedRange[1])));

      printf("%6.1f %6.1f\n", *rc0, *rc1);
      }
    }

  ReleaseVarDB();

  return(0);

}	/* END MAIN */

/* END VDBDUMP.C */
