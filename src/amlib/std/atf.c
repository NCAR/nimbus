/*
-------------------------------------------------------------------------
OBJECT NAME:    atf.c

FULL NAME:      Ambient Temperature Fast/Unheated

DESCRIPTION:    Ambient Temperature for Rosemount 102 Unheated:

COPYRIGHT:      University Corporation for Atmospheric Research, 1992-2017
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static NR_TYPE RF_C0 = 0.9959;

/* -------------------------------------------------------------------- */
void atfInit(var_base *varp)
{
  char buffer[1024];
  float *tmp;

  if ( (tmp = GetDefaultsValue("UNHEATED_RF", varp->name)) )
    RF_C0 = (int)tmp[0];

  sprintf(buffer, "%.4f + 0.0283 log10(mach) + 0.0374 (log10(mach))^2 + 0.0762 (log10(mach))^3", RF_C0);
  AddToAttributes(varp->name, "RecoveryFactor", buffer);
}

/* -------------------------------------------------------------------- */
/* Implement Mach number dependent recovery factor per Al Cooper's 8 Oct 2014 
 * memo with constants per Cooper 14 Oct 2014 memo. */
double unheatedRecoveryFactor(double mach)
{
  double logMach = log10(mach);
  return RF_C0 + 0.0283 * logMach + 0.0374 * logMach * logMach + 0.0762 * logMach * logMach * logMach;
}

/* -------------------------------------------------------------------- */
void satf(DERTBL * varp)
{
  NR_TYPE rt = GetSample(varp, 0);
  NR_TYPE mach = GetSample(varp, 1);
  NR_TYPE recovery = unheatedRecoveryFactor(mach);
	
  if (rt < -Kelvin)
    rt  = -Kelvin;

  NR_TYPE atf = AMBIENT(rt, recovery, mach*mach);

  PutSample(varp, atf);
}

/* END ATF.C */
