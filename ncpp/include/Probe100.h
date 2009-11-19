/*
-------------------------------------------------------------------------
OBJECT NAME:	Probe100.h

FULL NAME:	Probe100 base class

COPYRIGHT:	University Corporation for Atmospheric Research, 1999-2009
-------------------------------------------------------------------------
*/

#ifndef PROBE100_H
#define PROBE100_H

#include "Probe.h"


/* -------------------------------------------------------------------- */
class Probe100 : public Probe
{
public:
	Probe100(NcFile *file, NcVar *av);

  friend class EditDiameters;


protected:
  float	_PLWfac, _DBZfac, _DENS, _DOF, _beamDiameter;
  float	_totalConcen, _dbar, _plwc, _disp, _dbz;

  int	_tasIdx;					// Inputs
  int	_concIdx, _lwIdx, _dbzIdx, _dbarIdx, _dispIdx;	// Outputs

};	// END PROBE100.H

#endif
