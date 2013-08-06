/*
-------------------------------------------------------------------------
OBJECT NAME:    atkp.c

FULL NAME:      Ambient Temperature Wrapper

ENTRY POINTS:   satkp()

STATIC FNS:     none

DESCRIPTION:    Ambient Temperature for NCAR K-probe temp sensor
			Queen Airs.

COPYRIGHT:      University Corporation for Atmospheric Research, 1992-2008
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

extern NR_TYPE recfkp;	// Recovery Factor, see initAC.c

/* -------------------------------------------------------------------- */
void satkp(DERTBL * varp)
{
  NR_TYPE ttkp, mach;

  ttkp	= GetSample(varp, 0);
  mach	= GetSample(varp, 1);
	
  if (ttkp < -Kelvin)
    ttkp  = -Kelvin;

  PutSample(varp, AMBIENT(ttkp, recfkp, mach*mach));
}
