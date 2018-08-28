/*******      Potential Temperature (K)                                 theta
                 REQUIRES --- ATX, PSXC
 	Input:
 		atx - derived ambient temperature
 		psxc - derived static pressure
 	Output:
 		theta - derived potential temperature
*/
 
#include "nimbus.h"
#include "amlib.h"

double theta(double Tk, double psxc)
{
  if (psxc == 0.0)
    return 0.0;

  return Tk * pow((double)1000.0 / psxc, Rd_DIV_Cpd);
}

/* -------------------------------------------------------------------- */
void stheta(DERTBL *varp)
{
  double atx	= GetSample(varp, 0);
  double psxc	= GetSample(varp, 1);

  PutSample(varp, (NR_TYPE)theta(atx + Kelvin, psxc));
}
