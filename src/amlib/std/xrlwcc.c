/**************************** XRLWCC *******************************  XRLWCC
 ** Corrected PMS liquid water content (g/m3) - RAF digital controlled probe
   Input:
     plwc - raw liquid water content
     tasx - derived true air speed
     atx - derived ambient temperature
     psxc - derived static pressure
   Output:
     xrlwcc - corrected PMS liquid water content
*/

#include "nimbus.h"
#include "amlib.h"

/*  Value from /home/local/proj/Defaults on 23 April 1998           RLR */
static NR_TYPE twire = 155.0;

NR_TYPE kinglwcc(NR_TYPE plwc, NR_TYPE tasx, NR_TYPE atx, NR_TYPE psxc, NR_TYPE twire, NR_TYPE diam);

/* -------------------------------------------------------------------- */
void xrlwccInit(var_base *varp)
{
  float  *tmp;
  if ((tmp = GetDefaultsValue("TWIRE_RAF", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function xrlwccInit.\n", twire);
    LogMessage(buffer);
  }
  else
    twire = tmp[0];

}  /* END XRLWCCINIT */

/* -------------------------------------------------------------------- */
void sxrlwcc(DERTBL *varp)
{
  NR_TYPE plwc,tasx,atx,psxc;

  plwc  = GetSample(varp, 0);
  tasx  = GetSample(varp, 1);
  atx  = GetSample(varp, 2);
  psxc  = GetSample(varp, 3);

  /* Blow-up protection:  output zero while on ground (TASX < 30.0 m/s)
       installed by Ron Ruth  18 October 2001 */

  if (tasx < 30.0)
    {
    plwc = 0.0;
    PutSample(varp, plwc);
    }
  else
    {
    PutSample(varp, kinglwcc(plwc, tasx, atx, psxc, twire));
    }
}
