/*
-------------------------------------------------------------------------
OBJECT NAME:	s200.h

FULL NAME:	DMT ASAS/PCASP

TYPE:		Derived class

COPYRIGHT:	University Corporation for Atmospheric Research, 2001
-------------------------------------------------------------------------
*/

#ifndef S200_H
#define S200_H

#include "Probe.h"


/* -------------------------------------------------------------------- */
class S200 : public Probe
{
public:
	S200(NcFile *file, NcVar *av);

  void	ComputeConcentration(float *accum, float *conc, long countV[],
		const std::vector<float *> & otherVarData);

private:
  float	_totalConcen, _dbar, _pvol, _disp;

  int	_flowIdx;				// Inputs
  int	_concIdx, _dbarIdx, _dispIdx, _volIdx;	// Outputs

};	// END S200.H

#endif
