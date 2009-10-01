/*
-------------------------------------------------------------------------
OBJECT NAME:	2D_C.h

FULL NAME:	2D_C

TYPE:		Derived class

COPYRIGHT:	University Corporation for Atmospheric Research, 2000
-------------------------------------------------------------------------
*/

#ifndef TWODC_H
#define TWODC_H

#include "Probe200.h"


/* -------------------------------------------------------------------- */
class TwoDC : public Probe200
{
public:
	TwoDC(NcFile *file, NcVar *av);

  void	ComputeConcentration(float *accum, float *conc, long countV[],
		const std::vector<float *> & otherVarData);

private:

};	// END 2D_C.H

#endif
