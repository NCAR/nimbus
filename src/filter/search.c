/*
-------------------------------------------------------------------------
OBJECT NAME:	search.c

FULL NAME:	Searches

ENTRY POINTS:	SearchTable()
		SearchTableSansLocation()
		SearchList()
		LinearSearchTable()

DESCRIPTION:	Search for target in list.  The last pointer in the list
		array must be NULL.  It only compares the first word of
		what the list items points to.

INPUT:		An array of pointers and target pointer.

OUTPUT:		pointer to located item or NULL

REFERENCES:	none

REFERENCED BY:	hdr_decode.c, order.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include <ctype.h>

#include "nimbus.h"


/* -------------------------------------------------------------------- */
int SearchTable(char *table[], int ntable, char target[])
{
  int	beg, mid, end, rc;

  if (ntable == 0)
    return(ERR);

  beg = 0;
  end = ntable - 1;

  do
    {
    mid = (end + beg) >> 1;

    if ((rc = strcmp(target, table[mid])) == 0)
      return(mid);

    if (rc < 0)
      end = mid - 1;
    else
      beg = mid + 1;
    }
  while (beg <= end);

  return(ERR);

}	/* END SEARCHTABLE */

/* -------------------------------------------------------------------- */
int SearchTableSansLocation(char *table[], int ntable, char target[])
{
  int	i, rc;
  char	*p, c;

  for (i = 0; i < ntable; ++i)
    {
    if ( (p = strchr(table[i], '_')) )
      {
      c = *p;
      *p = '\0';
      }

    rc = strcmp(target, table[i]);

    if (p)
      *p = c;

    if (rc == 0)
      return(i);
    }

  return(ERR);

}	/* END SEARCHTABLESANSLOCATION */

/* -------------------------------------------------------------------- */
char *SearchList(char **list, char target[])
{
  int	len = strlen(target);

  while (*list)
    if (strncmp(*list, target, len) == 0 &&
        (isspace((*list)[len]) || (*list)[len] == '\0'))
      break;
    else
      ++list;

  return(*list);

}	/* END SEARCHLIST */

/* -------------------------------------------------------------------- */
int LinearSearchTable(char **list, int n, char target[])
{
  int	i;

  for (i = 0; i < n; ++i)
    if (strcmp(list[i], target) == 0)
      return(i);

  return(ERR);

}	/* END LINEARSEARCHVATBL */

/* END SEARCH.C */
