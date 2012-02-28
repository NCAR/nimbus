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
void sEvcsel(DERTBL *varp)
{
  NR_TYPE E_vcsel = floatNAN, h2o_conc_vxl, atx;

  h2o_conc_vxl = GetSample(varp, 0);
  atx = GetSample(varp, 1);

  if (h2o_conc_vxl > 1.0)
    E_vcsel = Boltzmann * (atx+Kelvin) * (h2o_conc_vxl * 1.0e6) * 1.0e-2;

  PutSample(varp, E_vcsel);
}

/* -------------------------------------------------------------------- */
void svmr(DERTBL *varp)
{
  NR_TYPE vmr_vxl = floatNAN, h2o_conc_vxl, atx, ps;

  h2o_conc_vxl = GetSample(varp, 0);
  ps = GetSample(varp, 1) * 133.322;	// Torr -> Pascal
  atx = GetSample(varp, 2) + Kelvin;

  if (h2o_conc_vxl > 1.0)
    vmr_vxl = h2o_conc_vxl / (ps / Boltzmann / atx) * 1.0e12;

  PutSample(varp, vmr_vxl);

}	/* END SVMR */

/* END VCSEL.C */
