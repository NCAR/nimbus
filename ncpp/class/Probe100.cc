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

  if ((attr = _cvar->get_att("Density")) || (attr = _avar->get_att("Density")))
    _DENS = attr->as_float(0);
  else
    _DENS = 1.0;

  if ((attr = _cvar->get_att("PLWfactor")) || (attr = _avar->get_att("PLWfactor")))
    _PLWfac = attr->as_float(0);
  else
    _PLWfac = 1.0e-6;

  if ((attr = _cvar->get_att("DBZfactor")) || (attr = _avar->get_att("DBZfactor")))
    _DBZfac = attr->as_float(0);
  else
    _DBZfac = 1.0e6;

  for (int i = 0; i < _otherVars.size(); ++i)
    {
    if (strcmp(_otherVars[i]->name(), "TASX") == 0)
      _tasIdx = i;

    if (strncmp(_otherVars[i]->name(), "CONC", 4) == 0)
      _concIdx = i;

    if (strncmp(_otherVars[i]->name(), "PLWC", 4) == 0)
      _lwIdx = i;

    if (strncmp(_otherVars[i]->name(), "DBZ", 3) == 0)
      _dbzIdx = i;

    if (strncmp(_otherVars[i]->name(), "DBAR", 4) == 0)
      _dbarIdx = i;

    if (strncmp(_otherVars[i]->name(), "DISP", 4) == 0)
      _dispIdx = i;
    }

}	/* END CONSTRUCTOR */

// END PROBE100.CC
