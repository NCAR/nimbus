/*
-------------------------------------------------------------------------
OBJECT NAME:	akrd.c

FULL NAME:	Attack Angle from the Radome

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

static NR_TYPE coeff[3];

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

      if (c130_radome_ssn == 1)
      {
        coeff[0] = 5.7763;
        coeff[1] = 15.0308;
      }
      else
      {
        coeff[0] = 4.8;		// Coopers latest memo 8/5/2014
        coeff[1] = 12.715;
      }
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
        coeff[0] = 5.529309;
        coeff[1] = 20.427622;
        coeff[2] = 0.0;	// not used.
      }
      else
      {	// New radome in 2013 - SANGRIAA-TEST and later.
        coeff[0] = 4.604;
        coeff[1] = 18.67;
        coeff[2] = 6.49;
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
        akrd = coeff[0] + coeff[1] * ratio;
        break;

      case Config::ELECTRA:
      case Config::NRL_P3:
      case Config::KINGAIR:
        akrd = (ratio + coeff[0]) / coeff[1];
        break;

      case Config::HIAPER:
        if (gv_radome_ssn == 1)
        {
          akrd = coeff[0] + coeff[1] * ratio;
        }
        else	// Radome #2, Jan 2013 and later.
        {
          double mach = GetSample(varp, 2);

          akrd = coeff[0] + ratio * (coeff[1] + coeff[2] * mach);
        }
        break;

      case Config::SABRELINER:
      case Config::B57:
        {
        double    mach;

        mach = GetSample(varp, 2);
        akrd = adifr / (qcxc * (0.0719786 - 0.0331033 *
                      mach + 0.0109213 * mach*mach));
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
