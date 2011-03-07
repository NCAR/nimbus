/*
-------------------------------------------------------------------------
OBJECT NAME:	ias.c

FULL NAME:	Indicated Air Speed

DESCRIPTION:	

INPUT:		Uncorrected QC

OUTPUT:		IAS knots

COPYRIGHT:	University Corporation for Atmospheric Research, 1996-2011
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void sias(DERTBL *varp)
{
  NR_TYPE	ias;

  double qcx = GetSample(varp, 0);

  ias = StdSpeedOfSound * sqrt(fabs(pow(((StdPress + qcx) / StdPress), Rd_DIV_Cpd) - 1.0));

  PutSample(varp, ias * MPS_TO_KNOTS);	/* Convert to Knots	*/
}
