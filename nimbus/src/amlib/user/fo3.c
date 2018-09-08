/*
-------------------------------------------------------------------------
OBJECT NAME:	fo3.c

DESCRIPTION:	FO3 correction.

		B.A. Ridley, F.E. Grahek, and J.G. Walega, 'A small,
		high-sensitivity, medium-response ozone detector suitable
		for measurements from light aircraft', 1992, J. Atmos.
		Ocean. Technol., vol. 9, pp. 142-148.

COPYRIGHT:	University Corporation for Atmospheric Research, 2017
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void sfo3c(DERTBL *varp)
{
  NR_TYPE	mr, fo3, fo3c;
  NR_TYPE	mr_ppth, H2OcorrFactor;

  fo3 = GetSample(varp, 0);
  mr = GetSample(varp, 1);

  mr_ppth = mr * 1.608;
  H2OcorrFactor = 1.0 + (4.3e-3 * mr_ppth);
  fo3c = fo3 * H2OcorrFactor;

  PutSample(varp, fo3c);
}
