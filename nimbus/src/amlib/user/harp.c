/*
-------------------------------------------------------------------------
OBJECT NAME:	harp.c

FULL NAME:	

DESCRIPTION:	HARP jNO2 proxy calculation.

COPYRIGHT:	University Corporation for Atmospheric Research, 2013
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static const double uvn382_cals[] = { 354.1, 1.56e-6 };    // Nadir
static const double uvz382_cals[] = { 1337.6, 1.50e-6 };   // Zenith

/* -------------------------------------------------------------------- */
void sjno2(DERTBL *varp)
{
  NR_TYPE	cnts_n, cnts_z;
  static NR_TYPE yn, yz;

  cnts_n = GetSample(varp, 0);
  cnts_z = GetSample(varp, 1);

  if (!isnan(cnts_n))
    yn = (cnts_n - uvn382_cals[0]) * uvn382_cals[1];

  if (!isnan(cnts_z))
    yz = (cnts_z - uvz382_cals[0]) * uvz382_cals[1];

  PutSample(varp, yn+yz);
}
