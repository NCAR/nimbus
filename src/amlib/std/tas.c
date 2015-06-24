/*   TAS = TRUE AIRSPEED CALCULATION FUNCTION (m/s)                     TAS */

#include "nimbus.h"
#include "amlib.h"


static const double Yd = Cpd / Cvd;

/* -------------------------------------------------------------------- */
NR_TYPE compute_tas(NR_TYPE t, NR_TYPE m)
{
  double Y = Cp() / Cv();
  double term = m * sqrt(Y * R() * (t + Kelvin));	// Using ATX

  if (term < 0.0)
    term = -term;

  return term;
}

/* -------------------------------------------------------------------- */
void stas_flag(DERTBL *varp)
{
  NR_TYPE q = GetSample(varp, 0);
  NR_TYPE p = GetSample(varp, 1);
  NR_TYPE rt = GetSample(varp, 2);
  NR_TYPE ew = GetSample(varp, 3);

  void setEOP(double);
  setEOP(0.0);

/*
  if (!isnan(ew) && !isnan(p))
  {
    setEOP(ew/p);
  }
*/
  PutSample(varp, 0.0);
}

/* -------------------------------------------------------------------- */
void stas(DERTBL *varp)
{
  NR_TYPE mach	= GetSample(varp, 0);
  NR_TYPE at	= GetSample(varp, 1);

  NR_TYPE tas	= compute_tas(at, mach);

  PutSample(varp, tas);
}
