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


static const double  arinc20bits = (1.0 / (1 << 20)) * 180.0;
static const double  arinc15bits = (1.0 / (1 << 15));

/* IRS box alignment corrections  */
/*  Values from /home/local/proj/defaults/Defaults on 30 April 1998  RLR  */
static NR_TYPE	pitchlcorr = 0.0,	/* PITCH_BIAS_1   */
		thdglcorr  = 0.0,	/* HEADING_BIAS_1 */
		pitchrcorr = 0.0,	/* PITCH_BIAS_2   */
		thdgrcorr  = 0.0;	/* HEADING_BIAS_2 */

/* -------------------------------------------------------------------- */
void irsInit(RAWTBL *varp)
{
  NR_TYPE  *tmp;

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
  int	label;
  Irs_blk  *p = (Irs_blk *)input;

  /* (degs) 20 bits, +- 0.5 pirad, label 310  */

  for (size_t i = 0; i < varp->SampleRate; ++i)
    p->present_lat[i] = ntohl(p->present_lat[i]);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
    label = p->present_lat[i] & 0x000000ff;

    if (!RawData && label != 0xc8)
      {
      if (label)
        printf("irsHw: %s arinc label of %o\n", varp->name, label);

      if (i == 0)
        {
        for (size_t j = 1; j < varp->SampleRate && label != 0xc8; ++j)
          if ((label = p->present_lat[j] & 0x000000ff) == 0xc8)
            p->present_lat[i] = p->present_lat[j];
        }
      else
        p->present_lat[i] = p->present_lat[i-1];
      }

    np[i] = (double)(p->present_lat[i] >> 11) * arinc20bits;
    }

  varp->DynamicLag = ntohl(p->lag_5hz_frame);

} /* END XLILAT */

/* -------------------------------------------------------------------- */
void xlilon(RAWTBL *varp, void *input, NR_TYPE *np)
{
  int	label;
  Irs_blk  *p = (Irs_blk *)input;

  /* (degs) 20 bits, +- 0.5 pirad, label 311  */

  for (size_t i = 0; i < varp->SampleRate; ++i)
    p->present_lon[i] = ntohl(p->present_lon[i]);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
    label = p->present_lon[i] & 0x000000ff;

    if (!RawData && label != 0xc9)
      {
      if (label)
        printf("irsHw: %s arinc label of %o\n", varp->name, label);

      if (i == 0)
        {
        for (size_t j = 1; j < varp->SampleRate && label != 0xc9; ++j)
          if ((label = p->present_lon[j] & 0x000000ff) == 0xc9)
            p->present_lon[i] = p->present_lon[j];
        }
      else
        p->present_lon[i] = p->present_lon[i-1];
      }

    np[i] = (double)(p->present_lon[i] >> 11) * arinc20bits;
    }

  varp->DynamicLag = ntohl(p->lag_5hz_frame);

} /* END XLILON */

/* -------------------------------------------------------------------- */
void xlipitch(RAWTBL *varp, void *input, NR_TYPE *np)
{
  int	label;
  Irs_blk  *p = (Irs_blk *)input;
  float  correction;

  if (varp->name[5] != '\0')
    correction = pitchrcorr;
  else
    correction = pitchlcorr;

  /* (degs) 20 bits, +- 0.5 pirad, label 324  */

  for (size_t i = 0; i < varp->SampleRate; ++i)
    p->pitch_angle[i] = ntohl(p->pitch_angle[i]);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
    label = p->pitch_angle[i] & 0x000000ff;

    if (!RawData && label != 0xd4)
      {
      if (label)
        printf("irsHw: %s arinc label of %o\n", varp->name, label);

      if (i == 0)
        {
        for (size_t j = 1; j < varp->SampleRate && label != 0xd4; ++j)
          if ((label = p->pitch_angle[j] & 0x000000ff) == 0xd4)
            p->pitch_angle[i] = p->pitch_angle[j];
        }
      else
        p->pitch_angle[i] = p->pitch_angle[i-1];
      }

    np[i] = (double)(p->pitch_angle[i] >> 11) * arinc20bits + correction;
    }

  varp->DynamicLag = ntohl(p->lag_50hz_frame);

} /* END XLIPITCH */

