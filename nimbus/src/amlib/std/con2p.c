/*******CON2P  (N/LTR) *(from 2D-P probe)***************************** CON2P
   Input:
     sdwrp - raw "shadow or" counts
     tasx - derived true air speed
   Output:
     con2p1 - derived concentration
*/

#include "nimbus.h"
#include "amlib.h"

/*  Values from /home/local/proj/PMSspecs on 23 April 1998           RLR */
static NR_TYPE  P1_EAW = 3.2, P2_EAW = 3.2;

/* -------------------------------------------------------------------- */
void conc2pInit(DERTBL *varp)
{
  NR_TYPE  *tmp;
  if ((tmp = GetDefaultsValue("P1_EAW", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function conc2pInit.\n", P1_EAW);
    LogMessage(buffer);
  }
  else
    P1_EAW = tmp[0];

  if ((tmp = GetDefaultsValue("P2_EAW", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function conc2pInit.\n", P2_EAW);
    LogMessage(buffer);
  }
  else
    P2_EAW = tmp[0];

}  /* END CON2PINIT */

/* -------------------------------------------------------------------- */
void scon2p(DERTBL *varp)
{
  NR_TYPE  sdwrp, tasx, EAW;

  sdwrp  = GetSample(varp, 0);
  tasx  = GetSample(varp, 1);

  /* Blow-up protection:  output zero while on ground (TASX < 30.0 m/s)
       installed by Ron Ruth  18 October 2001 */

  if (tasx < 30.0)
    {
    sdwrp = 0.0;
    tasx  = 30.0;
    }
  
  if (varp->name[5] == '1')
    EAW = P1_EAW;
  else
    EAW = P2_EAW;

  PutSample(varp, 1000.0 * (sdwrp / (EAW * 261.0 * tasx)));
}
