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

static double	P_COEFF_WEAK[nCoeffs], P_COEFF_DIRECT[nCoeffs], P_COEFF_STRONG[nCoeffs],
		T_COEFF_WEAK[nCoeffs], T_COEFF_DIRECT[nCoeffs], T_COEFF_STRONG[nCoeffs],
		LI_COEFF_WEAK[2][nCoeffs], LI_COEFF_DIRECT[2][nCoeffs], LI_COEFF_STRONG[2][nCoeffs],
		ABS_COEFF_WEAK[nCoeffs], ABS_COEFF_DIRECT[nCoeffs], ABS_COEFF_STRONG[nCoeffs];

static double	ps_conv = 1.0;

static char flightDateString[32];


/* -------------------------------------------------------------------- */
static void readDefs(const char varName[], const char line[],
	double P_COEFF[], double T_COEFF[], double LI_COEFF[][nCoeffs], double ABS_COEFF[])
{
  float *tmp;

  sprintf(buffer, "VXL_T_%s", line);
  if ((tmp = GetDefaultsValue(buffer, varName)) == NULL)
  {
    sprintf(buffer, "%s value defaulting to %f in AMLIB function vcselInit.\n",
		buffer, T_COEFF[0]);
    LogMessage(buffer);
  }
  else
    for (int i = 0; i < nCoeffs; ++i)
      T_COEFF[i] = tmp[i];

  sprintf(buffer, "VXL_P_%s", line);
  if ((tmp = GetDefaultsValue(buffer, varName)) == NULL)
  {
    sprintf(buffer, "%s values defaulting to %f, %f, %f in AMLIB function vcselInit.\n",
		buffer, P_COEFF[0], P_COEFF[1], P_COEFF[2]);
    LogMessage(buffer);
  }
  else
    for (int i = 0; i < nCoeffs; ++i)
      P_COEFF[i] = tmp[i];

  sprintf(buffer, "VXL_LI_%s", line);
  if ((tmp = GetDefaultsValue(buffer, varName)) == NULL)
  {
    sprintf(buffer, "%s values defaulting to %f, %f, %f in AMLIB function vcselInit.\n",
		buffer, LI_COEFF[0][0], LI_COEFF[0][1], LI_COEFF[0][2]);
    LogMessage(buffer);
  }
  else
    for (int i = 0; i < nCoeffs; ++i)
      LI_COEFF[0][i] = tmp[i];

  sprintf(buffer, "VXL_LILO_%s", line);
  if ((tmp = GetDefaultsValue(buffer, varName)) == NULL)
  {
    sprintf(buffer, "%s values defaulting to %f, %f, %f in AMLIB function vcselInit.\n",
		buffer, LI_COEFF[1][0], LI_COEFF[1][1], LI_COEFF[1][2]);
    LogMessage(buffer);
  }
  else
    for (int i = 0; i < nCoeffs; ++i)
      LI_COEFF[1][i] = tmp[i];

  sprintf(buffer, "VXL_ABS_%s", line);
  if ((tmp = GetDefaultsValue(buffer, varName)) == NULL)
  {
    sprintf(buffer, "%s values defaulting to %f, %f, %f in AMLIB function vcselInit.\n",
		buffer, ABS_COEFF[0], ABS_COEFF[1], ABS_COEFF[2]);
    LogMessage(buffer);
  }
  else
    for (int i = 0; i < nCoeffs; ++i)
      ABS_COEFF[i] = tmp[i];


  // was briefly used, currently unused, check if cruft in 2014.
  extern int FlightDate[];
  sprintf(flightDateString, "%04d%02d", FlightDate[2], FlightDate[0]);

}

/* -------------------------------------------------------------------- */
void vcselInit(var_base *varp)
{
  readDefs(varp->name, "STRONG", P_COEFF_STRONG, T_COEFF_STRONG, LI_COEFF_STRONG, ABS_COEFF_STRONG);
  readDefs(varp->name, "DIRECT", P_COEFF_DIRECT, T_COEFF_DIRECT, LI_COEFF_DIRECT, ABS_COEFF_DIRECT);
  readDefs(varp->name, "WEAK", P_COEFF_WEAK, T_COEFF_WEAK, LI_COEFF_WEAK, ABS_COEFF_WEAK);

  // Conversion factor from torr to hPa.
  if (((DERTBL *)varp)->depends[1]->Units.compare("torr") == 0)
    ps_conv = StdPress / 760;
}


