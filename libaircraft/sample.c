#include <stdio.h>
#include "ac.h"

main()
{
	int		i;
	char	*p, *acList[12];

	InitAircraftSpecs("AircraftSpecs");


	GetAircraftList(acList);

	for (i = 0; acList[i]; ++i)
		printf("%s\n", acList[i]);


	if ((p = GetAircraftParameter("N308D", "BOOM_LEN")) == NULL)
		{
		printf("Not found.\n");
		exit(1);
		}

	printf("%s\n", p);

	ReleaseAircraftSpecs();

}
