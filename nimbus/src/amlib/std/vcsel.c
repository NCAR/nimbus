/*
-------------------------------------------------------------------------
OBJECT NAME:	vcsel.c

FULL NAME:	VCSEL

ENTRY POINTS:	svmr()

DESCRIPTION:	Derived calculations for the VCSEL.

COPYRIGHT:	University Corporation for Atmospheric Research, 2011
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void svmr(DERTBL *varp)
{
  NR_TYPE vmr_vxl, h2o_conc_vxl, atx, ps;

  h2o_conc_vxl = GetSample(varp, 0);
  ps = GetSample(varp, 1) * 133.322;	// Torr -> Pascal
  atx = GetSample(varp, 2) + Kelvin;

  vmr_vxl = h2o_conc_vxl / (ps / Boltzmann / atx) * 1e12;

  PutSample(varp, vmr_vxl);

}	/* END SVMR */

/* END VCSEL.C */
