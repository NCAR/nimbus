/*
-------------------------------------------------------------------------
OBJECT NAME:	cmigit3.c

FULL NAME:	C-MIGITS III

ENTRY POINTS:	xletorq()

STATIC FNS:	none

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 2003-2005
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static const double	cm31 = 4.656613e-10;
static const double	cm21 = 4.768372e-07;
static const double	cm16 = 1.525879e-05;

static NR_TYPE thdgCorr, pitchCorr;

/* -------------------------------------------------------------------- */
void cmigitsInit(var_base *varp)
{
  NR_TYPE  *tmp;

  if (cfg.isADS3()) return;

  if ((tmp = GetDefaultsValue("CM_HEADING_BIAS_1", varp->name)) == NULL)
    {
    sprintf(buffer,"Values set to %f, in AMLIB function cmigitInit.\n", thdgCorr);
    LogMessage(buffer);
    }
  else
    thdgCorr = tmp[0];

  if ((tmp = GetDefaultsValue("CM_PITCH_BIAS_1", varp->name)) == NULL)
    {
    sprintf(buffer,"Values set to %f, in AMLIB function cmigitInit.\n", pitchCorr);    LogMessage(buffer);
    }
  else
    pitchCorr = tmp[0];
}

/* -------------------------------------------------------------------- */
void xlclat(RAWTBL *varp, void *input, NR_TYPE *output)
{
  Cmigits3_blk	*cm = (Cmigits3_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    output[i] = 180.0 * cm31 * cm->Clatitude[i];
/*
  long		*lp = (long *)input;
    printf("lat = %f %f\n", (float)lp[i] * pow(2, -31)*180.0, pow(2,-31));
    printf("lon = %f %f\n", (float)lp[10+i] * pow(2, -31)*180.0, pow(2,-31));
    printf("alt = %f %f\n", (float)lp[20+i] * pow(2, -16), pow(2,-16));
*/
}	/* END XLCLAT */

/* -------------------------------------------------------------------- */
void xlclon(RAWTBL *varp, void *input, NR_TYPE *output)
{
  Cmigits3_blk	*cm = (Cmigits3_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    output[i] = 180.0 * cm31 * cm->Clongitude[i];

}	/* END XLCLON */

/* -------------------------------------------------------------------- */
void xlcalt(RAWTBL *varp, void *input, NR_TYPE *output)
{
  Cmigits3_blk	*cm = (Cmigits3_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    output[i] = cm16 * cm->Caltitude[i];

}	/* END XLCALT */

/* -------------------------------------------------------------------- */
void xlcvew(RAWTBL *varp, void *input, NR_TYPE *output)
{
  Cmigits3_blk	*cm = (Cmigits3_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    output[i] = cm21 * cm->Cvelocity_east[i];

}	/* END XLCVEW */

/* -------------------------------------------------------------------- */
void xlcvns(RAWTBL *varp, void *input, NR_TYPE *output)
{
  Cmigits3_blk	*cm = (Cmigits3_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    output[i] = cm21 * cm->Cvelocity_north[i];

}	/* END XLCVNS */

/* -------------------------------------------------------------------- */
void xlcvspd(RAWTBL *varp, void *input, NR_TYPE *output)
{
  Cmigits3_blk	*cm = (Cmigits3_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    output[i] = cm21 * cm->Cvelocity_up[i];

}	/* END XLCVSPD */

/* -------------------------------------------------------------------- */
void xlcpitch(RAWTBL *varp, void *input, NR_TYPE *output)
{
  Cmigits3_blk	*cm = (Cmigits3_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    output[i] = 180.0 * cm31 * cm->Cpitch[i] + pitchCorr;

}	/* END XLCPITCH */

/* -------------------------------------------------------------------- */
void xlcroll(RAWTBL *varp, void *input, NR_TYPE *output)
{
  Cmigits3_blk	*cm = (Cmigits3_blk *)input;
  NR_TYPE	roll;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
    roll = 180.0 * cm31 * cm->Croll[i];

    while (roll < -90) roll += 180;
    while (roll > 90) roll -= 180;

    output[i] = roll;
    }

}	/* END XLCROLL */

/* -------------------------------------------------------------------- */
void xlcthdg(RAWTBL *varp, void *input, NR_TYPE *output)
{
  Cmigits3_blk	*cm = (Cmigits3_blk *)input;
  NR_TYPE	hdg;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
    hdg = 180.0 * cm31 * cm->Ctrue_heading[i] + thdgCorr;

    if (hdg < 0) hdg += 360.0;

    output[i] = hdg;
    }

}	/* END XLCTHDG */

/* END CMIGIT3.C */
