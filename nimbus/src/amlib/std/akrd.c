/*
-------------------------------------------------------------------------
OBJECT NAME:    akrd.c

FULL NAME:    Attack Angle of the Radome

ENTRY POINTS:    sakrd()

STATIC FNS:    none

DESCRIPTION:    

INPUT:        

OUTPUT:        

REFERENCES:    none

REFERENCED BY:    ComputeDerived()

COPYRIGHT:    University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "raf.h"
#include "nimbus.h"
#include "amlib.h"

extern int    Aircraft;

/* -------------------------------------------------------------------- */
void sakrd(DERTBL *varp)
{
  NR_TYPE    qcxc, adifr, pitch, thedot, pfactor;
  NR_TYPE    akrd, ratio;

  adifr    = GetSample(varp, 0);
  qcxc    = GetSample(varp, 1);

  /* Blow-up protection:  output zero while on ground (QCX < 5.5 mbar)
       installed by Ron Ruth  18 October 2001 */

  if (qcxc < 5.5)
    {
      akrd = 0.0;
    }
  else
    {
    ratio = adifr / qcxc;
    switch (Aircraft)
      {
      case C130:
        if ( ratio >= -0.28)        
        akrd = (( ratio + 0.3843 ) / (0.06653));
/*        akrd = (( ratio + 0.3898 ) / (0.06630));        */
        else

/*    Adjustment made 12/5/96 based on results of C130 - Electra
      Intercomparison flight conducted on 11/18/96 - ajs     

        akrd = -17.150 + ratio * (-201.5707 + ratio * (-673.6239 
          -691.433565 * ratio));                 */

/*    Modified 5/14/98 to eliminate inflection point         */

        akrd = -69.9020453 + ratio * (-830.086276 + ratio * (-3497.34097
          + ratio * (-6339.88129 - 4219.58508 * ratio)));     


        break;

      case ELECTRA:
	akrd = ((adifr / qcxc) + 0.4095) / 0.07155;
	break;

      case NRL_P3:
        akrd = ((adifr / qcxc) + 0.3472) / 0.071442;
        break;

      case KINGAIR:
        akrd = ((adifr / qcxc) + 0.01414) / 0.08485;
        break;

      case SABRELINER:
      case B57:
        {
        double    xmach2;

        xmach2 = GetSample(varp, 2);
        akrd = adifr / (qcxc * (0.0719786 - 0.0331033 *
                      sqrt(xmach2) + 0.0109213*xmach2));
        }
        break;

      default:
        akrd = 0.0;
      }
    }

  PutSample(varp, akrd);

}    /* END SAKRD */

/* END AKRD.C */
