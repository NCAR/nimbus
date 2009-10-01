/*
-------------------------------------------------------------------------
OBJECT NAME:	fssp.h

FULL NAME:	FSSP-100

TYPE:		Derived class

COPYRIGHT:	University Corporation for Atmospheric Research, 1999
-------------------------------------------------------------------------
*/

#ifndef FSSP_H
#define FSSP_H

#include "Probe100.h"


/* -------------------------------------------------------------------- */
class FSSP : public Probe100
{
public:
	FSSP(NcFile *file, NcVar *av);

  friend class EditDiameters;

  void	ComputeConcentration(float *accum, float *conc, long countV[],
		const std::vector<float *> & otherVarData);
  float CellSize(int idx) const	{ return(diameter[idx + (range << 4)]); }
  float BinWidth(int idx) const	{ return(binWidth[idx + (range << 4)]); }

  void	SetRange(int newRange)	{ range = (newRange < 0 ? 0 : newRange); }

private:
  int	range, rangeIdx;

  int	actIdx, bmFrIdx;			// Inputs

};	// END FSSP.H

#endif
