/*
-------------------------------------------------------------------------
OBJECT NAME:	pms1d.c

FULL NAME:	1D OAP

ENTRY POINTS:	ComputePMS1DParams()
		ComputeDOF()

STATIC FNS:	none

DESCRIPTION:	Functions used by all PMS1D OAP probes.

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	s260x(), stwoDc(), stwoDp()

NOTES:		Calculations taken from Bulletin 24 dated 1/89.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2002
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static NR_TYPE diodeDiameter = 0.2;

/* -------------------------------------------------------------------- */
void ComputePMS1DParams(
	NR_TYPE	radius[],
	NR_TYPE	eaw[],
	NR_TYPE	diam[],
	float	minRange,
	float	resolution,
	size_t	nDiodes,
	size_t	nBins)
{
  float	mag = diodeDiameter / (resolution / 1000);

  minRange += resolution / 2;	/* Create mid-points for diam. */

  for (size_t i = 1; i < nBins; ++i, minRange += resolution)
    {
    diam[i]	= minRange;
    radius[i]	= minRange / 2000; /* Units: mm */
    eaw[i]	= diodeDiameter * (nDiodes - i - 1) / mag; /* Units: mm */
//printf("%f %f\n", diam[i], eaw[i]);
    }

}	/* END COMPUTEPMS1DPARAMS */

/* -------------------------------------------------------------------- */
void ComputeDOF(
	NR_TYPE	radius[],
	NR_TYPE	tasx,
	NR_TYPE	dof[],		/* Output	*/
	size_t	FirstBin,
	size_t	LastBin,
	float	RESOLUTION,
	NR_TYPE	RESPONSE_TIME)
{
  NR_TYPE	szz, timex, frac, f, z;
  float		mag = diodeDiameter / (RESOLUTION / 1000);

  for (size_t i = FirstBin; i < LastBin; ++i)
    {
    /* Determine size in terms of # of array elements
     */
    szz = 2.0 * radius[i];

    /* Diminish this radius by an amount that accounts for
     * the fact that the particle is not on the diode array
     * 100% of the time 50% shadowed. This amount is approximately
     * one half diode array width wide.
     */
    szz += (diodeDiameter / mag) / 2.0;

    /* Calculate amount of time particle will be on the array
     */
    timex = szz / (tasx * 1000.0) * 1.0e6;

    /* This is the fraction of total amplitude caused by response
     * time
     */
    frac = (double)1.0 - exp((double)-timex / RESPONSE_TIME);

    /* Have to meet 70% occultation for the 1D.
     */
    if (frac < 0.7)
      dof[i] = 0.0;
    else
      /* Cautionary note to users of this routine, the relationship
       * between dof and Z was derived from laboratory studies which
       * are somewhat prelimnary in nature and need additional
       * verification, A new algorithm should be available by 9/1/92
       */
      {
      f = (0.7 / frac) * 100.0;
      z = 30.46 - 0.628 * f + 0.003246 * f * f;
      dof[i] = 1580.0 * z * radius[i] * radius[i];
      }
    }

}	/* END COMPUTEDOF */

/* END PMS1D.C */
