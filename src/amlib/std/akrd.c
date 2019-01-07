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

static double filter(double, double *);
static double zf[nFeedBackTypes][4][6];

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

  memset((void *)zf, 0, sizeof(zf));

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
        //akrd_coeff.push_back(4.852);	// Coopers latest memo 11/26/2014
        //akrd_coeff.push_back(13.23);
	akrd_coeff.push_back(4.7532); // Cooper 15 Sept 2016 memo
	akrd_coeff.push_back(9.7908);
	akrd_coeff.push_back(6.0781);
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
  if (cfg.ProjectName().compare("CSET") == 0 ||
      cfg.ProjectName().compare("ORCAS") == 0 ||
      cfg.ProjectName().compare("SOCRATES") == 0)
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

  adifr	= GetSample(varp, 0);	// ADIFR
  qc	= GetSample(varp, 1);	// QCF
  psf   = GetSample(varp, 2);	// PSF

  mach = sqrt( 5.0 * (pow((qc+psf)/psf, Rd_DIV_Cpd) - 1.0) ); // Mach #

  /* Blow-up protection:  output zero while on ground (QCX < 5.5 mbar)
   * installed by Ron Ruth  18 October 2001
   */
  if (qc > 5.5)
  {
    NR_TYPE ratio = adifr / qc;
    switch (cfg.Aircraft())
    {
      case Config::C130:
        //akrd = akrd_coeff[0] + akrd_coeff[1] * ratio;
        akrd = akrd_coeff[0] + ratio * (akrd_coeff[1] + akrd_coeff[2] * mach); // 15 Sept 2016 memo
        break;

      case Config::ELECTRA:
      case Config::NRL_P3:
      case Config::KINGAIR:
        akrd = (ratio + akrd_coeff[0]) / akrd_coeff[1];
        break;

      case Config::HIAPER:
        psf = GetSample(varp, 2);	// PSF
        mach = sqrt( 5.0 * (pow((qc+psf)/psf, Rd_DIV_Cpd) - 1.0) ); // Mach #
        if (varp->nDependencies == 4)
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
  if (std::isnan(adifr) || std::isnan(qc))
    akrd = floatNAN;

  PutSample(varp, akrd);

}    /* END SAKRD */

/* -------------------------------------------------------------------- */
void saky(DERTBL *varp)
{
  NR_TYPE qc, adif, AKY = 0.0;

  adif	= GetSample(varp, 0);
  qc	= GetSample(varp, 1);

  static const double c1 = 21.481, d0 = 4.51107, d1 = 19.84095, d2 = -0.0018806;

  if (std::isnan(adif) || std::isnan(qc))
  {
    AKY = floatNAN;
    qc = 0.1;
  }

  if (qc > 5.5)	// Avoid blow-ups on the ground.
  {
    double ratio = adif / qc;
    double AQS = filter(ratio, zf[FeedBack][0]);
    double QCFS = filter(qc, zf[FeedBack][1]);
    double AQF = ratio - AQS;

    AKY = d0 + d1 * AQS + d2 * QCFS + c1 * AQF;
  }

  PutSample(varp, AKY);
}

/* -------------------------------------------------------------------- */
static double filter(double x, double zf[])
{
  double xf;
  static double bfb[4];
  static double bfa[4];
  static bool   firstTime[nFeedBackTypes] = { true, true };

  if (firstTime[FeedBack])
    {
    /* double   b1, b2, c, e, f; */
    sprintf(buffer,"Processing rate = %d\n",cfg.ProcessingRate());
    LogMessage(buffer);

    if (FeedBack == HIGH_RATE_FEEDBACK)
    {
      /* TAU *= (float)cfg.ProcessingRate(); */
      /* These constants were calculated with tau = 25sps*300 */
      if (cfg.ProcessingRate() == 25) {
        bfb[0] = 9.1831978905817848e-12;
        bfb[1] = 2.7549593671745355e-11;
        bfb[2] = 2.7549593671745355e-11;
        bfb[3] = 9.1831978905817848e-12;
        bfa[0] = 1.0;
        bfa[1] = -2.99916224196516756;
        bfa[2] =  2.99832483481284928;
        bfa[3] = -0.99916259277421626;
      } else {
          sprintf(buffer,"Butterworth filter not implemented for processing rate %d. Edit gpsc.c and follow instructions for generating coefficients for this rate.\n",cfg.ProcessingRate());
          LogMessage(buffer);
      }
    } else {
      bfb[0] = 1.4205608170642102e-07;
      bfb[1] = 4.2616824511926303e-07;
      bfb[2] = 4.2616824511926303e-07;
      bfb[3] = 1.4205608170642102e-07;
      bfa[0] = 1.0;
      bfa[1] = -2.97905614466460555;
      bfa[2] =  2.95833103772366979;
      bfa[3] = -0.97927375661041016;
    }

    firstTime[FeedBack] = false;
    }

  xf =  (bfb[0] * x + bfb[1] * zf[0] + bfb[2]*zf[1] + bfb[3]*zf[2]
        - (bfa[1]*zf[3] + bfa[2]*zf[4] + bfa[3]*zf[5]));

  /* Store terms for the next call. */
  zf[2] = zf[1];
  zf[1] = zf[0];
  zf[0] = x;
  zf[5] = zf[4];
  zf[4] = zf[3];
  zf[3] = xf;

  return(xf);

}       /* END FILTER */

/* END AKRD.C */
