/*************************** XMACH2 *******************************  XMACH2
 ** primary mach number squared
        Input:
                qcxc - derived dynamic pressure
                psxc - derived static pressure
        Output:
                xmach2 - derived mach number squared
        Include:
                xmac2 - mach number funtion
*/

#include "nimbus.h"
#include "amlib.h"
      
/* -------------------------------------------------------------------- */
void sxmach2(DERTBL *varp)
{
  NR_TYPE	qcxc, psxc;

  qcxc = GetSample(varp, 0);
  psxc = GetSample(varp, 1);

  PutSample(varp, XMAC2(qcxc / psxc));
}
