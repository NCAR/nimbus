/*
-------------------------------------------------------------------------
OBJECT NAME:	akrd.c

FULL NAME:	Attack Angle of the Radome

ENTRY POINTS:	sakrd()

STATIC FNS:	none

DESCRIPTION:    

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2013
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

// Serial number for GV radome.  #2 was installed in January of 2013.
static int	gv_radome_ssn = 1;	// default to first radome.

// Serial number for C130 radome.  #2 was installed in May of 2013.
static int      c130_radome_ssn = 1;      // default to first radome.

static NR_TYPE coeff[2];

/* -------------------------------------------------------------------- */
void initAKRD(var_base *varp)
{
  float *tmp;

  /* Set default values per aircraft. */
  switch (cfg.Aircraft())
  {
    case Config::C130:
      if ( (tmp = GetDefaultsValue("C130_RADOME_SSN", varp->name)) )
        c130_radome_ssn = (int)tmp[0];

      coeff[0] = 0.3843;
      coeff[1] = 0.06653;
      break;

    case Config::ELECTRA:
      coeff[0] = 0.4095;
      coeff[1] = 0.07155;
      break;

    case Config::NRL_P3:
      coeff[0] = 0.3472;
      coeff[1] = 0.071442;
      break;

    case Config::KINGAIR:
      coeff[0] = 0.01414;
      coeff[1] = 0.08485;
      break;

    case Config::HIAPER:
      if ( (tmp = GetDefaultsValue("GV_RADOME_SSN", varp->name)) )
        gv_radome_ssn = (int)tmp[0];

      if (gv_radome_ssn == 1)
      {
        coeff[0] = 0.2571;
        coeff[1] = 0.04727;
      }
      else
      {	// New radome in 2013 - SANGRIAA-TEST and later.
        coeff[0] = 0.20725;
        coeff[1] = 0.04688;
      }

      break;

    default:
      HandleFatalError("akrd.c: No valid aircraft, no coefficients, exiting.");
  }
}

/* -------------------------------------------------------------------- */
void sakrd(DERTBL *varp)
{
  NR_TYPE qcxc, adifr, akrd = 0.0;

  adifr	= GetSample(varp, 0);
  qcxc	= GetSample(varp, 1);

  /* Blow-up protection:  output zero while on ground (QCX < 5.5 mbar)
   * installed by Ron Ruth  18 October 2001
   */
  if (qcxc > 5.5)
  {
    NR_TYPE ratio = adifr / qcxc;
    switch (cfg.Aircraft())
    {
      case Config::C130:
      case Config::ELECTRA:
      case Config::NRL_P3:
      case Config::KINGAIR:
        akrd = (ratio + coeff[0]) / coeff[1];
        break;

      case Config::HIAPER:
        {
        double akcor = 0.0;

        if (gv_radome_ssn == 1)
        {
          double xmach2 = GetSample(varp, 2);
          akcor = std::max(0.6195 - 1.02758 * xmach2, 0.42);

          /* Attempt to repair poor AOA during low/slow flying (high pitch) due
           * to non perfectly hemispherical radome.  Added by AJS 4/3/12.
           */
         if (akrd > 4.0)
           akrd = (1.27 + akrd * (0.56714 + 0.028571 * akrd));
        }
        else	// Radome #2, Jan 2013 and later.
        {
          double atx = GetSample(varp, 2);
          double psxc = GetSample(varp, 3);
          double density_cor = (psxc / (atx + 273.15));

          if (density_cor >= 0.9074 && density_cor <= 2.0)
            akcor = (5.34 + density_cor * (-11.192 + density_cor * (7.1669 - 1.4519 * density_cor)));
        }

        akrd = ((ratio + coeff[0]) / coeff[1]) + akcor;
        }
        break;

      case Config::SABRELINER:
      case Config::B57:
        {
        double    xmach2;

        xmach2 = GetSample(varp, 2);
        akrd = adifr / (qcxc * (0.0719786 - 0.0331033 *
                      sqrt(xmach2) + 0.0109213 * xmach2));
        }
        break;

      default:
        akrd = 0.0;
      }
    }
  else
  if (isnan(adifr) || isnan(qcxc))
    akrd = floatNAN;

  PutSample(varp, akrd);

}    /* END SAKRD */

/* END AKRD.C */
