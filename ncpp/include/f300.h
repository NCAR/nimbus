/*
-------------------------------------------------------------------------
OBJECT NAME:	f300.h

FULL NAME:	FSSP-300

TYPE:		Derived class

COPYRIGHT:	University Corporation for Atmospheric Research, 1999
-------------------------------------------------------------------------
*/

#ifndef F300_H
#define F300_H

#include "define.h"
#include <netcdf.hh>

#include "Probe.h"


/* -------------------------------------------------------------------- */
class F300 : public Probe
{
public:
	F300(NcFile *file, NcVar *av);

  void	ComputeConcentration(float *accum, float *conc, long countV[],
		const std::vector<float *> & otherVarData);

private:
  float	_totalConcen, _dbar, _pvol, _disp;

  int	_tasIdx;			// Inputs
  int	_concIdx, _dbarIdx, _volIdx;	// Outputs

};	// END F300.H

#endif
