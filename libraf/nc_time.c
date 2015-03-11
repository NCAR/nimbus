/*
<pre class="CDL">
        int Time(Time) ;
                Time:long_name = "time of measurement" ;
                Time:standard_name = "time" ;
                Time:units = "seconds since 2001-09-20 14:38:33 +0000" ;
                Time:strptime_format = "seconds since %F %T %z" ;
*/

#include <netcdf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/* Given an open netCDF file (ncid), return the Unix time (seconds since
 * 01/01/1970) of Time::units.  Subsequent values from 'Time' should be added
 * to this return value.
 */

static int _file_nc_version = 1;
static time_t _start_t = -1;
static int * _time_variable = 0;
static size_t _timeLength = 0;

time_t InitFlightTime(int ncid)
{
  int id;
  char version[32];

  putenv("TZ=UTC");	// Force all time routines to work in UTC.

  if (nc_get_att_text(ncid, NC_GLOBAL, "ConventionsVersion", version) == NC_NOERR)
    _file_nc_version = atoi(&version[2]);
  else
  if (nc_get_att_text(ncid, NC_GLOBAL, "Version", version) == NC_NOERR)
    _file_nc_version = atoi(&version[2]);
  else
    fprintf(stderr, "libraf: InitFlightTime: No global attribute Version.\nAre you sure this netCDF file meets conventions?\n");

  
  if (_file_nc_version >= 3 && nc_inq_varid(ncid, "Time", &id) == NC_NOERR)
  {
    struct tm StartFlight;
    char t_units[128], units_frmt[128];

    nc_get_att_text(ncid, id, "units", t_units);
    if (nc_get_att_text(ncid, id, "strptime_format", units_frmt) != NC_NOERR)
      strcpy(units_frmt, "seconds since %F %T %z");

    strptime(t_units, units_frmt, &StartFlight);

    _start_t = mktime(&StartFlight);
    if (_start_t <= 0)
    {
      fprintf(stderr, "libraf: InitFlightTime: problem decoding units from Time variable, fatal.\n");
      exit(1);
    }
  }

  if (_file_nc_version < 3 && nc_inq_varid(ncid, "base_time", &id) == NC_NOERR)
  {
    long l;
    size_t s = 0;
    nc_get_var1_long(ncid, id, &s, &l);
    _start_t = (time_t)l;
printf("base_time: %d\n", _start_t);
  }

  if (_start_t <= 0)
  {
    fprintf(stderr, "libraf: InitFlightTime: No 'Time' variable, this is fatal.\nnetCDF conventions version is %d\n", _file_nc_version);
    exit(1);
  }

  return _start_t;
}


static void read_time_variable(int ncid)
{
  int varid;

  if (_time_variable != 0)
    return;	// We've already been here.

  nc_inq_dimid(ncid, "Time", &varid);
  nc_inq_dimlen(ncid, varid, &_timeLength);

  if (_file_nc_version >= 3 && nc_inq_varid(ncid, "Time", &varid) == NC_NOERR)
  {
    _time_variable = malloc(sizeof(int) * _timeLength);
    nc_get_var_int(ncid, varid, _time_variable);
  }

  if (_file_nc_version < 2 && nc_inq_varid(ncid, "time_offset", &varid) == NC_NOERR)
  {
    _time_variable = malloc(sizeof(int) * _timeLength);
    nc_get_var_int(ncid, varid, _time_variable);
  }

  if (_time_variable == 0)
  {
    fprintf(stderr, "libraf: read_time_variable: No 'Time' variable to read, this is fatal.\n");
    exit(1);
  }
}

/* Given a time, hh:mm:ss, return the record number in the netCDF file.
 */
int GetFlightRecordNumber(int ncid, const char userTime[])
{
  struct tm * desiredTime;
  time_t desired_t;
  int idx = 0, h, m, s;
  int offset = 0;

  if (_start_t == -1)
    InitFlightTime(ncid);

  if (_time_variable == 0)
    read_time_variable(ncid);

  desiredTime = gmtime(&_start_t);

  sscanf(userTime, "%02d:%02d:%02d", &h, &m, &s);

  /* Since we deal with time only (no date), typical request that cross midnight
   * will come be e.g. 22:00:00 - 27:00:00.
   */
  if (h > 23)
  {
    h -= 23;
    offset = 86400;
  }

  desiredTime->tm_hour = h;
  desiredTime->tm_min = m;
  desiredTime->tm_sec = s;

  desired_t = mktime(desiredTime) + offset;
  if (desired_t < _start_t)
  {
    desired_t = _start_t;
  }
  else
  {
    int i;
    for (i = 0; i < _timeLength; ++i)
      if (_start_t + _time_variable[i] == desired_t)
        break;

    idx = i;
    if (i >= _timeLength)
      idx = _timeLength - 1;
  }

  return idx;
}


int GetFlightYear(int currentTimeOffset)
{
  time_t x = _start_t + currentTimeOffset;
  struct tm * stm = gmtime(&x);
  return stm->tm_year + 1900;
}

int GetFlightMonth(int currentTimeOffset)
{
  time_t x = _start_t + currentTimeOffset;
  struct tm * stm = gmtime(&x);
  return stm->tm_mon + 1;
}

int GetFlightDay(int currentTimeOffset)
{
  time_t x = _start_t + currentTimeOffset;
  struct tm * stm = gmtime(&x);
  return stm->tm_mday;
}

int GetFlightHour(int currentTimeOffset)
{
  time_t x = _start_t + currentTimeOffset;
  struct tm * stm = gmtime(&x);
  return stm->tm_hour;
}

int GetFlightMinute(int currentTimeOffset)
{
  time_t x = _start_t + currentTimeOffset;
  struct tm * stm = gmtime(&x);
  return stm->tm_min;
}

int GetFlightSecond(int currentTimeOffset)
{
  time_t x = _start_t + currentTimeOffset;
  struct tm * stm = gmtime(&x);
  return stm->tm_sec;
}
