#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>

/* ************************************
Function for time stamping data records.
It will provide a time stamp in the form of both human readable date and time and seconds since midnight in UTC. */
float getTimeStamp ( time_t *t )
{
	struct timeval curr_time;
	float SecSinceMidnight;
	static float StartTime = 0; // Record the start time stamp to keep track of overnigh roll-over.
	static short isFirstRun = 1;
	
	gettimeofday( &curr_time, NULL ); // Get time stamp to usec accuracy.
	*t = curr_time.tv_sec; // Return whole seconds since the Epoch in case they are needed to calc the date.
	SecSinceMidnight = curr_time.tv_sec % 86400 + 1E-6 * curr_time.tv_usec; // Create sub-second time stamp.
	
	if ( isFirstRun ) // Record start time.
	{
		StartTime = SecSinceMidnight;
		isFirstRun = 0;
	}
	
	if ( StartTime > SecSinceMidnight ) // If time rolled over midnight, add StartTime to time stamp to keep continuity.
		SecSinceMidnight += StartTime;
	
	return SecSinceMidnight;
}

/* ************************************
Function for time stamping data records.
It will provide a time stamp in the form of both human readable date and time and seconds since midnight in UTC. */
char * getDate ( time_t *t )
{
	struct tm *currDate;
	static char out[21];
	
	currDate = gmtime( t ); //localtime( &t ); // Convert t to date and time.
	strftime( out, 21, "%F %T", currDate ); // Present currDate in desired format.
	
	return out;
}
