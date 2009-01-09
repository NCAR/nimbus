/**************************** PLWCC *******************************  PLWCC
 ** Corrected PMS liquid water content (g/m3)
   Input:
     plwc - raw liquid water content
     tasx - derived true air speed
     atx  - derived ambient temperature
     psxc - derived static pressure
   Output:
     plwcc - corrected PMS liquid water content
*/

#include "nimbus.h"
#include "amlib.h"
 
/*  Values from /home/local/proj/Defaults on 23 April 1998, RLR		*/
static NR_TYPE  twire[3] = {155.0, 155.0, 155.0}; /* Wire temperature (K)	*/
static NR_TYPE  tasFac[3] = {1.0, 1.0, 1.0};	/* True airspeed factor	*/
static NR_TYPE  twireDiam[3] = {0.21, 0.21, 0.21}; /* Diameter	*/

NR_TYPE kinglwcc(NR_TYPE plwc, NR_TYPE tasx, NR_TYPE atx, NR_TYPE psxc, NR_TYPE twire, NR_TYPE diam);


/* -------------------------------------------------------------------- */
void plwccInit(var_base *varp)
{
  NR_TYPE	*tmp;

  if ((tmp = GetDefaultsValue("TWIRE_PMS", varp->name)) == NULL)
  {
    sprintf(buffer,"Value set to %f in AMLIB function plwccInit.\n", twire[0]);
    LogMessage(buffer);
  }
  else
    twire[0] = tmp[0];

  if ((tmp = GetDefaultsValue("TWIRE_TASFAC", varp->name)) == NULL)
  {
    sprintf(buffer,"Value set to %f in AMLIB function plwccInit.\n", tasFac[0]);
    LogMessage(buffer);
  }
  else
    tasFac[0] = tmp[0];

  if ((tmp = GetDefaultsValue("TWIRE_DIAM", varp->name)) == NULL)
  {
    sprintf(buffer,"Value set to %f in AMLIB function plwccInit.\n", twireDiam[0]);
    LogMessage(buffer);
  }
  else
    twireDiam[0] = tmp[0];

}	/* END PLWCCINIT */

/* -------------------------------------------------------------------- */
void plwcc1Init(var_base *varp)
{
  NR_TYPE	*tmp;

  if ((tmp = GetDefaultsValue("TWIRE_PMS1", varp->name)) == NULL)
  {
    sprintf(buffer,"Value set to %f in AMLIB function plwccInit.\n", twire[1]);
    LogMessage(buffer);
  }
  else
    twire[1] = tmp[0];

  if ((tmp = GetDefaultsValue("TWIRE_TASFAC1", varp->name)) == NULL)
  {
    sprintf(buffer,"Value set to %f in AMLIB function plwccInit.\n", tasFac[1]);
    LogMessage(buffer);
  }
  else
    tasFac[1] = tmp[0];

  if ((tmp = GetDefaultsValue("TWIRE_DIAM1", varp->name)) == NULL)
  {
    sprintf(buffer,"Value set to %f in AMLIB function plwccInit.\n", twireDiam[1]);
    LogMessage(buffer);
  }
  else
    twireDiam[1] = tmp[0];

}	/* END PLWCC1INIT */

/* -------------------------------------------------------------------- */
void splwcc(DERTBL *varp)
{
  NR_TYPE plwc, tasx, atx, psxc, concf;
  NR_TYPE prev_lwc, plwcc;

  static const int nSeconds = 5;
  static int buffIndex = 0;
  static NR_TYPE lwc_buffer[nSeconds], lwc_sum;

  plwc  = GetSample(varp, 0);
  tasx  = GetSample(varp, 1);
  atx   = GetSample(varp, 2);
  psxc  = GetSample(varp, 3);
 
  if (tasx < 30.0)
  {
    PutSample(varp, 0.0);
    return;
  }

  tasx *= tasFac[0];
  plwcc = kinglwcc(plwc, tasx, atx, psxc, twire[0], twireDiam[0]);

  if (varp->ndep >= 5)
  {
    concf = GetSample(varp, 4);

    if (concf < 0.25 && !isnan(plwcc))
    {
      if (++buffIndex >= nSeconds)
        buffIndex = 0;
  
      prev_lwc = lwc_buffer[buffIndex];
      lwc_buffer[buffIndex] = -plwcc;
      lwc_sum -= prev_lwc;
      lwc_sum += -plwcc;
    }
  }

  PutSample(varp, plwcc + lwc_sum / nSeconds);
}

/* -------------------------------------------------------------------- */
void splwcc1(DERTBL *varp)
{
  NR_TYPE  plwc, tasx, atx, psxc, concf;
  NR_TYPE prev_lwc, plwcc;

  static const int nSeconds = 10;
  static int buffIndex = 0;
  static NR_TYPE lwc_buffer[nSeconds], lwc_sum;

  plwc  = GetSample(varp, 0);
  tasx  = GetSample(varp, 1);
  atx   = GetSample(varp, 2);
  psxc  = GetSample(varp, 3);
 
  if (tasx < 30.0)
  {
    PutSample(varp, 0.0);
    return;
  }
  
  tasx *= tasFac[1];
  plwcc = kinglwcc(plwc, tasx, atx, psxc, twire[1], twireDiam[1]);

  if (varp->ndep >= 5)
  { 
    concf = GetSample(varp, 4);
    
    if (concf < 0.25 && !isnan(plwcc))
    {
      if (++buffIndex >= nSeconds)
        buffIndex = 0;

      prev_lwc = lwc_buffer[buffIndex];
      lwc_buffer[buffIndex] = -plwcc;
      lwc_sum -= prev_lwc;
      lwc_sum += -plwcc;
    }
  }
  
  PutSample(varp, plwcc + lwc_sum / nSeconds);
}
