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

extern NR_TYPE	(*pcorQCR)(NR_TYPE, NR_TYPE);
NR_TYPE pcorf5_3(NR_TYPE Qm, NR_TYPE Pm, NR_TYPE Adif, NR_TYPE Qr);


/* -------------------------------------------------------------------- */
void sqcrc(DERTBL *varp)
{
  NR_TYPE	qcr, qcrc = 0.0, adifr, bdifr, psf, mach, aqratio, qcf;
  NR_TYPE	bqcrc, satk3, sbeta3;
  double	atk3, beta3;

  qcr	= GetSample(varp, 0);

  if (qcr < 0.01)
    qcr = 0.01;

  switch (cfg.Aircraft())
  {
    case Config::HIAPER:
      psf = GetSample(varp, 0);
      qcf = GetSample(varp, 1);
      adifr = GetSample(varp, 2);
      qcr = GetSample(varp, 3);

      qcrc = qcr - pcorf5_3(qcf, psf, adifr, qcr);
      break;

    case Config::C130:
      aqratio = GetSample(varp, 1);	// aqratio = adfir / qcf
      psf = GetSample(varp, 2);
      mach = GetSample(varp, 3);
      qcrc = qcr - psf * (*pcorQCR)(aqratio, mach*mach);
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

      qcrc = qcr - (*pcorQCR)(qcr, 1.0);
      break;

    case Config::NRL_P3:
      adifr = GetSample(varp, 1);
      bqcrc = 0.3 + adifr * (0.0402 + 0.0013 * adifr);

      if (bqcrc > 0.8)
        bqcrc = 0.8;

      qcrc = qcr - (*pcorQCR)(qcr,1.0) + bqcrc;
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

      qcrc	= (qcr / bqcrc) - (*pcorQCR)(qcr,1.0);
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

      qcrc    = (qcr / bqcrc) - (*pcorQCR)(qcr,1.0);
      break;
    }

  if (qcr < 5.0)
    qcrc = qcr;

  PutSample(varp, qcrc);

}	/* END SQCRC */

/* END QCRC.C */
