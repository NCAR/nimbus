/* Events.h
 * Handle Events communications between WINDS and discComm.  CJW 10/25/95
 */

#ifndef EVENTS_H
#define EVENTS_H

#define EVENTS_PORT	1105

/* Structure of the command packet received from the Display Computer. */
 
struct EVTPKT
	{
	int	number;
	char	station[24];
	};
 
#endif
