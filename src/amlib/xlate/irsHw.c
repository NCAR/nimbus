/*
-------------------------------------------------------------------------
OBJECT NAME:	irsHw.c

FULL NAME:	Convert IRS block data

ENTRY POINTS:	xlilat(), xlilon(), xlipitch(), xliroll(), xlithdg(),
		xligspd(), xlvacc(), xlivspd(), xlivns(), xlivew(),
		xllag50(), xllag25(), xllag10(), xllag5(), xlialt()

DESCRIPTION:  

INPUT:    

OUTPUT:    

REFERENCES:	none

REFERENCED BY:	rec_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-8
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

#include <map>

static const double  arinc20bits = (1.0 / (1 << 20)) * 180.0;
static const double  arinc15bits = (1.0 / (1 << 15));

/* IRS box alignment corrections  */
/*  Values from /home/local/proj/defaults/Defaults on 30 April 1998  RLR  */
static NR_TYPE	pitchlcorr = 0.0,	/* PITCH_BIAS_1   */
		thdglcorr  = 0.0,	/* HEADING_BIAS_1 */
		pitchrcorr = 0.0,	/* PITCH_BIAS_2   */
		thdgrcorr  = 0.0;	/* HEADING_BIAS_2 */

/* Start generating some stats on missing labels & 0.0 values from the
 * IRS.
 */
static int	label_errors[256];	// One for each possible arinc label.
static int	zero_errors[256];	// One for each possible arinc label.
static void	preProcessData(var_base *varp, long input[], int thisLabel);
static void	postProcessData(var_base *varp, long input[], NR_TYPE *out);

static std::map<int, std::string> _labelMap;

/* -------------------------------------------------------------------- */
void irsInit(RAWTBL *varp)
{
  NR_TYPE  *tmp;

  // Setup ARINC label --> variable map.
  _labelMap[0xc8] = "Latitude";
  _labelMap[0xc9] = "Longitude";
  _labelMap[0xd4] = "Pitch";
  _labelMap[0xd5] = "Roll";
  _labelMap[0xcc] = "True Heading";
  _labelMap[0xf1] = "Altitude";
  _labelMap[0xf5] = "Vertical Speed";
  _labelMap[0xf4] = "Vertical Acceleration";
  _labelMap[0xf7] = "EW Velocity";
  _labelMap[0xf6] = "NS Velocity";
  _labelMap[0xce] = "Wind Diretion";
  _labelMap[0xcd] = "Wind Speed";
  _labelMap[0xca] = "Ground Speed";
  _labelMap[0xcb] = "Track Angle";
  _labelMap[0xd1] = "Drift Angle";

  for (int i = 0; i < 256; ++i)
    {
    label_errors[i] = 0;
    zero_errors[i] = 0;
    }

  if ((tmp = GetDefaultsValue("HEADING_BIAS_1", varp->name)) == NULL)
    {
    sprintf(buffer,"Values set to %f, in AMLIB function irsInit.\n",thdglcorr);
    LogMessage(buffer);
    }
  else
    thdglcorr = tmp[0];

  if ((tmp = GetDefaultsValue("HEADING_BIAS_2", varp->name)) == NULL)
    {
    sprintf(buffer,"Values set to %f, in AMLIB function irsInit.\n",thdgrcorr);
    LogMessage(buffer);
    }
  else
    thdgrcorr = tmp[0];

  if ((tmp = GetDefaultsValue("PITCH_BIAS_1", varp->name)) == NULL)
    {
    sprintf(buffer,"Values set to %f, in AMLIB function irsInit.\n",pitchlcorr);
    LogMessage(buffer);
    }
  else
    pitchlcorr = tmp[0];

  if ((tmp = GetDefaultsValue("PITCH_BIAS_2", varp->name)) == NULL)
    {
    sprintf(buffer,"Values set to %f, in AMLIB function irsInit.\n",pitchrcorr);
    LogMessage(buffer);
    }
  else
    pitchrcorr = tmp[0];

}  /* END IRSINIT */

