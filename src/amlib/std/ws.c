/*
-------------------------------------------------------------------------
OBJECT NAME:	ws.c

FULL NAME:	Wind Speed

ENTRY POINTS:	sws()

STATIC FNS:	none

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2026
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

  PutSample(varp, (NR_TYPE)sqrtf(ui*ui + vi*vi));

}	/* END SWS */

/* END WS.C */
