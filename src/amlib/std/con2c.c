/*******CON2C  (N/LTR) *(from 2D-C probe)***************************** CON2C
   Input:
     sdwrc - raw "shadow or" counts
     tasx - derived true air speed
   Output:
     con2c1 - derived concentration
*/

#include "nimbus.h"
#include "amlib.h"

/*  Values from /home/local/proj/PMSspecs on 23 April 1998           RLR */
static NR_TYPE  C1_EAW = 0.8, C2_EAW = 0.8; /* PMS-2D Effective Area Width (MM^2) */

/* -------------------------------------------------------------------- */
void conc2cInit(var_base *varp)
{
  NR_TYPE  *tmp;

  if ((tmp = GetDefaultsValue("C1_EAW", varp->name)) == NULL)
    {
    sprintf(buffer, "Value set to %f in AMLIB function conc2cInit.\n", C1_EAW);
    LogMessage(buffer);
    }
  else
    C1_EAW = tmp[0];

  if ((tmp = GetDefaultsValue("C2_EAW", varp->name)) == NULL)
    {
    sprintf(buffer, "Value set to %f in AMLIB function conc2cInit.\n", C2_EAW);
    LogMessage(buffer);
    }
  else
    C2_EAW = tmp[0];

}  /* END CON2CINIT */

/* -------------------------------------------------------------------- */
void scon2c(DERTBL *varp)
{
  NR_TYPE  sdwrc, tasx, EAW;

  sdwrc  = GetSample(varp, 0);
  tasx  = GetSample(varp, 1);

  /* Blow-up protection:  output zero while on ground (TASX < 30.0 m/s)
       installed by Ron Ruth  18 October 2001 */

  if (tasx < 30.0)
    {
    sdwrc = 0.0;
    tasx  = 30.0;
    }

  if (varp->name[5] == '1')
    EAW = C1_EAW;
  else
    EAW = C2_EAW;

  PutSample(varp, 1000.0 * (sdwrc / (tasx * EAW * 61.0)));
}