/* -------------------------------------------------------------------- */
void xlilat(RAWTBL *varp, void *input, NR_TYPE *np)
{
  long  *p = ((Irs_blk *)input)->present_lat;

  /* (degs) 20 bits, +- 0.5 pirad, label 310  */

  preProcessData(varp, p, 0xc8);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (double)(p[i] >> 11) * arinc20bits;

  postProcessData(varp, p, np);

  varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_5hz_frame);

} /* END XLILAT */

/* -------------------------------------------------------------------- */
void xlilon(RAWTBL *varp, void *input, NR_TYPE *np)
{
  long  *p = ((Irs_blk *)input)->present_lon;

  /* (degs) 20 bits, +- 0.5 pirad, label 311  */

  preProcessData(varp, p, 0xc9);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (double)(p[i] >> 11) * arinc20bits;

  postProcessData(varp, p, np);

  varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_5hz_frame);

} /* END XLILON */

/* -------------------------------------------------------------------- */
void xlipitch(RAWTBL *varp, void *input, NR_TYPE *np)
{
  long  *p = ((Irs_blk *)input)->pitch_angle;
  float  correction;

  if (varp->name[5] != '\0')
    correction = pitchrcorr;
  else
    correction = pitchlcorr;

  /* (degs) 20 bits, +- 0.5 pirad, label 324  */

  preProcessData(varp, p, 0xd4);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (double)(p[i] >> 11) * arinc20bits + correction;

  postProcessData(varp, p, np);

  varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_50hz_frame);

} /* END XLIPITCH */

/* -------------------------------------------------------------------- */
void xliroll(RAWTBL *varp, void *input, NR_TYPE *np)
{
  long  *p = ((Irs_blk *)input)->roll_angle;

  /* (degs) 20 bits, +- 0.5 pirad, label 325  */

  preProcessData(varp, p, 0xd5);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (double)(p[i] >> 11) * arinc20bits;

  postProcessData(varp, p, np);

  varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_50hz_frame);

}  /* END XLIROLL */

/* -------------------------------------------------------------------- */
void xlithdg(RAWTBL *varp, void *input, NR_TYPE *np)
{
  long  *p = ((Irs_blk *)input)->true_heading;
  float  correction;

  if (varp->name[4] != '\0')
    correction = thdgrcorr;
  else
    correction = thdglcorr;


  /* (degs) 20 bits, +- 0.5 pirad, label 314  */

  preProcessData(varp, p, 0xcc);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
    np[i] = (double)(p[i] >> 11) * arinc20bits + correction;

    if (np[i] < 0.0)
      np[i] += 360.0;

    if (np[i] > 360.0)
      np[i] -= 360.0;
    }

  postProcessData(varp, p, np);

  varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_25hz_frame);

}  /* END XLITHDG */

/* -------------------------------------------------------------------- */
void xlialt(RAWTBL *varp, void *input, NR_TYPE *np)
{
  long  *p = ((Irs_blk *)input)->inertial_alt;

  /* (degs) 20 bits, +- 0.5 pirad, label 361  */

  preProcessData(varp, p, 0xf1);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (double)(p[i] >> 11)* 0.125 * FTMTR;

  postProcessData(varp, p, np);

  varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_25hz_frame);

}  /* END XLIALT */

/* -------------------------------------------------------------------- */
void xlivspd(RAWTBL *varp, void *input, NR_TYPE *np)
{
  long  *p = ((Irs_blk *)input)->inrt_vert_speed;

  /* (degs) 20 bits, +- 0.5 pirad, label 365  */

  preProcessData(varp, p, 0xf5);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (double)(p[i] >> 11) * 0.03125 * FTMIN;

  postProcessData(varp, p, np);

  varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_25hz_frame);

}  /* END XLIVSPD */

