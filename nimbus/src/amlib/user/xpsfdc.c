/* Only used for project 280????
 */



/*********************** PSFDC ************************************  PSFDC
 ** Corrected static pressure (fuselage digital) (mb)
 	Input:
 		psfd - raw digital fuselage static pressure
 		qcx  - default raw dynamic pressure
 	Output:
 		psfdc - corrected static pressure
 	Include:
 		pcord - dynamic correction for digital static pressure
*/

#include "nimbus.h"
#include "amlib.h"

/* -------------------------------------------------------------------- */
NR_TYPE newPCOR(NR_TYPE mach, NR_TYPE attack)
{
  return(-0.09356 + 4.47474 * mach - 0.145661 * attack);

}

/* -------------------------------------------------------------------- */
void sxpsfdc(DERTBL *varp)
{
  NR_TYPE	psfd, mach, attack, psfdc, qcx;

  psfd	= GetSample(varp, 0);
  qcx	= GetSample(varp, 1);
  mach	= GetSample(varp, 2);
  attack	= GetSample(varp, 3);

  psfdc = psfd + newPCOR(mach, attack);

  if (qcx < 10.0)
    psfdc = psfd;

  if (psfdc < 50.0)
    psfdc = 50.0;

  PutSample(varp, psfdc);

}	/* END SPSFDC */

/* END PSFDC.C */
