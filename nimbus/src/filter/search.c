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


var_match_name::
var_match_name (const std::string& target) :
  _target (target)
{}

bool
var_match_name::
operator()(var_base* vb) const
{
  return _target == vb->name;
}

var_match_name_sans_location::
var_match_name_sans_location (const std::string& target) :
  _target (target)
{
}

bool
var_match_name_sans_location::
operator()(var_base* vb) const
{
  std::string match = vb->name;
  return _target == match.substr(0, match.find('_'));
}


/* END SEARCH.C */
