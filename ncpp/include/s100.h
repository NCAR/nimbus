/*
-------------------------------------------------------------------------
OBJECT NAME:	s100.h

FULL NAME:	DMT modified FSSP-100

TYPE:		Derived class

COPYRIGHT:	University Corporation for Atmospheric Research, 2000
-------------------------------------------------------------------------
*/

#ifndef S100_H
#define S100_H

#include "Probe100.h"


/* -------------------------------------------------------------------- */
class S100 : public Probe100
{
public:
	S100(NcFile *file, NcVar *av);

  friend class EditDiameters;

  void	ComputeConcentration(float *accum, float *conc, long countV[],
		const std::vector<float *> & otherVarData);
  float CellSize(int idx) const	{ return(diameter[idx + (range << 4)]); }
  float BinWidth(int idx) const	{ return(binWidth[idx + (range << 4)]); }

  void	SetRange(int newRange)	{ range = (newRange < 0 ? 0 : newRange); }

private:
  int	range, rangeIdx;

  int	rejATIdx, oFlowIdx;			// Inputs

};	// END PROBE100.H

#endif
