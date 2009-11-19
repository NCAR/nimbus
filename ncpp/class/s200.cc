/*
-------------------------------------------------------------------------
OBJECT NAME:	s200.cc

FULL NAME:	DMT ASAS/PCASP

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 2001
-------------------------------------------------------------------------
*/

#include "s200.h"


/* -------------------------------------------------------------------- */
S200::S200(NcFile *file, NcVar *av) : Probe(file, av)
{
  _concIdx = _dispIdx = _dbarIdx = _volIdx = -1;

  for (int i = 0; i < _otherVars.size(); ++i)
    {
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
void S200::ComputeConcentration(float *accum, float *conc, long countV[],
	const std::vector<float *> & otherVarData)
{
  int		i, bin;
  std::vector<float> dia;
  float		*counts, *concentration;
  float		*flow;

  flow = otherVarData[_flowIdx];

  for (i = 0; i < countV[0] * countV[1]; ++i)
    {
    counts = &accum[i * VectorLength()];
    concentration = &conc[i * VectorLength()];

    for (bin = FirstBin(); bin <= LastBin(); ++bin)
      _sampleVolume[bin] = flow[i];

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

/* END S200.CC */
