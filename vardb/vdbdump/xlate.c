/*
-------------------------------------------------------------------------
OBJECT NAME:	xlate.c

FULL NAME:	Translate to VarDB

ENTRY POINTS:	main()

STATIC FNS:	none

DESCRIPTION:	This program converts an old bulletin.9 and plot_attr into
		a new VarDB.

INPUT:		none

OUTPUT:		File called "VarDB"

REFERENCES:	libVarDB.a

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include <stdio.h>
#include <raf/vardb.h>

main(argc, argv)
int	argc;
char	*argv[];
{
  char	buff1[1024], buff2[1024];

  if (argc < 2)
    {
    fprintf(stderr, "Usage: xlate proj_num\n");
    exit(1);
    }

  sprintf(buff1, "/home/local/proj/defaults/bulletin.9", argv[1]);
  sprintf(buff2, "/home/local/proj/%s/plot_attr", argv[1]);

  if (CreateVarDB(buff1, buff2) == ERR)
    {
    fprintf(stderr, "xlate: Translation failed.\n");
    exit(1);
    }


  sprintf(buff1, "/home/local/proj/%s/VarDB", argv[1]);

  if (SaveVarDB(buff1) == ERR)
    {
    fprintf(stderr, "xlate: save failure.\n");
    exit(1);
    }

  ReleaseVarDB();

}	/* END MAIN */

/* END XLATE.C */
