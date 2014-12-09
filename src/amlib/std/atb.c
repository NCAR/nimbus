/*
-------------------------------------------------------------------------
OBJECT NAME:    atb.c

FULL NAME:      Ambient Temperature Wrapper

ENTRY POINTS:   satb()

DESCRIPTION:    Ambient Temperature for Rosemount 102 Unheated:
			Saberliner Boom
			Electra Boom
			GV Radome

COPYRIGHT:      University Corporation for Atmospheric Research, 1992-2014
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

/* -------------------------------------------------------------------- */
/* Implement Mach number dependent recovery factor per Al Cooper's 8 Oct 2014 
 * memo with constants per Cooper 14 Oct 2014 memo. */
double unheatedRecoveryFactor(double mach)
{
  double logMach = log10(mach);
  return 0.9959 + 0.0283 * logMach + 0.0374 * logMach * logMach + 0.0762 * logMach * logMach * logMach;
}

/* -------------------------------------------------------------------- */
void atbInit(var_base *varp)
{
   AddToAttributes(varp->name, "RecoveryFactor", "0.9959 + 0.0283 log10(mach) + 0.0374 (log10(mach))^2 + 0.0762 (log10(mach))^3");
}

/* -------------------------------------------------------------------- */
void satb(DERTBL * varp)
{
  NR_TYPE ttb = GetSample(varp, 0);
  NR_TYPE mach = GetSample(varp, 1);
  
  NR_TYPE recovery = unheatedRecoveryFactor(mach);
	
  if (ttb < -Kelvin)
    ttb  = -Kelvin;

  /* New calculation using mach-dependent recovery factor */
  PutSample(varp, AMBIENT(ttb, recovery, mach*mach));
}

/* END atb.c */
