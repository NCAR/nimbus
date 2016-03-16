/*
-------------------------------------------------------------------------
OBJECT NAME:	akrd.c

FULL NAME:	Attack Angle from the Radome

DESCRIPTION:    

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2016
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

// Serial number for GV radome.  #2 was installed in January of 2013.
static int	gv_radome_ssn = 1;	// default to first radome.

// Serial number for C130 radome.  #2 was installed in May of 2013.
static int      c130_radome_ssn = 1;      // default to first radome.

static std::vector<float> akrd_coeff;
static std::vector<float> low, mid, high;	// Altitude specific coef's.


/* -------------------------------------------------------------------- */
static std::vector<float> load_AKRD_Default(var_base *varp, const char name[])
{
  float *tmp;
  std::vector<float> c = akrd_coeff;

  if ((tmp = GetDefaultsValue(name, varp->name)) != NULL)
  {
    c.clear();
    c.push_back(tmp[0]);
    c.push_back(tmp[1]);
    c.push_back(tmp[2]);
    sprintf(buffer,
	"akrd: %s set to %f, %f, %f from Defaults file.\n", name,
	  c[0], c[1], c[2]);
    LogMessage(buffer);
  }
  else
    AddToDefaults(varp->name, "CalibrationCoefficients", c);

  return c;
}

/* -------------------------------------------------------------------- */
void initAKRD(var_base *varp)
{
  float *tmp;

  if (strstr(varp->name, "_GP"))	// Don't do anything yet for GP.
    return;

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
        akrd_coeff.push_back(4.852);	// Coopers latest memo 11/26/2014
        akrd_coeff.push_back(13.23);
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

  akrd_coeff = load_AKRD_Default(varp, "AKRD_COEFF");
  if (cfg.ProjectName().compare("CSET") == 0)
  {
    low = load_AKRD_Default(varp, "AKRD_COEFF_LOW");
    mid = load_AKRD_Default(varp, "AKRD_COEFF_MID");
    high = load_AKRD_Default(varp, "AKRD_COEFF_HIGH");
  }
}

/* -------------------------------------------------------------------- */
void sakrd(DERTBL *varp)
{
  NR_TYPE qc, psf, adifr, akrd = 0.0, mach;

  adifr	= GetSample(varp, 0);
  qc	= GetSample(varp, 1);

  /* Blow-up protection:  output zero while on ground (QCX < 5.5 mbar)
   * installed by Ron Ruth  18 October 2001
   */
  if (qc > 5.5)
  {
    NR_TYPE ratio = adifr / qc;
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
        psf = GetSample(varp, 2);	// PSF
        mach = sqrt( 5.0 * (pow((qc+psf)/psf, Rd_DIV_Cpd) - 1.0) ); // Mach #
        if (varp->ndep == 4)
        {
          NR_TYPE alt = GetSample(varp, 3);
          if (alt < 6500) akrd_coeff = low; else
          if (alt < 9300) akrd_coeff = mid; else
          akrd_coeff = high;
        }

        akrd = akrd_coeff[0] + ratio * (akrd_coeff[1] + akrd_coeff[2] * mach);
        break;

      case Config::SABRELINER:
      case Config::B57:
        mach = GetSample(varp, 2);
        akrd = adifr / (qc * (0.0719786 - 0.0331033 *
                      mach + 0.0109213 * mach*mach));
        break;

      default:
        akrd = 0.0;
      }
    }
  else
  if (isnan(adifr) || isnan(qc))
    akrd = floatNAN;

  PutSample(varp, akrd);

}    /* END SAKRD */

/* END AKRD.C */
