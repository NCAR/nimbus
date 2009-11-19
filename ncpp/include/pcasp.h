/*
-------------------------------------------------------------------------
OBJECT NAME:	pcasp.h

FULL NAME:	ASAS/PCASP

TYPE:		Derived class

COPYRIGHT:	University Corporation for Atmospheric Research, 1999
-------------------------------------------------------------------------
*/

#ifndef PCASP_H
#define PCASP_H

#include "Probe.h"


/* -------------------------------------------------------------------- */
class PCASP : public Probe
{
public:
	PCASP(NcFile *file, NcVar *av);

  void	ComputeConcentration(float *accum, float *conc, long countV[],
		const std::vector<float *> & otherVarData);

private:
  float	_totalConcen, _dbar, _pvol, _disp;

  int	_actIdx, _flowIdx;			// Inputs
  int	_concIdx, _dbarIdx, _dispIdx, _volIdx;	// Outputs

};	// END PCASP.H

#endif
