/*
-------------------------------------------------------------------------
OBJECT NAME:	2D.cc

FULL NAME:	2D Class

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 2000
-------------------------------------------------------------------------
*/

#include "2D.h"


/* -------------------------------------------------------------------- */
TwoDC::TwoDC(NcFile *file, NcVar *av) : TwoD(file, av)
{
  if (resolution == 0)
    resolution = 0.025;
  if (armDistance == 0)
    resolution = 61;
}

/* -------------------------------------------------------------------- */
TwoDP::TwoDP(NcFile *file, NcVar *av)
 : TwoD(file, av)
{
  if (resolution == 0)
    resolution = 0.2;
  if (armDistance == 0)
    resolution = 261;
}

/* -------------------------------------------------------------------- */
TwoD::TwoD(NcFile *file, NcVar *av) : Probe200(file, av)
{
  NcAtt		*attr;

  if ((attr = avar->get_att("Resolution")))
    resolution = attr->as_float(0) / 1000;

  if ((attr = avar->get_att("ArmDistance")))
    armDistance = attr->as_float(0);

  ComputeWidths();

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void TwoD::ComputeConcentration(float *accum, float *conc, long countV[],
	const std::vector<float *> & otherVarData)
{
  int	time, bin;
  std::vector<float> dia;
  float	*counts, *concentration;
  float	*tas, tasx;

  tas = otherVarData[tasIdx];

  for (time = 0; time < countV[0] * countV[1]; ++time)
    {
    counts = &accum[time * VectorLength()];
    concentration = &conc[time * VectorLength()];

    tasx = tas[time] / dataRate;

    for (bin = FirstBin(); bin <= LastBin(); ++bin)
      {
      sampleVolume[bin] = tasx * (dof[bin] * esw[bin]) * 0.001;
      if (deadTimeIdx >= 0)
        sampleVolume[bin] *= (((float)1000 - otherVarData[deadTimeIdx][time]) / 1000);
      }

    dia = midPointDiam;

#define PLWC
#define DBZ

#include "pms1d_cv"

    otherVarData[concIdx][time] = totalConcen;
    otherVarData[lwIdx][time] = plwc;
    otherVarData[dbarIdx][time] = dbar;
    otherVarData[dispIdx][time] = disp;
    otherVarData[dbzIdx][time] = dbz;
    }

}	/* END COMPUTECONCENTRATION */

/* END 2D.CC */
