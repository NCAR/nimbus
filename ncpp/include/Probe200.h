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

  float EffectiveAreaWidth() const	{ return _resolution * _nDiodes; }


protected:
  virtual void	ComputeWidths();

  /**
   * Compute depth of field for the 260x, 200x, and 200y probes.
   */
  virtual void	ComputeDOF200(float tasx);

  static const float	DiodeDiameter;

  int	_nDiodes;
  float _resolution;	// in meters (0.025 or 0.2)

  float	_responseTime, _armDistance, _PLWfac, _DENS, _DBZfac;
  std::vector<float> _radius, _esw, _dof;
  float	_mag, _totalConcen, _dbar, _plwc, _disp, _dbz;

  int	_tasIdx, _deadTimeIdx;				// Inputs
  int	_concIdx, _lwIdx, _dbarIdx, _dispIdx, _dbzIdx;	// Outputs

};	// END PROBE200.H

#endif
