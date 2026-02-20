/*
-------------------------------------------------------------------------
OBJECT NAME:	vcsel.c

FULL NAME:	VCSEL

ENTRY POINTS:	svmr()		Volume Mixing Ratio (ppmv)
		sconcv()	Calibrated CONCV
		sEvsel()	Water Vapor Pressure

DESCRIPTION:	Derived calculations for the VCSEL.
		In DC3 the equation for weak mode changed significantly.
		Strong and Direct modes will also change in the future.

COPYRIGHT:	University Corporation for Atmospheric Research, 2012
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

enum { STRONG=83, DIRECT=68, WEAK=87 };

static const int nCoeffs = 4;
/*
static double	P_COEFF_WEAK[nCoeffs], P_COEFF_DIRECT[nCoeffs], P_COEFF_STRONG[nCoeffs],
		T_COEFF_WEAK[nCoeffs], T_COEFF_DIRECT[nCoeffs], T_COEFF_STRONG[nCoeffs],
		LI_COEFF_WEAK[2][nCoeffs], LI_COEFF_DIRECT[2][nCoeffs], LI_COEFF_STRONG[2][nCoeffs],
		ABS_COEFF_WEAK[nCoeffs], ABS_COEFF_DIRECT[nCoeffs], ABS_COEFF_STRONG[nCoeffs];
*/
static std::vector<float>
		P_COEFF_WEAK, P_COEFF_DIRECT, P_COEFF_STRONG,
		T_COEFF_WEAK, T_COEFF_DIRECT, T_COEFF_STRONG,
		LI_COEFF_WEAK[3], LI_COEFF_DIRECT[3], LI_COEFF_STRONG[3],
		ABS_COEFF_WEAK[3], ABS_COEFF_DIRECT[3], ABS_COEFF_STRONG[3];


// linear coeffs if static pressure needs scaling/adjusting.
static std::vector<float>	ps_linear;

static char flightDateString[32];


/* -------------------------------------------------------------------- */
static void readDefs(const char varName[], const char line[],
	std::vector<float> &P_COEFF, std::vector<float> &T_COEFF, std::vector<float> LI_COEFF[], std::vector<float> ABS_COEFF[])
{
  float *tmp;
  char name[256];

  sprintf(name, "VXL_T_%s", line);
  if ((tmp = GetDefaultsValue(name, varName)) == NULL)
  {
    sprintf(buffer, "%s value defaulting to 0 in AMLIB function vcselInit.\n",
		name);
    LogMessage(buffer);
  }
  else
    for (int i = 0; i < nCoeffs; ++i)
      T_COEFF.push_back(tmp[i]);

  sprintf(name, "VXL_P_%s", line);
  if ((tmp = GetDefaultsValue(name, varName)) == NULL)
  {
    sprintf(buffer, "%s values defaulting to 0, 0, 0 in AMLIB function vcselInit.\n",
		name);
    LogMessage(buffer);
  }
  else
    for (int i = 0; i < nCoeffs; ++i)
      P_COEFF.push_back(tmp[i]);

  sprintf(name, "VXL_LI_%s", line);
  if ((tmp = GetDefaultsValue(name, varName)) == NULL)
  {
    sprintf(buffer, "%s values defaulting to 0, 0, 0 in AMLIB function vcselInit.\n",
		name);
    LogMessage(buffer);
  }
  else
    for (int i = 0; i < nCoeffs; ++i)
      LI_COEFF[0].push_back(tmp[i]);

  sprintf(name, "VXL_LILO_%s", line);
  if ((tmp = GetDefaultsValue(name, varName)) == NULL)
  {
    sprintf(buffer, "%s values defaulting to 0, 0, 0 in AMLIB function vcselInit.\n",
		name);
    LogMessage(buffer);
  }
  else
    for (int i = 0; i < nCoeffs; ++i)
      LI_COEFF[1].push_back(tmp[i]);

  sprintf(name, "VXL_LILO2_%s", line);
  if ((tmp = GetDefaultsValue(name, varName)) == NULL)
  {
    // For backwards compatability, there was no LI[2], it defaulted to LI[1].
    LI_COEFF[2] = LI_COEFF[1];
  }
  else
    for (int i = 0; i < nCoeffs; ++i)
      LI_COEFF[2].push_back(tmp[i]);


  sprintf(name, "VXL_ABS_%s", line);
  if ((tmp = GetDefaultsValue(name, varName)) == NULL)
  {
    sprintf(buffer, "%s values defaulting to 0, 0, 0 in AMLIB function vcselInit.\n",
		name);
    LogMessage(buffer);
  }
  else
    for (int i = 0; i < nCoeffs; ++i)
    {
      ABS_COEFF[0].push_back(tmp[i]);
      ABS_COEFF[1].push_back(tmp[i]);
    }

  sprintf(name, "VXL_ABS2_%s", line);
  if ((tmp = GetDefaultsValue(name, varName)) == NULL)
  {
    // For backwards compatability, there was no ABS[2], it defaulted to ABS[0].
    ABS_COEFF[2] = ABS_COEFF[0];
  }
  else
    for (int i = 0; i < nCoeffs; ++i)
      ABS_COEFF[2].push_back(tmp[i]);


  // was briefly used, currently unused, check if cruft in 2014.
  extern int FlightDate[];
  sprintf(flightDateString, "%04d%02d", FlightDate[2], FlightDate[0]);

}

