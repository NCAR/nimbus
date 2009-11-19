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
  float CellSize(int idx) const	{ return(_diameter[idx + (_range << 4)]); }
  float BinWidth(int idx) const	{ return(_binWidth[idx + (_range << 4)]); }

  void	SetRange(int newRange)	{ _range = (newRange < 0 ? 0 : newRange); }

private:
  int	_range, _rangeIdx;

  int	_rejATIdx, _oFlowIdx;			// Inputs

};	// END PROBE100.H

#endif
