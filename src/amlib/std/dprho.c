/*
-------------------------------------------------------------------------
OBJECT NAME:    dprho.c

FULL NAME:  Dew Point from Absolute Humidity (C)

     Input:
         rho - derived absolute humidity (g/m3)
         atx - derived ambient temperature (C)
     Output:
         dprho - derived dew point (C)

ENTRY POINTS:   sdpho()

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

/* -------------------------------------------------------------------- */
void sdprho(DERTBL *varp)
{
    NR_TYPE dprho, rho, atx;

    double  ela, z;

    rho = GetSample(varp, 0);
    atx = GetSample(varp, 1);

/*  If RHO and/or ATX are missing, so is DPRHO  */
    if ( isnan(rho) || isnan(atx) )
      {
      dprho = floatNAN;
      }
    else
      {
      if (rho < 0.001) rho = 0.001;
      ela = (atx+273.15)*rho/1322.3;
      if (ela > 0.0)
        {
        z = log(ela);
/*  Use commented code below if frost point is desired below 0C.
        if (atx >= 0.0)
          {
 */
          dprho = 237.3*z/(17.27-z);
/*
          }
        else
          {
          dprho = 273.0*z/(22.51-z);
          }
 *  Use commented code above if frost point is desired below 0C.  */
        }
      else
        {
        dprho = floatNAN;
        }
      }

    PutSample(varp, dprho);

}    /* END SDPRHO */

/* END DPRHO.C */
