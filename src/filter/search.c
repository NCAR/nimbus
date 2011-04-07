/*
-------------------------------------------------------------------------
OBJECT NAME:	search.c

FULL NAME:	Searches

ENTRY POINTS:	SearchList()

DESCRIPTION:	Search for target in list.  The last pointer in the list
		array must be NULL.  It only compares the first word of
		what the list items points to.

NOTES:		See nimbus.h for other searches.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2011
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include <cctype>

/* -------------------------------------------------------------------- */
char *SearchList(char **list, const char target[])
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
