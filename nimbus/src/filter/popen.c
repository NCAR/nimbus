/*
-------------------------------------------------------------------------
OBJECT NAME:	popen.c

FULL NAME:	Open Project File

ENTRY POINTS:	OpenProjectFile()
		AccessProjectFile()

DESCRIPTION:	Open a Project file and return a file pointer to it.  It
		is the user's responsibilty to fclose the file.  The
		filename is actually one of the format strings from
		winc/files.h.  Mode is the same as that for fopen(3).
		By specifing either RETURN or EXIT OpenProjectFile will
		take that action if the open fails.

INPUT:		Filename, Mode, action

OUTPUT:		FILE	*fp

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2006
-------------------------------------------------------------------------
*/

#include <sys/param.h>
#include "nimbus.h"
#include "decode.h"
#include <raf/ac.h>


/* -------------------------------------------------------------------- */
void MakeProjectFileName(char file[], const std::string& format)
{
  char platform[128];

  InitAircraftSpecs(AIRCRAFT.c_str());
  GetAircraftFullyQualifiedName(cfg.TailNumber().c_str(), platform);
  ReleaseAircraftSpecs();

  if (strlen(platform) == 0)
  {
    fprintf(stderr, "popen.c:MakeProjectFileName: No platform, fatal.\n");
    exit(1);
  }

  (void)sprintf(file, format.c_str(),
	cfg.ProjectDirectory().c_str(), cfg.ProjectNumber().c_str(), platform);
}

/* -------------------------------------------------------------------- */
FILE *OpenProjectFile(const std::string& filename, const char mode[], int action)
{
  FILE	*fp;
  char	file[MAXPATHLEN];

  if (filename.length() == 0)
    return(NULL);

  MakeProjectFileName(file, filename);

  if ((fp = fopen(file, mode)) == NULL && action == EXIT)
  {
    char msg[MAXPATHLEN];

    sprintf(msg, "ProjectOpen: can't open %s", file);
    perror(msg);
    exit(ERROR);
  }

  return(fp);

}	/* END OPENPROJECTFILE */

/* -------------------------------------------------------------------- */
int AccessProjectFile(const std::string& filename, const char mode[])
{
  FILE	*fp;
  int	accessable = false;
  char	file[MAXPATHLEN];

  MakeProjectFileName(file, filename);

  if ((fp = fopen(file, mode)) != NULL)
  {
    accessable = true;
    fclose(fp);
  }

  return(accessable);

}	/* END ACCESSPROJECTFILE */

/* END POPEN.C */
