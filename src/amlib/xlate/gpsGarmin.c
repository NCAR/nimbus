/*
-------------------------------------------------------------------------
OBJECT NAME:	gpsGarmin.c

FULL NAME:	Translate Garmin WAAS GPS block

ENTRY POINTS:	xlggstat(), xlgglon(), xlgglat(), xlggalt(),
		xlggspd(), xlggtrk(), xlggnsat()

DESCRIPTION:	

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	rec_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 2003
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "amlib.h"

static int	velLag = 0, posLag = 0;

extern char *ADSrecord;

static int calcDateTime(int week, int secs, int gps_off);
int computeJulianDay(int year, int month, int day);
void decodeJulianDay(int julian_day, int year, int *month, int *day);


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
    *output = MISSING_VALUE;
  else
    *output = lat;

  varp->DynamicLag = posLag;

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
    *output = MISSING_VALUE;
  else
    *output = lon;

  varp->DynamicLag = posLag;

}

/* -------------------------------------------------------------------- */
void xlggalt(RAWTBL *varp, void *input, NR_TYPE *output)
{
  float		posTime;
  Hdr_blk	*h = (Hdr_blk *)ADSrecord;
  Garmin_blk	*gps = (Garmin_blk *)input;

//  posTime = ntohf(gps->postime) - (int)ntohf(gps->postime);
//  posLag = (int)((posTime - 0.5) * 1000);
//printf("ADS = %02d:%02d:%02d  GGPS = %s\n", ntohs(h->hour), ntohs(h->minute), ntohs(h->second), gps->utctime);
  *output = (NR_TYPE)ntohf(gps->height);

  varp->DynamicLag = posLag;

}

/* -------------------------------------------------------------------- */
void xlggspd(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = (NR_TYPE)ntohf(((Garmin_blk *)input)->ground_speed) * KTS2MS;

  varp->DynamicLag = velLag;
}

/* -------------------------------------------------------------------- */
void xlggtrk(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = (NR_TYPE)ntohf(((Garmin_blk *)input)->course);

  varp->DynamicLag = velLag;
}

/* -------------------------------------------------------------------- */
void xlggnsat(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = (NR_TYPE)ntohs(((Garmin_blk *)input)->nsat);

}

/* END GPSGARMIN.C */
