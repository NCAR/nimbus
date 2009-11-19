/*
-------------------------------------------------------------------------
OBJECT NAME:	x260.cc

FULL NAME:	260X Class

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1999
-------------------------------------------------------------------------
*/

#include "x260.h"


/* -------------------------------------------------------------------- */
X260::X260(NcFile *file, NcVar *av) : Probe200(file, av)
{
  NcAtt		*attr;

  _resolution = 0.01;

  if ((attr = _avar->get_att("nDiodes")))
    _nDiodes = attr->as_int(0);
  else
    _nDiodes = 64;

  if ((attr = _avar->get_att("ArmDistance")))
    _armDistance = attr->as_float(0);
  else
    _armDistance = 61.0;

  ComputeWidths();

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void X260::ComputeConcentration(float *accum, float *conc, long countV[],
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

    ComputeDOF200(tasx);

    for (bin = FirstBin(); bin <= LastBin(); ++bin)
      _sampleVolume[bin] = tasx * (_dof[bin] * _esw[bin]) * 0.001;

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

/* END X260.CC */
