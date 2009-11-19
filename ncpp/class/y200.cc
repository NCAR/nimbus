/*
-------------------------------------------------------------------------
OBJECT NAME:	y200.cc

FULL NAME:	200Y Class

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1999
-------------------------------------------------------------------------
*/

#include "y200.h"


/* -------------------------------------------------------------------- */
Y200::Y200(NcFile *file, NcVar *av) : Probe200(file, av)
{
  NcAtt *attr;

  if ((attr = _avar->get_att("nDiodes")))
    _nDiodes = attr->as_int(0);
  else
    _nDiodes = 24;

  if ((attr = _avar->get_att("ArmDistance")))
    _armDistance = attr->as_float(0);
  else
    _armDistance = 263.0;

  ComputeWidths();

}	/* END CONSTRUCTOR */

/* END Y200.CC */
