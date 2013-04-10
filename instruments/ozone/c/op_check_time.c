#include <time.h>
#include <sys/time.h>

extern float getTimeStamp ( time_t *t );
extern char * getDate ( time_t *t );

int main ( void )
{
	int i;
	time_t t;
	
	for ( i=0; i<5; i++ )
	{
		printf("SSM: %.3f; ", getTimeStamp( &t ));
		
		printf("Date: %s\n", getDate( &t ) );
		sleep ( 2 );
	}
	return 0;
}
