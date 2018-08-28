/*
-------------------------------------------------------------------------
OBJECT NAME:	CSET.c

FULL NAME:	

DESCRIPTION:	For Kyle to support flight operations 

COPYRIGHT:	University Corporation for Atmospheric Research, 2015
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static const NR_TYPE m2f = 3.28084;  // Feet in a meter


/* -------------------------------------------------------------------- */
// Give GPS altitude in feet

void sggaltf(DERTBL *varp)
{

  NR_TYPE	ggaltf;

  NR_TYPE ggaltm = GetSample(varp, 0);

  ggaltf = ggaltm * m2f;


  PutSample(varp, ggaltf);
}
