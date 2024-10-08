/************************ QCRC ***********************************  QCRC
 ** CORRECTED RADOME DYNAMIC PRESSURE (MB) -- KING AIR 312
 ** REQUIRES:1-QCR;2-ADIFR;3-BDIFR
 	Input:
 		qcr - raw radome dynamic pressure
 		adifr - raw differential pressure
 		bdifr - raw differential pressure
 	Output:
 		qcrc - corrected radome dynamic pressure
 	Include:
 		pcorr - radome static defect correction
*/


#include "nimbus.h"
#include "amlib.h"

extern NR_TYPE	(*pcorQCR)(NR_TYPE);
extern NR_TYPE	(*pcorQCRv2)(NR_TYPE, NR_TYPE, NR_TYPE);

NR_TYPE defaultATTACK();

/* -------------------------------------------------------------------- */
void sqcrc(DERTBL *varp)
{
  NR_TYPE	qcr, qcrc = 0.0, attack, adifr, bdifr, psf, mach, aqratio, qcf;
  NR_TYPE	bqcrc, satk3, sbeta3;
  double	atk3, beta3;

  qcr	= GetSample(varp, 0);

  if (qcr < 0.1)
    qcr = 0.1;

  switch (cfg.Aircraft())
  {
    case Config::HIAPER:
    case Config::C130:
      psf = GetSample(varp, 1);
      qcf = GetSample(varp, 2);
      attack = GetSample(varp, 3);

      if (std::isnan(attack))
        attack = defaultATTACK();

      qcrc = qcr - (*pcorQCRv2)(qcf, psf, attack);
      break;

    case Config::ELECTRA:
      adifr = GetSample(varp, 1);
      bdifr = GetSample(varp, 2);

      atk3 = fabs((double)((adifr / qcr) + 0.4095) / 0.0715);
      beta3= fabs((double)((bdifr / qcr) + 0.0375) / 0.06577);
      satk3	= sin(atk3 * DEG_RAD);
      sbeta3	= sin(beta3 * DEG_RAD);
      bqcrc	= (1.0 - 2.25 * satk3 * satk3) *
		  (1.0 - 2.25 * sbeta3 * sbeta3);

      if (bqcrc == 0.0)
        bqcrc = 0.0001;
/* Electra only
      qcrc	= qcr * (1.02633 - 0.00819 * atk3)  / bqcrc -
						(*pcorQCR)(qcr,1.0);	*/

      qcrc = qcr - (*pcorQCR)(qcr);
      break;

    case Config::NRL_P3:
      adifr = GetSample(varp, 1);
      bqcrc = 0.3 + adifr * (0.0402 + 0.0013 * adifr);

      if (bqcrc > 0.8)
        bqcrc = 0.8;

      qcrc = qcr - (*pcorQCR)(qcr) + bqcrc;
      break;

    case Config::KINGAIR:
      adifr = GetSample(varp, 1);
      bdifr = GetSample(varp, 2);

      atk3	= fabs((double)adifr / (qcr * 0.08485));
      beta3	= fabs((double)bdifr / (qcr * 0.07448));
      satk3	= sin(atk3 * DEG_RAD);
      sbeta3	= sin(beta3 * DEG_RAD);
      bqcrc	= (1.0 - 2.25 * satk3 * satk3) *
		  (1.0 - 2.25 * sbeta3 * sbeta3);

      if (bqcrc == 0.0)
        bqcrc = 0.0001;

      qcrc	= (qcr / bqcrc) - (*pcorQCR)(qcr);
      break;

    case Config::SABRELINER:
      adifr = GetSample(varp, 1);
      bdifr = GetSample(varp, 2);

      atk3    = fabs((double)adifr / (qcr * 0.057));
      beta3   = fabs((double)bdifr / (qcr * 0.041));
      satk3   = sin(atk3 * DEG_RAD);
      sbeta3  = sin(beta3 * DEG_RAD);
      bqcrc   = (1.0 - 2.25 * satk3 * satk3) *
		(1.0 - 2.25 * sbeta3 * sbeta3);

      if (bqcrc == 0.0)
        bqcrc = 0.0001;

      qcrc    = (qcr / bqcrc) - (*pcorQCR)(qcr);
      break;
    }

  if (qcr < 5.0)
    qcrc = qcr;

  PutSample(varp, qcrc);

}	/* END SQCRC */

/* END QCRC.C */
