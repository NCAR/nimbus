/*
-------------------------------------------------------------------------
OBJECT NAME:    atf.c

FULL NAME:      Ambient Temperature Fast/Unheated

ENTRY POINTS:   satb()

DESCRIPTION:    Ambient Temperature for Rosemount 102 Unheated:

COPYRIGHT:      University Corporation for Atmospheric Research, 1992-2015
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

/* -------------------------------------------------------------------- */
void atfInit(var_base *varp)
{
   AddToAttributes(varp->name, "RecoveryFactor", "0.9959 + 0.0283 log10(mach) + 0.0374 (log10(mach))^2 + 0.0762 (log10(mach))^3");
}

/* -------------------------------------------------------------------- */
/* Implement Mach number dependent recovery factor per Al Cooper's 8 Oct 2014 
 * memo with constants per Cooper 14 Oct 2014 memo. */
double unheatedRecoveryFactor(double mach)
{
  double logMach = log10(mach);
  return 0.9959 + 0.0283 * logMach + 0.0374 * logMach * logMach + 0.0762 * logMach * logMach * logMach;
}

/* -------------------------------------------------------------------- */
void satf(DERTBL * varp)
{
  NR_TYPE rt = GetSample(varp, 0);
  NR_TYPE mach = GetSample(varp, 1);
  NR_TYPE recovery = unheatedRecoveryFactor(mach);
	
  if (rt < -Kelvin)
    rt  = -Kelvin;

  /* New calculation using mach-dependent recovery factor */
  PutSample(varp, AMBIENT(rt, recovery, mach*mach));
}

/* END ATF.C */
