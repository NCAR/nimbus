/*
-------------------------------------------------------------------------
OBJECT NAME:	licor.c

FULL NAME:	LI-COR 6262

ENTRY POINTS:	sxco2c()
		sxlh2oc()
		sxmrli()

STATIC FNS:	none

DESCRIPTION:	Calculations for Chem folks

INPUT:    

REFERENCES:	none

REFERENCED BY:	compute.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1994-2002
-------------------------------------------------------------------------
*/

#include <math.h>

#include "nimbus.h"
#include "amlib.h"

static NR_TYPE  *CO2_CAL, *H2O_CAL;

static NR_TYPE  C2T = 309.45;
static NR_TYPE  CO2_cal[5] = { 0.15207, 1.032e-5, 7.2522e-9, -9.8728e-13,
				6.5812e-17 };
static NR_TYPE  H2T = 314.65;
static NR_TYPE  H2O_cal[3] = { 7.1352e-3, 3.6484e-6, -3.8355e-11 };

static NR_TYPE  CAL_FACTOR = 1.475,
                OFFSET = 0.7;  /*  CO2_OFFSET  */

/* -------------------------------------------------------------------- */
void UWO_LICORinit(DERTBL *varp)
{
  NR_TYPE  *tmp;

  if ((tmp = GetDefaultsValue("C2T", varp->name)) == NULL) {
    sprintf(buffer, "Value set to %f in AMLIB function LICORinit.\n", C2T);
    LogMessage(buffer);
  }
  else
    C2T = tmp[0];

  if ((tmp = GetDefaultsValue("CO2_CAL", varp->name)) == NULL) {
  CO2_CAL = CO2_cal;
    sprintf(buffer, "Values set to %e, %e, %e, %e, %e in AMLIB function LICORinit.\n", CO2_CAL[0], CO2_CAL[1], CO2_CAL[2], CO2_CAL[3], CO2_CAL[4]);
    LogMessage(buffer);
  }
  else
    CO2_CAL = tmp;

  if ((tmp = GetDefaultsValue("CAL_FACTOR", varp->name)) == NULL) {
    sprintf(buffer, "Value set to %f in AMLIB function LICORinit.\n", CAL_FACTOR);
    LogMessage(buffer);
  }
  else
    CAL_FACTOR = tmp[0];

  if ((tmp = GetDefaultsValue("CO2_OFFSET", varp->name)) == NULL) {
    sprintf(buffer, "Value set to %f in AMLIB function LICORinit.\n", OFFSET);
    LogMessage(buffer);
  }
  else
    OFFSET = tmp[0];

  if ((tmp = GetDefaultsValue("H2T", varp->name)) == NULL) {
    sprintf(buffer, "Value set to %f in AMLIB function LICORinit.\n", H2T);
    LogMessage(buffer);
  }
  else
    H2T = tmp[0];

  if ((tmp = GetDefaultsValue("H2O_CAL", varp->name)) == NULL) {
    H2O_CAL = H2O_cal;
    sprintf(buffer, "Values set to %e, %e, %e in AMLIB function LICORinit.\n", H2O_CAL[0], H2O_CAL[1], H2O_CAL[2]);
    LogMessage(buffer);
  }
  else
    H2O_CAL = tmp;

}  /* LICORINIT */

/* -------------------------------------------------------------------- */
void sxwco2c(DERTBL *varp)
{
  NR_TYPE  co2c, co2p, co2s, co2t, cpress, co2ml;

  co2p  = GetSample(varp, 0);
  co2s  = GetSample(varp, 1);
  co2t  = GetSample(varp, 2);

  cpress = 1000.0 * co2s * 101.325 / co2p;
  co2ml = cpress * (CO2_CAL[0] + cpress * (CO2_CAL[1] + cpress * (CO2_CAL[2])));
  co2ml *= (co2t + 273.15) / C2T;

  PutSample(varp, co2ml);

}  /* END SXCO2C */

/* -------------------------------------------------------------------- */
void sxwlh2oc(DERTBL *varp)
{
  NR_TYPE  co2p, co2t, h2os, hpress, h2oml;

  co2p  = GetSample(varp, 0);
  co2t  = GetSample(varp, 1);
  h2os  = GetSample(varp, 2);

  hpress = 1000.0 * h2os * 101.325 / co2p;
  h2oml = hpress * (H2O_CAL[0] + hpress * (H2O_CAL[1] + hpress * (H2O_CAL[2])));
  h2oml *= (co2t + 273.15) / H2T;

  PutSample(varp, h2oml);

}  /* END SXH2OC */

/* -------------------------------------------------------------------- */
void sxwmrli(DERTBL *varp)
{
  NR_TYPE  h2oc, m;

  h2oc = GetSample(varp, 0);

  m = (28.84 * (1.0 - h2oc / 1000.0)) + (18.01 * h2oc / 1000.0);

  PutSample(varp, 18.01 * h2oc / m);

}  /* END SXMRLI */

/* END LICOR.C */
