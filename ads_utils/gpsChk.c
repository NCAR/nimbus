#include "ctape.h"

char	buffer[32000];
int	year, month, day, hour, minute, second;

int	calcDateTime(int week, int secs, int gps_off);


/* -------------------------------------------------------------------- */
main(int argc, char *argv[])
{
	int	infd, firstRecord, timesMatch = TRUE, print = FALSE;
	long	start, lrLen;
	char	*vn, *item_type;
	Hdr_blk *hdr;
	Gps_blk	*gpsDat;

	if ((infd = InitFlightHeader(argv[1], LEAVE_OPEN)) == ERR)
		return(1);

	get_lrlen(&lrLen);

	for (vn = GetFirst(); vn; vn = GetNext())
	{
		GetItemType(vn, &item_type);

		if (strcmp(item_type, GPS_TANS3_STR))
			continue;

		GetStart(vn, &start);
		break;
	}

	hdr = (Hdr_blk *)buffer;
	gpsDat = (Gps_blk *)&buffer[start];

	while (read(infd, buffer, 2) == 2)
	{
		if (*((unsigned short *)buffer) != 0x8681)
			continue;

		if (read(infd, &buffer[2], lrLen - 2) != lrLen -2)
			break;

		calcDateTime(	(int)gpsDat->gpsweek,
				(int)gpsDat->gpstime,
				(int)gpsDat->gpsutc);

		if (firstRecord)
		{
			if (hdr->year != year || hdr->month != month || hdr->day != day)
				{
				printf("*** Date doesn't match. ***\n");
				print = TRUE;
				}
			firstRecord = FALSE;
		}

		if (hdr->hour == hour && hdr->minute == minute && hdr->sec == second)
		{
			if (!timesMatch)
			{
				printf("Time synchronized\n");
				print = TRUE;
			}

			timesMatch = TRUE;
		}
		else
		{
			if (timesMatch)
			{
				printf("Times do not match.\n");
				print = TRUE;
			}

			timesMatch = FALSE;
		}

		if (print)
		{
			printf("  ADS stamp = %02d/%02d/%02d %02d:%02d:%02d\n",
				hdr->year, hdr->month, hdr->day,
				hdr->hour, hdr->minute, hdr->sec);

			printf("  GPS stamp = %02d/%02d/%02d %02d:%02d:%02d - week=%d, secs = %f, health=%d\n",
				year, month, day, hour, minute, second,
				gpsDat->gpsweek, gpsDat->gpstime, gpsDat->ghealth);
		}

		print = FALSE;
	}

	close(infd);
	return(0);

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
       (julian_day > *(cal_ptr + month));
       month++);
 
// Compute the day of the month for the start of the week.
  day = julian_day - *(cal_ptr + month - 1);
 
// Add the base year, 1980, to the computed year.
  year += GPS3_BASE_YEAR;
 
  return(1);
 
}

