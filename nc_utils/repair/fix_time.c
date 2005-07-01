/* gcc -I/jnet/local/include fix_time.c -o fix_time -L/jnet/local/lib -lnetcdf
 */

#include <netcdf.h>

main(int argc, char *argv[])
{
  int	fd, id, rid, i, rDims[5];
  long  zero = 0, nRecs;
  long	*my_time;
  char buffer[64];

  fd = ncopen(argv[1], NC_WRITE);
  id = ncvarid(fd, "Time");
  ncvarinq(fd, id, 0, 0, 0, rDims, 0);
  ncdiminq(fd, rDims[0], 0, &nRecs);

  my_time = malloc(nRecs * 4);

  for (i = 0; i < nRecs; ++i)
  {
    my_time[i] = i;
  }

  ncvarput(fd, id, &zero, &nRecs, (void *)my_time);

  ncclose(fd);

}
