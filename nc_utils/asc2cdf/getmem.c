/*
-------------------------------------------------------------------------
OBJECT NAME:	getmem.c

FULL NAME:	Get Memory

ENTRY POINTS:	GetMemory()

STATIC FNS:	none

DESCRIPTION:	malloc data space, report error and exit if necassary

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include <cstdio>
#include <cstdlib>


/* -------------------------------------------------------------------- */
void *GetMemory(size_t nbytes)
{
  char	*p;

  if ((p = (char *)malloc(nbytes)) == NULL)
  {
    fprintf(stderr, "Memory allocation failure, exiting.\n");
    exit(1);
  }

  return(p);

}	/* END GETMEMORY */

/* END GETMEM.C */
