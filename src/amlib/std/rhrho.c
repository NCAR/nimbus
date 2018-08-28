/*
-------------------------------------------------------------------------
OBJECT NAME:    rhrho.c

FULL NAME:  Relative Humidity from Absolute Humidity (%)

     Input:
         rho - derived absolute humidity (g/m3)
         atx - derived ambient temperature (C)
        psxc - reference static pressure (mbar)
     Output:
         rhrho - derived relative humidity (%)

ENTRY POINTS:   srhrho()

STATIC FNS: none

DESCRIPTION:

REFERENCES: none

REFERENCED BY:  compute.c

COPYRIGHT:  University Corporation for Atmospheric Research, 2001
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "amlib.h"

double esubt(double, double);

/* -------------------------------------------------------------------- */
void srhrho(DERTBL *varp)
{
  NR_TYPE rhrho, rho, atx, psxc, dprho;
  double  ela, z, edpc, eatx;

  rho  = GetSample(varp, 0);
  atx  = GetSample(varp, 1);
  psxc = GetSample(varp, 2);

/*  If RHO, ATX and/or PSXC are missing, so is RHRHO  */
  if ( isnan(rho) || isnan(atx) || isnan(psxc) )
    {
    rhrho = floatNAN;
    }
  else
    {
    if (rho < 0.001) rho = 0.001;
/*  Calculate dew point first (via Teten's equation) */
    ela = (atx+Kelvin)*rho/1322.3;
    if (ela > 0.0)
      {
      z = log(ela);
      dprho = 237.3*z/(17.27-z);

/*  Calculate relative humidity */
      edpc = esubt(dprho,psxc);
      eatx = esubt(atx,psxc);
      if (eatx < 0.0001) eatx = 0.0001;
      rhrho = 100.*edpc/eatx;
      }
    else
      {
      rhrho = floatNAN;
      }
    }

  PutSample(varp, rhrho);

}	/* END SRHRHO */

/* END RHRHO.C */
