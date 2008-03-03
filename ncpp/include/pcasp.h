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

#include "define.h"
#include <netcdf.hh>

#include "Probe.h"


/* -------------------------------------------------------------------- */
class PCASP : public Probe
{
public:
	PCASP(NcFile *file, NcVar *av);

  void	ComputeConcentration(float *accum, float *conc, long countV[],
		const std::vector<float *> & otherVarData);

private:
  float	totalConcen, dbar, pvol, disp;

  int	actIdx, flowIdx;			// Inputs
  int	concIdx, dbarIdx, dispIdx, volIdx;	// Outputs

};	// END PCASP.H

#endif
