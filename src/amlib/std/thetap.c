/*
-------------------------------------------------------------------------
OBJECT NAME:	thetap.c

FULL NAME:	Pseudo-adiabatic equivalent potential temperature

DESCRIPTION:	

INPUT:		ATX, PSXC, EDPC, MR

OUTPUT:		THETAP deg_K

REFERENCES:	Davies-Jones 2009, Bolton 1980

COPYRIGHT:	University Corporation for Atmospheric Research, 2011
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

// L0 and L1 from Davies-Jones 2009.
static const double L0 = 2.56313e+6;	// Jkg-1
static const double L1 = 1754.0;	// Jkg-1K-1
static const double K2 = 1.137e+6;	// Jkg-1

double theta(double Tk, double psxc);
double Tlcl(double Tk, double ew);


/* -------------------------------------------------------------------- */
void sthetap(DERTBL *varp)
{
  double Tk	= GetSample(varp, 0) + Kelvin;	// ATX
  double psxc	= GetSample(varp, 1);
  double ew	= GetSample(varp, 2);
  double r	= GetSample(varp, 3) / 1000.0;	// MR

  if (Tk <= 0.0)
    Tk = 0.0001;
  if (ew <= 0.0)
    ew = 0.0001;

  // Temperature at LCL.  Bolton 1980, Eq 21.
  double Tl = Tlcl(Tk, ew);

  // Potential Temperature of dry air at the LCL.
  double thetaDL = theta(Tk, psxc-ew) * pow((Tk / Tl), 0.28e-3 * r);

  // Davies-Jones 2009.
  double thetap = thetaDL * exp( ((L0-L1 * (Tl - Kelvin) + K2 * r) * r) / (Cpd * Tl) );

  PutSample(varp, (NR_TYPE)thetap);
}

/* -------------------------------------------------------------------- */
void sthetaq(DERTBL *varp)
{
  double Tk	= GetSample(varp, 0) + Kelvin;
  double psxc	= GetSample(varp, 1);
  double ew	= GetSample(varp, 2);
  double r	= GetSample(varp, 3) / 1000.0;	// MR / 1000.0
  double lwcc	= GetSample(varp, 4);	// PLWCC, equation fails with no LWC.
  double rhum	= GetSample(varp, 5);

  double Pd = (psxc - ew) * 100.0;
  double rt = r + lwcc / 1000.0 * Rd * Tk / Pd;
  double cpt = Cpd + rt * Cw;
  double lv = 2.501e6 + Lv * (Tk - Kelvin);

  double F1 = 1.0;
  if (rhum < 100.0)
    F1 = pow(rhum / 100.0, -(r * Rw / cpt));

  double thetaq = Tk * pow(100000.0 / Pd, Rd / cpt) * F1 * exp(lv * r / (cpt * Tk));

  PutSample(varp, (NR_TYPE)thetaq);
}
