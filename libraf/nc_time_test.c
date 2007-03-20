/*
<pre class="CDL">
        int Time(Time) ;
                Time:long_name = "time of measurement" ;
                Time:standard_name = "time" ;
                Time:units = "seconds since 2001-09-20 14:38:33 +0000" ;
                Time:strptime_format = "seconds since %F %T %z" ;
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "/home/local/include/netcdf.h"


int GetFlightRecordNumber(int ncid, const char userTime[]);
time_t InitFlightTime(int ncid);

int main(int argc, char *argv[])
{
  int	ncid, varID, nDims, dimids[3];
  int	i, total, len;
  int	*tp;
  time_t  StartFlightUnixTime;

  if (argc < 2)
  {
    fprintf(stderr, "Usage: a.out file.nc\n");
    exit(1);
  }

  nc_open(argv[1], NC_NOWRITE, &ncid);

  StartFlightUnixTime = InitFlightTime(ncid);

  nc_inq_varid(ncid, "Time", &varID);
  nc_inq_varndims(ncid, varID, &nDims);
  nc_inq_vardimid(ncid, varID, dimids);

  total = 1;
  for (i = 0; i < nDims; ++i)
  {
     nc_inq_dimlen(ncid, dimids[i], &len);
     total *= len;
  }

  tp = malloc(total * sizeof(int));
  nc_get_var_int(ncid, varID, tp);

  for (i = 0; i < total; ++i)
  {
    printf("%5d %04d/%02d/%02d %02d:%02d:%02d\n", tp[i],
	GetFlightYear((int)tp[i]),
	GetFlightMonth((int)tp[i]),
	GetFlightDay((int)tp[i]),
	GetFlightHour((int)tp[i]),
	GetFlightMinute((int)tp[i]),
	GetFlightSecond((int)tp[i]));
  }

  printf("rec# = %d\n", GetFlightRecordNumber(ncid, "13:39:00"));
  printf("rec# = %d\n", GetFlightRecordNumber(ncid, "14:39:00"));
  printf("rec# = %d\n", GetFlightRecordNumber(ncid, "22:53:00"));
  printf("rec# = %d\n", GetFlightRecordNumber(ncid, "23:20:00"));

  nc_close(ncid);
}
