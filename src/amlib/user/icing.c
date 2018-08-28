/*
-------------------------------------------------------------------------
OBJECT NAME:	icing.c

FULL NAME:	

ENTRY POINTS:	sicing()

STATIC FNS:	

DESCRIPTION:	Template for AMLIB std & user directories.

COPYRIGHT:	University Corporation for Atmospheric Research, 2003
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void sicing(DERTBL *varp)
{
  int		i;
  NR_TYPE	atx, plwcc, *fssp, *x260, y, sum;

  atx = GetSample(varp, 0);
  plwcc = GetSample(varp, 1);
  fssp = GetVector(varp, 2);
  x260 = GetVector(varp, 3);

  y = sum = 0.0;

  if (atx < 0.0)
    y += 1;

  if (plwcc > 0.1)
    y += 1;

  for (i = 8; i < 63; ++i)
    sum += x260[i];

  if (sum > 5.0)
    y += 1;


  sum = 0.0;

  for (i = 16; i < 31; ++i)
    sum += x260[i];

  if (sum > 5.0)
    y += 1;


  PutSample(varp, y);

}	/* END SICING */

/* END ICING.C */
