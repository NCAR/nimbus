/*
-------------------------------------------------------------------------
OBJECT NAME:	cdp.h

FULL NAME:	CDP

COPYRIGHT:	University Corporation for Atmospheric Research, 1999-2009
-------------------------------------------------------------------------
*/

#ifndef CDP_H
#define CDP_H

#include "Probe100.h"


/* -------------------------------------------------------------------- */
class CDP : public Probe100
{
public:
	CDP(NcFile *file, NcVar *av);

  friend class EditDiameters;

  void	ComputeConcentration(float *accum, float *conc, long countV[],
		const std::vector<float *> & otherVarData);

};	// END CDP.H

#endif
