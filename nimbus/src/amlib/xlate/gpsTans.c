/*
-------------------------------------------------------------------------
OBJECT NAME:	gpsTans.c

FULL NAME:	Translate Trimble GPS block

ENTRY POINTS:	xlgstat(), xlglon(), xlglat(), xlgalt(), xlgtimp(),
		xlgvew(), xlgvns(), xlgvzi(), xlgtimv, xlgmode()

DESCRIPTION:	

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	rec_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "amlib.h"

static int	velLag, posLag;

extern char *ADSrecord;
int calcDateTime(int week, int secs, int gps_off);
int computeJulianDay(int year, int month, int day);
void decodeJulianDay(int julian_day, int year, int *month, int *day);


/* -------------------------------------------------------------------- */
void xlgstat(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = (NR_TYPE)ntohf(((Gps_blk *)input)->ghealth);

  if (*output != 0.0)
    printf("gpsTans: GSTAT = %d\n", (int)*output);

/*
  Hdr_blk *hdr = (Hdr_blk *)ADSrecord;
printf("%02d:%02d:%02d\n",
	ntohs(hdr->hour), ntohs(hdr->minute), ntohs(hdr->second));

printf("week=%d, secs=%f, pos_time=%f, vel_time=%f, bias=%f, biasrt=%f\n",
	ntohl(((Gps_blk *)input)->gpsweek),
	((Gps_blk *)input)->gpstime, ((Gps_blk *)input)->postime,
	((Gps_blk *)input)->veltime,
	((Gps_blk *)input)->bias, ((Gps_blk *)input)->biasrt);
*/
calcDateTime(	(int)((Gps_blk *)input)->gpsweek,
		(int)((Gps_blk *)input)->gpstime,
		(int)((Gps_blk *)input)->gpsutc);

}	/* END XLGSTAT */

/* -------------------------------------------------------------------- */
void xlglat(RAWTBL *varp, void *input, NR_TYPE *output)
{
  float	lat, lon;

  lat = (NR_TYPE)ntohf(((Gps_blk *)input)->glat) * RAD_DEG;
  lon = (NR_TYPE)ntohf(((Gps_blk *)input)->glon) * RAD_DEG;

  if (lat == 0.0 && lon == 0.0)
    *output = floatNAN;
  else
    *output = lat;

  varp->DynamicLag = posLag;
}

/* -------------------------------------------------------------------- */
void xlglon(RAWTBL *varp, void *input, NR_TYPE *output)
{
  float	lat, lon;

  lat = (NR_TYPE)ntohf(((Gps_blk *)input)->glat) * RAD_DEG;
  lon = (NR_TYPE)ntohf(((Gps_blk *)input)->glon) * RAD_DEG;

  if (lat == 0.0 && lon == 0.0)
    *output = floatNAN;
  else
    *output = lon;

  varp->DynamicLag = posLag;
}

/* -------------------------------------------------------------------- */
void xlgalt(RAWTBL *varp, void *input, NR_TYPE *output)
{
  float		posTime;
  Gps_blk	*gps = (Gps_blk *)input;

  posTime = ntohf(gps->postime) - (int)ntohf(gps->postime);
  posLag = (int)((posTime - 0.5) * 1000);

  *output = (NR_TYPE)ntohf(gps->galt);

  varp->DynamicLag = posLag;
}

/* -------------------------------------------------------------------- */
void xlgtimp(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = (NR_TYPE)ntohf(((Gps_blk *)input)->postime);
}

/* -------------------------------------------------------------------- */
void xlgvew(RAWTBL *varp, void *input, NR_TYPE *output)
{
  float	velTime;
  Gps_blk	*gps = (Gps_blk *)input;

  velTime = ntohf(gps->postime) - (int)ntohf(gps->postime);
  velLag = (int)((velTime - 0.5) * 1000);

  *output = (NR_TYPE)ntohf(gps->veleast);

  varp->DynamicLag = velLag;
}

/* -------------------------------------------------------------------- */
void xlgvns(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = (NR_TYPE)ntohf(((Gps_blk *)input)->velnrth);

  varp->DynamicLag = velLag;
}

/* -------------------------------------------------------------------- */
void xlgvzi(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = (NR_TYPE)ntohf(((Gps_blk *)input)->velup);

  varp->DynamicLag = velLag;
}

/* -------------------------------------------------------------------- */
void xlgtimv(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = (NR_TYPE)ntohf(((Gps_blk *)input)->veltime);
}

