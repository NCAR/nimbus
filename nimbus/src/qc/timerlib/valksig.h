/*
** file: valklist.h
**
** Author: Ed Stautler
** Date: 3/95
**
** Define the data dependent functions that interface the
** GUI to the socket library.
*/

#ifndef _VALKSIG_H_
#define _VALKSIG_H_

#include <signal.h>
#include <sys/time.h>
#include "valklist.h"

#define SIGMASK 0x2000		/* mask value to block interrupt */

/*
** Define constants for time conversions.
*/
#define MILLION    1000000
#define THOUSAND      1000
#define ZERO             0

#ifdef __cplusplus
extern "C" {
#endif

/*
** struct itimerval SetClock()
** Initialize the clock to send an interrupt.
*/
#ifdef __STDC__
struct itimerval SetClock(int);
#else
struct itimerval SetClock();
#endif


/*
** int CancelTimer()
** Cancel an interrupt that has been previously set.
*/
#ifdef __STDC__
int CancelTimer(LinkedList*,ListNode*);
#else
int CancelTimer();
#endif


/*
** int SetTimer()
** Add a new Procedure to the list of waiting procedures
** to be called by alarm interrupt.
*/
#ifdef __STDC__
int SetTimer(LinkedList*,int,int(*)(char*),char*);
#else
int SetTimer();
#endif

#ifdef __cplusplus
}
#endif

#endif