/* -------------------------------------------------------------------- */
void xlvacc(RAWTBL *varp, void *input, NR_TYPE *np)
{
  long  *p = ((Irs_blk *)input)->vertical_accel;

  /* (G) 15 bits, +- 4.0 kts, label 364  */

  preProcessData(varp, p, 0xf4);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (double)(p[i] >> 16) * 1.22e-04 * MPS2;

  postProcessData(varp, p, np);

  varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_50hz_frame);

}  /* END XLVACC */

/* -------------------------------------------------------------------- */
void xlivew(RAWTBL *varp, void *input, NR_TYPE *np)
{
  long  *p = ((Irs_blk *)input)->velocity_ew;

  /* 20 bits, +-4095 kts, label 367  */

  preProcessData(varp, p, 0xf7);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (double)(p[i] >> 11) * 0.00390625 * KTS2MS;

  postProcessData(varp, p, np);

  varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_10hz_frame);

} /* END XLIVEW */

/* -------------------------------------------------------------------- */
void xlivns(RAWTBL *varp, void *input, NR_TYPE *np)
{
  long  *p = ((Irs_blk *)input)->velocity_ns;

  /* 20 bits, +-4095 kts, label 366  */

  preProcessData(varp, p, 0xf6);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (double)(p[i] >> 11) * 0.00390625 * KTS2MS;

  postProcessData(varp, p, np);

  varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_10hz_frame);

} /* END XLIVNS */

/* -------------------------------------------------------------------- */
void xliwd(RAWTBL *varp, void *input, NR_TYPE *np)
{
  long  *p = ((Irs_blk *)input)->wind_dir_true;

  /* 20 bits, +-4095 kts, label 366  */

  preProcessData(varp, p, 0xce);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
    np[i] = (double)(p[i] >> 11) * arinc20bits;
 
    if (np[i] < 0.0)
      np[i] += 360.0;
 
    if (np[i] > 360.0)
      np[i] -= 360.0;
    }

  postProcessData(varp, p, np);

  varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_10hz_frame);

} /* END XLIWD */

/* -------------------------------------------------------------------- */
void xliws(RAWTBL *varp, void *input, NR_TYPE *np)
{
  long  *p = ((Irs_blk *)input)->wind_speed;

  /* 20 bits, +-4095 kts, label 366  */

  preProcessData(varp, p, 0xcd);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (double)(p[i] >> 10) * 1.22e-04 * KTS2MS;

  postProcessData(varp, p, np);

  varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_10hz_frame);

} /* END XLIWS */

/* -------------------------------------------------------------------- */
void xligspd(RAWTBL *varp, void *input, NR_TYPE *np)
{
  long  *p = ((Irs_blk *)input)->ground_speed;

  /* 20 bits, 0 to 4095 kts, label 312  */

  preProcessData(varp, p, 0xca);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (double)(p[i] >> 11) * 0.00390625 * KTS2MS;

  postProcessData(varp, p, np);

  varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_10hz_frame);

} /* END XLIGSPD */

/* -------------------------------------------------------------------- */
void xllag50(RAWTBL *varp, void *p, NR_TYPE *np)
{
  *np = (NR_TYPE)ntohl(((Irs_blk *)p)->lag_50hz_frame);

}  /* END XLLAG50 */

/* -------------------------------------------------------------------- */
void xllag25(RAWTBL *varp, void *p, NR_TYPE *np)
{
  *np = (NR_TYPE)ntohl(((Irs_blk *)p)->lag_25hz_frame);

}  /* END XLLAG25 */

/* -------------------------------------------------------------------- */
void xllag10(RAWTBL *varp, void *p, NR_TYPE *np)
{
  *np = (NR_TYPE)ntohl(((Irs_blk *)p)->lag_10hz_frame);

}  /* END XLLAG10 */

/* -------------------------------------------------------------------- */
void xllag5(RAWTBL *varp, void *p, NR_TYPE *np)
{
  *np = (NR_TYPE)ntohl(((Irs_blk *)p)->lag_5hz_frame);

}  /* END XLLAG5 */

