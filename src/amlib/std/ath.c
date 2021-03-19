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

/* See Al Cooper memo titled "Study of the Recovery Factor" Sep 23, 2015.
 */
static NR_TYPE rosemount_C0[] = { 0.958, 0.957, 0.958, 0.957 };

// Default to HARCO        RTH1   RTH2   RTH3   RTH4
static NR_TYPE RF_C0[] = { 0.986, 0.969, 0.986, 0.969 };

/* -------------------------------------------------------------------- */
void athInit(var_base *varp)
{
  char buffer[1024];
  float *tmp;
  int sensor = varp->name[3] - '1';

  // Copy in Rosemount C0, if appropriate.
  std::string s = ((DERTBL *)varp)->depends[0]->LongName;
  std::transform(s.begin(), s.end(), s.begin(), ::tolower);
  if ( s.find("rosemount") != std::string::npos )
    RF_C0[sensor] = rosemount_C0[sensor];

  sprintf(buffer, "%.4f + 0.053 log10(mach) + 0.090 (log10(mach))^2 + 0.091 (log10(mach))^3",
	RF_C0[sensor]);
  AddToAttributes(varp->name, "RecoveryFactor", buffer);

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
double heatedRecoveryFactor(double mach, int sensor)
{
  double logMach = log10(mach);
  return RF_C0[sensor] + 0.053 * logMach + 0.090 * logMach * logMach + 0.091 * logMach * logMach * logMach;
}

/* -------------------------------------------------------------------- */
void sath(DERTBL *varp)
{
  int sensor = varp->name[3] - '1';
  NR_TYPE rt = GetSample(varp, 0);
  NR_TYPE mach = GetSample(varp, 1);
  NR_TYPE recovery = heatedRecoveryFactor(mach, sensor);

  if (rt < -Kelvin)
    rt  = -Kelvin;

  NR_TYPE ath = AMBIENT(rt, recovery, mach*mach);

  PutSample(varp, ath);
}

/* END ATH.C */
