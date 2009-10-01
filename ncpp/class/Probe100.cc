/*
-------------------------------------------------------------------------
OBJECT NAME:	Probe100.cc

FULL NAME:	Probe100 Class

COPYRIGHT:	University Corporation for Atmospheric Research, 1999-2009
-------------------------------------------------------------------------
*/

#include "Probe100.h"


/* -------------------------------------------------------------------- */
Probe100::Probe100(NcFile *file, NcVar *av) : Probe(file, av)
{
  NcAtt		*attr;

  if ((attr = cvar->get_att("Density")) || (attr = avar->get_att("Density")))
    DENS = attr->as_float(0);
  else
    DENS = 1.0;

  if ((attr = cvar->get_att("PLWfactor")) || (attr = avar->get_att("PLWfactor")))
    PLWfac = attr->as_float(0);
  else
    PLWfac = 1.0e-6;

  if ((attr = cvar->get_att("DBZfactor")) || (attr = avar->get_att("DBZfactor")))
    DBZfac = attr->as_float(0);
  else
    DBZfac = 1.0e6;

  for (int i = 0; i < otherVars.size(); ++i)
    {
    if (strcmp(otherVars[i]->name(), "TASX") == 0)
      tasIdx = i;

    if (strncmp(otherVars[i]->name(), "CONC", 4) == 0)
      concIdx = i;

    if (strncmp(otherVars[i]->name(), "PLWC", 4) == 0)
      lwIdx = i;

    if (strncmp(otherVars[i]->name(), "DBZ", 3) == 0)
      dbzIdx = i;

    if (strncmp(otherVars[i]->name(), "DBAR", 4) == 0)
      dbarIdx = i;

    if (strncmp(otherVars[i]->name(), "DISP", 4) == 0)
      dispIdx = i;
    }

}	/* END CONSTRUCTOR */

// END PROBE100.CC
