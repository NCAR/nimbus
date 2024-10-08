/*******SSRD  SIDESLIP ANGLE OF THE RADOME (deg)                       SSRD

        Input:
                bdifr - raw radome differential pressure
                qcxc - corrected dynamic pressure for radome calcs
                mach - derived mach number (Sabreliner and B-57 only)
        Output:
                ssrd - attack angle of the radome (deg)
*/

#include "nimbus.h"
#include "amlib.h"

// Serial number for GV radome.  #2 was installed in January of 2013.
static int      gv_radome_ssn = 1;      // default to first radome.

// Serial number for C130 radome.  #2 was installed in May of 2013.
static int      c130_radome_ssn = 1;      // default to first radome.

// Fallback value for SSLIP if it goes missing.
// Propeller planes - https://en.wikipedia.org/wiki/P-factor
static const NR_TYPE GV_SSLIP_DEFAULT_VALUE = 0.0;
static const NR_TYPE C130_SSLIP_DEFAULT_VALUE = 1.1;

static std::vector<float> coeff;

extern int	FlightDate[];

/* -------------------------------------------------------------------- */
NR_TYPE defaultSSLIP()
{
  if (cfg.Aircraft() == Config::HIAPER)
    return(GV_SSLIP_DEFAULT_VALUE);
  else
    return(C130_SSLIP_DEFAULT_VALUE);
}

/* -------------------------------------------------------------------- */
void initSSRD(var_base *varp)
{
  float *tmp, ssn;

  /* Set default values per aircraft. */
  switch (cfg.Aircraft())
  {
    case Config::C130:
      if ( (tmp = GetDefaultsValue("C130_RADOME_SSN", varp->name)) )
      {
        c130_radome_ssn = (int)tmp[0];
        ssn = tmp[0];
      }
      varp->addToMetadata("RadomeSerialNumber", ssn);

      if (FlightDate[2] < 1998)
      {
        coeff.push_back(-0.886);
        coeff.push_back(12.658);
      }
      else
      {
        if (c130_radome_ssn == 1)
        {
          /*  New coefficients to handle ssrd offset to BDIFR - SOLAR CORONA on.
           *  Kept inactive pending final approval per AJS 12/18/98.
           */
          coeff.push_back(-0.012);
          coeff.push_back(12.21);
        }
        else	// 2015 and on
        {
          //coeff.push_back(1.545);	// Coopers memo 11/26/2014
          //coeff.push_back(12.852);
          coeff.push_back(1.5478);	// Coopers memo 09/15/2016
          coeff.push_back(12.3612);
        }
      }
      break;

    case Config::ELECTRA:
      coeff.push_back(0.57);
      coeff.push_back(15.204);
      break;

    case Config::NRL_P3:
      coeff.push_back(0.038);
      coeff.push_back(15.204);
      break;

    case Config::KINGAIR:
      coeff.push_back(-0.0379);
      coeff.push_back(13.4264);
      break;

    case Config::HIAPER:
      if ( (tmp = GetDefaultsValue("GV_RADOME_SSN", varp->name)) )
      {
        gv_radome_ssn = (int)tmp[0];
        ssn = tmp[0];
      }
      varp->addToMetadata("RadomeSerialNumber", ssn);

      if (gv_radome_ssn == 1)
        coeff.push_back(0.1051);
      else
        coeff.push_back(-0.05288);
      coeff.push_back(21.155);
      break;

    default:
      HandleFatalError("ssrd.c: No valid aircraft, no coefficients, exiting.");
  }

  if ((tmp = GetDefaultsValue("SSRD_COEFF", varp->name)) != NULL)
  {
    coeff.clear();
    coeff.push_back(tmp[0]);
    coeff.push_back(tmp[1]);
    sprintf(buffer,
	"ssrd: SSRD_COEFF set to %f, %f from Defaults file.\n",
	  coeff[0], coeff[1]);
    LogMessage(buffer);
  }
  varp->addToMetadata("CalibrationCoefficients", coeff);
}

/* -------------------------------------------------------------------- */
void sssrd(DERTBL *varp)
{
  NR_TYPE	qcxc, bdifr, ssrd = 0.0;

  bdifr	= GetSample(varp, 0);
  qcxc	= GetSample(varp, 1);

  if (std::isnan(qcxc))
    ssrd = floatNAN;
  else
  /* Blow-up protection:  output zero while on ground (QCX < 5.5 mbar)
   * installed by Ron Ruth  18 October 2001
   */
  if (qcxc > 5.5)
  {
    NR_TYPE ratio = bdifr / qcxc;
    switch (cfg.Aircraft())
    {
      case Config::SABRELINER:
      case Config::B57:
      {
        NR_TYPE mach = GetSample(varp, 2);
        ssrd = (ratio + 0.0241628 * mach - 0.0754196)
		/ (0.030640 + 0.0664595 * mach - 0.0881156 * mach*mach);
      }
        break;

      default:
        ssrd = coeff[0] + coeff[1] * ratio;
    }
  }

  PutSample(varp, ssrd);

}	/* END SSSRD */

/* END SSRD.C */
