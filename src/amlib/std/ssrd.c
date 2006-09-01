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

extern int	FlightDate[];

/* -------------------------------------------------------------------- */
void sssrd(DERTBL *varp)
{
  NR_TYPE	qcxc, bdifr;
  NR_TYPE	ssrd, xmach2;
  double	xmach;

  bdifr	= GetSample(varp, 0);
  qcxc	= GetSample(varp, 1);

  /* Blow-up protection:  output zero while on ground (QCX < 5.5 mbar)
       installed by Ron Ruth  18 October 2001 */

  if (qcxc < 5.5)
    {
      ssrd = 0.0;
    }
  else
    {
    switch (cfg.Aircraft())
      {
      case Config::ELECTRA:
	ssrd = ((bdifr / qcxc) + 0.0375) / 0.06577;
	break;

      case Config::NRL_P3:
        ssrd = ((bdifr / qcxc) + 0.0025) / 0.06577;
        break;

      case Config::C130:
        if (FlightDate[2] < 1998)
          ssrd = ((bdifr / qcxc) - 0.070) / 0.079;
        else
/*  New coefficients to handle ssrd offset to BDIFR - SOLAR CORONA on.
 *  Kept inactive pending final approval per AJS 12/18/98.
 */	
          ssrd = ((bdifr / qcxc) - 0.000983) / 0.08189;
        break;

      case Config::KINGAIR:
        ssrd = ((bdifr / qcxc) - 0.002825) / 0.07448;
        break;

      case Config::HIAPER:
//        ssrd = ((bdifr / qcxc) -0.0013) / 0.075;
        ssrd = ((bdifr / qcxc) -0.0025) / 0.04727;
        break;

      case Config::SABRELINER:
      case Config::B57:
        xmach2 = GetSample(varp, 2);

        xmach = sqrt(xmach2);
        ssrd = ((bdifr / qcxc) + 0.0241628 * xmach - 0.0754196)
			/ (0.030640 + 0.0664595 * xmach - 0.0881156 * xmach2);
        break;

      default:
        ssrd = 0.0;
      }
    }

  PutSample(varp, ssrd);

}	/* END SSSRD */

/* END SSRD.C */
