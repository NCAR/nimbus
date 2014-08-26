/*******AKDF  ATTACK ANGLE OF THE BOOM (deg)                         AKDF
 
     Input:
	adif	- raw boom differential pressure
	qcb	- raw boom dynamic pressure
	mach	- derived mach number
     Output:
	akdf	- attack angle of the boom (deg)
*/

#include "nimbus.h"
#include "amlib.h"

/* -------------------------------------------------------------------- */
void sakdf(DERTBL *varp)
{
  NR_TYPE	qcb,adif;
  NR_TYPE	akdf;
  NR_TYPE	gr,divr;
  NR_TYPE	mach;

  adif	= GetSample(varp, 0);
  qcb	= GetSample(varp, 1);
  mach	= GetSample(varp, 2);

  /* Blow-up protection:  output zero while on ground (QCX < 5.5 mbar)
       installed by Ron Ruth  18 October 2001 */

  if (qcb < 5.5)
  {
    akdf = 0.0;
  }
  else
  {
    gr   = 0.086577797 - 0.03560256 * mach + 0.00006143 * mach*mach;
    divr = gr * qcb;

    if (divr == 0)
      divr = 0.00079;

    akdf = adif / divr;
  }

  PutSample(varp, akdf);

}    /* END SAKDF */

/* END AKDF.C */
