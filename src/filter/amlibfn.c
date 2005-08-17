/*
-------------------------------------------------------------------------
OBJECT NAME:	amlibfn.c

FULL NAME:	

ENTRY POINTS:	SearchDERIVEFTNS()
		SetLookupSuffix()
		RunAMLIBinitializers()

STATIC FNS:	none

DESCRIPTION:	This module performs all lookups into the list of variables
		that have xlate or compute functions (deriveftns[]).

INPUT:		target variable

OUTPUT:		index / ERR

REFERENCES:	none

REFERENCED BY:	hdr_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "amlib.h"
#include "amlibfn.h"

static char	*suffix;

/* -------------------------------------------------------------------- */
void RunAMLIBinitializers()
{
  for (size_t i = 0; i < raw.size(); ++i)
    if (raw[i]->Initializer)
      (*raw[i]->Initializer)(raw[i]);

  for (size_t i = 0; i < derived.size(); ++i)
    if (derived[i]->Initializer)
      (*derived[i]->Initializer)(derived[i]);

}	/* END RUNAMLIBINITIALIZERS */

/* -------------------------------------------------------------------- */
void SetLookupSuffix(char *new_suffix)
{
  suffix = new_suffix;
}

/* -------------------------------------------------------------------- */
int SearchDERIVEFTNS(const char target[])
{
  int		beg, mid, end, rc;
  char		name[32];
  static int	n_variables = 0;
  static bool	firstTime = true;

  if (firstTime)
    {
    firstTime = false;
    SetLookupSuffix((char *)NULL);

    while (deriveftns[n_variables].name)
      ++n_variables;
    }

  if (suffix)
    {
    strcpy(name, target);
    strcat(name, suffix);
    target = name;
    }

  beg = 0;
  end = n_variables;

  do
    {
    mid = (end + beg) >> 1;

    if ((rc = strcmp(target, deriveftns[mid].name)) == 0)
      return(mid);

    if (rc < 0)
      end = mid - 1;
    else
      beg = mid + 1;
    }
  while (beg <= end);

  return(ERR);

}	/* END SEARCHDERIVEFTNS */

/* END AMLIBFN.C */
