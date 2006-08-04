/*
-------------------------------------------------------------------------
OBJECT NAME:	init.c

FULL NAME:	Initialize Global Variables

ENTRY POINTS:	Initialize()

STATIC FNS:	none

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2006
-------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <Xm/List.h>

#include "define.h"
#include "vardb.h"

char	buffer[1024], FileName[1024], *catList[128], *ProjectDirectory,
	*stdNameList[512];

void ShowError(char msg[]);
void WarnUser(char msg[], void (*okCB)(Widget, XtPointer, XtPointer), void (*cancelCB)(Widget, XtPointer, XtPointer));

const char * getAircraftName(int num);

/* -------------------------------------------------------------------- */
void Initialize(int argc, char *argv[])
{
  int	pnum = atoi(argv[1]);

  ProjectDirectory = (char *)getenv("PROJ_DIR");

  if (argc < 2)
    {
    fprintf(stderr, "Usage: vared var_db_file\n");
    exit(1);
    }

  if (pnum > 99)
    sprintf(FileName, "%s/%d/%s/VarDB", ProjectDirectory, pnum, getAircraftName(pnum));
  else
    strcpy(FileName, argv[1]);

  catList[0] = NULL;
  stdNameList[0] = NULL;
  OpenNewFile_OK(NULL, NULL, NULL);

}	/* END INITIALIZE */

/* END INIT.C */
