/*
-------------------------------------------------------------------------
OBJECT NAME:	irsHw.c

FULL NAME:	Convert IRS block data

ENTRY POINTS:	xlilat(), xlilon(), xlipitch(), xliroll(), xlithdg(),
		xligspd(), xlvacc(), xlivspd(), xlivns(), xlivew(),
		xllag50(), xllag25(), xllag10(), xllag5(), xlialt()

DESCRIPTION:	Decode routines for the Honeywell Laser Ref 4/5.
		Include interpolation for missing data.  True raw
		data may be obtained by setting cfg.HoneyWellCleanup()
		to false (see GUI).

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2005
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

#include <map>

/* All clients of arinc20bit are in pirad's, so multiply by 180 right
 * now to get to degree's.
 */
static const double  arinc20bits = (1.0 / (1 << 20)) * 180.0;
static const double  arinc15bits = (1.0 / (1 << 15));
static const double  arinc13bits = (1.0 / (1 << 13));
static const double  arinc8bits = (1.0 / (1 << 8));

/* IRS box alignment corrections  */
/*  Values from /home/local/proj/defaults/Defaults on 30 April 1998  RLR  */
static NR_TYPE	pitchlcorr = 0.0,	/* PITCH_BIAS_1   */
		thdglcorr  = 0.0,	/* HEADING_BIAS_1 */
		pitchrcorr = 0.0,	/* PITCH_BIAS_2   */
		thdgrcorr  = 0.0;	/* HEADING_BIAS_2 */

/* Start generating some stats on missing labels & 0.0 values from the
 * IRS.  These persist through entire flight/processing.
 */
static int	label_errors[256];	// One for each possible arinc label.
static int	zero_errors[256];	// One for each possible arinc label.

static bool	groundSpeedIsAlternating = false;

static void	preProcessData(var_base *varp, long input[], int thisLabel);
static void	postProcessData(var_base *varp, long input[], NR_TYPE *out, int thisLabel);

static std::map<int, std::string> _labelMap;

/* -------------------------------------------------------------------- */
void irsInit(var_base *varp)
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
  _labelMap[0xca] = "Ground Speed";
  _labelMap[0xcb] = "Track Angle";
  _labelMap[0xd1] = "Drift Angle";
//  _labelMap[0xce] = "Wind Direction";
//  _labelMap[0xcd] = "Wind Speed";
//  _labelMap[0xd7] = "Roll Rate";
//  _labelMap[0xd6] = "Pitch Rate";
//  _labelMap[0xd8] = "Yaw Rate";
//  _labelMap[0xda] = "Lateral Accel";
//  _labelMap[0xd9] = "Long Accel";
//  _labelMap[0xdb] = "Normal Accel";
//  _labelMap[0xdd] = "Track Angle Rate";

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
  int label = 0xc8;

  preProcessData(varp, p, label);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (double)(p[i] >> 11) * arinc20bits;

  postProcessData(varp, p, np, label);

  if ((varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_5hz_frame)) > 1000)
    varp->DynamicLag = 0;

} /* END XLILAT */

/* -------------------------------------------------------------------- */
void xlilon(RAWTBL *varp, void *input, NR_TYPE *np)
{
  long  *p = ((Irs_blk *)input)->present_lon;

  /* (degs) 20 bits, +- 1.0 pirad, label 311  */
  int label = 0xc9;

  preProcessData(varp, p, label);

  for (size_t i = 0; i < varp->SampleRate; ++i)
  {
    np[i] = (double)(p[i] >> 11) * arinc20bits;
    if (cfg.ProjectName() == "TPARC" && cfg.FlightNumber() == "rf10")
      np[i] += 289.87;
  }

  postProcessData(varp, p, np, label);

  if ((varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_5hz_frame)) > 1000)
    varp->DynamicLag = 0;

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
  int label = 0xd4;

  preProcessData(varp, p, label);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (double)(p[i] >> 11) * arinc20bits + correction;

  postProcessData(varp, p, np, label);

  if ((varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_50hz_frame)) > 1000)
    varp->DynamicLag = 0;

} /* END XLIPITCH */

