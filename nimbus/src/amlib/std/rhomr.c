/*
-------------------------------------------------------------------------
OBJECT NAME:    rhomr.c

FULL NAME:  Vapor Density (RHO) from Mixing Ratio (for TDL)

     Input:
          mr - mixing ratio (g/kg)
         atx - derived ambient temperature (C)
        psxc - reference static pressure (mbar)
     Output:
         rhomr - derived vapor density (g/m3)

ENTRY POINTS:   srhomr()

STATIC FNS: none

DESCRIPTION:

REFERENCES: none

REFERENCED BY:  compute.c

COPYRIGHT:  University Corporation for Atmospheric Research, 2002
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "amlib.h"

/* -------------------------------------------------------------------- */
void stdlmr(DERTBL *varp)
{
  NR_TYPE	lo, hi, out;

  hi = GetSample(varp, 0);
  lo = GetSample(varp, 1);

  out = 409.6 * ((4096.0 * hi) + lo) / 100.0;

  PutSample(varp, out / 1607.8);

}

/* -------------------------------------------------------------------- */
void srhomr(DERTBL *varp)
{
  NR_TYPE	rhomr, mr, atx, psxc;
  NR_TYPE	x1, x2;
  static const double To = 273.16;

  mr   = GetSample(varp, 0);
  atx  = GetSample(varp, 1);
  psxc = GetSample(varp, 2);

/*  If MR, ATX and/or PSXC are missing, so is RHOMR  */
  if ( isnan(mr) || isnan(atx) || isnan(psxc) )
    {
    rhomr = floatNAN;
    }
  else
    {
    x1 = 622.0 + mr;
    x2 = atx + To;
/*     Blow-up protection  */
    if ( x1 != 0. && x2 != 0.)
      {
/*
      rhomr = 216.68 * ( psxc * mr / (622.0 + mr)) / (atx + To);
 */
      rhomr = 216.68 * psxc * mr / x1 / x2;
      }
    else
      {
      rhomr = floatNAN;
      }
    }

  PutSample(varp, rhomr);

}	/* END SRHOMR */

/* END RHOMR.C */
