/*
-------------------------------------------------------------------------
OBJECT NAME:	chownDP.c

FULL NAME:	Change ownership for nimbus support files in hl proj/###

ENTRY POINTS:	main

STATIC FNS:	none

DESCRIPTION:	

INPUT:		Project directory

OUTPUT:		Modified ownerships.

REFERENCES:	none

REFERENCED BY:	DPinit script in $WINDS/scripts

NOTES:		

COPYRIGHT:	University Corporation for Atmospheric Research, 1995
-------------------------------------------------------------------------
*/

#include <errno.h>
#include <pwd.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

char	buffer[2048];

main(int argc, char *argv[])
{
  struct passwd	*pwEnt;
  struct stat	statBuf;

  if (argc < 2)
    {
    fprintf(stderr, "chownDP: Usage chownDP $PROJ_DIR/###\n");
    exit(1);
    }


  pwEnt = getpwnam("nimbus");

  sprintf(buffer, "%s", argv[1]);
  stat(buffer, &statBuf);
  if (chown(buffer, pwEnt->pw_uid, statBuf.st_gid) != 0)
    fprintf(stderr, "chownDP: chown failure, for %s.\n", buffer);

  sprintf(buffer, "%s/Categories", argv[1]);
  stat(buffer, &statBuf);
  if (chown(buffer, pwEnt->pw_uid, statBuf.st_gid) != 0)
    fprintf(stderr, "chownDP: chown failure, for %s.\n", buffer);

  sprintf(buffer, "%s/Defaults", argv[1]);
  stat(buffer, &statBuf);
  if (chown(buffer, pwEnt->pw_uid, statBuf.st_gid) != 0)
    fprintf(stderr, "chownDP: chown failure, for %s.\n", buffer);

  sprintf(buffer, "%s/DependTable", argv[1]);
  stat(buffer, &statBuf);
  if (chown(buffer, pwEnt->pw_uid, statBuf.st_gid) != 0)
    fprintf(stderr, "chownDP: chown failure, for %s.\n", buffer);

  sprintf(buffer, "%s/Despike", argv[1]);
  stat(buffer, &statBuf);
  if (chown(buffer, pwEnt->pw_uid, statBuf.st_gid) != 0)
    fprintf(stderr, "chownDP: chown failure, for %s.\n", buffer);

  sprintf(buffer, "%s/Lags", argv[1]);
  stat(buffer, &statBuf);
  if (chown(buffer, pwEnt->pw_uid, statBuf.st_gid) != 0)
    fprintf(stderr, "chownDP: chown failure, for %s.\n", buffer);

  sprintf(buffer, "%s/PMSspecs", argv[1]);
  stat(buffer, &statBuf);
  if (chown(buffer, pwEnt->pw_uid, statBuf.st_gid) != 0)
    fprintf(stderr, "chownDP: chown failure, for %s.\n", buffer);

  sprintf(buffer, "%s/LowRateVars", argv[1]);
  stat(buffer, &statBuf);
  if (chown(buffer, pwEnt->pw_uid, statBuf.st_gid) != 0)
    fprintf(stderr, "chownDP: chown failure, for %s.\n", buffer);

  sprintf(buffer, "%s/Production", argv[1]);
  stat(buffer, &statBuf);
  if (chown(buffer, pwEnt->pw_uid, statBuf.st_gid) != 0)
    fprintf(stderr, "chownDP: chown failure, for %s.\n", buffer);

  sprintf(buffer, "%s/Production/FlightGroups", argv[1]);
  stat(buffer, &statBuf);
  if (chown(buffer, pwEnt->pw_uid, statBuf.st_gid) != 0)
    fprintf(stderr, "chownDP: chown failure, for %s.\n", buffer);

  sprintf(buffer, "%s/VarDB", argv[1]);
  stat(buffer, &statBuf);
  if (chown(buffer, pwEnt->pw_uid, statBuf.st_gid) != 0)
    fprintf(stderr, "chownDP: chown failure, for %s.\n", buffer);

}	/* END MAIN */

/* END CHOWNDP.C */
