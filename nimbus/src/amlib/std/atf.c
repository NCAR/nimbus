/*
-------------------------------------------------------------------------
OBJECT NAME:    atf.c

FULL NAME:      Ambient Temperature Wrapper

ENTRY POINTS:   satf()

STATIC FNS:     none

DESCRIPTION:    Ambient Temperature for Rosemount 102 Unheated:
			Saberliner Fueselage

COPYRIGHT:      University Corporation for Atmospheric Research, 1992-2008
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

extern NR_TYPE	recff;	// Recovery Factor, see initAC.c

/* -------------------------------------------------------------------- */
void satf(DERTBL *varp)
{
  NR_TYPE ttf, mach;

  ttf = GetSample(varp, 0);
  mach = GetSample(varp, 1);

  if (ttf < -Kelvin)
    ttf = -Kelvin;

  PutSample(varp, AMBIENT(ttf, recff, mach*mach));
}
