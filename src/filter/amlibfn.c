/*
-------------------------------------------------------------------------
OBJECT NAME:	amlibfn.c

FULL NAME:	

ENTRY POINTS:	SearchDERIVEFTNS()
		SetLookupSuffix()
		RunAMLIBinitializers()

DESCRIPTION:	This module performs all lookups into the list of variables
		that have xlate or compute functions (deriveftns[]).

REFERENCED BY:	hdr_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2012
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "amlib.h"
#include "amlibfn.h"

static const char *suffix;

/* -------------------------------------------------------------------- */
void RunAMLIBinitializers()
{
  ILOG(("RunAMLIBinitializers"));

  for (size_t i = 0; i < raw.size(); ++i)
    if (raw[i]->Initializer)
      (*raw[i]->Initializer)(raw[i]);

  for (size_t i = 0; i < derived.size(); ++i)
    if (derived[i]->Initializer)
      (*derived[i]->Initializer)(derived[i]);

}	/* END RUNAMLIBINITIALIZERS */

/* -------------------------------------------------------------------- */
void SetLookupSuffix(const char *new_suffix)
{
  suffix = new_suffix;
}

/* -------------------------------------------------------------------- */
int SearchDERIVEFTNS(const char target[])
{
  int		i;
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

  for (i = 0; i < n_variables; ++i)
    if (strcmp(target, deriveftns[i].name) == 0)
      return(i);

  return(ERR);

}	/* END SEARCHDERIVEFTNS */

/* END AMLIBFN.C */
