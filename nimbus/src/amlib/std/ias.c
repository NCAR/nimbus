/*
-------------------------------------------------------------------------
OBJECT NAME:	ias.c

FULL NAME:	Indicated Air Speed

ENTRY POINTS:	sias()

STATIC FNS:	none

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1996-2011
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void sias(DERTBL *varp)
{
  NR_TYPE	ias, qcxc;

  qcxc = GetSample(varp, 0);

  ias = 760.9166 * sqrt(fabs(pow(((StdPress + qcxc) / StdPress), 0.28571) - 1.0));

  PutSample(varp, ias * 1.94253479);	/* Convert to Knots	*/

}	/* END SIAS */

/* END IAS.C */
