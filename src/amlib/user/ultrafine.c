/*
-------------------------------------------------------------------------
OBJECT NAME:	ultrafine.c

FULL NAME:	UltraFine CN

ENTRY POINTS:	sxufcn()

STATIC FNS:	

DESCRIPTION:	Al Cooper UltraFine CN counter correction.

COPYRIGHT:	University Corporation for Atmospheric Research, 2004
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void sxufcn(DERTBL *varp)
{
  NR_TYPE	xufcn, xufct, xufflow, psxc, cntemp, atx, xn9psx;

  xufct = GetSample(varp, 0);
  xufflow = GetSample(varp, 1);
  psxc = GetSample(varp, 2);
  cntemp = GetSample(varp, 3);
  atx = GetSample(varp, 4);
  xn9psx = GetSample(varp, 5);


  xufcn = (xufct / xufflow) * (psxc/xn9psx) * (cntemp+273.15)/(atx+273.15);

  PutSample(varp, xufcn);

}	/* END SXUFCN */

/* END ULTRAFINE.C */
