/* gcc -I/home/local/include ncfixhdr.c -o ncfixhdr -lnetcdf
 */

#include <netcdf.h>
#include <time.h>

main(argc, argv)
int	argc;
char	*argv[];
{
	int		fd, id, BaseTime;
	char		buffer[128];
	struct tm	StartFlight;

	fd = ncopen(argv[1], NC_WRITE);

	ncattget(fd, NC_GLOBAL, "FlightDate", (void *)buffer);
	sscanf(buffer, "%d/%d/%d",	&StartFlight.tm_mon,
					&StartFlight.tm_mday,
					&StartFlight.tm_year);

	ncattget(fd, NC_GLOBAL, "TimeInterval", (void *)buffer);

	sscanf(buffer, "%02d:%02d:%02d",&StartFlight.tm_hour,
					&StartFlight.tm_min,
					&StartFlight.tm_sec);

	StartFlight.tm_mon--;
	StartFlight.tm_year -= 1900;
	BaseTime = timegm(&StartFlight);

	printf ("New date/time is %s", asctime(gmtime(&BaseTime)));

	ncvarput1(fd, 0, 0, (void *)&BaseTime);

	ncclose(fd);

}
