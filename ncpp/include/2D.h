/*
-------------------------------------------------------------------------
OBJECT NAME:	2D.h

FULL NAME:	2D

TYPE:		Derived class

COPYRIGHT:	University Corporation for Atmospheric Research, 2000
-------------------------------------------------------------------------
*/

#ifndef TWOD_H
#define TWOD_H

#include "Probe200.h"


/* -------------------------------------------------------------------- */
class TwoD : public Probe200
{
public:
	TwoD(NcFile *file, NcVar *av);

  void	ComputeConcentration(float *accum, float *conc, long countV[],
		const std::vector<float *> & otherVarData);

};	// END 2D.H


class TwoDC : public TwoD
{
public:
	TwoDC(NcFile *file, NcVar *av);
};

class TwoDP : public TwoD
{
public:
	TwoDP(NcFile *file, NcVar *av);
};
#endif
