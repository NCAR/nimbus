/*
-------------------------------------------------------------------------
OBJECT NAME:	chemcal.c

FULL NAME:	Chemistry Calculations.

ENTRY POINTS:	so3fsc()
		sxeco2c()
		sxnoym()
		sxnom()

STATIC FNS:	none

DESCRIPTION:	Calculations for Greg Kok.

INPUT:    

REFERENCES:  none

REFERENCED BY:  compute.c

COPYRIGHT:  University Corporation for Atmospheric Research, 1993,1994,1998
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static NR_TYPE  *xnoym_cal, *xnom_cal, *xo3f1t_cal;

/*  Values from /home/local/proj/130/Defaults on 29 April 1998     RLR  */
static NR_TYPE  o3fsm_cal = 9.32, xno2c_cal = 958.0;
static NR_TYPE  Xnoym_cal[2] = {-20.0, 5.2};
static NR_TYPE  Xnom_cal[2] = {-10.0, 43.0};
/*  Value from /home/local/proj/818/Defaults on 29 April 1998      RLR  */
static NR_TYPE  Xo3f1t_cal[2] = {40.0, 0.0184};

/* -------------------------------------------------------------------- */
void kokInit(DERTBL *varp)
{
  NR_TYPE  *tmp;
  if ((tmp = GetDefaultsValue("O3FSM_CAL", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function kokInit.\n", o3fsm_cal);
    LogMessage(buffer);
  }
  else
    o3fsm_cal = tmp[0];
/*  o3fsm_cal = (GetDefaultsValue("O3FSM_CAL", varp->name))[0]; <-- original code */

  if ((tmp = GetDefaultsValue("XNOYM_CAL", varp->name)) == NULL)
  {
    xnoym_cal = Xnoym_cal;
    sprintf(buffer, "Values set to %f, %f in AMLIB function kokInit.\n", xnoym_cal[0], xnoym_cal[1]);
    LogMessage(buffer);
  }
  else
    xnoym_cal = tmp;
/*  xnoym_cal = GetDefaultsValue("XNOYM_CAL", varp->name); <-- original code */

  if ((tmp = GetDefaultsValue("XNOM_CAL", varp->name)) == NULL)
  {
	xnom_cal = Xnom_cal;
    sprintf(buffer, "Values set to %f, %f in AMLIB function kokInit.\n", xnom_cal[0], xnom_cal[1]);
    LogMessage(buffer);
  }
  else
    xnom_cal = tmp;
/*  xnom_cal = GetDefaultsValue("XNOM_CAL", varp->name); <-- original code */

  if ((tmp = GetDefaultsValue("XNO2C_CAL", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function kokInit.\n", xno2c_cal);
    LogMessage(buffer);
  }
  else
    xno2c_cal = tmp[0];
/*  xno2c_cal = (GetDefaultsValue("XNO2C_CAL", varp->name))[0]; <-- original code */

  if ((tmp = GetDefaultsValue("XO3F1T_CAL", varp->name)) == NULL)
  {
    xo3f1t_cal = Xo3f1t_cal;
    sprintf(buffer, "Values set to %f in AMLIB function kokInit.\n", xo3f1t_cal[0]);
    LogMessage(buffer);
  }
  else
    xo3f1t_cal = tmp;
/*  xo3f1t_cal = GetDefaultsValue("XO3F1T_CAL", varp->name); <-- original code */

}  /* KOKINIT */

/* -------------------------------------------------------------------- */
void so3fc(DERTBL *varp)
{
  NR_TYPE  o3_sig, edpc, psfdc, fudge;

  o3_sig  = GetSample(varp, 0);
  edpc  = GetSample(varp, 1);
  psfdc  = GetSample(varp, 2);
  fudge  = GetSample(varp, 3);

  PutSample(varp, o3fsm_cal * o3_sig * (1.0 + 3.43 * edpc / psfdc)*fudge);

}  /* END SXO3FC */

/* -------------------------------------------------------------------- */
void sxeco2c(DERTBL *varp)
{
  NR_TYPE  xecco2, xecco2p, xecco2t, x, y;

  xecco2  = GetSample(varp, 0);
  xecco2p  = GetSample(varp, 1);
  xecco2t  = GetSample(varp, 2);

  x = xecco2 * 1013.0 / xecco2p * 1000.0;
  y = (xecco2t + 273.16) / 308.11;

  PutSample(varp, x * (0.1362 + 2.008e-5 * x + 2.029e-9 * x * x) * y);

}  /* END SXECO2C */

/* -------------------------------------------------------------------- */
void sxnoym(DERTBL *varp)
{
  NR_TYPE  xnoy;

  xnoy = GetSample(varp, 0);

  PutSample(varp, xnoym_cal[0] + (xnoym_cal[1] * xnoy));

}  /* END SXNOYM */

/* -------------------------------------------------------------------- */
void sxnom(DERTBL *varp)
{
  NR_TYPE  xno;

  xno = GetSample(varp, 0);

  PutSample(varp, xnom_cal[0] + (xnom_cal[1] * xno));

}  /* END SXNOM */

/* -------------------------------------------------------------------- */
void sxno2(DERTBL *varp)
{
  NR_TYPE  lums;

  lums  = GetSample(varp, 0);

  PutSample(varp, 0.0035 + (5.881 - 0.2667 * lums) * lums);

}  /* END SXNO2 */

/* -------------------------------------------------------------------- */
void sxno2c(DERTBL *varp)
{
  NR_TYPE  xno2, psfd, pcor;

  xno2  = GetSample(varp, 0);
  psfd  = GetSample(varp, 1);

  if (psfd >= 0.0)
    pcor = xno2c_cal / psfd;
  else
    pcor = 0.0;

  PutSample(varp, xno2 * pcor);

}  /* END SXNO2C */

/* -------------------------------------------------------------------- */
void sxo3fdg(DERTBL *varp)
{
  PutSample(varp,
    1.0 - (GetSample(varp, 0) - xo3f1t_cal[0]) * xo3f1t_cal[1]);

}  /* END SXO3FDG */

/* END CHEMCAL.C */
