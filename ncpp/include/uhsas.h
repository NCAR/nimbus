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

#include "define.h"
#include <netcdf.hh>

#include "Probe.h"


/* -------------------------------------------------------------------- */
class UHSAS : public Probe
{
public:
	UHSAS(NcFile *file, NcVar *av);

  void	ComputeConcentration(float *accum, float *conc, long countV[],
		const std::vector<float *> & otherVarData);

private:
  float	totalConcen, dbar, pvol, disp;

  int	flowIdx;			// Inputs
  int	concIdx, dbarIdx, dispIdx, volIdx;	// Outputs

};	// END UHSAS.H

#endif
