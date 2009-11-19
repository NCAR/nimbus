/*
-------------------------------------------------------------------------
OBJECT NAME:	pcasp.cc

FULL NAME:	ASAS/PCASP

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1999
-------------------------------------------------------------------------
*/

#include "pcasp.h"


/* -------------------------------------------------------------------- */
PCASP::PCASP(NcFile *file, NcVar *av) : Probe(file, av)
{
  _concIdx = _dispIdx = _dbarIdx = _volIdx = -1;

  for (int i = 0; i < _otherVars.size(); ++i)
    {
    if (strncmp(_otherVars[i]->name(), "PACT", 4) == 0)
      _actIdx = i;

    if (strncmp(_otherVars[i]->name(), "PFLWC", 5) == 0)
      _flowIdx = i;

    if (strncmp(_otherVars[i]->name(), "CONC", 4) == 0)
      _concIdx = i;

    if (strncmp(_otherVars[i]->name(), "DISP", 4) == 0)
      _dispIdx = i;

    if (strncmp(_otherVars[i]->name(), "DBAR", 4) == 0)
      _dbarIdx = i;

    if (strncmp(_otherVars[i]->name(), "PVOL", 4) == 0)
      _volIdx = i;
    }

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void PCASP::ComputeConcentration(float *accum, float *conc, long countV[],
	const std::vector<float *> & otherVarData)
{
  int		i, bin;
  std::vector<float> dia;
  float		*counts, *concentration;
  float		*flow, *activity;
  double	vol;

  flow = otherVarData[_flowIdx];
  activity = otherVarData[_actIdx];

  for (i = 0; i < countV[0] * countV[1]; ++i)
    {
    counts = &accum[i * VectorLength()];
    concentration = &conc[i * VectorLength()];

    vol = (1.0 - activity[i] * DataRate()) * (flow[i] / DataRate());

    for (bin = FirstBin(); bin <= LastBin(); ++bin)
      _sampleVolume[bin] = (float)vol;

    dia = _midPointDiam;

#define P_VOLUME

#include "pms1d_cv"

    otherVarData[_concIdx][i] = _totalConcen;
    otherVarData[_dispIdx][i] = _disp;
    otherVarData[_dbarIdx][i] = _dbar;

    if (_volIdx >= 0)
      otherVarData[_volIdx][i] = _pvol;
    }

}	/* END COMPUTECONCENTRATION */

/* END PCASP.CC */
