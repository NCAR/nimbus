/*
-------------------------------------------------------------------------
OBJECT NAME:	init.c

FULL NAME:	Initialize Global Variables

ENTRY POINTS:	Initialize()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		none

OUTPUT:		none

REFERENCES:	none

REFERENCED BY:	setup.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include <stdio.h>
#include <Xm/List.h>

#include "define.h"
#include "vardb.h"

char	buffer[1024], FileName[1024], *catList[64], *ProjectDirectory;

char	*getenv();


/* -------------------------------------------------------------------- */
void Initialize(int argc, char *argv[])
{
  ProjectDirectory = (char *)getenv("PROJ_DIR");

  if (argc < 2)
    {
    fprintf(stderr, "Usage: vared var_db_file\n");
    exit(1);
    }

  if (atoi(argv[1]) > 100)
    sprintf(FileName, "%s/%s/VarDB", ProjectDirectory, argv[1]);
  else
    strcpy(FileName, argv[1]);

  catList[0] = NULL;
  OpenNewFile_OK(NULL, NULL, NULL);

}	/* END INITIALIZE */

/* END INIT.C */
