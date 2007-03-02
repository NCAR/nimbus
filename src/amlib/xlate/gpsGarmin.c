/*
-------------------------------------------------------------------------
OBJECT NAME:	gpsGarmin.c

FULL NAME:	Translate Garmin WAAS GPS block

ENTRY POINTS:	xlggstat(), xlgglon(), xlgglat(), xlggalt(),
		xlggspd(), xlggtrk(), xlggnsat()

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 2003-05
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "amlib.h"

#include "GoogleEarth.h"

extern GoogleEarthKML * googleEarth;


/* -------------------------------------------------------------------- */
void xlggstat(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = (NR_TYPE)ntohs(((Garmin_blk *)input)->quality);
}

/* -------------------------------------------------------------------- */
void xlgglat(RAWTBL *varp, void *input, NR_TYPE *output)
{
  int	ilat;
  float	lat, lon;
  Garmin_blk *gp = (Garmin_blk *)input;

  lat = (NR_TYPE)ntohf(gp->glat);
  lon = (NR_TYPE)ntohf(gp->glon);
  ilat = (int)lat / 100;
  lat = (NR_TYPE)ilat + ((lat - (ilat * 100)) / 60.0);

  if (gp->north == 'S')
    lat = -lat;

  if (lat == 0.0 && lon == 0.0)
    *output = floatNAN;
  else
    *output = lat;

  if (googleEarth)
    googleEarth->SetLatestLAT(lat);
}

/* -------------------------------------------------------------------- */
void xlgglon(RAWTBL *varp, void *input, NR_TYPE *output)
{
  int	ilon;
  float	lat, lon;
  Garmin_blk *gp = (Garmin_blk *)input;

  lat = (NR_TYPE)ntohf(gp->glat);
  lon = (NR_TYPE)ntohf(gp->glon);
  ilon = (int)lon / 100;
  lon = (NR_TYPE)ilon + ((lon - (ilon * 100)) / 60.0);

  if (gp->east == 'W')
    lon = -lon;

  if (lat == 0.0 && lon == 0.0)
    *output = floatNAN;
  else
    *output = lon;

  if (googleEarth)
    googleEarth->SetLatestLON(lon);
}

/* -------------------------------------------------------------------- */
void xlggalt(RAWTBL *varp, void *input, NR_TYPE *output)
{
  Garmin_blk	*gps = (Garmin_blk *)input;

  *output = (NR_TYPE)ntohf(gps->height);

  if (googleEarth)
    googleEarth->SetLatestALT(*output);
}

/* -------------------------------------------------------------------- */
void xlggnsat(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = (NR_TYPE)ntohs(((Garmin_blk *)input)->nsat);
}

/* -------------------------------------------------------------------- */
static NR_TYPE	gspd, trk;

void xlggspd(RAWTBL *varp, void *input, NR_TYPE *output)
{
  gspd = *output = (NR_TYPE)ntohf(((Garmin_blk *)input)->ground_speed) * KTS2MS;
}

/* -------------------------------------------------------------------- */
void xlggtrk(RAWTBL *varp, void *input, NR_TYPE *output)
{
  trk = *output = (NR_TYPE)ntohf(((Garmin_blk *)input)->course);
}

/* -------------------------------------------------------------------- */
/* These next two should be derived, but the HRT FIR filtering of TRK
 * above causes headaches, so I moved them into raw.  This works as
 * long as variables are processed in alphabetical order.
 */
void xlggvew(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = (gspd * sin((double)(DEG_RAD*trk)));
}

/* -------------------------------------------------------------------- */
void xlggvns(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = (gspd * cos((double)(DEG_RAD*trk)));
}

/* END GPSGARMIN.C */
