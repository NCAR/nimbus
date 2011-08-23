#include <stdio.h>
#include "pms.h"

int main()
{
  char	*p;

  InitPMSspecs("/home/local/raf/libpms/PMSspecs.v3");

  if ((p = GetPMSparameter("PCAS108", "CELL_SIZE")) == NULL)
  {
    printf("Not found.\n");
    return 1;
  }

  printf("%s\n", p);

  ReleasePMSspecs();

  return 0;

}
