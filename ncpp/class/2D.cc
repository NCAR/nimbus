/*
-------------------------------------------------------------------------
OBJECT NAME:	2D.cc

FULL NAME:	2D Class

DESCRIPTION:	Probe classes for 2DC, 2DP, and 2DS/3V-CPI.

COPYRIGHT:	University Corporation for Atmospheric Research, 2000-2013
-------------------------------------------------------------------------
*/

#include "2D.h"


/* -------------------------------------------------------------------- */
TwoDS::TwoDS(NcFile *file, NcVar *av) : TwoD(file, av)
{
  if (_resolution == 0)
    _resolution = 0.010;
  if (_armDistance == 0)
    _armDistance = 50.8;
}

/* -------------------------------------------------------------------- */
TwoDC::TwoDC(NcFile *file, NcVar *av) : TwoD(file, av)
{
  if (_resolution == 0)
    _resolution = 0.025;
  if (_armDistance == 0)
    _armDistance = 61;
}

/* -------------------------------------------------------------------- */
TwoDP::TwoDP(NcFile *file, NcVar *av)
 : TwoD(file, av)
{
  if (_resolution == 0)
    _resolution = 0.2;
  if (_armDistance == 0)
    _armDistance = 261;
}

/* -------------------------------------------------------------------- */
TwoD::TwoD(NcFile *file, NcVar *av) : Probe200(file, av)
{
  NcAtt		*attr;

  if ((attr = _avar->get_att("Resolution")))
    _resolution = attr->as_float(0) / 1000;

  if ((attr = _avar->get_att("ArmDistance")))
    _armDistance = attr->as_float(0);

  ComputeWidths();

  for (int i = 0; i < _otherVars.size(); ++i)
    {
    // Search here.  Probe200 search turns up shado-or conc.
    if (strncmp(_otherVars[i]->name(), "CONC1D", 6) == 0 ||
        strncmp(_otherVars[i]->name(), "CONC2D", 6) == 0)
      _concIdx = i;
    }

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void TwoD::ComputeConcentration(float *accum, float *conc, long countV[],
	const std::vector<float *> & otherVarData)
{
  int	time, bin;
  std::vector<float> dia;
  float	*counts, *concentration;
  float	*tas, tasx;

  tas = otherVarData[_tasIdx];

  for (time = 0; time < countV[0] * countV[1]; ++time)
    {
    counts = &accum[time * VectorLength()];
    concentration = &conc[time * VectorLength()];

    tasx = tas[time] / DataRate();

    for (bin = FirstBin(); bin <= LastBin(); ++bin)
      {
      _sampleVolume[bin] = tasx * (_dof[bin] * _esw[bin]) * 0.001;
      if (_deadTimeIdx >= 0)
        _sampleVolume[bin] *= (((float)1000 - otherVarData[_deadTimeIdx][time]) / 1000);
      }

    dia = _midPointDiam;

#define PLWC
#define DBZ

#include "pms1d_cv"

    otherVarData[_concIdx][time] = _totalConcen;
    otherVarData[_lwIdx][time] = _plwc;
    otherVarData[_dbarIdx][time] = _dbar;
    otherVarData[_dispIdx][time] = _disp;
    otherVarData[_dbzIdx][time] = _dbz;
    }

}	/* END COMPUTECONCENTRATION */

/* END 2D.CC */
