/*
-------------------------------------------------------------------------
OBJECT NAME:	licor.c

FULL NAME:	LI-COR 6262

ENTRY POINTS:	sxco2c()
		sxlh2oc()
		sxmrli()

DESCRIPTION:	Calculations for Chem folks

COPYRIGHT:  University Corporation for Atmospheric Research, 1994, 1998
-------------------------------------------------------------------------
*/

#include <math.h>

#include "nimbus.h"
#include "amlib.h"

/*  Values from /home/local/proj/defaults/Defaults on 29 April 1998  RLR  */
static NR_TYPE  C2T = 309.45;
static NR_TYPE  CO2_CAL[5] = {0.15207, 1.032e-5, 7.2522e-9, -9.8728e-13,
                              6.5812e-17};
static NR_TYPE  H2T = 314.65;
static NR_TYPE  H2O_CAL[3] = {7.1352e-3, 3.6484e-6, -3.8355e-11};

/*  Values from /home/local/proj/818/Defaults on 29 April 1998     RLR  */
static NR_TYPE  CAL_FACTOR = 1.475,
                OFFSET = 0.7;  /*  CO2_OFFSET  */

/* -------------------------------------------------------------------- */
void LICORinit(var_base *varp)
{
  float *tmp;
  if ((tmp = GetDefaultsValue("C2T", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function LICORinit.\n", C2T);
    LogMessage(buffer);
  }
  else
    C2T = tmp[0];

  if ((tmp = GetDefaultsValue("CO2_CAL", varp->name)) == NULL)
  {
    sprintf(buffer, "Values set to %e, %e, %e, %e, %e in AMLIB function LICORinit.\n", CO2_CAL[0], CO2_CAL[1], CO2_CAL[2], CO2_CAL[3], CO2_CAL[4]);
    LogMessage(buffer);
  }
  else
  {
    for (int i = 0; i < 5; ++i)
      CO2_CAL[i] = tmp[i];
  }

  if ((tmp = GetDefaultsValue("CAL_FACTOR", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function LICORinit.\n", CAL_FACTOR);
    LogMessage(buffer);
  }
  else
    CAL_FACTOR = tmp[0];

  if ((tmp = GetDefaultsValue("CO2_OFFSET", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function LICORinit.\n", OFFSET);
    LogMessage(buffer);
  }
  else
    OFFSET = tmp[0];

  if ((tmp = GetDefaultsValue("H2T", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function LICORinit.\n", H2T);
    LogMessage(buffer);
  }
  else
    H2T = tmp[0];

  if ((tmp = GetDefaultsValue("H2O_CAL", varp->name)) == NULL)
  {
    sprintf(buffer, "Values set to %e, %e, %e in AMLIB function LICORinit.\n", H2O_CAL[0], H2O_CAL[1], H2O_CAL[2]);
    LogMessage(buffer);
  }
  else
  {
    for (int i = 0; i < 3; ++i)
      H2O_CAL[i] = tmp[i];
  }

}  /* LICORINIT */

/* -------------------------------------------------------------------- */
void sxco2c(DERTBL *varp)
{
  NR_TYPE  co2c, co2p, co2s, co2t, h2oc, x, y, w;

  co2p  = GetSample(varp, 0);
  co2s  = GetSample(varp, 1);
  co2t  = GetSample(varp, 2);
  h2oc  = GetSample(varp, 3);

  w = h2oc / 1000.0 * 0.5 + 1.0;
  x = co2s / w;
  y = (co2t + Kelvin) / C2T;

  co2c = x*(CO2_CAL[0] + x*(CO2_CAL[1] + x*(CO2_CAL[2] + x*(CO2_CAL[3] + x*CO2_CAL[4]))));

  co2c = w * CAL_FACTOR * co2c * y * pow(1013.0 / co2p, 1.261) - OFFSET;

  PutSample(varp, co2c);

}  /* END SXCO2C */

/* -------------------------------------------------------------------- */
void sxlh2oc(DERTBL *varp)
{
  NR_TYPE  co2p, co2t, h2os, x, y, x2;

  co2p  = GetSample(varp, 0);
  co2t  = GetSample(varp, 1);
  h2os  = GetSample(varp, 2);

  x = h2os * 1.013e6 / co2p;
  y = (co2t + Kelvin) / H2T;
  x2 = x * x;

  PutSample(varp, ((H2O_CAL[0] * x) + (H2O_CAL[1] * x2) + (H2O_CAL[2] * x2 * x)) * y);

}  /* END SXH2OC */

/* -------------------------------------------------------------------- */
void sxmrli(DERTBL *varp)
{
  NR_TYPE  h2oc, m;

  h2oc = GetSample(varp, 0);

  m = (28.84 * (1.0 - h2oc / 1000.0)) + (18.01 * h2oc / 1000.0);

  PutSample(varp, 18.01 * h2oc / m);

}  /* END SXMRLI */

/* END LICOR.C */
