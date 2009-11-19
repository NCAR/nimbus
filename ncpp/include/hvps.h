/*
-------------------------------------------------------------------------
OBJECT NAME:	hvps.h

FULL NAME:	HVPS

TYPE:		Derived class

COPYRIGHT:	University Corporation for Atmospheric Research, 2003
-------------------------------------------------------------------------
*/

#ifndef HVPS_H
#define HVPS_H

#include "define.h"
#include <netcdf.hh>

#include "Probe.h"


/* -------------------------------------------------------------------- */
class HVPS : public Probe
{
public:
	HVPS(NcFile *file, NcVar *av);

  void	ComputeConcentration(float *accum, float *conc, long countV[],
		const std::vector<float *> & otherVarData);

private:
  float	_PLWfac, _DBZfac, _DENS;
  float	_totalConcen, _dbar, _plwc, _dbz, _disp;

  int	_tasIdx;				// Inputs
  int	_concIdx, _dbarIdx, _lwIdx, _dispIdx, _dbzIdx;	// Outputs

};	// END HVPS.H

#endif
