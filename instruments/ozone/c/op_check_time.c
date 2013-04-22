#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include "config_extern.h"

int main ( void )
{
	int i;
	time_t t;
	
	for ( i=0; i<5; i++ )
	{
		printf("SSM: %.3f; ", getTimeStamp( &t ));
		
		printf("Date: %s\n", getDate( &t, "%Y%m%d_%H%M%S" ) );
		sleep ( 2 );
	}
	return 0;
}
