/*
-------------------------------------------------------------------------
OBJECT NAME:	forkTape.c

FULL NAME:	exec's discTape

ENTRY POINTS:	main

STATIC FNS:	none

DESCRIPTION:	exec's discTape, leaving it with root UID.

NOTES:		This program must be set-uid root.

COPYRIGHT:	University Corporation for Atmospheric Research, 1996
-------------------------------------------------------------------------
*/

#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

char	buffer[512];

/* -------------------------------------------------------------------- */
main()
{
  int	rc;

  if ((rc = setuid(0)) == (-1))
    fprintf(stderr, "forkTape: setuid failed, errno = %d\n", errno);

  sprintf(buffer, "%s/bin/discTape", getenv("DISC"));
  if ((rc = execl(buffer, buffer, NULL)) == (-1))
    fprintf(stderr, "forkTape: exec failed, errno = %d\n", errno);

  return(rc);

}  /* END MAIN */

/* END FORKTAPE.C */
