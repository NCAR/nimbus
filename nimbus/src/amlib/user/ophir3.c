/*
-------------------------------------------------------------------------
OBJECT NAME:	ophir3.c

FULL NAME:	Ophir III

ENTRY POINTS:	soat(), 

STATIC FNS:	planck_rad()
		planck_t()
		therm()

DESCRIPTION:  

INPUT:    

OUTPUT:    

REFERENCES:	none

REFERENCED BY:	ComputeDerived()

NOTES:		See bottom of file for variable name descriptions.

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include <math.h>
#include "nimbus.h"
#include "amlib.h"

#define h	6.62606876e-34	/* Planck's const, J-s    */
#define k	1.3806503e-23	/* Boltzman's const, J/K  */
#define lam	4.29e-6		/* Wavelength, um         */
#define TZERO	273.16		/* 0C (K)                 */
#define c	2.99792458e8	/* Speed of light, m/s    */
#define PI	M_PI

/*  Values from /home/local/proj/defaults/Defaults on 30 April 1998  RLR  */
static NR_TYPE  MAXCNT      = 62500,
                THERM_BIAS  = 5.0,
                THERM_RZ    = 36.4,
                THERM_COEFF = -2700.0,
                DFLT_GAIN   = -4.4913167e-5,
                DFLT_OFFSET = 31515.0,
                A1          = -3.910e-5,
                A2          = -4.285e-5,
                A3          = 0.972,
                A4          = 1.0,
                A5          = 0.0;

static double  planck_rad(double), planck_t(double), therm(double);

/* Outputs  */
static double  gain_factor, bbcrad, av_detofset, detem, detsig;

