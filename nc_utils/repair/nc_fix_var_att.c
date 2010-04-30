/* gcc -I/usr/include/netcdf nc_fix_var_att.c -o nc_fix_var_att -lnetcdf
 *
 * This is a skeleton program to be custom tailored.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netcdf.h>

int main(int argc, char *argv[])
{
  int	fd, id;
  char *att = "seconds since 2005-01-07 13:55:00 ";

  if (nc_open(argv[1], NC_WRITE, &fd) != NC_NOERR)
  {
    fprintf(stderr, "Can't open [%s]\n", argv[1]);
    exit(1);
  }

  nc_redef(fd);

  nc_inq_varid(fd, "Time", &id);
  nc_put_att_text(fd, id, "units", strlen(att)+1, att);

  nc_enddef(fd);
  nc_close(fd);

  return 0;
}
