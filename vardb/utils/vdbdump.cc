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

#include <raf/vardb.h>

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
      printf("%-12.12s%-8.8s%-60.60s\n", ((struct var_v2 *)VarDB)[i].Name,
               ((struct var_v2 *)VarDB)[i].Units, ((struct var_v2 *)VarDB)[i].Title);
    else
    {
      printf("%-12.12s %-12.12s %-50.50s", ((struct var_v2 *)VarDB)[i].Name,
               ((struct var_v2 *)VarDB)[i].Units, ((struct var_v2 *)VarDB)[i].Title);

      if (ntohl(((struct var_v2 *)VarDB)[i].is_analog) == true)
      {
        ulong	p;

        printf("%4d sps, ", ntohl(((struct var_v2 *)VarDB)[i].defaultSampleRate));
        printf("%d %d Vdc",
		ntohl(((struct var_v2 *)VarDB)[i].voltageRange[0]),
		ntohl(((struct var_v2 *)VarDB)[i].voltageRange[1]));
      }
      printf("\n");
    }
  }

  ReleaseVarDB();

  return(0);

}	/* END MAIN */

/* END VDBDUMP.C */
