/*
-------------------------------------------------------------------------
OBJECT NAME:	fssp.cc

FULL NAME:	FSSP-100 Class

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1999
-------------------------------------------------------------------------
*/

#include "fssp.h"


/* -------------------------------------------------------------------- */
FSSP::FSSP(NcFile *file, NcVar *av) : Probe100(file, av)
{
  NcAtt		*attr;

  if ((attr = _cvar->get_att("BeamDiameter")) || (attr = _avar->get_att("BeamDiameter")))
    _beamDiameter = attr->as_float(0);
  else
    _beamDiameter = 0.19;

  if ((attr = _cvar->get_att("DepthOfField")) || (attr = _avar->get_att("DepthOfField")))
    _DOF = attr->as_float(0);
  else
    _DOF = 2.81;

  _range = 0;	// For FSSP, all other probes must be 0.

  for (int i = 0; i < _otherVars.size(); ++i)
    {
    if (strncmp(_otherVars[i]->name(), "FACT", 4) == 0)
      _actIdx = i;

    if (strncmp(_otherVars[i]->name(), "FBMFR", 5) == 0)
      _bmFrIdx = i;
    }

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void FSSP::ComputeConcentration(float *accum, float *conc, long countV[],
	const std::vector<float *> & otherVarData)
{
  int	i, bin;
  float	*dia, *counts, *concentration;
  float	*tas, *fbmfr, *activity, sampleArea;
  double vol;

  tas = otherVarData[_tasIdx];
  activity = otherVarData[_actIdx];
  fbmfr = otherVarData[_bmFrIdx];

  sampleArea = _DOF * _beamDiameter;

  for (i = 0; i < countV[0] * countV[1]; ++i)
    {
    counts = &accum[i * VectorLength()];
    concentration = &conc[i * VectorLength()];

    vol = tas[i] / DataRate() * sampleArea * fbmfr[i] *
				(1.0 - (activity[i] * DataRate() * 0.71));

    for (bin = FirstBin(); bin <= LastBin(); ++bin)
      _sampleVolume[bin] = (float)vol;

    dia = &_midPointDiam[_range << 4];

#define PLWC
#define DBZ

#include "pms1d_cv"

    otherVarData[_concIdx][i] = _totalConcen;
    otherVarData[_lwIdx][i] = _plwc;
    otherVarData[_dbarIdx][i] = _dbar;
    otherVarData[_dispIdx][i] = _disp;
    otherVarData[_dbzIdx][i] = _dbz;
    }

}	/* END COMPUTECONCENTRATION */

/* END FSSP.CC */
