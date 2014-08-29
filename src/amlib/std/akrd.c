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

// not static for export to initAC.c::pcor5_3().
std::vector<float> akrd_coeff;

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
        akrd_coeff.push_back(5.7763);
        akrd_coeff.push_back(15.0308);
      }
      else
      {
        akrd_coeff.push_back(4.7958);	// Coopers latest memo 8/5/2014
        akrd_coeff.push_back(12.7151);
      }
      break;

    case Config::ELECTRA:
      akrd_coeff.push_back(0.4095);
      akrd_coeff.push_back(0.07155);
      break;

    case Config::NRL_P3:
      akrd_coeff.push_back(0.3472);
      akrd_coeff.push_back(0.071442);
      break;

    case Config::KINGAIR:
      akrd_coeff.push_back(0.01414);
      akrd_coeff.push_back(0.08485);
      break;

    case Config::HIAPER:
      if ( (tmp = GetDefaultsValue("GV_RADOME_SSN", varp->name)) )
        gv_radome_ssn = (int)tmp[0];

      if (gv_radome_ssn == 1)
      {
        akrd_coeff.push_back(5.5156);	// Use with QCF, not QCXC
        akrd_coeff.push_back(19.0686);
        akrd_coeff.push_back(2.084);
      }
      else
      {	// New radome in 2013 - SANGRIAA-TEST and later.
//        akrd_coeff.push_back(4.604);	// Obsolete.
//        akrd_coeff.push_back(18.67);
//        akrd_coeff.push_back(6.49);

        akrd_coeff.push_back(4.6049);	// Cooper memo 8/15/2014; with QCF
        akrd_coeff.push_back(18.4376);
        akrd_coeff.push_back(6.7646);

//        akrd_coeff.push_back(4.422);	// CONTRAST specific, get into Defaults file.
//        akrd_coeff.push_back(20.58);
//        akrd_coeff.push_back(1.717);
      }
      break;

    default:
      HandleFatalError("akrd.c: No valid aircraft, no coefficients, exiting.");
  }

  if ((tmp = GetDefaultsValue("AKRD_COEFF", varp->name)) != NULL)
  {
    akrd_coeff.clear();
    akrd_coeff.push_back(tmp[0]);
    akrd_coeff.push_back(tmp[1]);
    akrd_coeff.push_back(tmp[2]);
    sprintf(buffer,
	"akrd: AKRD_COEFF set to %f, %f, %f from Defaults file.\n",
	  akrd_coeff[0], akrd_coeff[1], akrd_coeff[2]);
    LogMessage(buffer);
  }
  else
    AddToDefaults(varp->name, "CalibrationCoefficients", akrd_coeff);
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
        akrd = akrd_coeff[0] + akrd_coeff[1] * ratio;
        break;

      case Config::ELECTRA:
      case Config::NRL_P3:
      case Config::KINGAIR:
        akrd = (ratio + akrd_coeff[0]) / akrd_coeff[1];
        break;

      case Config::HIAPER:
        if (gv_radome_ssn == 1)
        {
          akrd = akrd_coeff[0] + akrd_coeff[1] * ratio;
        }
        else	// Radome #2, Jan 2013 and later.
        {
          double mach = GetSample(varp, 2);

          akrd = akrd_coeff[0] + ratio * (akrd_coeff[1] + akrd_coeff[2] * mach);
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
