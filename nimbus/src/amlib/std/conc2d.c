/*
-------------------------------------------------------------------------
OBJECT NAME:	conc2d.c

FULL NAME:	PMS 2d concentrations

ENTRY POINTS:	sconc2dc(), sconc2dp()

STATIC FNS:	none

DESCRIPTION:	Concentrations from 2d probes with NEW housekeeping block.

INPUT:		True airspeed, shadow or

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	compute.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1997
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

/* -------------------------------------------------------------------- */
void sconc2dc(DERTBL *varp)
{
  NR_TYPE	shador, tasx;

  shador = GetSample(varp, 0);
  tasx = GetSample(varp, 1);

  if (tasx == -32767)	/* Lab setting, for spinning disk */
    tasx = 34.0;

  /* 0.8 is a hardwire "EAW" that needs to be pulled out of PMSspecs */
  PutSample(varp, shador / (tasx * 61.0 * 0.8) * 1000.0);

}	/* END SCONC2DC */

/* -------------------------------------------------------------------- */
void sconc2dp(DERTBL *varp)
{
  NR_TYPE	shador, tasx;

  shador = GetSample(varp, 0);
  tasx = GetSample(varp, 1);

  if (tasx == -32767)	/* Lab setting, for spinning disk */
    {
    tasx = 34.0;
    }

  /* Blow-up protection:  output zero while on ground (TASX < 30.0 m/s)
       installed by Ron Ruth  18 October 2001 */

  else if (tasx < 30.0)
    {
    shador = 0.0;
    tasx  = 30.0;
    }


  /* 6.4 is a hardwire "EAW" that needs to be pulled out of PMSspecs */
  PutSample(varp, shador / (tasx * 261.0 * 6.4) * 1000.0);

}	/* END SCONC2DP */

/* END CONC2D.C */
