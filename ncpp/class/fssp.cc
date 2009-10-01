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

  if ((attr = cvar->get_att("BeamDiameter")) || (attr = avar->get_att("BeamDiameter")))
    beamDiameter = attr->as_float(0);
  else
    beamDiameter = 0.19;

  if ((attr = cvar->get_att("DepthOfField")) || (attr = avar->get_att("DepthOfField")))
    DOF = attr->as_float(0);
  else
    DOF = 2.81;

  range = 0;	// For FSSP, all other probes must be 0.

  for (int i = 0; i < otherVars.size(); ++i)
    {
    if (strncmp(otherVars[i]->name(), "FACT", 4) == 0)
      actIdx = i;

    if (strncmp(otherVars[i]->name(), "FBMFR", 5) == 0)
      bmFrIdx = i;
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

  tas = otherVarData[tasIdx];
  activity = otherVarData[actIdx];
  fbmfr = otherVarData[bmFrIdx];

  sampleArea = DOF * beamDiameter;

  for (i = 0; i < countV[0] * countV[1]; ++i)
    {
    counts = &accum[i * VectorLength()];
    concentration = &conc[i * VectorLength()];

    vol = tas[i] / dataRate * sampleArea * fbmfr[i] *
				(1.0 - (activity[i] * dataRate * 0.71));

    for (bin = FirstBin(); bin <= LastBin(); ++bin)
      sampleVolume[bin] = (float)vol;

    dia = &midPointDiam[range << 4];

#define PLWC
#define DBZ

#include "pms1d_cv"

    otherVarData[concIdx][i] = totalConcen;
    otherVarData[lwIdx][i] = plwc;
    otherVarData[dbarIdx][i] = dbar;
    otherVarData[dispIdx][i] = disp;
    otherVarData[dbzIdx][i] = dbz;
    }

}	/* END COMPUTECONCENTRATION */

/* END FSSP.CC */