/* -------------------------------------------------------------------- */
void xlgmode(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = (NR_TYPE)ntohl(((Gps_blk *)input)->gpsmode);
/*
  if (*output != 4.0)
    printf("gpsTans: GMODE = %d\n", (int)*output);
*/
}


/* -------------------------------------------------------------------- */
// Time conversion defines.
#define SECS_PER_DAY            86400
#define SECS_PER_HOUR           3600
#define SECS_PER_MIN            60
#define DAYS_PER_YEAR           365
#define DAYS_PER_LEAP_YEAR      366
#define DAYS_PER_WEEK           7
#define CALENDAR_SIZE           13
#define LEAP_YEAR_MOD           4
#define GPS3_BASE_YEAR          80

int calcDateTime(int week, int secs, int gps_off)
{
  int tot_days;                                 // total days since 1/1/1980
  int julian_day;                               // current julian day
  int *cal_ptr;                                 // calendar pointer
  int hour, minute, second, year, month, day;
//  Hdr_blk *hdr = (Hdr_blk *)ADSrecord;

  static int calendar[] =
         {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
  static int leap_calendar[] =
         {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366};
 
// Apply the gps/utc offset. See the Trimble manual.
  secs -= gps_off;
  if (secs < 0)
    return(0);                          // time is not yet valid
 
// Compute the day, hour, minute, and second of the current week.
  day = secs / SECS_PER_DAY;
  hour = (secs % SECS_PER_DAY) / SECS_PER_HOUR;
  minute = (secs % SECS_PER_HOUR) / SECS_PER_MIN;
  second = (secs % SECS_PER_MIN);
 
// GPS time begins on 1/6/80.  Compute the total number of days.  Then
// compute the year and julian day.
  tot_days = week * DAYS_PER_WEEK + day + 5;
  year = tot_days / DAYS_PER_YEAR;
  julian_day = tot_days - (year * DAYS_PER_YEAR) + 1;

month = 0;
 
// Compute number of previous leap days, and correct the julian day.
  julian_day -= (year - 1) / LEAP_YEAR_MOD + 1;
 
// If julian day is <= 0, roll back to the previous year, and correct the
// julian day.
  if (julian_day <= 0) {
    year--;
    if (year % LEAP_YEAR_MOD)
      julian_day += DAYS_PER_YEAR;
    else
      julian_day += DAYS_PER_LEAP_YEAR;
  }
 
  if (year % LEAP_YEAR_MOD)
    cal_ptr = calendar;
  else
    cal_ptr = leap_calendar;
 
// Compute the month index.
  for (month = 1; (month < CALENDAR_SIZE) &&
      (julian_day > *(cal_ptr + month)); month++)
    ;
 
// Compute the day of the month for the start of the week.
  day = julian_day - *(cal_ptr + month - 1);
 
// Add the base year, 1980, to the computed year.
  year += GPS3_BASE_YEAR;
/*
printf ("julian day = %3d, %02d/%02d/%02d %02d:%02d:%02d GPS\n", julian_day,
     year, month, day, hour, minute, second);

printf ("julian day = %3d, %02d/%02d/%02d %02d:%02d:%02d Hdr_blk\n", 
     computeJulianDay(year, month, day),
     hdr->year, hdr->month, hdr->day, hdr->hour, hdr->minute, hdr->second);
*/
  return(1);

}

int computeJulianDay(int year, int month, int day)
// Computes the julian day from the current year, month, day.
{
  int *cal_ptr;                                 // calendar pointer
 
  static int calendar[] =
         {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
  static int leap_calendar[] =
         {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366};
 
  if (year % 4)
    cal_ptr = calendar;
  else
    cal_ptr = leap_calendar;
 
  return(*(cal_ptr + month - 1) + day);
}


void decodeJulianDay(int julian_day, int year, int *month, int *day)
{
  int *cal_ptr;                                 // calendar pointer
 
  static int calendar[] =
         {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
  static int leap_calendar[] =
         {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366};
 
  if (year % 4)
    cal_ptr = calendar;
  else
    cal_ptr = leap_calendar;
 
// Find the month.
  for (*month = 1; (*month < 13) &&
       (julian_day > *(cal_ptr + *month));
       (*month)++);
 
  *day = julian_day - *(cal_ptr + *month - 1);
}

/* END GPSTANS.C */
