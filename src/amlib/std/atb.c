/*
-------------------------------------------------------------------------
OBJECT NAME:    atb.c

FULL NAME:      Ambient Temperature Wrapper

ENTRY POINTS:   satb()

DESCRIPTION:    Ambient Temperature for Rosemount 102 Unheated:
			Saberliner Boom
			Electra Boom
			GV Radome

COPYRIGHT:      University Corporation for Atmospheric Research, 1992-2013
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

extern NR_TYPE recfb;	// Recovery Factor, see initAC.c

/* -------------------------------------------------------------------- */
void atbInit(var_base *varp)
{
  std::vector<float> values;
  values.push_back((float)recfb);
  AddToDefaults(varp->name, "RecoveryFactor", values);
}

/* -------------------------------------------------------------------- */
void satb(DERTBL * varp)
{
  NR_TYPE ttb = GetSample(varp, 0);
  NR_TYPE mach = GetSample(varp, 1);
	
  if (ttb < -Kelvin)
    ttb  = -Kelvin;

  PutSample(varp, AMBIENT(ttb, recfb, mach*mach));
}
