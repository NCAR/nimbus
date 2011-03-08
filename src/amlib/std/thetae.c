/*******       Equivalent Potential Temperature (K)                     thetae
	Bolton (1980) parameterization

 	Input:
 		atx - derived ambient temperature
 		psxc - derived static pressure
 		mr - derived mixing ration
 	Output:
 		thetae - derived equivalent potential temperature
*/
 
#include "nimbus.h"
#include "amlib.h"

double theta(double Tk, double psxc);


/* -------------------------------------------------------------------- */
// Temperature at LCL.  Bolton 1980, Eq 21.
double Tlcl(double Tk, double edpc)
{
  double tl = (2840.0 / (3.5 * log(Tk) - log(edpc) - 4.805)) + 55.0;

  if (tl <= 0.0)
    return 0.0001;

  return tl;
}

/* -------------------------------------------------------------------- */
void sthetae(DERTBL *varp)
{
  double Tk	= GetSample(varp, 0) + Kelvin;	// ATX
  double psxc	= GetSample(varp, 1);
  double edpc	= GetSample(varp, 2);
  double mr	= GetSample(varp, 3);

  if (Tk <= 0.0)
    Tk = 0.0001;
  if (edpc <= 0.0)
    edpc = 0.0001;

  double exparg = ((3.376 / Tlcl(Tk, edpc)) - 0.00254) * (mr * (1.0 + (0.00081 * mr)));
  if (exparg > 741.0)
    exparg = 0.0001;

  double thetae = theta(Tk, psxc) * exp((double)exparg);
  PutSample(varp, (NR_TYPE)thetae);
}
