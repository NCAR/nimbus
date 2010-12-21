/*
-------------------------------------------------------------------------
OBJECT NAME:	akrd.c

FULL NAME:	Attack Angle of the Radome

ENTRY POINTS:	sakrd()

STATIC FNS:	none

DESCRIPTION:    

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2006
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static NR_TYPE coeff[2];

/* -------------------------------------------------------------------- */
void initAKRD(var_base *varp)
{
  /* Set default values per aircraft. */
  switch (cfg.Aircraft())
  {
    case Config::C130:
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
      coeff[0] = 0.2571;
      coeff[1] = 0.04727;
      break;

    default:
      LogMessage("akrd.c: No valid aircraft, no coefficients, exiting.");
      exit(1);
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
        double xmach2 = GetSample(varp, 2);
        double akcor = (0.6195 - 1.02758 * xmach2);

        if (akcor > 0.42)
          akcor = 0.42;
 
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

  PutSample(varp, akrd);

}    /* END SAKRD */

/* END AKRD.C */
