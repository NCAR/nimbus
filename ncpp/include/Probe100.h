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
  float	PLWfac, DBZfac, DENS, DOF, beamDiameter;
  float	totalConcen, dbar, plwc, disp, dbz;

  int	tasIdx;						// Inputs
  int	concIdx, lwIdx, dbzIdx, dbarIdx, dispIdx;	// Outputs

};	// END PROBE100.H

#endif
