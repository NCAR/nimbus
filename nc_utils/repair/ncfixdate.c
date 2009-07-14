/* gcc -I/opt/local/include ncfixdate.c -o ncfixdate -L/opt/local/lib -lnetcdf
 */

#include <netcdf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const char *timeUnitsFormat = "seconds since %F %T %z";

int main(int argc, char *argv[])
{
  int		fd, id, edge[3];
  time_t	BaseTime;
  char		buffer[128];
  struct tm	StartFlight;

  putenv("TZ=UTC");     // Perform all time calculations at UTC.

  if (nc_open(argv[1], NC_WRITE, &fd) != NC_NOERR)
  {
    fprintf(stderr, "Can't open [%s]\n", argv[1]);
    exit(1);
  }

  nc_get_att_text(fd, NC_GLOBAL, "FlightDate", (void *)buffer);
  printf("Current FlightDate is [%s]\n", buffer);

  printf("Enter new date [e.g. 09/02/1994] : ");
  fgets(buffer, 128, stdin);
  buffer[strlen(buffer)-1] = '\0';


  nc_redef(fd);
  nc_put_att_text(fd, NC_GLOBAL, "FlightDate", strlen(buffer)+1,
						(void *)buffer);

  nc_enddef(fd);

  sscanf(buffer, "%d/%d/%d",	&StartFlight.tm_mon,
				&StartFlight.tm_mday,
				&StartFlight.tm_year);

  nc_get_att_text(fd, NC_GLOBAL, "TimeInterval", (void *)buffer);

  sscanf(buffer, "%02d:%02d:%02d",&StartFlight.tm_hour,
				&StartFlight.tm_min,
				&StartFlight.tm_sec);

  StartFlight.tm_mon--;
  StartFlight.tm_year -= 1900;
  BaseTime = timegm(&StartFlight);

  printf ("New date/time is %s", asctime(gmtime(&BaseTime)));

  edge[0] = edge[1] = edge[2] = 0;
  if (nc_inq_varid(fd, "base_time", &id) == NC_NOERR)
    nc_put_var1_int(fd, id, edge, (void *)&BaseTime);

  if (nc_inq_varid(fd, "time_offset", &id) == NC_NOERR)
  {
    strftime(buffer, 128, timeUnitsFormat, &StartFlight);
    nc_put_att_text(fd, id, "units", strlen(buffer)+1, (void *)buffer);
  }

  if (nc_inq_varid(fd, "Time", &id) == NC_NOERR)
  {
    strftime(buffer, 128, timeUnitsFormat, &StartFlight);
    nc_put_att_text(fd, id, "units", strlen(buffer)+1, (void *)buffer);
  }

  nc_close(fd);

  return 0;
}
