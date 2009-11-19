/*
-------------------------------------------------------------------------
OBJECT NAME:	s100.cc

FULL NAME:	DMT modified FSSP-100 Class

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 2000
-------------------------------------------------------------------------
*/

#include "s100.h"


/* -------------------------------------------------------------------- */
S100::S100(NcFile *file, NcVar *av) : Probe100(file, av)
{
  NcAtt		*attr;

  if ((attr = _cvar->get_att("BeamDiameter")) || (attr = _avar->get_att("BeamDiameter")))
    _beamDiameter = attr->as_float(0);
  else
    _beamDiameter = 0.19;

  if ((attr = _cvar->get_att("DepthOfField")) || (attr = _avar->get_att("DepthOfField")))
    _DOF = attr->as_float(0);
  else
    _DOF = 3.05;

  _range = 0;	// For FSSP, all other probes must be 0.

  for (int i = 0; i < _otherVars.size(); ++i)
    {
    if (strncmp(_otherVars[i]->name(), "REJAT", 5) == 0)
      _rejATIdx = i;

    if (strncmp(_otherVars[i]->name(), "OVFLW", 5) == 0)
      _oFlowIdx = i;
    }

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void S100::ComputeConcentration(float *accum, float *conc, long countV[],
	const std::vector<float *> & otherVarData)
{
  int	i, j, bin;
  float	*dia, *counts, *concentration, total_cnts;
  float	*tas, *oFlow, *rejAT, sampleArea;
  double vol;

  tas = otherVarData[_tasIdx];
  rejAT = otherVarData[_rejATIdx];
  oFlow = otherVarData[_oFlowIdx];

  sampleArea = _DOF * _beamDiameter;

  for (i = 0; i < countV[0] * countV[1]; ++i)
    {
    counts = &accum[i * VectorLength()];
    concentration = &conc[i * VectorLength()];

    total_cnts = 0.0;
    for (j = FirstBin(); j < LastBin(); ++j)
      total_cnts += accum[j];

    vol = tas[i] / DataRate() * sampleArea;

    if (total_cnts > 0 && rejAT[i] > 0.0)
      vol *= total_cnts / (total_cnts + rejAT[i]);

    for (bin = FirstBin(); bin <= LastBin(); ++bin)
      _sampleVolume[bin] = (float)vol;

    dia = &_midPointDiam[_range << 4];

#define PLWC
#define DBZ

#include "pms1d_cv"

    if (oFlow[i] > 0 && total_cnts > 0 && oFlow[i] < 5000)
      {
      float	ccc = (total_cnts + oFlow[i]) / total_cnts;

      for (bin = FirstBin(); bin <= LastBin(); ++bin)
        concentration[bin] *= ccc;

      _totalConcen *= ccc;
      }

    otherVarData[_concIdx][i] = _totalConcen;
    otherVarData[_lwIdx][i] = _plwc;
    otherVarData[_dbarIdx][i] = _dbar;
    otherVarData[_dispIdx][i] = _disp;
    otherVarData[_dbzIdx][i] = _dbz;
    }

}	/* END COMPUTECONCENTRATION */

/* END S100.CC */
