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

COPYRIGHT:  University Corporation for Atmospheric Research, 1993,1994,1998
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

/*  Values from /home/local/proj/130/Defaults on 29 April 1998     RLR  */
static const int nCals = 2;
static NR_TYPE  o3fsm_cal = 9.32, xno2c_cal = 958.0;
static NR_TYPE  xnoym_cal[2] = {-20.0, 5.2};
static NR_TYPE  xnom_cal[2] = {-10.0, 43.0};
/*  Value from /home/local/proj/818/Defaults on 29 April 1998      RLR  */
static NR_TYPE  xo3f1t_cal[2] = {40.0, 0.0184};

/* -------------------------------------------------------------------- */
void kokInit(var_base *varp)
{
  float *tmp;

  if ((tmp = GetDefaultsValue("O3FSM_CAL", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function kokInit.\n", o3fsm_cal);
    LogMessage(buffer);
  }
  else
    o3fsm_cal = tmp[0];

  if ((tmp = GetDefaultsValue("XNOYM_CAL", varp->name)) == NULL)
  {
    sprintf(buffer, "Values set to %f, %f in AMLIB function kokInit.\n", xnoym_cal[0], xnoym_cal[1]);
    LogMessage(buffer);
  }
  else
  {
    for (int i = 0; i < nCals; ++i)
      xnoym_cal[i] = tmp[i];
  }

  if ((tmp = GetDefaultsValue("XNOM_CAL", varp->name)) == NULL)
  {
    sprintf(buffer, "Values set to %f, %f in AMLIB function kokInit.\n", xnom_cal[0], xnom_cal[1]);
    LogMessage(buffer);
  }
  else
  {
    for (int i = 0; i < nCals; ++i)
      xnom_cal[i] = tmp[i];
  }

  if ((tmp = GetDefaultsValue("XNO2C_CAL", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function kokInit.\n", xno2c_cal);
    LogMessage(buffer);
  }
  else
    xno2c_cal = tmp[0];

  if ((tmp = GetDefaultsValue("XO3F1T_CAL", varp->name)) == NULL)
  {
    sprintf(buffer, "Values set to %f in AMLIB function kokInit.\n", xo3f1t_cal[0]);
    LogMessage(buffer);
  }
  else
  {
    for (int i = 0; i < nCals; ++i)
      xo3f1t_cal[i] = tmp[i];
  }
}

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
  y = (xecco2t + Kelvin) / 308.11;

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