/* -------------------------------------------------------------------- */
void xliroll(RAWTBL *varp, void *input, NR_TYPE *np)
{
  int	label;
  Irs_blk  *p = (Irs_blk *)input;

  /* (degs) 20 bits, +- 0.5 pirad, label 325  */

  for (size_t i = 0; i < varp->SampleRate; ++i)
    p->roll_angle[i] = ntohl(p->roll_angle[i]);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
    label = p->roll_angle[i] & 0x000000ff;

    if (!RawData && label != 0xd5)
      {
      if (label)
        printf("irsHw: %s arinc label of %o\n", varp->name, label);

      if (i == 0)
        {
        for (size_t j = 1; j < varp->SampleRate && label != 0xd5; ++j)
          if ((label = p->roll_angle[j] & 0x000000ff) == 0xd5)
            p->roll_angle[i] = p->roll_angle[j];
        }
      else
        p->roll_angle[i] = p->roll_angle[i-1];
      }

    np[i] = (double)(p->roll_angle[i] >> 11) * arinc20bits;
    }

  varp->DynamicLag = ntohl(p->lag_50hz_frame);

}  /* END XLIROLL */

/* -------------------------------------------------------------------- */
void xlithdg(RAWTBL *varp, void *input, NR_TYPE *np)
{
  int	label;
  Irs_blk  *p = (Irs_blk *)input;
  float  correction;

  if (varp->name[4] != '\0')
    correction = thdgrcorr;
  else
    correction = thdglcorr;


  /* (degs) 20 bits, +- 0.5 pirad, label 314  */

  for (size_t i = 0; i < varp->SampleRate; ++i)
    p->true_heading[i] = ntohl(p->true_heading[i]);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
    label = p->true_heading[i] & 0x000000ff;

    if (!RawData && label != 0xcc)
      {
      if (label)
        printf("irsHw: %s arinc label of %o\n", varp->name, label);

      if (i == 0)
        {
        for (size_t j = 1; j < varp->SampleRate && label != 0xcc; ++j)
          if ((label = p->true_heading[j] & 0x000000ff) == 0xcc)
            p->true_heading[i] = p->true_heading[j];
        }
      else
        p->true_heading[i] = p->true_heading[i-1];
      }

    np[i] = (double)(p->true_heading[i] >> 11) * arinc20bits + correction;

    if (np[i] < 0.0)
      np[i] += 360.0;

    if (np[i] > 360.0)
      np[i] -= 360.0;
    }

  varp->DynamicLag = ntohl(p->lag_25hz_frame);

}  /* END XLITHDG */

/* -------------------------------------------------------------------- */
void xlialt(RAWTBL *varp, void *input, NR_TYPE *np)
{
  int	label;
  Irs_blk  *p = (Irs_blk *)input;

  /* (degs) 20 bits, +- 0.5 pirad, label 361  */

  for (size_t i = 0; i < varp->SampleRate; ++i)
    p->inertial_alt[i] = ntohl(p->inertial_alt[i]);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
    label = p->inertial_alt[i] & 0x000000ff;

    if (!RawData && label != 0xf1)
      {
      if (label)
        printf("irsHw: %s arinc label of %o\n", varp->name, label);

      if (i == 0)
        {
        for (size_t j = 1; j < varp->SampleRate && label != 0xf1; ++j)
          if ((label = p->inertial_alt[j] & 0x000000ff) == 0xf1)
            p->inertial_alt[i] = p->inertial_alt[j];
        }
      else
        p->inertial_alt[i] = p->inertial_alt[i-1];
      }

    np[i] = (double)(p->inertial_alt[i] >> 11)* 0.125 * FTMTR;
    }

  varp->DynamicLag = ntohl(p->lag_25hz_frame);

}  /* END XLIALT */

/* -------------------------------------------------------------------- */
void xlivspd(RAWTBL *varp, void *input, NR_TYPE *np)
{
  int	label;
  Irs_blk  *p = (Irs_blk *)input;

  /* (degs) 20 bits, +- 0.5 pirad, label 365  */

  for (size_t i = 0; i < varp->SampleRate; ++i)
    p->inrt_vert_speed[i] = ntohl(p->inrt_vert_speed[i]);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
    label = p->inrt_vert_speed[i] & 0x000000ff;

    if (!RawData && label != 0xf5)
      {
      if (label)
        printf("irsHw: %s arinc label of %o\n", varp->name, label);

      if (i == 0)
        {
        for (size_t j = 1; j < varp->SampleRate && label != 0xf5; ++j)
          if ((label = p->inrt_vert_speed[j] & 0x000000ff) == 0xf5)
            p->inrt_vert_speed[i] = p->inrt_vert_speed[j];
        }
      else
        p->inrt_vert_speed[i] = p->inrt_vert_speed[i-1];
      }

    np[i] = (double)(p->inrt_vert_speed[i] >> 11) * 0.03125 * FTMIN;
    }

  varp->DynamicLag = ntohl(p->lag_25hz_frame);

}  /* END XLIVSPD */