/* -------------------------------------------------------------------- */
void vcselInit(var_base *varp)
{
  float *tmp;

  readDefs(varp->name, "STRONG", P_COEFF_STRONG, T_COEFF_STRONG, LI_COEFF_STRONG, ABS_COEFF_STRONG);
  readDefs(varp->name, "DIRECT", P_COEFF_DIRECT, T_COEFF_DIRECT, LI_COEFF_DIRECT, ABS_COEFF_DIRECT);
  readDefs(varp->name, "WEAK", P_COEFF_WEAK, T_COEFF_WEAK, LI_COEFF_WEAK, ABS_COEFF_WEAK);

  varp->addToMetadata("T_Strong", T_COEFF_STRONG);
  varp->addToMetadata("P_Strong", P_COEFF_STRONG);
  varp->addToMetadata("LI[0]_Strong", LI_COEFF_STRONG[0]);
  varp->addToMetadata("LILO[1]_Strong", LI_COEFF_STRONG[1]);
  varp->addToMetadata("LILO[2]_Strong", LI_COEFF_STRONG[2]);
  varp->addToMetadata("ABS[0]_Strong", ABS_COEFF_STRONG[0]);
  varp->addToMetadata("ABS[1]_Strong", ABS_COEFF_STRONG[1]);
  varp->addToMetadata("ABS[2]_Strong", ABS_COEFF_STRONG[2]);

  varp->addToMetadata("T_Direct", T_COEFF_DIRECT);
  varp->addToMetadata("P_Direct", P_COEFF_DIRECT);
  varp->addToMetadata("LI[0]_Direct", LI_COEFF_DIRECT[0]);
  varp->addToMetadata("LILO[1]_Direct", LI_COEFF_DIRECT[1]);
  varp->addToMetadata("LILO[2]_Direct", LI_COEFF_DIRECT[2]);
  varp->addToMetadata("ABS[0]_Direct", ABS_COEFF_DIRECT[0]);
  varp->addToMetadata("ABS[1]_Direct", ABS_COEFF_DIRECT[1]);
  varp->addToMetadata("ABS[2]_Direct", ABS_COEFF_DIRECT[2]);

  varp->addToMetadata("T_Weak", T_COEFF_WEAK);
  varp->addToMetadata("P_Weak", P_COEFF_WEAK);
  varp->addToMetadata("LI_Weak[0]", LI_COEFF_WEAK[0]);
  varp->addToMetadata("LILO_Weak[1]", LI_COEFF_WEAK[1]);
  varp->addToMetadata("LILO_Weak[2]", LI_COEFF_WEAK[2]);
  varp->addToMetadata("ABS[0]_Weak", ABS_COEFF_WEAK[0]);
  varp->addToMetadata("ABS[1]_Weak", ABS_COEFF_WEAK[1]);
  varp->addToMetadata("ABS[2]_Weak", ABS_COEFF_WEAK[2]);

  // Conversion factor from torr to hPa.
  if (((DERTBL *)varp)->depends[1]->Units().compare("torr") == 0)
  {
    ps_linear.push_back(0.0);
    ps_linear.push_back(StdPress / 760);
  }

 if ((tmp = GetDefaultsValue("PS_SCALE", varp->name)) != NULL)
  {
    ps_linear.clear();
    ps_linear.push_back(tmp[0]);
    ps_linear.push_back(tmp[1]);
    sprintf(buffer,
        "vcsel: PS_SCALE set to %f %f from Defaults file.\n", ps_linear[0], ps_linear[1]);
    LogMessage(buffer);
  }
  if (ps_linear.size() > 0)
    varp->addToMetadata("PS_Scale", ps_linear);

}


