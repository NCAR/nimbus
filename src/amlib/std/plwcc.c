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
#include "raf.h"
 
/*  Values from /home/local/proj/Defaults on 23 April 1998, RLR		*/
static NR_TYPE  twire[3] = {155.0, 155.0, 155.0}; /* Wire temperature (K)	*/
static NR_TYPE  tasFac[3] = {1.0, 1.0, 1.0};	/* True airspeed factor	*/
static NR_TYPE  twireDiam[3] = {0.21, 0.21, 0.21}; /* Diameter	*/

NR_TYPE kinglwcc(NR_TYPE plwc, NR_TYPE tasx, NR_TYPE atx, NR_TYPE psxc, NR_TYPE twire, NR_TYPE diam);


/* -------------------------------------------------------------------- */
void plwccInit(DERTBL *varp)
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
void plwcc1Init(DERTBL *varp)
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
void plwcc2Init(DERTBL *varp)
{
  NR_TYPE	*tmp;

  if ((tmp = GetDefaultsValue("TWIRE_PMS2", varp->name)) == NULL)
    {
    sprintf(buffer,"Value set to %f in AMLIB function plwccInit.\n", twire[2]);
    LogMessage(buffer);
    }
  else
    twire[2] = tmp[0];

  if ((tmp = GetDefaultsValue("TWIRE_TASFAC2", varp->name)) == NULL)
    {
    sprintf(buffer,"Value set to %f in AMLIB function plwccInit.\n", tasFac[2]);
    LogMessage(buffer);
    }
  else
    tasFac[2] = tmp[0];

  if ((tmp = GetDefaultsValue("TWIRE_DIAM2", varp->name)) == NULL)
    {
    sprintf(buffer,"Value set to %f in AMLIB function plwccInit.\n", twireDiam[2]);
    LogMessage(buffer);
    }
  else
    twireDiam[2] = tmp[0];

}	/* END PLWCC2INIT */

/* -------------------------------------------------------------------- */
void splwcc(DERTBL *varp)
{
  NR_TYPE  plwc, tasx, atx, psxc;

  plwc  = GetSample(varp, 0);
  tasx  = GetSample(varp, 1);
  atx   = GetSample(varp, 2);
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
    tasx *= tasFac[0];
    PutSample(varp, kinglwcc(plwc, tasx, atx, psxc, twire[0], twireDiam[0]));
    }

}  /* END SPLWCC */

/* -------------------------------------------------------------------- */
void splwcc1(DERTBL *varp)
{
  NR_TYPE  plwc, tasx, atx, psxc;

  plwc  = GetSample(varp, 0);
  tasx  = GetSample(varp, 1);
  atx   = GetSample(varp, 2);
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
    tasx *= tasFac[1];
    PutSample(varp, kinglwcc(plwc, tasx, atx, psxc, twire[1], twireDiam[1]));
    }

}  /* END SPLWCC1 */
/* -------------------------------------------------------------------- */
void splwcc2(DERTBL *varp)
{
  NR_TYPE  plwc, tasx, atx, psxc;

  plwc  = GetSample(varp, 0);
  tasx  = GetSample(varp, 1);
  atx   = GetSample(varp, 2);
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
    tasx *= tasFac[2];
    PutSample(varp, kinglwcc(plwc, tasx, atx, psxc, twire[2], twireDiam[2]));
    }

}  /* END SPLWCC1 */

/* END PLWCC.C */
