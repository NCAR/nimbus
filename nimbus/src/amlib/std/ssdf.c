/*******SSDF  SIDESLIP ANGLE OF THE BOOM (deg)                         SSDF
 
     Input:
         bdif - raw boom differential pressure
         qcb - raw boom dynamic pressure
        xmach2 - derived mach number
     Output:
         ssdf - sideslip angle of the boom (deg)
*/

#include "nimbus.h"
#include "amlib.h"

/* -------------------------------------------------------------------- */
void sssdf(DERTBL *varp)
{
    NR_TYPE    qcb, bdif;
    NR_TYPE    ssdf;
    NR_TYPE    gr, divr;
    double    xmach2;

    bdif   = GetSample(varp, 0);
    qcb    = GetSample(varp, 1);
    xmach2 = GetSample(varp, 2);

  /* Blow-up protection:  output zero while on ground (QCX < 5.5 mbar)
       installed by Ron Ruth  18 October 2001 */

    if (qcb < 5.5)
      {
      ssdf = 0.0;
      }
    else
      {

      gr = (0.086577797 - 0.03560256 * sqrt(xmach2) + 0.00006143 * xmach2);

      divr = gr * qcb;

      if (divr == 0.0)
          divr = 0.00079;

      ssdf = bdif / divr;

/*    if (ssdf > 20.0)
          ssdf = 20.0;

      if (ssdf < -20.0)
          ssdf = -20.0;

      ssdf -= 0.7;    */

      }

    PutSample(varp, ssdf);

}
