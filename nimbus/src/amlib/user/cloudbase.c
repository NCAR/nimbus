/*
-------------------------------------------------------------------------
OBJECT NAME:	cloudbase.c

FULL NAME:	

ENTRY POINTS:	scldbase()

STATIC FNS:	

DESCRIPTION:	Calculation for theoretical cloud base.

COPYRIGHT:	University Corporation for Atmospheric Research, 2003
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static NR_TYPE	t_lcl;

/* -------------------------------------------------------------------- */
void slclt(DERTBL *varp)
{
  PutSample(varp, t_lcl);

}	/* END LCLT */

/* -------------------------------------------------------------------- */
void slclp(DERTBL *varp)
{
  NR_TYPE	atx, psx, dpx, p_lcl;
  NR_TYPE	tx, del_t, f, fprime;

  psx = GetSample(varp, 0);
  atx = GetSample(varp, 1);
  dpx = GetSample(varp, 2);


  /* based on pascal program lcl.pas - dave rogers 871118
   * Predicts the lifting condensation level pressure and temperature,
   * p_lcl and t_lcl, given the initial pressure, temperature and dew
   * point, p, tk, dpk.
   *
   * Choose approximate starting point then iterate to find exact solution.
   */

  tx = dpx;

  do {
    f =  log(tx / atx) - 1544.0 * (1.0 / dpx - 1.0 / tx);
    fprime = 1.0 / tx - 1544.0 / (tx * tx);
    del_t = f / fprime;
    tx = tx - del_t;
    }
  while (fabs(del_t) > 0.01);

  t_lcl = tx;
  p_lcl = psx * pow(tx / atx, 3.497);

  PutSample(varp, p_lcl);

}	/* END LCLP */

/* END CLOUDBASE.C */
