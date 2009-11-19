/*
-------------------------------------------------------------------------
OBJECT NAME:	uhsas.h

FULL NAME:	UHSAS

TYPE:		Derived class

COPYRIGHT:	University Corporation for Atmospheric Research, 2001
-------------------------------------------------------------------------
*/

#ifndef UHSAS_H
#define UHSAS_H

#include "Probe.h"


/* -------------------------------------------------------------------- */
class UHSAS : public Probe
{
public:
	UHSAS(NcFile *file, NcVar *av);

  void	ComputeConcentration(float *accum, float *conc, long countV[],
		const std::vector<float *> & otherVarData);

private:
  float	_totalConcen, _dbar, _pvol, _disp;

  int	_flowIdx;				// Inputs
  int	_concIdx, _dbarIdx, _dispIdx, _volIdx;	// Outputs

};	// END UHSAS.H

#endif
