/*
-------------------------------------------------------------------------
OBJECT NAME:	ws.c

FULL NAME:	Wind Speed

ENTRY POINTS:	sws()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	compute.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void sws(DERTBL *varp)
{
  NR_TYPE	ui, vi;

  ui = GetSample(varp, 0);
  vi = GetSample(varp, 1);

  PutSample(varp, (NR_TYPE)sqrt((double)ui*ui + vi*vi));

}	/* END SWS */

/* END WS.C */
