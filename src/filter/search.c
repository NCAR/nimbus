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
int SearchTableSansLocation(std::vector<SDITBL *> &table, const char target[])
{
  int	rc;

  for (int i = 0; i < table.size(); ++i)
    {
    char	*p, c;

    if ( (p = strchr(table[i]->name, '_')) )
      {
      c = *p;
      *p = '\0';
      }

    rc = strcmp(target, table[i]->name);

    if (p)
      *p = c;

    if (rc == 0)
      return(i);
    }

  return(ERR);

}	/* END SEARCHTABLESANSLOCATION */

/* -------------------------------------------------------------------- */
int SearchTableSansLocation(std::vector<RAWTBL *> &table, const char target[])
{
  int	rc;

  for (int i = 0; i < table.size(); ++i)
    {
    char	*p, c;

    if ( (p = strchr(table[i]->name, '_')) )
      {
      c = *p;
      *p = '\0';
      }

    rc = strcmp(target, table[i]->name);

    if (p)
      *p = c;

    if (rc == 0)
      return(i);
    }

  return(ERR);

}	/* END SEARCHTABLESANSLOCATION */

/* -------------------------------------------------------------------- */
int SearchTableSansLocation(std::vector<DERTBL *> &table, const char target[])
{
  int	rc;

  for (int i = 0; i < table.size(); ++i)
    {
    char	*p, c;

    if ( (p = strchr(table[i]->name, '_')) )
      {
      c = *p;
      *p = '\0';
      }

    rc = strcmp(target, table[i]->name);

    if (p)
      *p = c;

    if (rc == 0)
      return(i);
    }

  return(ERR);

}	/* END SEARCHTABLESANSLOCATION */

/* -------------------------------------------------------------------- */
int SearchTable(std::vector<SDITBL *> &list, const char target[])
{
  for (int i = 0; i < list.size(); ++i)
    if (strcmp(list[i]->name, target) == 0)
      return(i);

  return(ERR);

}	/* END SEARCHTABLE */

/* -------------------------------------------------------------------- */
int SearchTable(std::vector<RAWTBL *> &list, const char target[])
{
  for (int i = 0; i < list.size(); ++i)
    if (strcmp(list[i]->name, target) == 0)
      return(i);

  return(ERR);

}	/* END SEARCHTABLE */

/* -------------------------------------------------------------------- */
int SearchTable(std::vector<DERTBL *> &list, const char target[])
{
  for (int i = 0; i < list.size(); ++i)
    if (strcmp(list[i]->name, target) == 0)
      return(i);

  return(ERR);

}	/* END SEARCHTABLE */

/* -------------------------------------------------------------------- */
int SearchTable(std::vector<DERTBL *> &list, int n, const char target[])
{
  for (int i = 0; i < n; ++i)
    if (strcmp(list[i]->name, target) == 0)
      return(i);

  return(ERR);

}	/* END SEARCHTABLE */

/* -------------------------------------------------------------------- */
int SearchTable(std::vector<DERTBL *> &list, int s, int e, const char target[])
{
  for (int i = s; i < e; ++i)
    if (strcmp(list[i]->name, target) == 0)
      return(i);

  return(ERR);

}	/* END SEARCHTABLE */

/* END SEARCH.C */