/* -------------------------------------------------------------------- */
void ophir3Init(var_base *varp)
{
  NR_TYPE  *tmp;

  if ((tmp = GetDefaultsValue("A1", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %e in AMLIB function ophir3Init.\n", A1);
    LogMessage(buffer);
  }
  else
    A1 = tmp[0];

  if ((tmp = GetDefaultsValue("A2", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %e in AMLIB function ophir3Init.\n", A2);
    LogMessage(buffer);
  }
  else
    A2 = tmp[0];

  if ((tmp = GetDefaultsValue("A3", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %e in AMLIB function ophir3Init.\n", A3);
    LogMessage(buffer);
  }
  else
    A3 = tmp[0];

  if ((tmp = GetDefaultsValue("A4", varp->name)) == NULL)
  {
    sprintf(buffer, "Value A4 set to %e in AMLIB function ophir3Init.\n", A4);
    LogMessage(buffer);
  }
  else
    A4 = tmp[0];

  if ((tmp = GetDefaultsValue("A5", varp->name)) == NULL)
  {
    sprintf(buffer, "Value A5 set to %e in AMLIB function ophir3Init.\n", A5);
    LogMessage(buffer);
  }
  else
    A5 = tmp[0];

  if ((tmp = GetDefaultsValue("MAXCNT", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function ophir3Init.\n", MAXCNT);
    LogMessage(buffer);
  }
  else
    MAXCNT = tmp[0];

  if ((tmp = GetDefaultsValue("THERM_BIAS", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function ophir3Init.\n", THERM_BIAS);
    LogMessage(buffer);
  }
  else
    THERM_BIAS = tmp[0];

  if ((tmp = GetDefaultsValue("THERM_RZ", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function ophir3Init.\n", THERM_RZ);
    LogMessage(buffer);
  }
  else
    THERM_RZ = tmp[0];

  if ((tmp = GetDefaultsValue("THERM_COEFF", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function ophir3Init.\n", THERM_COEFF);
    LogMessage(buffer);
  }
  else
    THERM_COEFF = tmp[0];

  if ((tmp = GetDefaultsValue("DFLT_GAIN", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %e in AMLIB function ophir3Init.\n", DFLT_GAIN);
    LogMessage(buffer);
  }
  else
    DFLT_GAIN = tmp[0];

  if ((tmp = GetDefaultsValue("DFLT_OFFSET", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function ophir3Init.\n", DFLT_OFFSET);
    LogMessage(buffer);
  }
  else
    DFLT_OFFSET = tmp[0];

}  /* END OPHIR3INIT */

/* -------------------------------------------------------------------- */
void soat(DERTBL *varp)
{
  NR_TYPE	oat, detdc, tdet, tbbc, detsg, sntem, sntrad, bntem, bnrad;
  int		bbcod;
  double	gain;

  static int	nbbc, nbbh, nofset;
  static double	sum_bbctem, sum_bbhtem, sum_detofset, sum_bbcsig,
		sum_bbhsig, sum_detdc,
		bbctem_k, bbhtem_k, bbhrad, detdc_v,
		av_detdc, av_bbctem, av_bbhtem, av_bbcsig, av_bbhsig;


  bbcod	= (int)GetSample(varp, 0);
  detdc	= GetSample(varp, 1);
  tdet	= GetSample(varp, 2);
  tbbc	= GetSample(varp, 3);
  detsg	= GetSample(varp, 4);
  sntem	= GetSample(varp, 5);

  // Older projects do not have this extra dependency.
  if (varp->ndep == 7)
    bntem = GetSample(varp, 6);
  else
    bntem = 0.0;

  oat = 0.0;

  switch (bbcod)
    {
    case 'z':    /* NULL record    */
      sum_bbctem = sum_bbhtem = sum_bbcsig = sum_bbhsig = sum_detdc = 0.0;

      sum_detofset += (double)detsg;
      av_detofset = sum_detofset / (double)++nofset;
      break;

    case 'c':    /* BB cooling calibration  */
      nofset = 0;
      sum_detofset = 0.0;
      sum_bbctem += (double)tbbc;
      sum_bbcsig += (double)detsg;
      sum_detdc += (double)detdc;

      nbbc++;
      av_bbctem = sum_bbctem / (double)nbbc;
      av_bbcsig = sum_bbcsig / (double)nbbc;

      bbctem_k = av_bbctem;
      bbcrad = planck_rad(bbctem_k);
      break;

    case 'h':    /* BB heating calibration  */
      nofset = 0;
      sum_detofset = 0.0;
      sum_bbhtem += (double)tbbc;
      sum_bbhsig += (double)detsg;
      sum_detdc += (double)detdc;

      nbbh++;
      av_bbhtem = sum_bbhtem / (double)nbbh;

      bbhtem_k = av_bbhtem;
      bbhrad = planck_rad(bbhtem_k);

      av_detdc = sum_detdc / (double)(nbbc + nbbh);
      av_bbhsig = sum_bbhsig / (double)nbbh;

      gain=-fabs((bbhrad-bbcrad)/((av_bbhsig-av_bbcsig)*10.0/MAXCNT));
      detdc_v = (-5.0 + av_detdc * 10.0 / MAXCNT);
      gain_factor = gain * detdc_v * detdc_v;
      break;

    case 'b':    /* Normal record with no BB cooling  */
      /*  Calculates parameters in eng units  */
      detem=therm((double)tdet);
      detsig = ((double)detsg - DFLT_OFFSET) * 10.0 / MAXCNT;
      bbcrad = planck_rad((double)tbbc);
      sntrad = planck_rad((double)sntem);
      bnrad = planck_rad((double)bntem);

//printf("%f %f %f %f\n", detsig, bbcrad, sntrad, oat);

      /* calc_airtem = OphirAirTem = oat */
      oat = detsig * A1 + A2 + bbcrad * A4 + sntrad * A3 + bnrad * A5;

      if (oat > 0.0) 
        oat = planck_t((double)oat);
      else 
        oat = TZERO;

      sum_bbctem = sum_bbhtem = sum_bbcsig = sum_bbhsig = 0.0;
      break;
    }

  PutSample(varp, oat - 273.16);

}  /* END SOAT */

/* -------------------------------------------------------------------- */
/*  planck_rad - returns hemishperical black body radiance  */
/*    in W/cm^2/um  */
static double planck_rad(double tk)
{
  double  rad;

  rad = (2.0*h*c*c*PI)/(pow(lam,5.0)*(exp(h*c/lam/k/tk)-1.0));

  return(rad * 1.0e-10);
}

/* -------------------------------------------------------------------- */
/*  planck_t - Returns temp in K from rad in W/cm^2/um  */
static double planck_t(double rad)
{
  double  t;

  t = h*c/(lam*k*log(2.0*h*c*c*PI/(rad*1.0e10)/pow(lam,5.0)+1.0));

  return(t);
}

/* -------------------------------------------------------------------- */
/*  therm - Temp of detector thermistor deg K  */
static double therm(double counts)
{
  double  tc, rt, v;

  v = (-5.0 + counts / MAXCNT * 10.0) / (1.0 + 50.0 / 18.2);

  if (v > -THERM_BIAS / 21.0)
    rt = 200.0 / (1.0 / (1.0 / 21.0 + v / THERM_BIAS) - 1.0);
  else
    rt = 1.0;

  tc = 1.0 / (TZERO-50.0) - log(rt / THERM_RZ) / THERM_COEFF;
  tc = 1.0 / tc;

  return(tc);
}

/* -------------------------------------------------------------------- */
void sotdetc(DERTBL *varp)
{
  PutSample(varp, (NR_TYPE)detem);
}
void sodsigc(DERTBL *varp)
{
  PutSample(varp, (NR_TYPE)detsig);
}
void sobbcr(DERTBL *varp)
{
  PutSample(varp, (NR_TYPE)bbcrad);
}
void sogainf(DERTBL *varp)
{
  PutSample(varp, (NR_TYPE)gain_factor);
}
void sodetof(DERTBL *varp)
{
  PutSample(varp, (NR_TYPE)av_detofset);
}

/* -------------------------------------------------------------------- */
/*
Raw variable names      Names in this software
 
ogain                   szg             (gain)
obbcod                  bbcod           (black body code)
odetdc                  detdc
otsnt                   sntem           (snout temperature)
otbnch                  bntem           (optical bench temperature)
otdet                   tdet            (detector temperature)
otbbc                   tbbc            (BBC temperature)
otbbf                   bbftem          (BBF temperature)
osmotr                  motor           (motor speed)
otbox                   boxtem          (box temperature)
odetsg                  detsg           (detector raw signal)
 
computed output values
 
odsigc                  detsig          (corrected detector signal)
otdetc                  detem           (corrected detector temperature)
obbcr                   bbcrad
ogainf                  gain_factor
odetof                  av_detofset
oat                     oat             (computed air temperature)
*/

/* END OPHIR3.C */
