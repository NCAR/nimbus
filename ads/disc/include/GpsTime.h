/* Gpstime.h
   Computes year month, day, hour, minute, and second from the gps time format.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef GPSTIME_H
#define GPSTIME_H

#define SECS_PER_DAY      86400
#define SECS_PER_HOUR      3600
#define SECS_PER_MIN         60
#define DAYS_PER_YEAR       365
#define DAYS_PER_LEAP_YEAR  366
#define DAYS_PER_WEEK         7
#define CALENDAR_SIZE        13
#define LEAP_YEAR_MOD         4
#define GPS_BASE_YEAR        80

class GpsTime {
public:
  GpsTime();					// constructor
  void setTime (int week, int second);		// initially set the date/time
  int year() 	{return gyear;}
  int month()	{return gmonth;}
  int day() 	{return gday;}
  int hour() 	{return ghour;}
  int minute() 	{return gminute;}
  int second() 	{return gsecond;}

private:
  int gyear;					// current year
  int gmonth;					// current month
  int gday;					// current day
  int ghour;					// current hour
  int gminute;					// current minute
  int gsecond;					// current second
};

#endif