/* -------------------------------------------------------------------- */
void xlvacc(RAWTBL *varp, void *input, NR_TYPE *np)
{
  int	label;
  Irs_blk  *p = (Irs_blk *)input;

  /* (G) 15 bits, +- 4.0 kts, label 364  */

  for (size_t i = 0; i < varp->SampleRate; ++i)
    p->vertical_accel[i] = ntohl(p->vertical_accel[i]);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
    label = p->vertical_accel[i] & 0x000000ff;

    if (!RawData && label != 0xf4)
      {
      if (label)
        printf("irsHw: %s arinc label of %o\n", varp->name, label);

      if (i == 0)
        {
        for (size_t j = 1; j < varp->SampleRate && label != 0xf4; ++j)
          if ((label = p->vertical_accel[j] & 0x000000ff) == 0xf4)
            p->vertical_accel[i] = p->vertical_accel[j];
        }
      else
        p->vertical_accel[i] = p->vertical_accel[i-1];
      }

    np[i] = (double)(p->vertical_accel[i] >> 16) * 1.22e-04 * MPS2;
    }

  varp->DynamicLag = ntohl(p->lag_50hz_frame);

}  /* END XLVACC */

/* -------------------------------------------------------------------- */
void xlivew(RAWTBL *varp, void *input, NR_TYPE *np)
{
  int	label;
  Irs_blk  *p = (Irs_blk *)input;

  /* 20 bits, +-4095 kts, label 367  */

  for (size_t i = 0; i < varp->SampleRate; ++i)
    p->velocity_ew[i] = ntohl(p->velocity_ew[i]);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
    label = p->velocity_ew[i] & 0x000000ff;

    if (!RawData && label != 0xf7)
      {
      if (label)
        printf("irsHw: %s arinc label of %o\n", varp->name, label);

      if (i == 0)
        {
        for (size_t j = 1; j < varp->SampleRate && label != 0xf7; ++j)
          if ((label = p->velocity_ew[j] & 0x000000ff) == 0xf7)
            p->velocity_ew[i] = p->velocity_ew[j];
        }
      else
        p->velocity_ew[i] = p->velocity_ew[i-1];
      }

    np[i] = (double)(p->velocity_ew[i] >> 11) * 0.00390625 * KTS2MS;
    }

  varp->DynamicLag = ntohl(p->lag_10hz_frame);

} /* END XLIVEW */

/* -------------------------------------------------------------------- */
void xlivns(RAWTBL *varp, void *input, NR_TYPE *np)
{
  int	label;
  Irs_blk  *p = (Irs_blk *)input;

  /* 20 bits, +-4095 kts, label 366  */

  for (size_t i = 0; i < varp->SampleRate; ++i)
    p->velocity_ns[i] = ntohl(p->velocity_ns[i]);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
    label = p->velocity_ns[i] & 0x000000ff;

    if (!RawData && label != 0xf6)
      {
      if (label)
        printf("irsHw: %s arinc label of %o\n", varp->name, label);

      if (i == 0)
        {
        for (size_t j = 1; j < varp->SampleRate && label != 0xf6; ++j)
          if ((label = p->velocity_ns[j] & 0x000000ff) == 0xf6)
            p->velocity_ns[i] = p->velocity_ns[j];
        }
      else
        p->velocity_ns[i] = p->velocity_ns[i-1];
      }

    np[i] = (double)(p->velocity_ns[i] >> 11) * 0.00390625 * KTS2MS;
    }

  varp->DynamicLag = ntohl(p->lag_10hz_frame);

} /* END XLIVNS */

