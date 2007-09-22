/*
-------------------------------------------------------------------------
OBJECT NAME:	inlet.c

FULL NAME:	Community Inlet

ENTRY POINTS:	sCItas()

STATIC FNS:	none

DESCRIPTION:	Routines for Community Inlet Parameters.

COPYRIGHT:	University Corporation for Atmospheric Research, 1995
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

extern NR_TYPE tas(NR_TYPE, NR_TYPE, NR_TYPE);
extern NR_TYPE recfra;

/* -------------------------------------------------------------------- */
void sCItas(DERTBL *varp)
{
  NR_TYPE	tt, qc, ps;

  qc = GetSample(varp, 0);
  ps = GetSample(varp, 1);	/* Static Pressure	*/
  tt = GetSample(varp, 2);	/* Total Temperature	*/

  /* Blow-up protection:  set output value while on ground (QCX < 5.5 mbar)
       installed by Ron Ruth  18 October 2001 */

  if (qc < 5.5) qc = 5.5;

  PutSample(varp, tas(tt, recfra, XMAC2(qc / ps)));

}	/* END SCITAS */

/* -------------------------------------------------------------------- */
void scDiff(DERTBL *varp)
{
  NR_TYPE	ps, tas;

  ps = GetSample(varp, 0);	/* Static Pressure	*/
  tas = GetSample(varp, 1);	/* True Airspeed	*/

  /* Blow-up protection:  set output while on ground (TASX < 30.0 m/s)
       installed by Ron Ruth  18 October 2001 */

  if (tas < 30.0) tas = 30.0;

  PutSample(varp, (ps / 785.0) * (0.001184 * tas * tas - 0.048275 * tas - 0.151));

}	/* END SCDIFF */

/* END INLET.C */
