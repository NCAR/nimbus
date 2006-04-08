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


/* -------------------------------------------------------------------- */
void MakeProjectFileName(char file[], const char format[])
{
  char *platform = 0;

  if (cfg.Aircraft() == Config::C130)
    platform = "C130";
  if (cfg.Aircraft() == Config::HIAPER)
    platform = "GV";
  if (cfg.Aircraft() == Config::ELECTRA)
    platform = "Electra";
  if (cfg.Aircraft() == Config::NRL_P3)
    platform = "NRL_P3";
  if (cfg.Aircraft() == Config::KINGAIR)
    platform = "KingAir";
  if (cfg.Aircraft() == Config::TADS)
    platform = "TADS";

  if (platform == 0)
  {
    fprintf(stderr, "popen.c:MakeProjectFileName: No platform, fatal.\n");
    exit(1);
  }

  (void)sprintf(file, format,
	ProjectDirectory, cfg.ProjectNumber().c_str(), platform);
}

/* -------------------------------------------------------------------- */
FILE *OpenProjectFile(const char filename[], const char mode[], int action)
{
  FILE	*fp;
  char	file[MAXPATHLEN];

  if (filename == NULL || filename[0] == '\0')
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
int AccessProjectFile(const char filename[], const char mode[])
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