/* -------------------------------------------------------------------- */
void xliwd(RAWTBL *varp, void *input, NR_TYPE *np)
{
  int	label;
  Irs_blk  *p = (Irs_blk *)input;

  /* 20 bits, +-4095 kts, label 366  */

  for (size_t i = 0; i < varp->SampleRate; ++i)
    p->wind_dir_true[i] = ntohl(p->wind_dir_true[i]);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
    label = p->wind_dir_true[i] & 0x000000ff;

    if (!RawData && label != 0xce)
      {
      if (label)
        printf("irsHw: %s arinc label of %o\n", varp->name, label);

      if (i == 0)
        {
        for (size_t j = 1; j < varp->SampleRate && label != 0xce; ++j)
          if ((label = p->wind_dir_true[j] & 0x000000ff) == 0xce)
            p->wind_dir_true[i] = p->wind_dir_true[j];
        }
      else
        p->wind_dir_true[i] = p->wind_dir_true[i-1];
      }

    np[i] = (double)(p->wind_dir_true[i] >> 11) * arinc20bits;
 
    if (np[i] < 0.0)
      np[i] += 360.0;
 
    if (np[i] > 360.0)
      np[i] -= 360.0;
    }

  varp->DynamicLag = ntohl(p->lag_10hz_frame);

} /* END XLIWD */

/* -------------------------------------------------------------------- */
void xliws(RAWTBL *varp, void *input, NR_TYPE *np)
{
  int	label;
  Irs_blk  *p = (Irs_blk *)input;

  /* 20 bits, +-4095 kts, label 366  */

  for (size_t i = 0; i < varp->SampleRate; ++i)
    p->wind_speed[i] = ntohl(p->wind_speed[i]);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
    label = p->wind_speed[i] & 0x000000ff;

    if (!RawData && label != 0xcd)
      {
      if (label)
        printf("irsHw: %s arinc label of %o\n", varp->name, label);

      if (i == 0)
        {
        for (size_t j = 1; j < varp->SampleRate && label != 0xcd; ++j)
          if ((label = p->wind_speed[j] & 0x000000ff) == 0xcd)
            p->wind_speed[i] = p->wind_speed[j];
        }
      else
        p->wind_speed[i] = p->wind_speed[i-1];
      }

    np[i] = (double)(p->wind_speed[i] >> 10) * 1.22e-04 * KTS2MS;
    }

  varp->DynamicLag = ntohl(p->lag_10hz_frame);

} /* END XLIWS */

/* -------------------------------------------------------------------- */
void xligspd(RAWTBL *varp, void *input, NR_TYPE *np)
{
  int	label;
  Irs_blk  *p = (Irs_blk *)input;

  /* 20 bits, 0 to 4095 kts, label 312  */

  for (size_t i = 0; i < varp->SampleRate; ++i)
    p->ground_speed[i] = ntohl(p->ground_speed[i]);

  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
    label = p->ground_speed[i] & 0x000000ff;

    if (!RawData && label != 0xca)
      {
      if (label)
        printf("irsHw: %s arinc label of %o\n", varp->name, label);

      if (i == 0)
        {
        for (size_t j = 1; j < varp->SampleRate && label != 0xca; ++j)
          if ((label = p->ground_speed[j] & 0x000000ff) == 0xca)
            p->ground_speed[i] = p->ground_speed[j];
        }
      else
        p->ground_speed[i] = p->ground_speed[i-1];
      }

    np[i] = (double)(p->ground_speed[i] >> 11) * 0.00390625 * KTS2MS;
    }

  varp->DynamicLag = ntohl(p->lag_10hz_frame);

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
  int	label;
  Irs_blk	*p = (Irs_blk *)input;

  /* (degs) 20 bits, +- 0.5 pirad, label 321  */

  for (size_t i = 0; i < varp->SampleRate; ++i)
    p->drift_angle[i] = ntohl(p->drift_angle[i]);
 
  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
    label = p->drift_angle[i] & 0x000000ff;
 
    if (!RawData && label != 0xd1)
      {
      if (label)
        printf("irsHw: %s arinc label of %o\n", varp->name, label);
 
      if (i == 0)
        {
        for (size_t j = 1; j < varp->SampleRate && label != 0xd1; ++j)
          if ((label = p->drift_angle[j] & 0x000000ff) == 0xd1)
            p->drift_angle[i] = p->drift_angle[j];
        }
      else
        p->drift_angle[i] = p->drift_angle[i-1];
      }
 
    np[i] = (double)(p->drift_angle[i] >> 11) * arinc20bits;
    }

  varp->DynamicLag = ntohl(p->lag_25hz_frame);

}  /* END XLIDRIFT */

/* END IRSHW.C */
