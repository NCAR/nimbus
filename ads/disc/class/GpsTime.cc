/* GpsTime.cc
   Computes year month, day, hour, minute, and second from the gps time format.

   Original Author: Jerry V. Pelk 
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <GpsTime.h>

/******************************************************************************
** Public Functions
******************************************************************************/

GpsTime::GpsTime ()

// Constructor.
{
}
/******************************************************************************/
 
void GpsTime::setTime (int week, int second)
 
// Compute the date and time from the week and second numbers received from
// the GPS receiver.  The second must have been previously corrected by the
// gps second offset to be fully accurate.
{
  int tot_days;                                 // total days since 1/1/1980
  int julian_day;                               // current julian day
  int *cal_ptr;                                 // calendar pointer
  static int calendar[CALENDAR_SIZE] =
         {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
  static int leap_calendar[CALENDAR_SIZE] =
         {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366};
 
// Compute the day, hour, minute, and second of the current week.
  gday = second / SECS_PER_DAY;
  ghour = (second % SECS_PER_DAY) / SECS_PER_HOUR;
  gminute = (second % SECS_PER_HOUR) / SECS_PER_MIN;
  gsecond = (second % SECS_PER_MIN);
 
// GPS time begins on 1/6/80.  Compute the total number of days.  Then
// compute the year and julian day.
  tot_days = week * DAYS_PER_WEEK + gday + 5;
  gyear = tot_days / DAYS_PER_YEAR;
  julian_day = tot_days - (gyear * DAYS_PER_YEAR) + 1;
 
// Compute number of previous leap days, and correct the julian day.
  julian_day -= (gyear - 1) / LEAP_YEAR_MOD + 1;
 
// If julian day is <= 0, roll back to the previous year, and correct the
// julian day.
  if (julian_day <= 0) {
    gyear--;
    if (gyear % LEAP_YEAR_MOD)
      julian_day += DAYS_PER_YEAR;
    else
      julian_day += DAYS_PER_LEAP_YEAR;
  }
 
  if (gyear % LEAP_YEAR_MOD)
    cal_ptr = calendar;
  else
    cal_ptr = leap_calendar;

// Compute the month index.
  for (gmonth = 1; (gmonth < CALENDAR_SIZE) &&
       (julian_day > *(cal_ptr + gmonth));
       gmonth++);
 
// Compute the day of the month for the start of the week.
  gday = julian_day - *(cal_ptr + gmonth - 1);
 
// Add the base year, 1980, to the computed year.
  gyear += GPS_BASE_YEAR;
 
/**
  printf ("julian day = %d,  %d %d %d %d %d %d\n", julian_day,
     gyear, gmonth, gday, ghour, gminute, gsecond);
**/
}
/******************************************************************************
** Private Functions
******************************************************************************/
 
/******************************************************************************/
