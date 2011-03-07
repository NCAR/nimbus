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


/* -------------------------------------------------------------------- */
void sthetap(DERTBL *varp)
{
  double Tk	= GetSample(varp, 0) + Kelvin;	// ATX
  double psxc	= GetSample(varp, 1);
  double edpc	= GetSample(varp, 2);
  double r	= GetSample(varp, 3) / 1000.0;	// MR

  if (Tk <= 0.0)
    Tk = 0.0001;
  if (edpc <= 0.0)
    edpc = 0.0001;

  // Temperature at LCL.  Bolton 1980, Eq 21.
  double Tl = 2840.0 / (3.5 * log(Tk) - log(edpc) - 4.805) + 55.0;

  if (Tl <= 0.0)
    Tl = 0.0001;

  // Potential Temperature of dry air at the LCL.
  double thetaDL = Tk * pow((1000.0 / (psxc-edpc)), Rd_DIV_Cpd) * pow((Tk / Tl), 0.28e-3 * r);

  // Davies-Jones 2009.
  double thetap = thetaDL * exp( ((L0-L1 * (Tl - Kelvin) + K2 * r) * r) / (Cp * Tl) );

  PutSample(varp, (NR_TYPE)thetap);
}
