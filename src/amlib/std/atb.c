/*
-------------------------------------------------------------------------
OBJECT NAME:    atb.c

FULL NAME:      Ambient Temperature Wrapper

ENTRY POINTS:   satb()

STATIC FNS:     none

DESCRIPTION:    Ambient Temperature for Rosemount 102 Unheated:
			Saberliner Boom
			Electra Boom
			GV Radome

COPYRIGHT:      University Corporation for Atmospheric Research, 1992-2008
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

extern NR_TYPE recfb;	// Recovery Factor, see initAC.c

/* -------------------------------------------------------------------- */
void satb(DERTBL * varp)
{
  NR_TYPE ttb, xmach2;

  ttb = GetSample(varp, 0);
  xmach2 = GetSample(varp, 1);
	
  if (ttb < -Kelvin)
    ttb  = -Kelvin;

  PutSample(varp, AMBIENT(ttb, recfb, xmach2));
}