/* -------------------------------------------------------------------- */
void xliroll(RAWTBL *varp, void *input, NR_TYPE *np)
{
  long  *p = ((Irs_blk *)input)->roll_angle;

  /* (degs) 20 bits, +- 1.0 pirad, label 325  */
  int label = 0xd5;

  preProcessData(varp, p, label);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (double)(p[i] >> 11) * arinc20bits;

  postProcessData(varp, p, np, label);

  if ((varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_50hz_frame)) > 1000)
    varp->DynamicLag = 0;

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


  /* (degs) 20 bits, +- 1.0 pirad, label 314  */
  int label = 0xcc;

  preProcessData(varp, p, label);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
    np[i] = (double)(p[i] >> 11) * arinc20bits + correction;

    if (np[i] < 0.0)
      np[i] += 360.0;

    if (np[i] > 360.0)
      np[i] -= 360.0;
    }

  postProcessData(varp, p, np, label);

  if ((varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_25hz_frame)) > 1000)
    varp->DynamicLag = 0;

}  /* END XLITHDG */

/* -------------------------------------------------------------------- */
void xlialt(RAWTBL *varp, void *input, NR_TYPE *np)
{
  long  *p = ((Irs_blk *)input)->inertial_alt;

  /* (degs) 20 bits, +- 131072 Ft, label 361  */
  int label = 0xf1;

  preProcessData(varp, p, label);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (double)(p[i] >> 11) * 0.125 * FTMTR;

  postProcessData(varp, p, np, label);

  if ((varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_25hz_frame)) > 1000)
    varp->DynamicLag = 0;

}  /* END XLIALT */

/* -------------------------------------------------------------------- */
void xlivspd(RAWTBL *varp, void *input, NR_TYPE *np)
{
  long  *p = ((Irs_blk *)input)->inrt_vert_speed;

  /* (degs) 20 bits, +- 32768 Ft/min, label 365  */
  int label = 0xf5;

  preProcessData(varp, p, label);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (double)(p[i] >> 11) * 0.03125 * FTMIN;

  postProcessData(varp, p, np, label);

  if ((varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_25hz_frame)) > 1000)
    varp->DynamicLag = 0;

}  /* END XLIVSPD */

/* -------------------------------------------------------------------- */
void xlvacc(RAWTBL *varp, void *input, NR_TYPE *np)
{
  long  *p = ((Irs_blk *)input)->vertical_accel;

  /* (G) 15 bits, +- 4.0 kts, label 364  */
  int label = 0xf4;

  preProcessData(varp, p, label);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (double)(p[i] >> 16) * arinc13bits * MPS2;

  postProcessData(varp, p, np, label);

  if ((varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_50hz_frame)) > 1000)
    varp->DynamicLag = 0;

}  /* END XLVACC */

/* -------------------------------------------------------------------- */
void xlivew(RAWTBL *varp, void *input, NR_TYPE *np)
{
  long  *p = ((Irs_blk *)input)->velocity_ew;

  /* 20 bits, +-4095 kts, label 367  */
  int label = 0xf7;

  preProcessData(varp, p, label);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (double)(p[i] >> 11) * arinc8bits * KTS2MS;

  postProcessData(varp, p, np, label);

  if ((varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_10hz_frame)) > 1000)
    varp->DynamicLag = 0;

  if (groundSpeedIsAlternating)
    varp->DynamicLag += 100;

} /* END XLIVEW */

/* -------------------------------------------------------------------- */
void xlivns(RAWTBL *varp, void *input, NR_TYPE *np)
{
  long  *p = ((Irs_blk *)input)->velocity_ns;

  /* 20 bits, +-4095 kts, label 366  */
  int label = 0xf6;

  preProcessData(varp, p, label);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (double)(p[i] >> 11) * arinc8bits * KTS2MS;

  postProcessData(varp, p, np, label);

  if ((varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_10hz_frame)) > 1000)
    varp->DynamicLag = 0;

  if (groundSpeedIsAlternating)
    varp->DynamicLag += 100;

} /* END XLIVNS */

/* -------------------------------------------------------------------- */
void xliwd(RAWTBL *varp, void *input, NR_TYPE *np)
{
  long  *p = ((Irs_blk *)input)->wind_dir_true;

  /* 20 bits, +-1 pirad, label 316  */
  int label = 0xce;

  preProcessData(varp, p, label);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
    np[i] = (double)(p[i] >> 11) * arinc20bits;
 
    if (np[i] < 0.0)
      np[i] += 360.0;
 
    if (np[i] > 360.0)
      np[i] -= 360.0;
    }

  postProcessData(varp, p, np, label);

  if ((varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_10hz_frame)) > 1000)
    varp->DynamicLag = 0;

} /* END XLIWD */

