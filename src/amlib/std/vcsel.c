/*
-------------------------------------------------------------------------
OBJECT NAME:	vcsel.c

FULL NAME:	VCSEL

ENTRY POINTS:	svmr()		Volume Mixing Ratio (ppmv)
		sconcv()	Calibrated CONCV
		sEvsel()	Water Vapor Pressure

DESCRIPTION:	Derived calculations for the VCSEL.

COPYRIGHT:	University Corporation for Atmospheric Research, 2012
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

enum { STRONG=83, DIRECT=68, WEAK=87 };

static double	P_COEFF_WEAK[2], P_COEFF_DIRECT[2], P_COEFF_STRONG[2],
		T_COEFF_WEAK, T_COEFF_DIRECT, T_COEFF_STRONG,
		LI_COEFF_WEAK[3], LI_COEFF_DIRECT[3], LI_COEFF_STRONG[3];

static double	ps_conv = 1.0;


/* -------------------------------------------------------------------- */
static void readDefs(const char varName[], const char line[],
	double *P_COEFF, double *T_COEFF, double *LI_COEFF)
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
    T_COEFF[0] = tmp[0];

  sprintf(buffer, "VXL_P_%s", line);
  if ((tmp = GetDefaultsValue(buffer, varName)) == NULL)
  {
    sprintf(buffer, "%s value defaulting to %f, %f in AMLIB function vcselInit.\n",
		buffer, P_COEFF[0], P_COEFF[1]);
    LogMessage(buffer);
  }
  else
    for (int i = 0; i < 2; ++i)
      P_COEFF[i] = tmp[i];

  sprintf(buffer, "VXL_LI_%s", line);
  if ((tmp = GetDefaultsValue(buffer, varName)) == NULL)
  {
    sprintf(buffer, "%s values defaulting to %f, %f,%f in AMLIB function vcselInit.\n",
		buffer, LI_COEFF[0], LI_COEFF[1], LI_COEFF[2]);
    LogMessage(buffer);
  }
  else
    for (int i = 0; i < 3; ++i)
      LI_COEFF[i] = tmp[i];
}

/* -------------------------------------------------------------------- */
void vcselInit(var_base *varp)
{
  readDefs(varp->name, "STRONG", P_COEFF_STRONG, &T_COEFF_STRONG, LI_COEFF_STRONG);
  readDefs(varp->name, "DIRECT", P_COEFF_DIRECT, &T_COEFF_DIRECT, LI_COEFF_DIRECT);
  readDefs(varp->name, "WEAK", P_COEFF_WEAK, &T_COEFF_WEAK, LI_COEFF_WEAK);

  // Conversion factor from torr to hPa.
  if (((DERTBL *)varp)->depends[1]->Units.compare("torr") == 0)
    ps_conv = StdPress / 760;
}


/* -------------------------------------------------------------------- */
void sconcv(DERTBL *varp)
{
  NR_TYPE p_coeff, *pCoeffs, t_coeff, coeff_light, *liCoeffs;

  NR_TYPE concv_raw = GetSample(varp, 0);
  NR_TYPE PS_vxl = GetSample(varp, 1) * ps_conv;
  NR_TYPE AT_vxl = GetSample(varp, 2);
  NR_TYPE light_intensity = GetSample(varp, 3);
  int mode = (int)GetSample(varp, 4);

  switch (mode)
  {
    case STRONG:
      pCoeffs = P_COEFF_STRONG;
      t_coeff = T_COEFF_STRONG;
      liCoeffs = LI_COEFF_STRONG;
      break;
    case WEAK:
      pCoeffs = P_COEFF_WEAK;
      t_coeff = T_COEFF_WEAK;
      liCoeffs = LI_COEFF_WEAK;
      break;
    case DIRECT:
      pCoeffs = P_COEFF_DIRECT;
      t_coeff = T_COEFF_DIRECT;
      liCoeffs = LI_COEFF_DIRECT;
      break;
    default:
      PutSample(varp, floatNAN);
      return;
  }

  p_coeff = (pCoeffs[0] + pCoeffs[1] * PS_vxl) / (pCoeffs[0] + pCoeffs[1] * StdPress);
  coeff_light =	liCoeffs[0] + liCoeffs[1] * exp(-liCoeffs[2] * light_intensity);

  NR_TYPE concv = concv_raw / (p_coeff * t_coeff * coeff_light);
  PutSample(varp, concv);
}

/* -------------------------------------------------------------------- */
void sEvcsel(DERTBL *varp)
{
  NR_TYPE E_vcsel = floatNAN, h2o_conc_vxl, atx;

  h2o_conc_vxl = GetSample(varp, 0);
  atx = GetSample(varp, 1);

  if (h2o_conc_vxl > 1.0)
    E_vcsel = Boltzmann * (atx+Kelvin) * (h2o_conc_vxl * 1.0e6) * 1.0e-2;

  PutSample(varp, E_vcsel);
}

/* -------------------------------------------------------------------- */
void svmr(DERTBL *varp)
{
  NR_TYPE vmr_vxl = floatNAN, h2o_conc_vxl, atx, ps;

  h2o_conc_vxl = GetSample(varp, 0);
  ps = GetSample(varp, 1) * 133.322;	// Torr -> Pascal
  atx = GetSample(varp, 2) + Kelvin;

  if (h2o_conc_vxl > 1.0)
    vmr_vxl = h2o_conc_vxl / (ps / Boltzmann / atx) * 1.0e12;

  PutSample(varp, vmr_vxl);

}	/* END SVMR */

/* END VCSEL.C */
