/*******SSRD  SIDESLIP ANGLE OF THE RADOME (deg)                       SSRD
 
        Input:
                bdifr - raw radome differential pressure
                qcxc - corrected dynamic pressure for radome calcs
                xmach2 - derived mach number (Sabreliner and B-57 only)
        Output:
                ssrd - attack angle of the radome (deg)
*/

#include "nimbus.h"
#include "amlib.h"

static NR_TYPE coeff[2];

extern int	FlightDate[];

/* -------------------------------------------------------------------- */
void initSSRD(var_base *varp)
{
  /* Set default values per aircraft. */
  switch (cfg.Aircraft())
  {
    case Config::C130:
      if (FlightDate[2] < 1998)
      {
        coeff[0] = -0.07;
        coeff[1] = 0.079;
      }
      else
      {
        /*  New coefficients to handle ssrd offset to BDIFR - SOLAR CORONA on.
         *  Kept inactive pending final approval per AJS 12/18/98.
         */	
        coeff[0] = -0.000983;
        coeff[1] = 0.08189;
      }
      break;

    case Config::ELECTRA:
      coeff[0] = 0.0375;
      coeff[1] = 0.06577;
      break;

    case Config::NRL_P3:
      coeff[0] = 0.0025;
      coeff[1] = 0.06577;
      break;

    case Config::KINGAIR:
      coeff[0] = -0.002825;
      coeff[1] = 0.07448;
      break;

    case Config::HIAPER:
      coeff[0] = -0.0025;
      coeff[1] = 0.04727;
      break;

    default:
      LogMessage("ssrd.c: No valid aircraft, no coefficients, exiting.");
      exit(1);
  }
}

/* -------------------------------------------------------------------- */
void sssrd(DERTBL *varp)
{
  NR_TYPE	qcxc, bdifr, ssrd = 0.0;

  bdifr	= GetSample(varp, 0);
  qcxc	= GetSample(varp, 1);

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
        NR_TYPE xmach2 = GetSample(varp, 2);
        double xmach = sqrt(xmach2);
        ssrd = (ratio + 0.0241628 * xmach - 0.0754196)
		/ (0.030640 + 0.0664595 * xmach - 0.0881156 * xmach2);
      }
        break;

      default:
        ssrd = (ratio + coeff[0]) / coeff[1];
    }
  }

  PutSample(varp, ssrd);

}	/* END SSSRD */

/* END SSRD.C */
