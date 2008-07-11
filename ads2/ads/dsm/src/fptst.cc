#include <vxWorks.h>
#include <stdio.h>

void fptst()
{
  float j, k;

  k = 2.0;
  j = k * 3.0;
  printf ("k = %f, k = %f\n", k, j);
}
