
#ifndef _timeseg_h_
#define _timeseg_h_

#include <time.h>

#define GAP_FOUND	true	/* For CheckForTimeGap()	*/

/* Used for defining time segments that start/end at one end of the tape */
#define BEG_OF_TAPE	(-1)
#define END_OF_TAPE	(-1)

void GetUserTimeIntervals();
int NextTimeInterval(time_t *start, time_t *end);
void GetTimeWindow(time_t* start, time_t* end);
void ResetTimeGapper();

#endif /* _timeseg_h_ */
