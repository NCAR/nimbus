/*
-------------------------------------------------------------------------
OBJECT NAME:	init.c

FULL NAME:	Initialize Global Variables

ENTRY POINTS:	Initialize()
		ProcessArgv()

STATIC FNS:		

DESCRIPTION:	

INPUT:		none

OUTPUT:		none

REFERENCES:	none

REFERENCED BY:	nimbus.c, cb_main.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1996
-------------------------------------------------------------------------
*/

#include "hdrbld.h"
#include "ac.h"
#include "pms.h"
#include "vardb.h"


char	*getenv();


/* -------------------------------------------------------------------- */
void PreWidgetInitialize()
{
  char	*p;

  if ((ProjectDirectory = (char *)getenv("PROJ_DIR")) == NULL)
    {
    fprintf(stderr, "Environment variable PROJ_DIR not defined, this is fatal.\n");
    exit(1);
    }

  InitAircraftSpecs("AircraftSpecs");
  GetAircraftList(aircraft);

  if (ProjectNumber)
    {
    FILE	*fp;

    MakeProjectFileName(FileName, HEADER);
    free(ProjectNumber);

    if ((fp = fopen(FileName, "rb")) == NULL)
      {
      fprintf(stderr, "Can't open %s.\n", FileName);
      ProjectNumber = NULL;
      }
    else
      {
      fread((char *)&flightInfo, sizeof(Fl), 1, fp);
      ProjectNumber = flightInfo.prnum;
      fclose(fp);
      }
    }

  nsdi = 0;
  sdi[0] = NULL;
  DSM[0] = NULL;
  Inertial[0] = Inertial[1] = 0;
  GPS[0] = GPS[1] = GPS[2] = 0;


  if (ProjectNumber)
    {
    MakeProjectFileName(buffer, "%s/%s/%s/PMSspecs");
    InitPMSspecs(buffer);
    MakeProjectFileName(buffer, "%s/%s/%s/VarDB");
    InitializeVarDB(buffer);
    p = flightInfo.acraft;
    }
  else
    {
    sprintf(buffer, "%s/Configuration/raf/PMSspecs", ProjectDirectory);
    InitPMSspecs(buffer);
    sprintf(buffer, "%s/Configuration/raf/VarDB", ProjectDirectory);
    InitializeVarDB(buffer);
    p = aircraft[0];
    }

  SetDSMlist(p);

}

/* -------------------------------------------------------------------- */
void PostWidgetInitialize()
{
  ClearCurrentSh(NULL, NULL, NULL);

  if (ProjectNumber)
    OpenNewFile_OK(NULL, NULL, NULL);

}	/* END INITIALIZE */

/* -------------------------------------------------------------------- */
void ProcessArgv(int argc, char **argv)
{
  int		i;

  ProjectNumber = 0;

  for (i = 1; i < argc; ++i)
    {
    if (argv[i][0] != '-')
      {
//      ProjectNumber = (char *)GetMemory(strlen(argv[i]+1));
//      strcpy(ProjectNumber, argv[i]);
      fprintf(stderr, "Project numbers may no longer be entered on the command line.\n");
      continue;
      }

    switch (argv[i][1])
      {
      default:
        fprintf(stderr, "Invalid option %s, ignoring.\n", argv[i]);
      }

    }

}	/* END PROCESSARGV */

/* END INIT.C */
