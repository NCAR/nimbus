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

COPYRIGHT:	University Corporation for Atmospheric Research, 2000
-------------------------------------------------------------------------
*/

#include "define.h"

char	buffer[1024], FileName[1024], *DataDirectory;
Fl	flInfo;

char	*getenv();


/* -------------------------------------------------------------------- */
void Initialize(int argc, char *argv[])
{
  DataDirectory = (char *)getenv("DATA_DIR");

  if (argc > 1)
    {
    strcpy(FileName, argv[1]);
    OpenNewFile_OK(NULL, NULL, NULL);
    }

}	/* END INITIALIZE */

/* END INIT.C */