/* -------------------------------------------------------------------- */
void sconcv(DERTBL *varp)
{
  NR_TYPE *pCoeffs, *tCoeffs, *liCoeffs, *absCoeffs;
  NR_TYPE concv, p_coeff, t_coeff, li_coeff, abs_coeff, mr_raw, mr_corr;
  static int mode;
  int pregain;

  NR_TYPE concv_raw = GetSample(varp, 0);
  NR_TYPE PS_vxl = GetSample(varp, 1) * ps_conv;
  NR_TYPE AT_vxl = GetSample(varp, 2) + Kelvin;
  NR_TYPE light_intensity = GetSample(varp, 3);

  /* Only acquire mode in during low_rate entrance.  The mode at 25Hz has
   * too much ringing, and it should remain a square wave.
   */
  if (FeedBack == LOW_RATE_FEEDBACK)
    mode = (int)GetSample(varp, 4);

  if (varp->ndep > 5)
  {
    pregain = (int)GetSample(varp, 5);
    if (pregain < 0) pregain = 0;
    if (pregain > 1) pregain = 1;
  }
  else
    pregain = 0;

  switch (mode)
  {
    case STRONG:
      pCoeffs = P_COEFF_STRONG;
      tCoeffs = T_COEFF_STRONG;
      liCoeffs = LI_COEFF_STRONG[pregain];
      absCoeffs = ABS_COEFF_STRONG;
      break;
    case WEAK:
      pCoeffs = P_COEFF_WEAK;
      tCoeffs = T_COEFF_WEAK;
      liCoeffs = LI_COEFF_WEAK[pregain];
      absCoeffs = ABS_COEFF_WEAK;
      break;
    case DIRECT:
      pCoeffs = P_COEFF_DIRECT;
      tCoeffs = T_COEFF_DIRECT;
      liCoeffs = LI_COEFF_DIRECT[pregain];
      absCoeffs = ABS_COEFF_DIRECT;
      break;
    default:
      PutSample(varp, floatNAN);
      return;
  }


  if (strcmp(flightDateString, "201204") > 0)   // DC3 (May/June 2012) and later.
  {
    p_coeff = pCoeffs[1] * pow(PS_vxl, pCoeffs[2]) + pCoeffs[0];
    t_coeff = tCoeffs[1] * pow(AT_vxl, tCoeffs[2]) + tCoeffs[0];
    li_coeff = liCoeffs[1] * pow(light_intensity, liCoeffs[2]) + liCoeffs[0];
    mr_raw = concv_raw / ((PS_vxl * 100) / Boltzmann / AT_vxl) * 1.0e12;
    abs_coeff = absCoeffs[1] * pow(mr_raw, absCoeffs[2]) + absCoeffs[0];
    mr_corr = mr_raw * p_coeff * t_coeff * li_coeff * abs_coeff;
    concv = (mr_corr * 1.0e-12) * ((PS_vxl * 100) / Boltzmann / (AT_vxl));
  }
  else
  {
    p_coeff = (pCoeffs[0] + pCoeffs[1] * PS_vxl) / (pCoeffs[0] + pCoeffs[1] * StdPress);
    li_coeff = liCoeffs[0] + liCoeffs[1] * exp(-liCoeffs[2] * light_intensity);
    concv = concv_raw / (p_coeff * tCoeffs[0] * li_coeff);
  }

  PutSample(varp, concv);
}

/* -------------------------------------------------------------------- */
void svmr(DERTBL *varp)
{
  NR_TYPE vmr_vxl = floatNAN, h2o_conc_vxl, atx, ps;

  h2o_conc_vxl = GetSample(varp, 0);
  ps = GetSample(varp, 1) * 100.0;	// hPa -> Pascal
  atx = GetSample(varp, 2) + Kelvin;

  if (h2o_conc_vxl > 1.0)
    vmr_vxl = h2o_conc_vxl / (ps / Boltzmann / atx) * 1.0e12;

  PutSample(varp, vmr_vxl);

}	/* END SVMR */

/* END VCSEL.C */
