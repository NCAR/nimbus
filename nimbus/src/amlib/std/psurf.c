/************************* PSURF **********************************  PSURF
 **    Surface Pressure (mb)
   Input:
     hgm - raw geometric (radio) altitude (m)   
     atx - derived ambient temperature 
     mr - derived mixing ratio
     psxc - corrected static pressure
   Output:
     psurf - derived surface pressure
*/

#include "nimbus.h"
#include "amlib.h"

/* Default should be 750 Meters for older projects with HGM installed.  */
/* static NR_TYPE PSURF_ALT_MAX = 750.0;
 */
/*  Value from /home/local/proj/Defaults on 23 April 1998           RLR */
static NR_TYPE PSURF_ALT_MAX = 7000.0; /* Altitude cut-off (M)          */

/* -------------------------------------------------------------------- */
void initPsurf(DERTBL *varp)
{
  NR_TYPE  *tmp;
  if ((tmp = GetDefaultsValue("PSURF_ALT_MAX", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function initPsurf.\n", PSURF_ALT_MAX);
    LogMessage(buffer);
  }
  else
/*  (Original threshold test was PSURF_ALT_MAX > 10.0)   */
    PSURF_ALT_MAX = tmp[0];

}       /* END INITPSURF */

/* -------------------------------------------------------------------- */
void spsurf(DERTBL *varp)
{
  NR_TYPE  psurf, atx, hgm, mr, psxc;
  NR_TYPE  tvir, tbarm;

  hgm  = GetSample(varp, 0);
  atx  = GetSample(varp, 1);
  mr  = GetSample(varp, 2);
  psxc  = GetSample(varp, 3);

  tvir  = (atx + 273.16) * ((1.0 + 1.6078e-3 * mr) / (1.0 + 0.001 * mr));
  tbarm = tvir + 0.5 * (hgm * 0.0098);
  psurf = psxc * exp((double)((hgm / tbarm) * 0.0341833194));

  if (hgm > PSURF_ALT_MAX)
    psurf = 1000.0;

  PutSample(varp, psurf);

}  /* END SPSURF */

/* END PSURF.C */
