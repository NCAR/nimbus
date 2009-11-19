/*
-------------------------------------------------------------------------
OBJECT NAME:	hvps.cc

FULL NAME:	HVPS Class

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 2003
-------------------------------------------------------------------------
*/

#include "hvps.h"


/* -------------------------------------------------------------------- */
HVPS::HVPS(NcFile *file, NcVar *av) : Probe(file, av)
{
  _tasIdx = _concIdx = _dbarIdx = _lwIdx = _dispIdx = _dbzIdx = -1;

  for (int i = 0; i < _otherVars.size(); ++i)
    {
    if (strcmp(_otherVars[i]->name(), "TASX") == 0)
      _tasIdx = i;

    if (strncmp(_otherVars[i]->name(), "CONC", 4) == 0)
      _concIdx = i;

    if (strncmp(_otherVars[i]->name(), "PLWC", 4) == 0)
      _lwIdx = i;

    if (strncmp(_otherVars[i]->name(), "DBAR", 4) == 0)
      _dbarIdx = i;

    if (strncmp(_otherVars[i]->name(), "DISP", 4) == 0)
      _dispIdx = i;

    if (strncmp(_otherVars[i]->name(), "DBZ", 3) == 0)
      _dbzIdx = i;
    }

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void HVPS::ComputeConcentration(float *accum, float *conc, long countV[],
	const std::vector<float *> & otherVarData)
{
  int	i, bin;
  std::vector<float> dia;
  float	*counts, *concentration, *tas;
  double vol;

  tas = otherVarData[_tasIdx];

  for (i = 0; i < countV[0] * countV[1]; ++i)
    {
    counts = &accum[i * VectorLength()];
    concentration = &conc[i * VectorLength()];

    vol = tas[i] / DataRate() * 200 * 203 * (256-80) * 1.0e-5;

    for (bin = FirstBin(); bin <= LastBin(); ++bin)
      _sampleVolume[bin] = vol;

    dia = _midPointDiam;

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

/* END HVPS.CC */
