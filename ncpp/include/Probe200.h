/*
-------------------------------------------------------------------------
OBJECT NAME:	Probe200.h

FULL NAME:	260x, 200x, 200y

TYPE:		Base class

COPYRIGHT:	University Corporation for Atmospheric Research, 1999
-------------------------------------------------------------------------
*/

#ifndef PROBE200_H
#define PROBE200_H

#include "Probe.h"


/* -------------------------------------------------------------------- */
class Probe200 : public Probe
{
public:
	Probe200(NcFile *file, NcVar *av);

  friend class EditDiameters;

  virtual void	ComputeConcentration(float *accum, float *conc, long countV[],
		const std::vector<float *> & otherVarData);

  void	UpdateCellDiams(const int first, const int last, const float *newDiams);

  virtual int   SetEditWindow(Widget txt[]);
  virtual int   ApplyEditWindow(Widget txt[]);

  float EffectiveAreaWidth()	{ return resolution * nDiodes; }


protected:
  void		ComputeWidths(), ComputeDOF(float tasx);

  static const float	diodeDiameter;

  int	nDiodes;
  float resolution;	// in meters (0.025 or 0.2)

  float	responseTime, armDistance, PLWfac, DENS, DBZfac;
  std::vector<float> radius, esw, dof;
  float	mag, totalConcen, dbar, plwc, disp, dbz;

  int	tasIdx;						// Inputs
  int	concIdx, lwIdx, dbarIdx, dispIdx, dbzIdx;	// Outputs

};	// END PROBE200.H

#endif
