/*
-------------------------------------------------------------------------
OBJECT NAME:    raltm.c

FULL NAME:  Aircraft Radar Altitude (converted from feet to meters)

     Input:
         ralt  - raw aircraft radar altitude signal (Vdc)
     Output:
         raltm - derived aircraft radar altitude (M)

ENTRY POINTS:   sraltm()

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
void sraltm(DERTBL *varp)
{
  NR_TYPE ralt, raltm;

  ralt = GetSample(varp, 0);

/*  If RALT is missing, so is RALTM  */
  if (ralt == MISSING_VALUE)
    {
    raltm = MISSING_VALUE;
    }
  else
    {
    if (ralt < 1.78)
      {
/*     0.35 to   1.78  vdc      *
 *     1.00 to 100.00  meters   */
/*      raltm = -23.2307692307 + ralt * 69.2307692308;	*/
	raltm = -26.5 + ralt * 75.456;
      }
    else
      {
/*       1.78 to    4.61  vdc      *
 *     100.00 to 2096.00  meters   */
/*      raltm = -1155.434628974 + ralt * 705.3003533568;	*/
	raltm = -941.0 + ralt * 587.136;
      }
    }

  PutSample(varp, raltm);

}   /* END SRALTM */

/* END RALTM.C */