/* -------------------------------------------------------------------- */
void sconcv(DERTBL *varp)
{
  std::vector<float> pCoeffs, tCoeffs, liCoeffs, absCoeffs;
  NR_TYPE concv, p_coeff, t_coeff, li_coeff, abs_coeff, mr_raw, mr_corr;
  static int mode;
  int pregain;

  NR_TYPE concv_raw = GetSample(varp, 0);
  NR_TYPE PS = GetSample(varp, 1);
  NR_TYPE AT = GetSample(varp, 2) + Kelvin;
  NR_TYPE light_intensity = GetSample(varp, 3);

  if (ps_linear.size() > 0)
    PS = ps_linear[0] + PS * ps_linear[1];


  /* Only acquire mode in during low_rate entrance.  The mode at 25Hz has
   * too much ringing, and it should remain a square wave.
   */
  if (FeedBack == LOW_RATE_FEEDBACK)
    mode = (int)GetSample(varp, 4);

  if (varp->nDependencies > 5)
  {
    pregain = (int)GetSample(varp, 5);
    if (pregain < 0) pregain = 0;
    if (pregain > 2) pregain = 2;
  }
  else
    pregain = 0;

  switch (mode)
  {
    case STRONG:
      pCoeffs = P_COEFF_STRONG;
      tCoeffs = T_COEFF_STRONG;
      liCoeffs = LI_COEFF_STRONG[pregain];
      absCoeffs = ABS_COEFF_STRONG[pregain];
      break;
    case WEAK:
      pCoeffs = P_COEFF_WEAK;
      tCoeffs = T_COEFF_WEAK;
      liCoeffs = LI_COEFF_WEAK[pregain];
      absCoeffs = ABS_COEFF_WEAK[pregain];
//fprintf(stderr, "vcsel.c: mode=%d, pregain=%d, absCoeff=%f %f %f\n", mode, pregain, absCoeffs[0], absCoeffs[1], absCoeffs[2]);
      break;
    case DIRECT:
      pCoeffs = P_COEFF_DIRECT;
      tCoeffs = T_COEFF_DIRECT;
      liCoeffs = LI_COEFF_DIRECT[pregain];
      absCoeffs = ABS_COEFF_DIRECT[pregain];
      break;
    default:
      PutSample(varp, floatNAN);
      return;
  }


  // Original new processing and co-efficients only went back to DC3.
  // Now extending back to PREDICT.
//  if (strcmp(flightDateString, "201204") > 0)   // DC3 (May/June 2012) and later.
  if (strcmp(flightDateString, "201002") > 0)   // HIPPO-3 (March 2010) and later.
  {
    p_coeff = pCoeffs[1] * pow(PS, pCoeffs[2]) + pCoeffs[0];
    t_coeff = tCoeffs[1] * pow(AT, tCoeffs[2]) + tCoeffs[0];
    li_coeff = liCoeffs[1] * pow(light_intensity, liCoeffs[2]) + liCoeffs[0];
    mr_raw = concv_raw / ((PS * 100) / Boltzmann / AT) * 1.0e12;
    abs_coeff = absCoeffs[1] * pow(mr_raw, absCoeffs[2]) + absCoeffs[0];
/* Per Teresa, starting with CAESAR - use absCoeffs[3] as an offset.
 * older projects adsCoeffs[3] set to zero in the Deefaults, so we should be
 * set for backwards compatability.
    mr_corr = mr_raw * p_coeff * t_coeff * li_coeff * abs_coeff;
 */
    mr_corr = mr_raw * p_coeff * t_coeff * li_coeff * abs_coeff + absCoeffs[3];
    concv = (mr_corr * 1.0e-12) * ((PS * 100) / Boltzmann / (AT));
  }
  else
  {
    fprintf(stderr, "vcsel.c: Old Pre-HIPPO-3 code !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    p_coeff = (pCoeffs[0] + pCoeffs[1] * PS) / (pCoeffs[0] + pCoeffs[1] * StdPress);
    li_coeff = liCoeffs[0] + liCoeffs[1] * exp(-liCoeffs[2] * light_intensity);
    concv = concv_raw / (p_coeff * tCoeffs[0] * li_coeff);
  }

  PutSample(varp, concv);
}

/* -------------------------------------------------------------------- */
void svmr(DERTBL *varp)
{
  NR_TYPE dry_vmr = floatNAN, ewv, ps;

  ewv = GetSample(varp, 0);
  ps  = GetSample(varp, 1);

  if (ps_linear.size() > 0)
    ps = ps_linear[0] + ps * ps_linear[1];

  if ((ps - ewv) != 0.0)
    dry_vmr = ewv / (ps - ewv) * 1.0e6;

  PutSample(varp, dry_vmr);

}	/* END SVMR */

/* END VCSEL.C */
