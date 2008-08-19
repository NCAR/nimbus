/*
-------------------------------------------------------------------------
OBJECT NAME:    atrf.c

FULL NAME:      Ambient Temperature Wrapper

ENTRY POINTS:   satrf()

STATIC FNS:     none

DESCRIPTION:    Ambient Temperature for NCAR Reverse Flow
			Queen Airs
			King Airs
			Sailplane

COPYRIGHT:      University Corporation for Atmospheric Research, 1992-2008
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"
 
extern NR_TYPE	recfrn;	// Recovery Factor, see initAC.c

/* -------------------------------------------------------------------- */
void satrf(DERTBL * varp)
{
  NR_TYPE ttrf, xmach2;

  ttrf = GetSample(varp, 0);
  xmach2 = GetSample(varp, 1);
	
  if (ttrf < -Kelvin)
    ttrf  = -Kelvin;

  PutSample(varp, AMBIENT(ttrf, recfrn, xmach2));
}
