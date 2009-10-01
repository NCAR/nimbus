/*
-------------------------------------------------------------------------
OBJECT NAME:	cdp.cc

FULL NAME:	CDP Class

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1999-2009
-------------------------------------------------------------------------
*/

#include "cdp.h"


/* -------------------------------------------------------------------- */
CDP::CDP(NcFile *file, NcVar *av) : Probe100(file, av)
{
  NcAtt		*attr;

  if ((attr = cvar->get_att("BeamDiameter")) || (attr = avar->get_att("BeamDiameter")))
    beamDiameter = attr->as_float(0);
  else
    beamDiameter = 0.2;

  if ((attr = cvar->get_att("DepthOfField")) || (attr = avar->get_att("DepthOfField")))
    DOF = attr->as_float(0);
  else
    DOF = 1.2;

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void CDP::ComputeConcentration(float *accum, float *conc, long countV[],
	const std::vector<float *> & otherVarData)
{
  int	i, bin;
  std::vector<float> dia;
  float	*counts, *concentration;
  float	*tas, *fbmfr, *activity, sampleArea;
  double vol;

  tas = otherVarData[tasIdx];

  sampleArea = DOF * beamDiameter;

  for (i = 0; i < countV[0] * countV[1]; ++i)
    {
    counts = &accum[i * VectorLength()];
    concentration = &conc[i * VectorLength()];

    for (bin = FirstBin(); bin <= LastBin(); ++bin)
      sampleVolume[bin] = (float)tas[i] * sampleArea;

    dia = midPointDiam;

#define PLWC
#define DBZ

#include "pms1d_cv"

    otherVarData[concIdx][i] = totalConcen;
    otherVarData[lwIdx][i] = plwc;
    otherVarData[dbzIdx][i] = dbz;
    }

}	// END COMPUTECONCENTRATION

// END CDP.CC
