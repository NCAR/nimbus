/*
-------------------------------------------------------------------------
OBJECT NAME:    ath.c

FULL NAME:      Ambient Temperature for Heated Sensors

DESCRIPTION:    Ambient Temperature for Harco and Rosemont anti-iced sensor

COPYRIGHT:      University Corporation for Atmospheric Research, 1992-2015
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static NR_TYPE RF_C0 = 0.988;

/* -------------------------------------------------------------------- */
void athInit(var_base *varp)
{
  char buffer[1024];
  float *tmp;

  if ( (tmp = GetDefaultsValue("HEATED_RF", varp->name)) )
    RF_C0 = tmp[0];

  sprintf(buffer, "%.4f + 0.053 log10(mach) + 0.090 (log10(mach))^2 + 0.091 (log10(mach))^3", RF_C0);
  AddToAttributes(varp->name, "RecoveryFactor", buffer);

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
double heatedRecoveryFactor(double mach)
{
  double logMach = log10(mach);
  return RF_C0 + 0.053 * logMach + 0.090 * logMach * logMach + 0.091 * logMach * logMach * logMach;
}

/* -------------------------------------------------------------------- */
void sath(DERTBL *varp)
{
  NR_TYPE rt = GetSample(varp, 0);
  NR_TYPE mach = GetSample(varp, 1);
  NR_TYPE recovery = heatedRecoveryFactor(mach);

  if (rt < -Kelvin)
    rt  = -Kelvin;

  NR_TYPE ath = AMBIENT(rt, recovery, mach*mach);

  PutSample(varp, ath);
}

/* END ATH.C */
