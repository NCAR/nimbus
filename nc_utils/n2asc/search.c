/*
-------------------------------------------------------------------------
OBJECT NAME:	search.c

FULL NAME:	Searches

ENTRY POINTS:	SearchTable()
		SearchTableSansLocation()

DESCRIPTION:	Search for target in list.  The last pointer in the list
		array must be NULL.  It only compares the first word of
		what the list items points to.

INPUT:		An array of pointers and target pointer.

OUTPUT:		pointer to located item or NULL

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2005
-------------------------------------------------------------------------
*/

#include <ctype.h>

#include "define.h"


/* -------------------------------------------------------------------- */
int SearchTable(std::vector<VARTBL*>& table, int ntable, char target[])
{
  int	beg, mid, end, rc;

  if (ntable == 0)
    return(ERR);

  beg = 0;
  end = ntable - 1;

  do
    {
    mid = (end + beg) >> 1;

    if ((rc = strcmp(target, table[mid]->name.c_str())) == 0)
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
int SearchTableSansLocation(std::vector<VARTBL*>& table, int ntable, char target[])
{
  int     i, rc;
  char    *p, c = '\0';
 
  for (i = 0; i < ntable; ++i)
    {
    if ( (p = strchr(table[i]->name.c_str(), '_')) )
      {
      c = *p;
      *p = '\0';
      }
 
    rc = strcmp(target, table[i]->name.c_str());
 
    if (p)
      *p = c;
 
 
    if (rc == 0)
      return(i);
      }
 
  return(ERR);

}	/* END SEARCHTABLESANSLOCATION */

/* END SEARCH.C */
