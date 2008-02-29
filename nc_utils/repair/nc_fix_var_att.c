/* gcc -I/jnet/local/include nc_fix_var_att.c -o nc_fix_var_att -L/jnet/local/lib -lnetcdf

 * This program should be custom tailored.
 */

#include <string.h>
#include <netcdf.h>

int main(int argc, char *argv[])
{
  int	fd, id;
  char *att = "seconds since 2005-01-07 13:55:00 ";

  fd = ncopen(argv[1], NC_WRITE);
  ncredef(fd);

  nc_inq_varid(fd, "Time", &id);
  nc_put_att_text(fd, id, "units", strlen(att)+1, att);

  ncendef(fd);
  ncclose(fd);

  return 0;
}
