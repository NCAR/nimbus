#include <stdio.h>
#include "pms.h"

main()
{
	char	*p;

	InitPMSspecs("/jnet/shared/dev/libpms/PMSspecs.v3");

	if ((p = GetPMSparameter("PCAS108", "CELL_SIZE")) == NULL)
		{
		printf("Not found.\n");
		exit(1);
		}

	printf("%s\n", p);

	ReleasePMSspecs();

}
