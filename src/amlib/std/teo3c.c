/*
-------------------------------------------------------------------------
OBJECT NAME:	teo3c.c

FULL NAME:	Ozone Calculations

ENTRY POINTS:	steo3c()

STATIC FNS:	none

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2024
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static const int nCals = 4;
static std::vector<float> teo3c_cal = { 0.0, 1.0, 0.0, 0.0 };


/* -------------------------------------------------------------------- */
void teo3cInit(var_base *varp)
{
  float *tmp;

  if ((tmp = GetDefaultsValue("TEO3CAL", varp->name)) == NULL)
  {
    sprintf(buffer,"Value set to 0,1,0 in AMLIB function TEO3CAL.\n");
    LogMessage(buffer);
  }
  else
  {
    teo3c_cal.clear();
    for (int i = 0; i < nCals; ++i)
      teo3c_cal.push_back(tmp[i]);
  }
  varp->addToMetadata("Coefficients", teo3c_cal);
}

/* -------------------------------------------------------------------- */
void steo3c(DERTBL *varp)
{
  NR_TYPE teo3c, teo3, tep, tet;

  teo3	= GetSample(varp, 0);
  tep	= GetSample(varp, 1);
  tet	= GetSample(varp, 2);

  teo3 *= (1013.16 / tep) * ((tet + Kelvin) / Kelvin);

  int corder = nCals-1;
  teo3c = teo3c_cal[corder];
  for (int k = 1; k < nCals; k++)
    teo3c = teo3c_cal[corder-k] + teo3 * teo3c;

  if (teo3c <= 1.0)
    teo3c = 0.0;

  PutSample(varp, teo3c);

}	/* END STEO3C */

/* END TEO3C.C */
