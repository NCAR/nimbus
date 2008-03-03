/*
-------------------------------------------------------------------------
OBJECT NAME:	uhsas.cc

FULL NAME:	UHSAS

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 2001
-------------------------------------------------------------------------
*/

#include "uhsas.h"


/* -------------------------------------------------------------------- */
UHSAS::UHSAS(NcFile *file, NcVar *av) : Probe(file, av)
{
  concIdx = dispIdx = dbarIdx = volIdx = -1;

  for (int i = 0; i < otherVars.size(); ++i)
    {
    if (strncmp(otherVars[i]->name(), "UFLWC", 5) == 0)
      flowIdx = i;

    if (strncmp(otherVars[i]->name(), "CONC", 4) == 0)
      concIdx = i;

    if (strncmp(otherVars[i]->name(), "DISP", 4) == 0)
      dispIdx = i;

    if (strncmp(otherVars[i]->name(), "DBAR", 4) == 0)
      dbarIdx = i;

    if (strncmp(otherVars[i]->name(), "PVOL", 4) == 0)
      volIdx = i;
    }
}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void UHSAS::ComputeConcentration(float *accum, float *conc, long countV[],
	const std::vector<float *> & otherVarData)
{
  int		i, bin;
  std::vector<float> dia;
  float		*counts, *concentration;
  float		*flow;

  flow = otherVarData[flowIdx];

  for (i = 0; i < countV[0] * countV[1]; ++i)
    {
    counts = &accum[i * VectorLength()];
    concentration = &conc[i * VectorLength()];

    for (bin = FirstBin(); bin <= LastBin(); ++bin)
      sampleVolume[bin] = flow[i];

    dia = midPointDiam;

#define P_VOLUME

#include "pms1d_cv"

    otherVarData[concIdx][i] = totalConcen;
    otherVarData[dispIdx][i] = disp;
    otherVarData[dbarIdx][i] = dbar;

    if (volIdx >= 0)
      otherVarData[volIdx][i] = pvol;
    }

}	/* END COMPUTECONCENTRATION */

/* END UHSAS.CC */