/* -------------------------------------------------------------------- */
void xlbrollr(RAWTBL *varp, void *input, NR_TYPE *np)
{
  Irs_blk  *p = (Irs_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    p->roll_rate[i] = ntohl(p->roll_rate[i]);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (NR_TYPE)(p->roll_rate[i] >> 16) * 0.0039;

  varp->DynamicLag = ntohl(p->lag_50hz_frame);

}  /* END XLBROLLR */

/* -------------------------------------------------------------------- */
void xlbpitchr(RAWTBL *varp, void *input, NR_TYPE *np)
{
  Irs_blk  *p = (Irs_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    p->pitch_rate[i] = ntohl(p->pitch_rate[i]);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (NR_TYPE)(p->pitch_rate[i] >> 16) * 0.0039;

  varp->DynamicLag = ntohl(p->lag_50hz_frame);

}  /* END XLBPITCHR */

/* -------------------------------------------------------------------- */
void xlbyawr(RAWTBL *varp, void *input, NR_TYPE *np)
{
  Irs_blk  *p = (Irs_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    p->yaw_rate[i] = ntohl(p->yaw_rate[i]);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (NR_TYPE)(p->yaw_rate[i] >> 16) * 0.0039;

  varp->DynamicLag = ntohl(p->lag_50hz_frame);

}  /* END XLBYAWR */

/* -------------------------------------------------------------------- */
void xlblata(RAWTBL *varp, void *input, NR_TYPE *np)
{
  Irs_blk	*p = (Irs_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    p->lat_accel[i] = ntohl(p->lat_accel[i]);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (NR_TYPE)(p->lat_accel[i] >> 16) * 1.22e-4; // * MPS2;

  varp->DynamicLag = ntohl(p->lag_50hz_frame);

}  /* END XLBLATA */

/* -------------------------------------------------------------------- */
void xlblona(RAWTBL *varp, void *input, NR_TYPE *np)
{
  Irs_blk	*p = (Irs_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    p->long_accel[i] = ntohl(p->long_accel[i]);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (NR_TYPE)(p->long_accel[i] >> 16) * 1.22e-4; // * MPS2;

  varp->DynamicLag = ntohl(p->lag_50hz_frame);

}  /* END XLBLONA */

/* -------------------------------------------------------------------- */
void xlbnorma(RAWTBL *varp, void *input, NR_TYPE *np)
{
  Irs_blk	*p = (Irs_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    p->normal_accel[i] = ntohl(p->normal_accel[i]);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (NR_TYPE)(p->normal_accel[i] >> 16) * 1.22e-4; // * MPS2;

  varp->DynamicLag = ntohl(p->lag_50hz_frame);

}  /* END XLBNORMA */

/* -------------------------------------------------------------------- */
void xlitrkart(RAWTBL *varp, void *input, NR_TYPE *np)
{
  Irs_blk	*p = (Irs_blk *)input;

  /* (degs) 20 bits, +- 32.0 Deg/S, label 335  */

  for (size_t i = 0; i < varp->SampleRate; ++i)
    p->track_ang_rate[i] = ntohl(p->track_ang_rate[i]);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (double)(p->track_ang_rate[i] >> 11) * 3.05e-05;

  varp->DynamicLag = ntohl(p->lag_50hz_frame);

}  /* END XLITRKART */

/* -------------------------------------------------------------------- */
void xlittrka(RAWTBL *varp, void *input, NR_TYPE *np)
{
  Irs_blk	*p = (Irs_blk *)input;

  /* (degs) 20 bits, +- 1.0 pirad, label 313  */

  for (size_t i = 0; i < varp->SampleRate; ++i)
    p->track_angle_true[i] = ntohl(p->track_angle_true[i]);
 
  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
/*          Diagnostic print below   
    if ( (p->track_angle_true[i] & 0xff) != 0xcb)
      printf ("irsHw:  TKAT ID incorrect (%o)\n", p->track_angle_true[i] & 0xff);
            Diagnostic print above   */
    np[i] = (double)(p->track_angle_true[i] >> 11) * arinc20bits;

    if (np[i] < 0.0)
      np[i] += 360.0;

    if (np[i] > 360.0)
      np[i] -= 360.0;
    }

  varp->DynamicLag = ntohl(p->lag_25hz_frame);

}  /* END XLITTRKA */

/* -------------------------------------------------------------------- */
void xlidrift(RAWTBL *varp, void *input, NR_TYPE *np)
{
  long  *p = ((Irs_blk *)input)->drift_angle;

  /* (degs) 20 bits, +- 0.5 pirad, label 321  */

  preProcessData(varp, p, 0xd1);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (double)(p[i] >> 11) * arinc20bits;

  postProcessData(varp, p, np);

  varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_25hz_frame);

}  /* END XLIDRIFT */

/* -------------------------------------------------------------------- */
void preProcessData(var_base *varp, long input[], int thisLabel)
{
  bool	allZeros = true;
  int	zeroCnt = 0;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    if (input[i] == 0)
    {
      ++zeroCnt;
      ++zero_errors[thisLabel];
    }
    else
      allZeros = false;

  /* Well we could print this even for 1 missing value, but that happens
   * every 15 minutes when the clocks walk, so only print out of ordinary.
   */
  if (zeroCnt > 1)
  {
    fprintf(stderr,
	"irsHw: %s: %d of %d zero values this second.\n",
	varp->name, zeroCnt, varp->SampleRate);
  }

  if (allZeros)
    return;


  for (size_t i = 0; i < varp->SampleRate; ++i)
  {
    // Byte swap.
    input[i] = ntohl(input[i]);

    int label = input[i] & 0x000000ff;
 
    if (label != thisLabel)
    {
      if (input[i] != 0)
        fprintf(stderr,
		"irsHw: %s: Incorrect arinc label of %o, zero'ing data.\n",
		varp->name, label);

      input[i] = 0;
      ++label_errors[thisLabel];
    }
    else
    if ((input[i] & 0xffffff00) == 0)
      fprintf(stderr, "irsHw: %s:  Label correct, value is pure zero though.\n",
		varp->name);
  }
}

/* -------------------------------------------------------------------- */
NR_TYPE polyinterp(NR_TYPE xa[], NR_TYPE ya[], int n, int x);

void postProcessData(var_base *varp, long input[], NR_TYPE *out)
{
  if (cfg.HoneyWellCleanup() == false)
    return;

  bool  allZeros = true;
  int   zeroCnt = 0;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    if (input[i] == 0)
    {
      ++zeroCnt;
    }
    else
      allZeros = false;

  // If it's all 0's or all good, then leave.
  if (allZeros || zeroCnt == 0)
    return;


  int	goodPoints = 0;
  NR_TYPE x[varp->SampleRate];
  NR_TYPE y[varp->SampleRate];

  for (size_t i = 0; i < varp->SampleRate; ++i)
    if (input[i] != 0)
    {
      x[goodPoints] = i;
      y[goodPoints] = out[i];
      ++goodPoints;
    }

  for (size_t i = 0; i < varp->SampleRate; ++i)
    if (input[i] == 0)
      out[i] = polyinterp(x, y, goodPoints, i);

}

/* -------------------------------------------------------------------- */
void LogIRSerrors()
{
  std::map<int, std::string>::iterator it;

  for (it = _labelMap.begin(); it != _labelMap.end(); ++it)
  {
    if (label_errors[it->first] > 0)
    {
      sprintf(buffer, "IRS Error report: %s had %d wrong ARINC labels.\n",
		(it->second).c_str(), label_errors[it->first]);
      LogMessage(buffer);
    }

    if (zero_errors[it->first] > 0)
    {
      sprintf(buffer, "IRS Error report: %s had %d values of 0 (zero).\n",
		(it->second).c_str(), zero_errors[it->first]);
      LogMessage(buffer);
    }
  }
}

/* END IRSHW.C */
