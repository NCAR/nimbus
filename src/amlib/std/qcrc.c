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
#include "raf.h"

extern NR_TYPE	(*pcorQCR)(NR_TYPE, NR_TYPE);

extern int	Aircraft;

/* -------------------------------------------------------------------- */
void sqcrc(DERTBL *varp)
{
	NR_TYPE	qcr, akrd, qcrc = 0.0, adifr, bdifr;
	NR_TYPE	bqcrc, satk3, sbeta3;
	double	atk3, beta3;

	qcr	= GetSample(varp, 0);
	adifr	= GetSample(varp, 1);
	bdifr	= GetSample(varp, 2);
	akrd    = GetSample(varp, 3);

	if (qcr < 0.001)
		qcr = 0.1;

	switch (Aircraft)
		{
		case C130:
			qcrc = qcr - (*pcorQCR)(akrd,1.0);
			break;

		case ELECTRA:
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

			qcrc = qcr - (*pcorQCR)(qcr,1.0);
			break;

		case NRL_P3:
/* This code for NRL P-3	*/

			bqcrc = 0.3 + adifr * (0.0402 + 0.0013 * adifr);

			if (bqcrc > 0.8)
				bqcrc = 0.8;

			qcrc = qcr - (*pcorQCR)(qcr,1.0) + bqcrc;
			break;

		case KINGAIR:
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

		case SABRELINER:
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
