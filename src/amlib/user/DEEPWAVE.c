/*
-------------------------------------------------------------------------
OBJECT NAME:	deepwave.c

FULL NAME:	CN Counter

ENTRY POINTS:	sconcn(), sxicnx(), sfcnc()

DESCRIPTION:	Computation for the CN Counter, see Mike Reeves.
		Specific for DEEPWAVE project since the PDUMP was used
		for PCN.

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2014
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void sconcnDW(DERTBL *varp)
{
  NR_TYPE  concn;
  static const NR_TYPE b[] = { 155.89, 26.06 };

  NR_TYPE cnts = GetSample(varp, 0);
  NR_TYPE flow = GetSample(varp, 1);
  NR_TYPE pcn  = GetSample(varp, 2);
  NR_TYPE tcab = GetSample(varp, 3);
  NR_TYPE psxc = GetSample(varp, 4);
  NR_TYPE atx  = GetSample(varp, 5);

  if (FeedBack == HIGH_RATE_FEEDBACK)
    cnts *= (float)Config::HighRate;

  // Concentration (#/cc, adjusted to ambient conditions)
  concn = cnts / flow * psxc / pcn * (tcab+Kelvin) / (atx+Kelvin);

  // Coincidence correction
  // This differs from existing code, but is mathematically equivalent
  concn *= (NR_TYPE) exp( (double)(4.0e-7 * cnts) );

  //Correction for reduced counting efficiency at low pressures:
  if (pcn < 300.0)
  {
    NR_TYPE eff = 1.0 - 1.0 / (1.0 + exp( (pcn-b[0])/b[1] ) );
    concn /= eff;
  }

  PutSample(varp, concn);

}  /* END SCONCN */

/* -------------------------------------------------------------------- */
void scFlowDW(DERTBL *varp)
{
  NR_TYPE  flow_dw, pcn;
  static const NR_TYPE a[] = { 1.20, 0.000242, 28.74, 73.11 };

  pcn = GetSample(varp, 0);

  if (pcn <= 0.0)
    pcn = 0.0001;

  // Pressure-dependent volumetric flow (vccs)
  // Leading factor 16.667 converts vlpm --> vccs:
  flow_dw = 16.667 * ( a[0] + a[1] * pcn - 1.0 / (1.0 + exp( (pcn-a[2])/a[3] ) ) );

  if (flow_dw <= 0.0)
    flow_dw = 0.0001;

  PutSample(varp, flow_dw);

}  /* END SCFLOW */