/* -------------------------------------------------------------------- */
void xliws(RAWTBL *varp, void *input, NR_TYPE *np)
{
  long  *p = ((Irs_blk *)input)->wind_speed;

  /* 20 bits, 0-255 kts, label 315  */
  int label = 0xcd;

  preProcessData(varp, p, label);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (double)(p[i] >> 10) * arinc13bits * KTS2MS;

  postProcessData(varp, p, np, label);

  if ((varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_10hz_frame)) > 1000)
    varp->DynamicLag = 0;

} /* END XLIWS */

/* -------------------------------------------------------------------- */
void xligspd(RAWTBL *varp, void *input, NR_TYPE *np)
{
  long  *p = ((Irs_blk *)input)->ground_speed;

  /* 20 bits, 0 to 4095 kts, label 312  */
  int label = 0xca;

  preProcessData(varp, p, label);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (double)(p[i] >> 11) * arinc8bits * KTS2MS;

  postProcessData(varp, p, np, label);

  if ((varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_10hz_frame)) > 1000)
    varp->DynamicLag = 0;

  if (groundSpeedIsAlternating)
    varp->DynamicLag += 100;


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
  long  *p = ((Irs_blk *)input)->roll_rate;

  /* 20 bits, +- 128 Deg/s, label 327  */
  int label = 0xd7;

  preProcessData(varp, p, label);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (double)(p[i] >> 16) * arinc8bits;

  postProcessData(varp, p, np, label);

  if ((varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_50hz_frame)) > 1000)
    varp->DynamicLag = 0;

}  /* END XLBROLLR */

/* -------------------------------------------------------------------- */
void xlbpitchr(RAWTBL *varp, void *input, NR_TYPE *np)
{
  long  *p = ((Irs_blk *)input)->pitch_rate;

  /* 20 bits, +- 128 Deg/s, label 326  */
  int label = 0xd6;

  preProcessData(varp, p, label);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (double)(p[i] >> 16) * arinc8bits;

  postProcessData(varp, p, np, label);

  if ((varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_50hz_frame)) > 1000)
    varp->DynamicLag = 0;

}  /* END XLBPITCHR */

/* -------------------------------------------------------------------- */
void xlbyawr(RAWTBL *varp, void *input, NR_TYPE *np)
{
  long  *p = ((Irs_blk *)input)->yaw_rate;

  /* 20 bits, +- 128 Deg/s, label 330  */
  int label = 0xd8;

  preProcessData(varp, p, label);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (double)(p[i] >> 16) * arinc8bits;

  postProcessData(varp, p, np, label);

  if ((varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_50hz_frame)) > 1000)
    varp->DynamicLag = 0;

}  /* END XLBYAWR */

/* -------------------------------------------------------------------- */
void xlblata(RAWTBL *varp, void *input, NR_TYPE *np)
{
  long  *p = ((Irs_blk *)input)->lat_accel;

  /* 20 bits, +-4 G's, label 332  */
  int label = 0xda;

  preProcessData(varp, p, label);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (double)(p[i] >> 16) * arinc13bits;

  postProcessData(varp, p, np, label);

  if ((varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_50hz_frame)) > 1000)
    varp->DynamicLag = 0;

}  /* END XLBLATA */

/* -------------------------------------------------------------------- */
void xlblona(RAWTBL *varp, void *input, NR_TYPE *np)
{
  long  *p = ((Irs_blk *)input)->long_accel;

  /* 20 bits, +-4 G's, label 331  */
  int label = 0xd9;

  preProcessData(varp, p, label);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (double)(p[i] >> 16) * arinc13bits;

  postProcessData(varp, p, np, label);

  if ((varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_50hz_frame)) > 1000)
    varp->DynamicLag = 0;

}  /* END XLBLONA */

/* -------------------------------------------------------------------- */
void xlbnorma(RAWTBL *varp, void *input, NR_TYPE *np)
{
  long  *p = ((Irs_blk *)input)->normal_accel;

  /* 20 bits, +-4 G's, label 333  */
  int label = 0xdb;

  preProcessData(varp, p, label);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (double)(p[i] >> 16) * arinc13bits;

  postProcessData(varp, p, np, label);

  if ((varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_50hz_frame)) > 1000)
    varp->DynamicLag = 0;

}  /* END XLBNORMA */

/* -------------------------------------------------------------------- */
void xlitrkart(RAWTBL *varp, void *input, NR_TYPE *np)
{
  long	*p = ((Irs_blk *)input)->track_ang_rate;

  /* (degs) 20 bits, +- 32.0 Deg/S, label 335  */
  int label = 0xdd;

  preProcessData(varp, p, label);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (double)(p[i] >> 11) * arinc15bits;

  postProcessData(varp, p, np, label);

  if ((varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_50hz_frame)) > 1000)
    varp->DynamicLag = 0;

}  /* END XLITRKART */

/* -------------------------------------------------------------------- */
void xlittrka(RAWTBL *varp, void *input, NR_TYPE *np)
{
  long	*p = ((Irs_blk *)input)->track_angle_true;

  /* (degs) 20 bits, +- 1.0 pirad, label 313  */
  int label = 0xcb;

  preProcessData(varp, p, label);
 
  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
/*          Diagnostic print below   
    if ( (p->track_angle_true[i] & 0xff) != 0xcb)
      printf ("irsHw:  TKAT ID incorrect (%o)\n", p->track_angle_true[i] & 0xff);
            Diagnostic print above   */
    np[i] = (double)(p[i] >> 11) * arinc20bits;

    if (np[i] < 0.0)
      np[i] += 360.0;

    if (np[i] > 360.0)
      np[i] -= 360.0;
    }

  postProcessData(varp, p, np, label);

  if ((varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_25hz_frame)) > 1000)
    varp->DynamicLag = 0;

}  /* END XLITTRKA */

/* -------------------------------------------------------------------- */
void xlidrift(RAWTBL *varp, void *input, NR_TYPE *np)
{
  long  *p = ((Irs_blk *)input)->drift_angle;

  /* (degs) 20 bits, +- 0.5 pirad, label 321  */
  int label = 0xd1;

  preProcessData(varp, p, label);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (double)(p[i] >> 11) * arinc20bits;

  postProcessData(varp, p, np, label);

  if ((varp->DynamicLag = ntohl(((Irs_blk *)input)->lag_25hz_frame)) > 1000)
    varp->DynamicLag = 0;

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
  if (false)	// Too much of a good thing, turn off by default....
//  if (zeroCnt > 1)
  {
    sprintf(buffer,
	"irsHw: %s: %d of %d zero values this second.",
	varp->name, zeroCnt, varp->SampleRate);
    LogXlateMsg(buffer);
  }

  if (allZeros)
    return;

  int label = 0;

  for (size_t i = 0; i < varp->SampleRate; ++i)
  {
    if (input[i] == 0)
      continue;

    // Byte swap.
    input[i] = ntohl(input[i]);

    label = input[i] & 0x000000ff;
 
    if (label != thisLabel)
    {
      sprintf(buffer,
		"irsHw: %s: Incorrect arinc label of %o, zero'ing data.",
		varp->name, label);
      LogXlateMsg(buffer);

      input[i] = 0;
      ++label_errors[thisLabel];
    }
    else
    if ((input[i] & 0xffffff00) == 0)
    {
      sprintf(buffer, "irsHw: %s:  Label correct, value is pure zero though.",
		varp->name);
      LogXlateMsg(buffer);
    }
  }


  // Shift samples forward if we are suffering 10Hz sampling problem.
  groundSpeedIsAlternating = false;
  if (varp->SampleRate == 10)
  {
    if (input[1] == 0 && input[3] == 0 && input[5] == 0 &&
        input[7] == 0 && input[9] == 0)
      groundSpeedIsAlternating = true;

    if (input[0] == 0 && input[2] == 0 && input[4] == 0 &&
        input[6] == 0 && input[8] == 0)
      groundSpeedIsAlternating = true;
  }
}

/* -------------------------------------------------------------------- */
void postProcessData(var_base *varp, long input[], NR_TYPE *out, int label)
{
  if (cfg.HoneyWellCleanup() == false)
    return;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    if (input[i] == 0)
      out[i] = floatNAN;
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
